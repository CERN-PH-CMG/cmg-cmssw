#include <iostream>
#include <boost/shared_ptr.hpp>

#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"
#include "CMGTools/HtoZZ2l2nu/interface/GammaEventHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/METUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"
#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/SmartSelectionMonitor.h"
#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"
#include "CMGTools/HtoZZ2l2nu/interface/MacroUtils.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

#include "Math/GenVector/Boost.h"

#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TRandom2.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

using namespace std;

LorentzVector min(const LorentzVector& a, const LorentzVector& b)
{
  if(a.pt()<=b.pt())return a;
  return b;
}

//
int main(int argc, char* argv[])
{
  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();
  
  //check arguments
  if ( argc < 2 ) 
    {
      std::cout << "Usage : " << argv[0] << " parameters_cfg.py" << std::endl;
      return 0;
    }
  
  //get configuration
  const edm::ParameterSet &runProcess = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("runProcess");
  TString url=runProcess.getParameter<std::string>("input");
  TString outdir=runProcess.getParameter<std::string>("outdir");
  bool isMC = runProcess.getParameter<bool>("isMC");
  int mctruthmode = runProcess.getParameter<int>("mctruthmode");
  TString dirname = runProcess.getParameter<std::string>("dirName");
  TString uncFile =  runProcess.getParameter<std::string>("jesUncFileName"); gSystem->ExpandPathName(uncFile);
  JetCorrectionUncertainty jecUnc(uncFile.Data());

  TRandom2 rndGen;
  EventCategory eventClassifComp;
  GammaEventHandler gammaEvHandler(runProcess);  

  //book histograms
  SmartSelectionMonitor mon;
  mon.addHistogram( new TH1F ("njets",     ";Jet multiplicity (p_{T}>30 GeV/c);Events",5,0,5) );
  mon.addHistogram( new TH1F ("nsoftjets", ";Jet multiplicity (p_{T}>15 GeV/c);Events",5,0,5) );
  mon.addHistogram( new TH1F ("nbtags",    ";b-tag multiplicity; Events", 5,0,5) );
  for(size_t ibin=1; ibin<=5; ibin++){
    TString label("");
    if(ibin==5) label +="#geq";
    else        label +="=";
    label += (ibin-1);
    mon.getHisto("njets")->GetXaxis()->SetBinLabel(ibin,label);
    mon.getHisto("nbtags")->GetXaxis()->SetBinLabel(ibin,label);
  }
  
  TString subCats[]={"eq0softjets","eq0hardjets","eq0jets","eq1jets","eq2jets","geq3jets",""};
  const size_t nSubCats=sizeof(subCats)/sizeof(TString);
  for(size_t isc=0; isc<nSubCats; isc++)
    {
      TString postfix(subCats[isc]);
      mon.addHistogram( new TH1F ("r9_"+postfix, ";R9;Events", 100,0.8,1) );
      mon.addHistogram( new TH1F ("sietaieta_"+postfix, ";#sigma i#eta i#eta;Events", 100,0,0.03) );
      mon.addHistogram( new TH1F( "mindphijmet_"+postfix, ";min #Delta#phi(jet,E_{T}^{miss});Events",40,0,4) );
      mon.addHistogram( new TH1F ("trkveto_"+postfix, ";Track veto;Events", 2,0,2) );
     
      mon.addHistogram( new TH1D( "qt_"+postfix,        ";p_{T}^{#gamma} [GeV/c];Events / (2.5 GeV/c)", 100,0,500) );
      mon.addHistogram( new TH1D( "metoverqt_"+postfix, ";E_{T}^{miss}/p_{T}^{#gamma} [GeV/c];Events", 100,0,3) );
      mon.addHistogram( new TH1D( "eta_"+postfix,       ";#eta;Events", 50,0,2.6) );  
      mon.addHistogram( new TH1F( "nvtx_"+postfix, ";Vertices;Events", 30,0,30) );  
      mon.addHistogram( new TH1F( "mt_"+postfix  , ";M_{T};Events", 100,0,1000) );
      mon.addHistogram( new TH1F( "met_rawmet_"+postfix  , ";E_{T}^{miss} (raw);Events", 50,0,500) );
      mon.addHistogram( new TH1F( "met_met_"+postfix  , ";E_{T}^{miss};Events", 50,0,500) );
      mon.addHistogram( new TH1F( "met_rawRedMet_"+postfix  , ";red(E_{T}^{miss},clustered-E_{T}^{miss}) (raw);Events", 50,0,500) );
      mon.addHistogram( new TH1F( "met_min3Met_"+postfix  , ";min(E_{T}^{miss},assoc-E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,500) );
      mon.addHistogram( new TH1F( "met_redMet_"+postfix  , ";red(E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,500) );
      mon.addHistogram( new TH1F( "met_redMetL_"+postfix  , ";red(E_{T}^{miss},clustered-E_{T}^{miss}) - longi.;Events", 50,-250,250) );
      mon.addHistogram( new TH1F( "met_redMetT_"+postfix  , ";red(E_{T}^{miss},clustered-E_{T}^{miss}) - perp.;Events", 50,-250,250) );
      mon.addHistogram( new TH1F( "zmass_"+postfix, ";M^{ll};Events", 15,76,106) );
      mon.addHistogram( new TH2F( TString("met_met_") + postfix + "_vspu"       , ";Vertices;E_{T}^{miss};Events", 50,0,50,50,0,500) );
      mon.addHistogram( new TH2F( TString("met_min3Met_") + postfix + "_vspu"       , ";Vertices;min(E_{T}^{miss},assoc-E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,50,50,0,500) );
      mon.addHistogram( new TH2F( TString("met_redMet_") + postfix + "_vspu"       , ";Vertices;red(E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,50,50,0,500) );
    }
  TH1F* Hcutflow     = (TH1F*) mon.addHistogram(  new TH1F ("cutflow"    , "cutflow"    ,3,0,3) ) ;

  //open the file and get events tree
  TFile *file = TFile::Open(url);
  if(file==0) return -1;
  if(file->IsZombie()) return -1;
  ZZ2l2nuSummaryHandler evSummaryHandler;
  if( !evSummaryHandler.attachToTree( (TTree *)file->Get(dirname) ) ) 
    {
      file->Close();
      return -1;
    }
  float cnorm=1.0;
  if(isMC)
    {
      TH1F* cutflowH = (TH1F *) file->Get("evAnalyzer/h2zz/cutflow");
      if(cutflowH) cnorm=cutflowH->GetBinContent(1);
      printf("cnorm = %f\n",cnorm);
    }
  Hcutflow->SetBinContent(1,cnorm);

  DuplicatesChecker duplicatesChecker;
  
  //pileup reweighting
  std::vector<double> dataPileupDistributionDouble = runProcess.getParameter< std::vector<double> >("datapileup");
  std::vector<float> dataPileupDistribution; 
  for(unsigned int i=0;i<dataPileupDistributionDouble.size();i++){dataPileupDistribution.push_back(dataPileupDistributionDouble[i]);}
  std::vector<float> mcPileupDistribution;
  if(isMC)
    {
      TH1F* histo = (TH1F *) file->Get("evAnalyzer/h2zz/pileup");
      if(!histo)std::cout<<"pileup histogram is null!!!\n";
      for(int i=1;i<=histo->GetNbinsX();i++){mcPileupDistribution.push_back(histo->GetBinContent(i));}
      delete histo;
    }
  else mcPileupDistribution=dataPileupDistribution;
  while(mcPileupDistribution.size()<dataPileupDistribution.size())  mcPileupDistribution.push_back(0.0);
  while(mcPileupDistribution.size()>dataPileupDistribution.size())  dataPileupDistribution.push_back(0.0);
  gROOT->cd();  //THIS LINE IS NEEDED TO MAKE SURE THAT HISTOGRAM INTERNALLY PRODUCED IN LumiReWeighting ARE NOT DESTROYED WHEN CLOSING THE FILE
  edm::LumiReWeighting LumiWeights(mcPileupDistribution,dataPileupDistribution);
   
  //run the analysis
  std::set<int> trigList;
  int evStart(0),evEnd(evSummaryHandler.getEntries());
  for( int iev=0; iev<evEnd; iev++)
    {
      if(iev%1000==0) printf("\r [ %d/100 ] ",int(100*float(iev-evStart)/float(evEnd)));
      evSummaryHandler.getEntry(iev);
      
      //interpret event
      ZZ2l2nuSummary_t &ev = evSummaryHandler.getEvent();
      PhysicsEvent_t phys  = getPhysicsEventFrom(ev);
      bool isGammaEvent    = gammaEvHandler.isGood(phys);

      if(duplicatesChecker.isDuplicate(ev.run,ev.lumi, ev.event))
	{
	  printf("event %i-%i-%i is duplicated\n", ev.run, ev.lumi, ev.event);
	  //NumberOfDuplicated++;
	  continue;
	}

      //check which event type is required to use (dilepton/or photon)
      if(mctruthmode==22 && !isGammaEvent) continue;
      if(mctruthmode==1  && isGammaEvent) continue;
      if(!isGammaEvent && ev.cat != EE && ev.cat !=MUMU) continue;

      //event categories
      std::vector<TString> dilCats;
      LorentzVector gamma(0,0,0,0);
      int triggerThr(0);
      float r9(0),sietaieta(0);
      bool hasTrkVeto(false),isConv(false);
      if(isGammaEvent)
	{
	  dilCats.push_back("ee");
	  dilCats.push_back("mumu");
	  dilCats.push_back("ll");
	  r9               = phys.gammas[0].r9*(isMC ? 1.005 : 1.0);
	  sietaieta        = phys.gammas[0].sihih;
	  hasTrkVeto       = phys.gammas[0].hasCtfTrkVeto;
	  gamma            = phys.gammas[0];
	  triggerThr       = gammaEvHandler.triggerThr();
	  isConv=(phys.gammas[0].isConv);
	}
      else
	{
	  gamma=phys.leptons[0]+phys.leptons[1];
	  if(fabs(gamma.mass()-91)>15) continue;
	  if(ev.cat==MUMU) dilCats.push_back("mumu");
	  if(ev.cat==EE)   dilCats.push_back("ee");
	  dilCats.push_back("ll");
	  triggerThr=gammaEvHandler.findTriggerCategoryFor( gamma.pt() );
	  if(triggerThr<0) continue;
	}
      
      //jet/MET
      LorentzVector metP4(phys.met[0]);
      int nbtags(0),njets30(0),njets15(0);
      double mindphijmet(9999.);
      std::vector<LorentzVector> jetsP4;
      std::vector<double> genJetsPt;
      LorentzVector rawClusteredMet(gamma); 
      rawClusteredMet *= -1;
      for(size_t ijet=0; ijet<phys.jets.size(); ijet++)
        {
	  LorentzVector ijetP4=phys.jets[ijet];
	  if(ijetP4.pt()<15 || fabs(ijetP4.eta())>5) continue;
	  if(isGammaEvent && deltaR(ijetP4,gamma)<0.4) continue;
	  njets15++;
	  jetsP4.push_back(ijetP4);
	  genJetsPt.push_back(phys.jets[ijet].genPt);
	  rawClusteredMet -= ijetP4;
	  double idphijmet=fabs(deltaPhi(metP4.phi(),ijetP4.phi()));
	  mindphijmet=min(idphijmet,mindphijmet);
	  if(ijetP4.pt()>30)
	    {
	      if(fabs(ijetP4.eta())<2.5) nbtags += (phys.jets[ijet].btag1>2.0);
	      njets30++;
	    }
	}
      TString subcat("eq"); subcat+=njets30; subcat+= "jets";
      if(njets30>=3) subcat="geq3jets";


      //
      LorentzVector nullP4(0,0,0,0);
      LorentzVector rawZvv(phys.met[0]);
      LorentzVector assocMetP4        = phys.met[1];
      LorentzVector rawRedMet(METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, gamma, 0, nullP4, 0, rawClusteredMet, rawZvv,true));
      LorentzVectorCollection zvvs,redMets,min3Mets;
      std::vector<Float_t>  redMetLs,redMetTs;
      std::vector<LorentzVectorCollection> jets;
      METUtils::computeVariation(jetsP4, genJetsPt, rawZvv, jets, zvvs, &jecUnc);
      for(size_t ivars=0; ivars<zvvs.size(); ivars++)
	{
          LorentzVector clusteredMetP4(gamma); clusteredMetP4 *= -1;
          for(size_t ijet=0; ijet<jets[ivars].size(); ijet++) clusteredMetP4 -= jets[ivars][ijet];
	  METUtils::stRedMET redMetOut;
	  redMets.push_back( METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, gamma, 0, nullP4, 0, clusteredMetP4, zvvs[ivars],true,&redMetOut) );
          redMetLs.push_back( redMetOut.redMET_l );
          redMetTs.push_back( redMetOut.redMET_t );
	  min3Mets.push_back( min(zvvs[ivars], min(assocMetP4,clusteredMetP4)) );
	}

      //event weight
      float weight = 1.0;
      if(isMC)                  { weight = LumiWeights.weight( ev.ngenITpu ); }
      //if(!isMC && isGammaEvent) { weight = gammaEvHandler.getTriggerPrescaleWeight(); }
      Hcutflow->Fill(1,1);
      Hcutflow->Fill(2,weight);
      
      //select event
      bool passMultiplicityVetoes (isGammaEvent ? (phys.leptons.size()==0 && ev.ln==0 && phys.gammas.size()==1) : (ev.ln==0) );
      bool passKinematics         (gamma.pt()>55);
      bool passEB                 (!isGammaEvent || fabs(gamma.eta())<1.4442);
      bool passR9                 (!isGammaEvent || r9<1.0);
      bool passBveto              (nbtags==0);
      bool passMinDphiJmet        (mindphijmet>0.5);
      
      //control plots
      std::map<TString, float> qtWeights;
      if(isGammaEvent) qtWeights = gammaEvHandler.getWeights(phys,subcat);
      for(size_t idc=0; idc<dilCats.size(); idc++)
	{
	  LorentzVector iboson(isGammaEvent ? gammaEvHandler.massiveGamma(dilCats[idc]) : gamma);
	  float zmass=iboson.mass();
	  //Float_t rawMt( METUtils::transverseMass(iboson,metP4,true) );
	  Float_t mt( METUtils::transverseMass(iboson,zvvs[0],true) );

	  TString ctf=dilCats[idc];
	  float iweight=weight;
	  if(isGammaEvent) iweight*=qtWeights[dilCats[idc]];
	  if(!passMultiplicityVetoes) continue;
	  if(!passEB || !passR9 || isConv) continue;
	  if(!passKinematics) continue;

	  mon.fillHisto("eta_"+subcat,ctf, fabs(gamma.eta()),iweight);
	  mon.fillHisto("qt_"+subcat,ctf, gamma.pt(),iweight);
	  mon.fillHisto("nvtx_"+subcat,ctf, ev.nvtx,iweight);
	  mon.fillHisto("zmass_"+subcat,ctf, zmass,iweight);

	  mon.fillHisto("nbtags",ctf, nbtags,iweight);
	  if(!passBveto) continue;
	  
	  mon.fillHisto("njets",ctf, njets30,iweight);
	  mon.fillHisto("nsoftjets",ctf, njets15,iweight);

	  std::vector<TString> subCatsToFill;
	  subCatsToFill.push_back(subcat);
	  subCatsToFill.push_back("");
	  if(njets30==0) subCatsToFill.push_back(njets15==0 ? "eq0softjets" : "eq0hardjets");
	  for(size_t isc=0; isc<subCatsToFill.size(); isc++)
	    {	 
	      if(isGammaEvent)  
		{
		  mon.fillHisto("r9_"+subCatsToFill[isc],ctf, r9,iweight);
		  mon.fillHisto("sietaieta_"+subCatsToFill[isc],ctf, sietaieta,iweight);
		  mon.fillHisto("trkveto_"+subCatsToFill[isc],ctf, hasTrkVeto,iweight);
		}

	      if(hasTrkVeto) continue;
	      if(zvvs[0].pt()>70) mon.fillHisto("mindphijmet_"+subCatsToFill[isc],ctf, mindphijmet,iweight);	      
	      if(passMinDphiJmet) continue;
	      mon.fillHisto("met_rawmet_"+subCatsToFill[isc],ctf, rawZvv.pt(),iweight);
	      mon.fillHisto("metoverqt_"+subCatsToFill[isc],ctf, zvvs[0].pt()/gamma.pt(),iweight);
	      mon.fillHisto("met_met_"+subCatsToFill[isc],ctf, zvvs[0].pt(),iweight);
	      mon.fillHisto("met_met_"+subCatsToFill[isc]+"_vspu",ctf, ev.nvtx,zvvs[0].pt(),iweight);
	      mon.fillHisto("met_min3Met_"+subCatsToFill[isc],ctf, min3Mets[0].pt(),iweight);
	      mon.fillHisto("met_min3Met_"+subCatsToFill[isc]+"_vspu",ctf, ev.nvtx,min3Mets[0].pt(),iweight);
	      mon.fillHisto("met_rawRedMet_"+subCatsToFill[isc],ctf, rawRedMet.pt(),iweight);
	      mon.fillHisto("met_redMet_"+subCatsToFill[isc],ctf, redMets[0].pt(),iweight);
	      mon.fillHisto("met_redMet_"+subCatsToFill[isc]+"_vspu",ctf, ev.nvtx,redMets[0].pt(),iweight);
	      mon.fillHisto("met_redMetT_"+subCatsToFill[isc],ctf, redMetLs[0],iweight);
	      mon.fillHisto("met_redMetL_"+subCatsToFill[isc],ctf, redMetTs[0],iweight);
	      mon.fillHisto("mt_"+subCatsToFill[isc],ctf,mt,iweight);
	    }
	  
	  // 	  if(njets30<1 && metP4.pt()>100)
	  // 	    cout << isGammaEvent << " " << passMultiplicityVetoes << " " << mctruthmode << " "
	  // 		 << " g=(" << gamma.pt() << "," << gamma.eta() << ") "
	  // 		 << " MET=" << metP4.pt() << " Ng=" << ev.gn << " Nj=" << ev.jn 
	  // 		 << " Nl=" << ev.ln << " Nl=" << phys.leptons.size()
	  // 	        // << " l1=" << phys.leptons[0].pt() << " l2=" << phys.leptons[1].pt() 
	  // 		 << endl;
	}
    }

  //all done with the events file
  file->Close();
  
  for(std::set<int>::iterator it = trigList.begin();
      it != trigList.end();
      it++)
    cout << *it << " ";
  cout << endl;
  cout << "Sample treated as MC? " << isMC << endl;

  //save histograms to file
  TString outUrl( outdir );
  gSystem->Exec("mkdir -p " + outUrl);
  outUrl += "/";
  outUrl += gSystem->BaseName(url);
  TFile *ofile=TFile::Open(outUrl, "recreate");
  mon.Write();
  ofile->Close();

}  


