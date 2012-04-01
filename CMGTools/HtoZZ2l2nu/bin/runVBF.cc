#include <iostream>
#include <boost/shared_ptr.hpp>
#include "Math/GenVector/Boost.h"

#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"
#include "CMGTools/HtoZZ2l2nu/interface/METUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/GammaEventHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"
#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/SmartSelectionMonitor.h"
#include "CMGTools/HtoZZ2l2nu/interface/TMVAUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/MacroUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"
#include "CMGTools/HtoZZ2l2nu/interface/BtagUncertaintyComputer.h"

#include "CondFormats/JetMETObjects/interface/JetResolution.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TEventList.h"
#include "TROOT.h"
 
using namespace std;



int main(int argc, char* argv[])
{
  //##############################################
  //########    GLOBAL INITIALIZATION     ########
  //##############################################

  // check arguments
  if(argc<2){ std::cout << "Usage : " << argv[0] << " parameters_cfg.py" << std::endl; exit(0); }
  
  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  // configure the process
  const edm::ParameterSet &runProcess = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("runProcess");
  TString url=runProcess.getParameter<std::string>("input");
  TString outdir=runProcess.getParameter<std::string>("outdir");
  TString outUrl( outdir );
  gSystem->Exec("mkdir -p " + outUrl);

  bool isMC = runProcess.getParameter<bool>("isMC");
  int mctruthmode=runProcess.getParameter<int>("mctruthmode");
  
  TString outTxtUrl= outUrl + "/" + gSystem->BaseName(url) + ".txt";
  FILE* outTxtFile = NULL;
  if(!isMC)outTxtFile = fopen(outTxtUrl.Data(), "w");
  printf("TextFile URL = %s\n",outTxtUrl.Data());

  //handler for gamma processes
  GammaEventHandler *gammaEvHandler=0;
  if(mctruthmode==22){
     isMC=false;
     gammaEvHandler = new GammaEventHandler(runProcess);
  }

  //tree info
  int evStart=runProcess.getParameter<int>("evStart");
  int evEnd=runProcess.getParameter<int>("evEnd");
  TString dirname = runProcess.getParameter<std::string>("dirName");

  //jet description
  TString etaFileName = runProcess.getParameter<std::string>("etaResolFileName"); gSystem->ExpandPathName(etaFileName);
  JetResolution stdEtaResol(etaFileName.Data(),false);
  TString phiFileName = runProcess.getParameter<std::string>("phiResolFileName"); gSystem->ExpandPathName(phiFileName);
  JetResolution stdPhiResol(phiFileName.Data(),false);
  TString ptFileName  = runProcess.getParameter<std::string>("ptResolFileName");  gSystem->ExpandPathName(ptFileName);
  JetResolution stdPtResol(ptFileName.Data(),true);
  TString uncFile =  runProcess.getParameter<std::string>("jesUncFileName"); gSystem->ExpandPathName(uncFile);
  JetCorrectionUncertainty jecUnc(uncFile.Data());
  bool runSystematics                        = runProcess.getParameter<bool>("runSystematics");

  btag::UncertaintyComputer bcomp(0.837, 0.95, 0.06, 0.286, 1.15, 0.11);
  if(runSystematics) { cout << "Systematics will be computed for this analysis" << endl; }


  //isMC_VBF sample ?
  double HiggsMass=0; string VBFString = ""; string GGString("");
  bool isMC_GG  = isMC && ( string(url.Data()).find("GG" )  != string::npos);
  bool isMC_VBF = isMC && ( string(url.Data()).find("VBF")  != string::npos);
  std::vector<TGraph *> hWeightsGrVec;
  if(isMC_GG){
    size_t GGStringpos =  string(url.Data()).find("GG");
    string StringMass = string(url.Data()).substr(GGStringpos+5,3);  sscanf(StringMass.c_str(),"%lf",&HiggsMass);
    GGString = string(url.Data()).substr(GGStringpos);
    TFile *fin=TFile::Open("~psilva/public/HtoZZ/HiggsQtWeights.root");
    if(fin){
	for(int ivar=0; ivar<5; ivar++){
	  double ren=HiggsMass; if(ivar==ZZ2l2nuSummary_t::hKfactor_renDown)  ren/=2; if(ivar==ZZ2l2nuSummary_t::hKfactor_renUp)  ren *= 2;
	  double fac=HiggsMass; if(ivar==ZZ2l2nuSummary_t::hKfactor_factDown) fac/=2; if(ivar==ZZ2l2nuSummary_t::hKfactor_factUp) fac *= 2;
	  char buf[100]; sprintf(buf,"kfactors_mh%3.0f_ren%3.0f_fac%3.0f",HiggsMass,ren,fac);
	  TGraph *gr= (TGraph *) fin->Get(buf);
	  if(gr) hWeightsGrVec.push_back((TGraph *)gr->Clone());
	}
	fin->Close();
	delete fin;
    }
  }else if(isMC_VBF){
     size_t VBFStringpos =  string(url.Data()).find("VBF");
     string StringMass = string(url.Data()).substr(VBFStringpos+6,3);  sscanf(StringMass.c_str(),"%lf",&HiggsMass);
     VBFString = string(url.Data()).substr(VBFStringpos);
  }

  //##############################################
  //########    INITIATING HISTOGRAMS     ########
  //##############################################
  SmartSelectionMonitor mon;


  //Renormalization
  TH1F* Hcutflow     = (TH1F*) mon.addHistogram(  new TH1F ("cutflow"    , "cutflow"    ,5,0,5) ) ;

  
  //VBF
  TH1F* h;
  h = new TH1F ("VBFNEventsInc", ";Selection cut;Events", 15,0,15);
  h->GetXaxis()->SetBinLabel(1,"All");
  h->GetXaxis()->SetBinLabel(2,"2Jets (30GeV)");
  h->GetXaxis()->SetBinLabel(3,"dEta");
  h->GetXaxis()->SetBinLabel(4,"inv. Mass");
  h->GetXaxis()->SetBinLabel(5,"Central lepton");
  h->GetXaxis()->SetBinLabel(6,"Jet Veto");
  h->GetXaxis()->SetBinLabel(7,"BJet Veto");
  h->GetXaxis()->SetBinLabel(8,"3rd Lepton Veto");
  h->GetXaxis()->SetBinLabel(9,"central dilepton");
  h->GetXaxis()->SetBinLabel(10,"jet-vtx = 1");
  h->GetXaxis()->SetBinLabel(11,"jet-vtx = 2");
  mon.addHistogram( h );
  mon.addHistogram( new TH1F ("VBFAssym", ";VBFAssym", 100, -1,1) );
  mon.addHistogram( new TH2F ("VBFAssymvspu", ";VBFAssymvspu",35, 0, 35, 100, -1,1) );
  mon.addHistogram( new TH1F ("VBFMinEta", ";VBFMinEta", 100, 0,10) );                                                                                                                                                             
  mon.addHistogram( new TH1F ("VBFMaxEta", ";VBFMaxEta", 100, 0,10) );     
  mon.addHistogram( new TH2F ("VBFMinEtavspu", ";VBFMinEtavspu",35, 0, 35, 100, 0,10) );
  mon.addHistogram( new TH2F ("VBFMaxEtavspu", ";VBFMaxEtavspu",35, 0, 35, 100, 0,10) );
  mon.addHistogram( new TH2F ("VBFThirdJetPtvspu", ";VBFThirdJetPtvspu",35, 0, 35, 100, 0,30) );
  mon.addHistogram( new TH2F ("VBFThirdJetEtavspu", ";VBFThirdJetEtavspu",35, 0, 35, 100, 0,10) );
  mon.addHistogram( new TH1F ("VBFdEtaInc", ";#Delta#eta", 100, 0,10) );
  mon.addHistogram( new TH1F ("VBFiMassInc",";Inv. Mass", 100, 0.,2000) );
  mon.addHistogram( new TH1F ("VBFcenLeptonVetoInc",";Central Lepton Veto", 10, 0.,10) );
  mon.addHistogram( new TH1F ("VBFcen30JetVetoInc",";Central 30 Jet Veto", 10, 0.,10) );
  mon.addHistogram( new TH1F ("VBFNBJets30Inc", ";N BJets (pT>30);Events", 10,0,10) );
  mon.addHistogram( new TH1F ("VBFdEtaNM1C", ";#Delta#eta", 100, 0,10) );
  mon.addHistogram( new TH1F ("VBFiMassNM1C",";Inv. Mass", 100, 0.,2000) );
  mon.addHistogram( new TH1F ("VBFcenLeptonVetoNM1C",";Central Lepton Veto", 10, 0.,10) );
  mon.addHistogram( new TH1F ("VBFcen30JetVetoNM1C",";Central 30 Jet Veto", 10, 0.,10) );
  mon.addHistogram( new TH1F ("VBFNBJets30NM1C", ";N BJets (pT>30);Events", 10,0,10) );
  mon.addHistogram( new TH2F ("VBFdEtaiMassNM1C", ";#Delta#eta;Inv. Mass", 50, 0,10,50, 0.,2000) );
  mon.addHistogram( new TH2F ("VBFdEtaiMassNM1C2", ";#Delta#eta;Inv. Mass", 50, 0,10,50, 0.,2000) );
  mon.addHistogram( new TH1F ("VBFdEta3rdlepton", ";#Delta#eta", 100, 0,10) );
  mon.addHistogram( new TH1F ("VBFiMass3rdlepton",";Inv. Mass", 100, 0.,2000) );
  mon.addHistogram( new TH1F ("VBFcen30JetVeto3rdlepton",";Central 30 Jet Veto", 10, 0.,10) );
  mon.addHistogram( new TH1F ("VBFNBJets303rdlepton", ";N BJets (pT>30);Events", 10,0,10) );
  mon.addHistogram( new TH1F ("VBFtotalvspu", ";#generated vertices", 35, 0,35) );                        
  mon.addHistogram( new TH1F ("VBFtagvspu_noveto", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("VBFtagvspu", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("VBFtagtk1vspu", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("VBFtagtk2vspu", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("VBFtag3rd20vspu", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("VBFtag3rd25vspu", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("VBFtotalvspu_cen", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("VBFtagvspu_noveto_cen", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("VBFtagvspu_cen", ";#generated vertices", 35, 0,35) ); 
  mon.addHistogram( new TH1F ("VBFtagtk1vspu_cen", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("VBFtagtk2vspu_cen", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("VBFtag3rd20vspu_cen", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("VBFtotalvspu_fwd", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("VBFtagvspu_fwd", ";#generated vertices", 35, 0,35) ); 
  mon.addHistogram( new TH1F ("VBFtagtk1vspu_fwd", ";#generated vertices", 35, 0,35) );
  mon.addHistogram( new TH1F ("VBFtagtk2vspu_fwd", ";#generated vertices", 35, 0,35) );

  mon.addHistogram( new TH2F ("VBFdEtavspu", ";#generated vertices;#Delta #eta", 35, 0,35,  60, -6,6) );
  mon.addHistogram( new TH2F ("VBFdEtatkvspu", ";#generated vertices;#Delta #eta", 35, 0,35,  60, -6,6) );
  mon.addHistogram( new TH2F ("VBFchargedF1vspu", ";#generated vertices;charged fraction", 35, 0,35,  100, 0,1) );
  mon.addHistogram( new TH2F ("VBFchargedF2vspu", ";#generated vertices;charged fraction", 35, 0,35,  100, 0,1) );

  mon.addHistogram( new TH2F ("VBFNCenJet15vspu", ";#generated vertices;centralJet", 35, 0,35,  10, 0,10) );
  mon.addHistogram( new TH2F ("VBFNCenJet15vsputk", ";#generated vertices;centralJet", 35, 0,35,  10, 0,10) );
  mon.addHistogram( new TH2F ("VBFNCenJet30vspu", ";#generated vertices;centralJet", 35, 0,35,  10, 0,10) );
  mon.addHistogram( new TH2F ("VBFNCenJet30vsputk", ";#generated vertices;centralJet", 35, 0,35,  10, 0,10) );

  mon.addHistogram( new TH1F ("VBFNJets15", ";VBFNJets15", 20, 0,20) );
  mon.addHistogram( new TH2F ("VBFNJets15vspu", ";VBFNJets15vspu", 35, 0,35, 40, 0,40) );
  mon.addHistogram( new TH1F ("VBFNJets30", ";VBFNJets30", 20, 0,20) );
  mon.addHistogram( new TH2F ("VBFNJets30vspu", ";VBFNJets30vspu", 35, 0,35, 40, 0,40) );
  mon.addHistogram( new TH1F ("VBFNJets15tk", ";VBFNJets15tk", 20, 0,20) );
  mon.addHistogram( new TH2F ("VBFNJets15tkvspu", ";VBFNJets15tkvspu", 35, 0,35, 40, 0,40) );
  mon.addHistogram( new TH1F ("VBFNJets30tk", ";VBFNJets30tk", 20, 0,20) );
  mon.addHistogram( new TH2F ("VBFNJets30tkvspu", ";VBFNJets30tkvspu", 35, 0,35, 40, 0,40) );
  mon.addHistogram( new TH2F ("VBFTaggedVsEta", ";Eta;Tagging", 20, 0,5, 100, 0, 1) );


  //##############################################
  //######## GET READY FOR THE EVENT LOOP ########
  //##############################################


  //open the file and get events tree
  ZZ2l2nuSummaryHandler evSummaryHandler;
  TFile *file = TFile::Open(url);
  printf("Looping on %s\n",url.Data());
  if(file==0) return -1;
  if(file->IsZombie()) return -1;
  if( !evSummaryHandler.attachToTree( (TTree *)file->Get(dirname) ) ){
      file->Close();
      return -1;
  }


  //check run range to compute scale factor (if not all entries are used)
  const Int_t totalEntries= evSummaryHandler.getEntries();
  float rescaleFactor( evEnd>0 ?  float(totalEntries)/float(evEnd-evStart) : -1 );
  if(evEnd<0 || evEnd>evSummaryHandler.getEntries() ) evEnd=totalEntries;
  if(evStart > evEnd ){
      file->Close();
      return -1;
  }

  //MC normalization (to 1/pb)
  float cnorm=1.0;
  if(isMC){
      TH1F* cutflowH = (TH1F *) file->Get("evAnalyzer/h2zz/cutflow");
      if(cutflowH) cnorm=cutflowH->GetBinContent(1);
      if(rescaleFactor>0) cnorm /= rescaleFactor;
      printf("cnorm = %f\n",cnorm);
  }
  Hcutflow->SetBinContent(1,cnorm);


  //pileup weighting
  std::vector<double> dataPileupDistributionDouble = runProcess.getParameter< std::vector<double> >("datapileup");
  std::vector<float> dataPileupDistribution; for(unsigned int i=0;i<dataPileupDistributionDouble.size();i++){dataPileupDistribution.push_back(dataPileupDistributionDouble[i]);}
  std::vector<float> mcPileupDistribution;
  if(isMC){
      TH1F* histo = (TH1F *) file->Get("evAnalyzer/h2zz/pileup");
      if(!histo)std::cout<<"pileup histogram is null!!!\n";
      for(int i=1;i<=histo->GetNbinsX();i++){mcPileupDistribution.push_back(histo->GetBinContent(i));}
      delete histo;
  }
  while(mcPileupDistribution.size()<dataPileupDistribution.size())  mcPileupDistribution.push_back(0.0);
  while(mcPileupDistribution.size()>dataPileupDistribution.size())dataPileupDistribution.push_back(0.0);
  gROOT->cd();  //THIS LINE IS NEEDED TO MAKE SURE THAT HISTOGRAM INTERNALLY PRODUCED IN LumiReWeighting ARE NOT DESTROYED WHEN CLOSING THE FILE
  edm::LumiReWeighting LumiWeights(mcPileupDistribution,dataPileupDistribution);
  reweight::PoissonMeanShifter PShiftUp(+0.6);
  reweight::PoissonMeanShifter PShiftDown(-0.6);


  //check PU normalized entries 
  //evSummaryHandler.getTree()->Draw(">>elist","normWeight==1");
  //TEventList *elist = (TEventList*)gDirectory->Get("elist");
  //const Int_t normEntries = (elist==0 ? 0 : elist->GetN()); 
  //if(normEntries==0) cout << "[Warning] Normalized PU entries is 0, check if the PU normalization producer was properly run" << endl;
 

  //event Categorizer
  EventCategory eventCategoryInst(true);


  //##############################################
  //########           EVENT LOOP         ########
  //##############################################


 
  //loop on all the events
  printf("Progressing Bar     :0%%       20%%       40%%       60%%       80%%       100%%\n");
  printf("Scanning the ntuple :");
  int treeStep = (evEnd-evStart)/50;if(treeStep==0)treeStep=1;
  for( int iev=evStart; iev<evEnd; iev++){
      if((iev-evStart)%treeStep==0){printf(".");fflush(stdout);}
      //##############################################   EVENT LOOP STARTS   ##############################################

      //load the event content from tree
      evSummaryHandler.getEntry(iev);
      ZZ2l2nuSummary_t &ev=evSummaryHandler.getEvent();
      PhysicsEvent_t phys=getPhysicsEventFrom(ev);

      if(ev.cat!=MUMU)continue;

      //categorize events
      TString tag_cat;
      switch(ev.cat){
         case EMU : tag_cat = "emu";   break;
         case MUMU: tag_cat = "mumu";  break;
         case EE  : tag_cat = "ee";    break;
         default  : tag_cat = "??";    break;

      }

      if(isMC && mctruthmode==1)
        {
          if(getGenProcess(ev.mccat)!=Z_CH) continue;
          if(getNgenLeptons(ev.mccat,ELECTRON)<2 && getNgenLeptons(ev.mccat,MUON)<2) continue;
        }
      if(isMC && mctruthmode==2)
        {
          if(getGenProcess(ev.mccat)!=Z_CH) continue;
          if(getNgenLeptons(ev.mccat,TAU)<2) continue;
        }

      bool isGammaEvent = false;
      if(gammaEvHandler){
          isGammaEvent=gammaEvHandler->isGood(phys);
          if(mctruthmode==22 && !isGammaEvent) continue;
          tag_cat = "gamma";
      }
     
      int eventSubCat  = eventCategoryInst.Get(phys);
      TString tag_subcat = eventCategoryInst.GetLabel(eventSubCat);

      //prepare the tag's vectors for histo filling
      std::vector<TString> tags_full;
      tags_full.push_back("all");
//      tags_full.push_back(tag_cat);
//      tags_full.push_back(tag_cat + tag_subcat);

      
      //OOT pu condition
      TString ootCond("");
      if(isMC){
          double sigma        = sqrt(double(2*ev.ngenITpu));
          double minCentralPu = -sigma;
          double maxCentralPu = sigma;
          double maxHighPu    = 2*sigma;
          double puDiff       = double(ev.ngenOOTpu-2*ev.ngenITpu);
          if(puDiff>=maxHighPu)         ootCond="VeryHighOOTpu";
          else if(puDiff>=maxCentralPu) ootCond="HighOOTpu";
          else if(puDiff>=minCentralPu) ootCond="MediumOOTpu";
          else                          ootCond="LowOOTpu";
      }

      //pileup and Higgs pT weight
      //float weight=ev.puWeight;
      float weight = 1.0;
      double TotalWeight_plus = 1.0;
      double TotalWeight_minus = 1.0;
      if(isMC){
        weight = LumiWeights.weight( ev.ngenITpu );
        TotalWeight_plus = PShiftUp.ShiftWeight( ev.ngenITpu );
        TotalWeight_minus = PShiftDown.ShiftWeight( ev.ngenITpu );
        if(isMC_VBF) weight *= weightVBF(VBFString,HiggsMass, phys.genhiggs[0].mass() );         
        if(isMC_GG)  {
	  for(size_t iwgt=0; iwgt<hWeightsGrVec.size(); iwgt++) ev.hptWeights[iwgt] = hWeightsGrVec[iwgt]->Eval(phys.genhiggs[0].pt());
	  weight *= ev.hptWeights[0];
	}
      }
     

      Hcutflow->Fill(1,1);
      Hcutflow->Fill(2,weight);
      Hcutflow->Fill(3,weight*TotalWeight_minus);
      Hcutflow->Fill(4,weight*TotalWeight_plus);


      //##############################################
      //########       GLOBAL VARIABLES       ########
      //##############################################

      float iweight=weight;
//FIXME
//              if(gammaEvHandler){
//                  TString dilCh=catsToFill[ic];
//                  if(dilCh=="all") dilCh="ll";
//                  iweight *= gammaEvHandler->getWeight(dilCh);
//              }              


      //z+met kinematics
      LorentzVector zll  = isGammaEvent ? gammaEvHandler->massiveGamma("ll") : phys.leptons[0]+phys.leptons[1];
      LorentzVector zvv  = phys.met[0];
      Float_t dphill     = isGammaEvent ? 0 : deltaPhi(phys.leptons[0].phi(),phys.leptons[1].phi());
      //Float_t detall     = isGammaEvent ? 0 : phys.leptons[0].eta()-phys.leptons[1].eta();
      Float_t drll       = isGammaEvent ? 0 : deltaR(phys.leptons[0],phys.leptons[1]);
      Float_t mindrlz    = isGammaEvent ? 0 : min( deltaR(phys.leptons[0],zll), deltaR(phys.leptons[1],zll) );
      Float_t maxdrlz    = isGammaEvent ? 0 : max( deltaR(phys.leptons[0],zll), deltaR(phys.leptons[1],zll) );
      Float_t ptl1       = isGammaEvent ? 0 : phys.leptons[0].pt();
      Float_t ptl2       = isGammaEvent ? 0 : phys.leptons[1].pt();
      Float_t ptsum      = ptl1+ptl2;
      Float_t mtl1       = isGammaEvent ? 0 : METUtils::transverseMass(phys.leptons[0],zvv,false);
      Float_t mtl2       = isGammaEvent ? 0 : METUtils::transverseMass(phys.leptons[1],zvv,false);
      Float_t mtsum      = mtl1+mtl2;
      Float_t zmass      = zll.mass();
      Float_t zpt        = zll.pt();
      Float_t zeta       = zll.eta();
      Float_t met        = zvv.pt();
      //Float_t dphizz     = deltaPhi(zll.phi(),zvv.phi());
      Float_t mt         = METUtils::transverseMass(zll,zvv,true);
      //Float_t dphizleadl = isGammaEvent ? 0 : ( ptl1>ptl2 ? deltaPhi(phys.leptons[0].phi(),zll.phi()) : deltaPhi(phys.leptons[1].phi(),zll.phi()) );



      //count jets and b-tags
      int njets(0),njetsinc(0);
      int nbtags(0), nbtags_tchel(0),   nbtags_tche2(0),  nbtags_csvl(0);
      LorentzVectorCollection jetsP4;
      int nheavyjets(0), nlightsjets(0);
      for(size_t ijet=0; ijet<phys.jets.size(); ijet++){
          jetsP4.push_back( phys.jets[ijet] );
          njetsinc++;
          if(fabs(phys.jets[ijet].eta())<2.5){
              njets++;
              bool passTCHEL(phys.jets[ijet].btag1>1.7);
              bool passTCHE2(phys.jets[ijet].btag1>2.0);
              bool passCSVL(phys.jets[ijet].btag2>0.244);
              if(phys.jets[ijet].pt()>30){
                nbtags          += passTCHE2;
                nbtags_tchel    += passTCHEL;
                nbtags_tche2    += passTCHE2;
                nbtags_csvl     += passCSVL;
                nheavyjets += (fabs(phys.jets[ijet].genid)==5);
                nlightsjets += (fabs(phys.jets[ijet].genid)!=5);
              }
          }
      }
      bcomp.compute(nheavyjets,nlightsjets);
      std::vector<btag::Weight_t> wgt = bcomp.getWeights();
      double p0btags = wgt[0].first;  
      double p0btags_err=wgt[0].second;
      

      //met variables: check BaseMetSelection @ StandardSelections_cfi.py
      LorentzVector metP4             = phys.met[0];
   
      //##############################################
      //########     GLOBAL SELECTION         ########
      //##############################################

      bool passLooseKinematics(zpt>20);
      bool passZmass(fabs(zmass-91)<15);
      bool passSideBand( !passZmass && fabs(zmass-91)<30 );
      bool pass3dLeptonVeto(true); for(unsigned int i=2;i<phys.leptons.size();i++){if(phys.leptons[i].pt()>10)pass3dLeptonVeto=false;}
      
      //##############################################
      //########           VBF PLOTS          ########
      //##############################################

      bool isVBF        = false;
      bool VBFPass2Jet30   = false;
      bool VBFPassdEtaCut  = false;
      bool VBFPassiMCut    = false;
      bool VBFPassBJetVeto = false;
      bool VBFPassJetVeto  = false;
      bool VBFPassLeptonIn = false;
      bool VBFPassJetAssocVeto = false;
      bool VBFPassCentralDilep = false;

      double  VBFMinEta = 0;                                                                                                                                                                                                                 
      double  VBFMaxEta = 0;       
      double  VBFdEta = -1;
      int     VBFCentral15Jets = 0;
      int     VBFCentral15Jetstk = 0;
      int     VBFCentral30Jets = 0;
      int     VBFCentral30Jetstk = 0;
      int     VBFCentralLeptons = 0;
      int     VBFNBJets=0;
      int     VBFNJetsAssoc=0;
      LorentzVector VBFSyst;

      if(passZmass){
          int VBFNJets15 = 0; int VBFNJets15tk = 0; for(size_t ijet=0; ijet<phys.jets.size(); ijet++){if(phys.jets[ijet].pt()>15){VBFNJets15++; if(phys.jets[ijet].chHadFrac>0.25)VBFNJets15tk++;} }
          int VBFNJets30 = 0; int VBFNJets30tk = 0; for(size_t ijet=0; ijet<phys.jets.size(); ijet++){if(phys.jets[ijet].pt()>30){VBFNJets30++; if(phys.jets[ijet].chHadFrac>0.25)VBFNJets30tk++;} }
          mon.fillHisto("VBFNJets15"  ,tags_full,    VBFNJets15 ,iweight);
          mon.fill2DHisto("VBFNJets15vspu"  ,tags_full,    ev.ngenITpu,  VBFNJets15 ,1);
          mon.fillHisto("VBFNJets30"  ,tags_full,    VBFNJets30 ,iweight);
          mon.fill2DHisto("VBFNJets30vspu"  ,tags_full,    ev.ngenITpu,  VBFNJets30 ,1);
          mon.fillHisto("VBFNJets15tk"  ,tags_full,    VBFNJets15tk ,iweight);
          mon.fill2DHisto("VBFNJets15tkvspu"  ,tags_full,    ev.ngenITpu,  VBFNJets15tk ,1);
          mon.fillHisto("VBFNJets30tk"  ,tags_full,    VBFNJets30tk ,iweight);
          mon.fill2DHisto("VBFNJets30tkvspu"  ,tags_full,    ev.ngenITpu,  VBFNJets30tk ,1);
      }


      VBFCentral30Jets = 0;
      VBFCentral30Jetstk = 0;
      VBFCentral15Jets = 0;
      VBFCentral15Jetstk = 0;
      if(passZmass){
         if(phys.jets.size()>=2){
             VBFSyst =  phys.jets[0] + phys.jets[1];
             VBFdEta = fabs(   phys.jets[0].eta() -    phys.jets[1].eta()); if(phys.jets[0].eta()* phys.jets[1].eta()>0)VBFdEta*=-1;
             double MinEta, MaxEta;

             if(phys.jets[0].eta()<phys.jets[1].eta()){MinEta=phys.jets[0].eta(); MaxEta=phys.jets[1].eta();}else{MinEta=phys.jets[1].eta(); MaxEta=phys.jets[0].eta();}
             if(isGammaEvent){  if(phys.leptons[0].eta()>MinEta && phys.leptons[0].eta()<MaxEta)VBFCentralLeptons++;  if(phys.leptons[1].eta()>MinEta && phys.leptons[1].eta()<MaxEta)VBFCentralLeptons++;
             }else{             if(zll.eta()>MinEta && zll.eta()<MaxEta) VBFCentralLeptons=2;
             }

             VBFNJetsAssoc=0;
             if(phys.jets[0].chHadFrac>0.25)VBFNJetsAssoc++;
             if(phys.jets[1].chHadFrac>0.25)VBFNJetsAssoc++;

             for(size_t ijet=2; ijet<phys.jets.size(); ijet++){
                 if(phys.jets[ijet].pt()<15)continue;
                 if(phys.jets[ijet].eta()>MinEta && phys.jets[ijet].eta()<MaxEta)VBFCentral15Jets++;
                 if(phys.jets[ijet].eta()>MinEta && phys.jets[ijet].eta()<MaxEta && phys.jets[ijet].chHadFrac>0.25)VBFCentral15Jetstk++;
                 if(phys.jets[ijet].pt()<30)continue; 
                 if(phys.jets[ijet].eta()>MinEta && phys.jets[ijet].eta()<MaxEta)VBFCentral30Jets++; 
                 if(phys.jets[ijet].eta()>MinEta && phys.jets[ijet].eta()<MaxEta && phys.jets[ijet].chHadFrac>0.25)VBFCentral30Jetstk++;
                 if(phys.jets[ijet].btag1>1.7) VBFNBJets++;
             }
          
             VBFPass2Jet30   = (phys.jets [0].pt()>30.0 && phys.jets [1].pt()>30.0);
             VBFPassdEtaCut  = VBFPass2Jet30 && (fabs(VBFdEta)>4.0);
             VBFPassiMCut    = VBFPass2Jet30 && (VBFSyst.M()>500);
             VBFPassLeptonIn = (VBFCentralLeptons==2);
             VBFPassJetVeto  = (VBFCentral30Jets==0);
             VBFPassBJetVeto = (VBFNBJets==0);
             VBFPassJetAssocVeto = (VBFNJetsAssoc>0); 
             VBFPassCentralDilep = (zll.eta()>MinEta && zll.eta()<MaxEta);
             isVBF        = (VBFPass2Jet30 && VBFPassdEtaCut && VBFPassiMCut && VBFPassBJetVeto && VBFPassJetVeto && VBFPassLeptonIn && VBFPassCentralDilep);
             VBFMinEta = std::min(fabs(phys.jets[0].eta()), fabs(phys.jets[1].eta()));
             VBFMaxEta = std::max(fabs(phys.jets[0].eta()), fabs(phys.jets[1].eta()));

            if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto)mon.fill2DHisto("VBFTaggedVsEta", tags_full, fabs(phys.jets [0].eta()), phys.jets[0].chHadFrac, 1);
            if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto)mon.fill2DHisto("VBFTaggedVsEta", tags_full, fabs(phys.jets [1].eta()), phys.jets[1].chHadFrac, 1);

            if(VBFPassdEtaCut && VBFPassiMCut){
               mon.fill2DHisto("VBFchargedF1vspu", tags_full, ev.ngenITpu, phys.jets[0].chHadFrac, 1);
               mon.fill2DHisto("VBFchargedF2vspu", tags_full, ev.ngenITpu, phys.jets[1].chHadFrac, 1);
            }
         }

         //VBF control
         if(true                                                                                        )mon.fillHisto("VBFNEventsInc"       ,tags_full,    0                ,iweight);
         if(VBFPass2Jet30                                                                                  )mon.fillHisto("VBFNEventsInc"       ,tags_full,    1                ,iweight);
         if(VBFPassdEtaCut                                                                                 )mon.fillHisto("VBFNEventsInc"       ,tags_full,    2                ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut                                                                    )mon.fillHisto("VBFNEventsInc"       ,tags_full,    3                ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn                                                    )mon.fillHisto("VBFNEventsInc"       ,tags_full,    4                ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto                                     )mon.fillHisto("VBFNEventsInc"       ,tags_full,    5                ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto                     )mon.fillHisto("VBFNEventsInc"       ,tags_full,    6                ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto && pass3dLeptonVeto )mon.fillHisto("VBFNEventsInc"       ,tags_full,    7                ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto && pass3dLeptonVeto && VBFPassCentralDilep)mon.fillHisto("VBFNEventsInc"       ,tags_full,    8                ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto && pass3dLeptonVeto && VBFPassCentralDilep && VBFNJetsAssoc>=1 )mon.fillHisto("VBFNEventsInc"       ,tags_full,    9                ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto && pass3dLeptonVeto && VBFPassCentralDilep && VBFNJetsAssoc>=2 )mon.fillHisto("VBFNEventsInc"       ,tags_full,    10                ,iweight);


         if(VBFPass2Jet30                                                                                  )mon.fillHisto("VBFdEtaInc"          ,tags_full,    fabs(VBFdEta)    ,iweight);
         if(VBFPassdEtaCut                                                                                 )mon.fillHisto("VBFiMassInc"         ,tags_full,    VBFSyst.M()      ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut                                                                    )mon.fillHisto("VBFcenLeptonVetoInc" ,tags_full,    VBFCentralLeptons,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn                                                    )mon.fillHisto("VBFcen30JetVetoInc"  ,tags_full,    VBFCentral30Jets ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto                                     )mon.fillHisto("VBFNBJets30Inc"      ,tags_full,    VBFNBJets        ,iweight);
       
         if(               VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto                     )mon.fillHisto("VBFdEtaNM1C"         ,tags_full,    fabs(VBFdEta)    ,iweight);
         if(VBFPassdEtaCut              && VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto                     )mon.fillHisto("VBFiMassNM1C"        ,tags_full,    VBFSyst.M()      ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut                 && VBFPassJetVeto && VBFPassBJetVeto                     )mon.fillHisto("VBFcenLeptonVetoNM1C",tags_full,    VBFCentralLeptons,iweight);
         if(VBFPass2Jet30                                              && VBFPassBJetVeto && !pass3dLeptonVeto){
             mon.fillHisto("VBFcen30JetVeto3rdlepton"  ,tags_full,    VBFCentral30Jets ,iweight);
             mon.fillHisto("VBFNBJets303rdlepton"      ,tags_full,    VBFNBJets        ,iweight);
             mon.fillHisto("VBFdEta3rdlepton"          ,tags_full,    fabs(VBFdEta)    ,iweight);
             mon.fillHisto("VBFiMass3rdlepton"         ,tags_full,    VBFSyst.M()      ,iweight);
         }
      
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn                && VBFPassBJetVeto )mon.fillHisto("VBFcen30JetVetoNM1C" ,tags_full,    VBFCentral30Jets ,iweight);
         if(VBFPassdEtaCut && VBFPassiMCut && VBFPassLeptonIn && VBFPassJetVeto                 )mon.fillHisto("VBFNBJets30NM1C"     ,tags_full,    VBFNBJets        ,iweight);
         if(                            VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto )mon.fill2DHisto("VBFdEtaiMassNM1C"  ,tags_full,    fabs(VBFdEta), VBFSyst.M(), iweight);
         if(met>50 &&                VBFPassLeptonIn && VBFPassJetVeto && VBFPassBJetVeto )mon.fill2DHisto("VBFdEtaiMassNM1C2" ,tags_full,    fabs(VBFdEta), VBFSyst.M(), iweight);


         mon.fillHisto("VBFtotalvspu"      ,tags_full,    ev.ngenITpu   ,1);
         if(fabs(VBFMinEta)<2.4){
            mon.fillHisto("VBFtotalvspu_cen"      ,tags_full,    ev.ngenITpu   ,1);
         }else{
            mon.fillHisto("VBFtotalvspu_fwd"      ,tags_full,    ev.ngenITpu   ,1);
         }

	 if(VBFPass2Jet30 && VBFPassdEtaCut && VBFPassiMCut){
            mon.fillHisto("VBFtagvspu_noveto"      ,tags_full,    ev.ngenITpu   ,1);

            if(fabs(VBFMinEta)<2.4){
               mon.fillHisto("VBFtagvspu_noveto_cen"      ,tags_full,    ev.ngenITpu   ,1);
            }else{
               mon.fillHisto("VBFtagvspu_noveto_fwd"      ,tags_full,    ev.ngenITpu   ,1);
            } 
         }

         if(isVBF){
            mon.fillHisto("VBFMinEta"  ,tags_full,    VBFMinEta ,iweight);
            mon.fillHisto("VBFMaxEta"  ,tags_full,    VBFMaxEta ,iweight);

            mon.fill2DHisto("VBFMinEtavspu"  ,tags_full, ev.ngenITpu,   VBFMinEta ,1);
            mon.fill2DHisto("VBFMaxEtavspu"  ,tags_full, ev.ngenITpu,   VBFMaxEta ,1);

            if(phys.jets.size()>2){
               mon.fill2DHisto("VBFThirdJetPtvspu"  ,tags_full, ev.ngenITpu, phys.jets[2].pt()   ,1);
               if(phys.jets[2].pt()>20){
               mon.fill2DHisto("VBFThirdJetEtavspu"  ,tags_full, ev.ngenITpu, fabs(phys.jets[2].eta())   ,1);
               }
            }

            mon.fillHisto("VBFtagvspu"      ,tags_full,    ev.ngenITpu   ,1); 
            if(VBFNJetsAssoc>=1)mon.fillHisto("VBFtagtk1vspu"      ,tags_full,    ev.ngenITpu   ,1);
            if(VBFNJetsAssoc>=2)mon.fillHisto("VBFtagtk2vspu"      ,tags_full,    ev.ngenITpu   ,1);

            if(fabs(VBFMinEta)<2.4){
               mon.fillHisto("VBFtagvspu_cen"      ,tags_full,    ev.ngenITpu   ,1);
               if(VBFNJetsAssoc>=1)mon.fillHisto("VBFtagtk1vspu_cen"      ,tags_full,    ev.ngenITpu   ,1);
               if(VBFNJetsAssoc>=2)mon.fillHisto("VBFtagtk2vspu_cen"      ,tags_full,    ev.ngenITpu   ,1);
            }else{
               mon.fillHisto("VBFtagvspu_fwd"      ,tags_full,    ev.ngenITpu   ,1);
               if(VBFNJetsAssoc>=1)mon.fillHisto("VBFtagtk1vspu_fwd"      ,tags_full,    ev.ngenITpu   ,1);
               if(VBFNJetsAssoc>=2)mon.fillHisto("VBFtagtk2vspu_fwd"      ,tags_full,    ev.ngenITpu   ,1);
            }

            if(phys.jets.size()<=2 || phys.jets[2].pt()<=20)mon.fillHisto("VBFtag3rd20vspu"      ,tags_full,    ev.ngenITpu   ,1);
            if(phys.jets.size()<=2 || phys.jets[2].pt()<=25)mon.fillHisto("VBFtag3rd25vspu"      ,tags_full,    ev.ngenITpu   ,1);
         }


         if(VBFPass2Jet30 && VBFPassiMCut)mon.fill2DHisto("VBFdEtavspu", tags_full, ev.ngenITpu, VBFdEta, 1);
         if(VBFPass2Jet30 && VBFPassiMCut && VBFNJetsAssoc>=1)mon.fill2DHisto("VBFdEtatkvspu", tags_full, ev.ngenITpu, VBFdEta, 1);

         if(VBFPass2Jet30 && VBFPassdEtaCut && VBFPassiMCut && VBFPassBJetVeto){
            mon.fill2DHisto("VBFNCenJet15vspu"  , tags_full, ev.ngenITpu, VBFCentral15Jets, 1);
            mon.fill2DHisto("VBFNCenJet15vsputk", tags_full, ev.ngenITpu, VBFCentral15Jetstk, 1);
            mon.fill2DHisto("VBFNCenJet30vspu"  , tags_full, ev.ngenITpu, VBFCentral30Jets, 1);
            mon.fill2DHisto("VBFNCenJet30vsputk", tags_full, ev.ngenITpu, VBFCentral30Jetstk, 1);
         }

      }


      //##############################################   EVENT LOOP ENDS   ##############################################
  }printf("\n"); 
  file->Close();
 

  //##############################################
  //########     SAVING HISTO TO FILE     ########
  //##############################################

 
  //save control plots to file
  outUrl += "/";
  outUrl += gSystem->BaseName(url);
  printf("Results save in %s\n", outUrl.Data());

  //save all to the file
  TFile *ofile=TFile::Open(outUrl, "recreate");
  mon.Write();
  ofile->Close();

  if(outTxtFile)fclose(outTxtFile);
}  





