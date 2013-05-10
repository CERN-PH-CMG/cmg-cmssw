#include <iostream>
#include <boost/shared_ptr.hpp>
#include <fstream>

#include "UserCode/EWKV/interface/MacroUtils.h"
#include "UserCode/EWKV/interface/SmartSelectionMonitor.h"
#include "UserCode/EWKV/interface/DataEventSummaryHandler.h"

#include "CondFormats/JetMETObjects/interface/JetResolution.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

#include "PhysicsTools/CondLiteIO/interface/RecordWriter.h"
#include "DataFormats/FWLite/interface/Record.h"
#include "DataFormats/FWLite/interface/EventSetup.h"
#include "DataFormats/FWLite/interface/ESHandle.h"
#include "CondFormats/PhysicsToolsObjects/interface/BinningPointByMap.h"
#include "RecoBTag/PerformanceDB/interface/BtagPerformance.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h"

#include "TSystem.h"
#include "TFile.h"
#include "TChain.h"
#include "TChainElement.h"
#include "TCanvas.h"
#include "TString.h"
#include "TDirectory.h"
#include "TEventList.h"
#include "TRandom.h"

#include <iostream>

using namespace std;


int main(int argc, char* argv[])
{
  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  //check arguments
  if ( argc < 2 ) {
    std::cout << "Usage : " << argv[0] << " parameters_cfg.py" << std::endl;
    return 0;
  }
  
  //
  // configure
  //
  const edm::ParameterSet &runProcess = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("runProcess");
  TString url=runProcess.getParameter<std::string>("input");
  TString baseDir = runProcess.getParameter<std::string>("dirName");
 
  bool isMC       = runProcess.getParameter<bool>("isMC");
  int mcTruthMode = runProcess.getParameter<int>("mctruthmode");
  double xsec     = runProcess.getParameter<double>("xsec");
  
  TString out=runProcess.getParameter<std::string>("outdir");

  
  //
  // check input file
  //
  TFile *inF = TFile::Open(url);
  if(inF==0) return -1;
  if(inF->IsZombie()) return -1;
  TString proctag=gSystem->BaseName(url);
  Ssiz_t pos=proctag.Index(".root");
  proctag.Remove(pos,proctag.Length());
  
  //
  // pileup reweighter
  //
  std::vector<double> dataPileupDistributionDouble = runProcess.getParameter< std::vector<double> >("datapileup");
  std::vector<float> dataPileupDistribution; for(unsigned int i=0;i<dataPileupDistributionDouble.size();i++){dataPileupDistribution.push_back(dataPileupDistributionDouble[i]);}
  std::vector<float> mcPileupDistribution;
  if(isMC){
    TString puDist(baseDir+"/pileup");
    TH1F* histo = (TH1F *) inF->Get(puDist);
    if(!histo)std::cout<<"pileup histogram is null!!!\n";
    for(int i=1;i<=histo->GetNbinsX();i++){mcPileupDistribution.push_back(histo->GetBinContent(i));}
    delete histo;
  }
  while(mcPileupDistribution.size()<dataPileupDistribution.size())  mcPileupDistribution.push_back(0.0);
  while(mcPileupDistribution.size()>dataPileupDistribution.size())dataPileupDistribution.push_back(0.0);
  gROOT->cd();  //THIS LINE IS NEEDED TO MAKE SURE THAT HISTOGRAM INTERNALLY PRODUCED IN LumiReWeighting ARE NOT DESTROYED WHEN CLOSING THE FILE
  edm::LumiReWeighting *LumiWeights= isMC ? new edm::LumiReWeighting(mcPileupDistribution,dataPileupDistribution): 0;
  
  //
  // control histograms
  //
  SmartSelectionMonitor controlHistos;
  TH1F* Hhepup        = (TH1F* )controlHistos.addHistogram(new TH1F ("heupnup"    , "hepupnup"    ,20,0,20) ) ;
  TH1F* Hcutflow      = (TH1F*) controlHistos.addHistogram(new TH1F ("cutflow"    , "cutflow"    ,5,0,5) ) ;

  //vertex multiplicity
  controlHistos.addHistogram( new TH1F ("nvertices", "; Vertex multiplicity; Events", 50, 0.,50.) );

  //event selection histogram
  TString labels[]={"2 leptons", "M>12 #wedge |M-M_{Z}|>15", "#geq 2 jets", "#geq 2 b-tags", "op. sign" };
  int nsteps=sizeof(labels)/sizeof(TString);
  TH1F *cutflowH = (TH1F *)controlHistos.addHistogram( new TH1F("evtflow",";Cutflow;Events",nsteps,0,nsteps) );
  for(int ibin=0; ibin<nsteps; ibin++) cutflowH->GetXaxis()->SetBinLabel(ibin+1,labels[ibin]);
  
  ///
  // process events file
  //
  DataEventSummaryHandler evSummary;
  if( !evSummary.attach( (TTree *) inF->Get(baseDir+"/data") ) )  { inF->Close();  return -1; }  
  const Int_t totalEntries=evSummary.getEntries();
  
  float cnorm=1.0;
  if(isMC){
    TH1F* cutflowH = (TH1F *) inF->Get(baseDir+"/cutflow");
    if(cutflowH) cnorm=cutflowH->GetBinContent(1);
  }
  Hcutflow->SetBinContent(1,cnorm);

  cout << "Processing: " << proctag << " @ " << url << endl
       << "Initial number of events: " << cnorm << endl
       << "Events in tree:           " << totalEntries << endl
       << " xSec x BR:               " << xsec << endl;
       
  //
  // analyze (puf...)
  //
  for (int inum=0; inum < totalEntries; ++inum)
    {
      if(inum%500==0) { printf("\r [ %d/100 ]",int(100*float(inum)/float(totalEntries))); cout << flush; }
      evSummary.getEntry(inum);
      DataEventSummary &ev = evSummary.getEvent();

      //pileup weight
      float weight(1.0);
      if(LumiWeights) weight = LumiWeights->weight(ev.ngenITpu);

      //FIXME FIXME FIXME for new ntuples
      bool eeTrigger   = ev.t_bits[0];
      bool emuTrigger  = ev.t_bits[3] || ev.t_bits[4];
      bool mumuTrigger = ev.t_bits[2];
      
      data::PhysicsObjectCollection_t leptons=evSummary.getPhysicsObject(DataEventSummaryHandler::LEPTONS);
      data::PhysicsObjectCollection_t selLeptons;
      for(size_t ilep=0; ilep<leptons.size(); ilep++)
	{
	  Int_t id=leptons[ilep].get("id");
	  bool passKin(true),passId(true),passIso(true);
	  if(abs(id)==11)
	    {
	      float sceta=leptons[ilep].getVal("sceta");
	      Float_t gIso    = leptons[ilep].getVal("gIso03");
	      Float_t chIso   = leptons[ilep].getVal("chIso03");
	      //Float_t puchIso = leptons[ilep].getVal("puchIso03");
	      Float_t nhIso   = leptons[ilep].getVal("nhIso03");
	      float relIso=(TMath::Max(nhIso+gIso-ev.rho*utils::cmssw::getEffectiveArea(11,leptons[ilep].getVal("sceta")),Float_t(0.))+chIso)/leptons[ilep].pt();
	      if(leptons[ilep].pt()<20)                      passKin=false;
	      if(fabs(leptons[ilep].eta())>2.5)              passKin=false;
	      if(fabs(sceta)>1.4442 && fabs(sceta)<1.5660)   passKin=false;
	      if(leptons[ilep].getFlag("isconv"))            passId=false;
	      if(leptons[ilep].getVal("tk_d0")>0.4)          passId=false;
	      if(leptons[ilep].getVal("tk_lostInnerHits")>0) passId=false;
	      if(leptons[ilep].getVal("mvatrig")<0.5)        passId=false;
	      if(relIso>0.15)                                passIso=false;
	    }
	  else
	    {
	      Int_t idbits    = leptons[ilep].get("idbits");
	      bool isPFlow    = ((idbits >> 7) & 0x1);
	      bool isGlobal   = ((idbits >> 6) & 0x1);
	      bool isTracker  = ((idbits >> 5) & 0x1);
	      Float_t gIso    = leptons[ilep].getVal("gIso04");
	      Float_t chIso   = leptons[ilep].getVal("chIso04");
	      Float_t puchIso = leptons[ilep].getVal("puchIso04");
	      Float_t nhIso   = leptons[ilep].getVal("nhIso04");
	      Float_t relIso=(TMath::Max(nhIso+gIso-0.5*puchIso,0.)+chIso)/leptons[ilep].pt();
	      if(leptons[ilep].pt()<20)                      passKin=false;
	      if(fabs(leptons[ilep].eta())>2.4)              passKin=false;
	      if(!isPFlow || !isGlobal || !isTracker)        passId=false;
	      if(relIso>0.12)                                passIso=false;
	    }

	  if(!passKin || !passId || !passIso) continue;
	  selLeptons.push_back(leptons[ilep]);
	}
      sort(selLeptons.begin(),selLeptons.end(),data::PhysicsObject_t::sortByPt);
      
      //select the leptons
      if(!eeTrigger && !emuTrigger && !mumuTrigger) continue;
      if(selLeptons.size()<2) continue;
      int lid1(selLeptons[0].get("id")), lid2(selLeptons[1].get("id"));
      TString ch("");
      if     (abs(lid1)*abs(lid2)==11*11 && eeTrigger)   ch="ee";
      else if(abs(lid1)*abs(lid2)==11*13 && emuTrigger)  ch="emu";
      else if(abs(lid1)*abs(lid2)==13*13 && mumuTrigger) ch="mumu";
    
      //select the jets
      data::PhysicsObjectCollection_t jets=evSummary.getPhysicsObject(DataEventSummaryHandler::JETS);
      data::PhysicsObjectCollection_t looseJets,selJets;
      for(size_t ijet=0; ijet<jets.size(); ijet++)
	{
	  //cross-clean with selected leptons 
	  double minDRlj(9999.);
	  for(size_t ilep=0; ilep<selLeptons.size(); ilep++)
	    minDRlj = TMath::Min( minDRlj, deltaR(jets[ijet],selLeptons[ilep]) );
	  if(minDRlj<0.4) continue;
	  
	  //require to pass the loose id
	  Int_t idbits=jets[ijet].get("idbits");
	  bool passPFloose( ((idbits>>0) & 0x1));
	  if(!passPFloose) continue;

	  //top candidate jets
	  looseJets.push_back(jets[ijet]);
	  if(jets[ijet].pt()<30 || fabs(jets[ijet].eta()>2.5) ) continue;
	  selJets.push_back(jets[ijet]);
	}
      sort(looseJets.begin(),looseJets.end(),data::PhysicsObject_t::sortByPt);
      sort(selJets.begin(),selJets.end(),data::PhysicsObject_t::sortByCSV);
    
      //select the event
      if(selLeptons.size()<2) continue;
      controlHistos.fillHisto("evtflow", ch, 0, weight);
      LorentzVector ll=selLeptons[0]+selLeptons[1];
      float mll=ll.mass();
      bool isZcand( (ch=="ee" || ch=="mumu") && fabs(mll-91)<15);
      bool isOS( selLeptons[0].get("id")*selLeptons[1].get("id") < 0 ); 

      if(mll<12 || isZcand) continue;
      controlHistos.fillHisto("evtflow", ch, 1, weight);
      
      if(selJets.size()<2) continue;
      controlHistos.fillHisto("evtflow", ch, 2, weight);
      
      float nbtags(0);
      for(size_t ijet=0; ijet<selJets.size(); ijet++) nbtags += (selJets[ijet].getVal("supercsv")>0.531);
      if(nbtags<2) continue;
      controlHistos.fillHisto("evtflow", ch, 3, weight);
      
      if(!isOS) continue;
      controlHistos.fillHisto("evtflow", ch, 4, weight);

      //the met
      data::PhysicsObject_t met=evSummary.getPhysicsObject(DataEventSummaryHandler::MET)[0];
      cout << met.pt() << " " << met.phi() << endl;
      
      //PF candidates
      data::PhysicsObjectCollection_t pf = evSummary.getPhysicsObject(DataEventSummaryHandler::PFCANDIDATES);
      cout << pf.size() << endl;

      //MC truth
      data::PhysicsObjectCollection_t gen=evSummary.getPhysicsObject(DataEventSummaryHandler::GENPARTICLES);
      LorentzVector top,antitop;
      for(size_t igen=0; igen<gen.size(); igen++)
	{
	  if(gen[igen].get("id")==6)  top=gen[igen];
	  if(gen[igen].get("id")==-6) antitop=gen[igen];
	}


      
      LorentzVector htvec=ll+met+selJets[0]+selJets[1];
      LorentzVector gen_ttbar=top+antitop;
      cout << ll.pt() << " " << met.pt() << " " << selJets[0].pt() << " " << selJets[1].pt() << " " << htvec.phi() << " " << gen_ttbar.phi() << endl;

    }

  //
  // close opened files
  // 
  inF->Close();
  
    
  //
  // save histos to local file
  //
  TString outUrl(out);
  gSystem->ExpandPathName(outUrl);
  gSystem->Exec("mkdir -p " + outUrl);
  outUrl += "/";
  outUrl += proctag;
  if(mcTruthMode!=0) { outUrl += "_filt"; outUrl += mcTruthMode; }
  outUrl += ".root";
  TFile *file=TFile::Open(outUrl, "recreate");
  controlHistos.Write();
  file->Close();
  
  //that's all folks!
}  
