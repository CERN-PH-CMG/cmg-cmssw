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
  
  TRandom2 rndGen;
  EventCategory eventClassifComp;
  TString categories[]={"eq0softjets","eq0jets","eq1jets","geq2jets","vbf"};
  GammaEventHandler gammaEvHandler(runProcess);  

  //book histograms
  SmartSelectionMonitor controlHistos;
  controlHistos.addHistogram( new TH1D ("qt", ";p_{T}^{#gamma} [GeV/c];Events / (2.5 GeV/c)", 200,0,600) );
  controlHistos.addHistogram( new TH1D ("eta", ";#eta;Events", 50,0,2.6) );  
  controlHistos.addHistogram( new TH1F ("hoe", ";H/E;Events", 100,0.,0.1) );
  controlHistos.addHistogram( new TH1F ("r9", ";R9;Events", 100,0.8,1) );
  controlHistos.addHistogram( new TH1F ("sietaieta", ";#sigma i#eta i#eta;Events", 100,0,0.03) );
  controlHistos.addHistogram( new TH1F ("nvtx", ";Vertices;Events", 30,0,30) );
  controlHistos.addHistogram( new TH1F ("trkveto", ";Track veto;Events", 3,0,3) );


  std::map<TString,LorentzVector> metTypeValues;
  std::map<TString,TString> metTypes;
  metTypes["met"                 ]="E_{T}^{miss}";
  metTypes["metSmear"            ]="E_{T}^{miss} - after smear";
  metTypes["redClusteredMet"     ]="red(E_{T}^{miss},clustered-E_{T}^{miss})";
  metTypes["redClusteredMetSmear"]="red(E_{T}^{miss},clustered-E_{T}^{miss}) - after smear";
  
  //  std::vector<TString> photonSubcats;
  // std::vector<TString> dilCats;
  //dilCats.push_back("ee");
  // dilCats.push_back("mumu");
  //  for(size_t icat=0; icat<gammaEvHandler.nCategories(); icat++)
  //  {
  //  for(size_t idilcat=0; idilcat<dilCats.size(); idilcat++)
  //  {
  //  int triggerThr=gammaEvHandler.category(icat);
  //TString subcat=dilCats[idilcat];
  //	  if(triggerThr==0) 
  //{
  
  //  controlHistos.addHistogram( new TH2D (subcat+"qtvseta", ";p_{T}^{#gamma} [GeV/c];#eta;Events / (2.5 GeV/c)", 200,0,600,50,0,2.6) );
  // controlHistos.addHistogram( new TH2D (subcat+"qtvsnvtx", ";p_{T}^{#gamma} [GeV/c];Vertices;Events", 200,0,600,30,0,30) );
  //    controlHistos.addHistogram( new TH1D (subcat+"zmass",";M^{ll};Events", 100,91-31,91+31) );
  //   controlHistos.addHistogram( new TH1D (subcat+"finaleventflowmet",";Category;Event count;",8,0,8) );
  //   controlHistos.addHistogram( new TH1D (subcat+"finaleventflowrmet",";Category;Event count;",8,0,8) );
  //    }
  //else
  //{
  //subcat ="photon"; subcat+=triggerThr; subcat += dilCats[idilcat];
  //}
  //  photonSubcats.push_back(subcat);
    
  //   controlHistos.addHistogram( new TH1D (subcat+"ptjets", ";p_{T}^{jet} [GeV/c];Jets", 50,0,250) );
  //       controlHistos.addHistogram( new TH1D (subcat+"ptclosejet", ";p_{T}(closest jet) [GeV/c];Events", 50,0,250) );
  //       controlHistos.addHistogram( new TH1D (subcat+"ht", ";H_{T} [GeV];Events", 100,0,500) );
  //       controlHistos.addHistogram( new TH1D (subcat+"htSmear", ";H_{T} [GeV] - after smear;Events", 100,0,500) );
  //       controlHistos.addHistogram( new TH1D (subcat+"mt", ";M_{T} [GeV];Events", 100,0,1000) );
  //       controlHistos.addHistogram( new TH1F (subcat+"njets", ";Jets;Events", 6,0,6) );
  //       controlHistos.addHistogram( new TH1F (subcat+"nbtags", ";b-tag multiplcity;Events", 5,0,5) );
  //       controlHistos.addHistogram( new TH1F (subcat+"mindphijmet", ";min #Delta#phi(jet,E_{T}^{miss});Events", 14,0,3.5) );
  //       controlHistos.addHistogram( new TH1F (subcat+"mindphijmet50", ";min #Delta#phi(jet,E_{T}^{miss});Events", 14,0,3.5) );
  //       controlHistos.addHistogram( new TH1F (subcat+"mindphijmet70", ";min #Delta#phi(jet,E_{T}^{miss});Events", 14,0,3.5) );
  //       controlHistos.addHistogram( new TH1F (subcat+"dphivmet", ";#Delta#phi(boson,E_{T}^{miss});Events", 14,0,3.5) );
  //       double metAxis[]={0,10,20,30,40,50,60,70,80,90,100,120,140,160,180,200,250,300,400,500};
  //       int nMetBins=sizeof(metAxis)/sizeof(double)-1;
  //       for(std::map<TString,TString>::iterator it = metTypes.begin(); it!= metTypes.end(); it++)
  // 	{
  // 	  metTypeValues[it->first]=LorentzVector(0,0,0,0);
  // 	  controlHistos.addHistogram( new TH1F( subcat+TString("met_") + it->first, ";"+it->second+";Events", nMetBins,metAxis) );
  // 	  controlHistos.addHistogram( new TH1F( subcat+TString("met_") + it->first+TString("L"), ";"+it->second+"_{L}/q_{T};Events", 50,0,2) );
  // 	  controlHistos.addHistogram( new TH2F( subcat+TString("met_") + it->first+"vsht", ";H_{T};"+it->second+";Events", 25,0,250,25,0,250) );
  // 	}
  //  }
  TH1F* Hcutflow     = (TH1F*) controlHistos.addHistogram(  new TH1F ("cutflow"    , "cutflow"    ,3,0,3) ) ;

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
  if(isMC)
    {
      TH1F *cutflowH = (TH1F *) file->Get("evAnalyzer/h2zz/cutflow");
      Hcutflow->SetBinContent(1,cutflowH ? cutflowH->GetBinContent(1):1.0);
    }
  
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
  while(mcPileupDistribution.size()>dataPileupDistribution.size())dataPileupDistribution.push_back(0.0);
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
      int eventCategory    = eventClassifComp.Get(phys);
      TString subcat       = eventClassifComp.GetLabel(eventCategory);
      bool isGammaEvent    = gammaEvHandler.isGood(phys);

      //check which event type is required to use (dilepton/or photon)
      if(mctruthmode==22 && !isGammaEvent) continue;
      if(mctruthmode==1  && isGammaEvent) continue;
      if(!isGammaEvent && ev.cat != EE && ev.cat !=MUMU) continue;
      float weight = 1.0;
      if(isMC && !isGammaEvent)	{ weight = LumiWeights.weight( ev.ngenITpu ); }

      //event categories
      std::vector<TString> dilCats;
      LorentzVector gamma(0,0,0,0);
      int triggerThr(0);
      float r9(0),sietaieta(0),hoe(0);
      bool hasCtfTrkVeto(false), hasGsfTrkVeto(false), hasElectronVeto(false);
      if(isGammaEvent)
	{
	  dilCats.push_back("ee");
	  dilCats.push_back("mumu");
	  dilCats.push_back("ll");
	  r9         = phys.gammas[0].r9*(isMC ? 1.005 : 1.0); 
	  sietaieta  = phys.gammas[0].sihih;
	  hoe        = phys.gammas[0].hoe;
	  hasCtfTrkVeto    = phys.gammas[0].hasCtfTrkVeto;
	  hasGsfTrkVeto    = phys.gammas[0].hasGsfTrkVeto;
	  hasElectronVeto    = phys.gammas[0].hasElectronVeto;
	  gamma      = gammaEvHandler.massiveGamma("ll");
	  triggerThr = gammaEvHandler.triggerThr();
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

      //minimum threshold
      cout << gamma.pt() << " " << fabs(gamma.eta()) << " " << ev.ln << endl;
      if(gamma.pt()<55) continue;
      //if(gamma.pt()<55 || fabs(gamma.eta())>1.4442) continue;

      //lepton veto
      if(ev.ln>0) continue;
      
      cout << gamma.pt() << triggerThr << " "  << " " << r9 << " " << hoe <<" " << hasCtfTrkVeto << " " << hasGsfTrkVeto <<" " << hasElectronVeto << endl;
      /*

      //select jets
      int njets=eventClassifComp.GetVBFJetCount();
      int nincjets(0), nbjets(0);
      std::vector<LorentzVector> jetsp4,smearjetsp4;
      LorentzVector jetSmearDiff(0,0,0,0);
      float ht(0.),htsmear(0.);
      for(size_t ijet=0; ijet<phys.jets.size(); ijet++)
	{
	  double dr=deltaR(phys.jets[ijet],gamma);
	  if(dr<0.1) continue;

	  bool passTCHEL(phys.jets[ijet].btag1>2);

	  //jet counting
	  nincjets   += (phys.jets[ijet].pt()>15 && fabs(phys.jets[ijet].eta())<5.0);
	  nbjets     += passTCHEL*(phys.jets[ijet].pt()>30 && fabs(phys.jets[ijet].eta())<2.5);
	  
	  //kinematics
	  LorentzVector ijetP4(phys.jets[ijet]);                                 	  ht += ijetP4.pt();
	  LorentzVector ismearJetP4( isMC ? METUtils::smearedJet(ijetP4) : ijetP4 );      htsmear += ismearJetP4.pt();
	  jetSmearDiff+=(ismearJetP4-ijetP4);
	  jetsp4.push_back( ijetP4  );
	  smearjetsp4.push_back( ismearJetP4 );
	}


      //met variables
      LorentzVector metP4=phys.met[0];
      LorentzVector assocChargedMetP4=phys.met[1];
      LorentzVector assocMetP4=phys.met[3];
      LorentzVector clusteredMetP4 = -1*gamma;       for(unsigned int i=0;i<jetsp4.size();i++)      { clusteredMetP4 -= jetsp4[i]; }
      LorentzVector smearClusteredMetP4 = -1*gamma;  for(unsigned int i=0;i<smearjetsp4.size();i++) { smearClusteredMetP4 -= smearjetsp4[i]; }
      if(isGammaEvent)
	{
	  assocChargedMetP4 -= gamma;
	  if(!phys.gammas[0].isConv) assocMetP4 -= gamma;
	}
      METUtils::stRedMET redMetInfo;
      LorentzVector nullP4   = LorentzVector(0,0,0,0);
      LorentzVector lep1     = gamma;
      LorentzVector lep2     = nullP4;
      LorentzVector rCMetP4  = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, clusteredMetP4      , metP4                  , true);
      LorentzVector rSCMetP4 = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, smearClusteredMetP4 , metP4 -jetSmearDiff    , true);
      metTypeValues["met"]                 = metP4;
      metTypeValues["metSmear"]            = metP4 - jetSmearDiff;
      metTypeValues["assocChargedMet"]     = assocChargedMetP4;
      metTypeValues["assocMet"]            = assocMetP4;
      metTypeValues["redClusteredMet"]     = rCMetP4;
      metTypeValues["redClusteredMetSmear"]= rSCMetP4;
      
      
      //more kinematics
      double dphivmet(deltaPhi(metP4.phi(),gamma.phi()));

      double mindphijmet(9999.),ptclosejet(0.),etaclosejet(0.);
      LorentzVector leadJet(0,0,0,0), jetSyst;
      for(size_t ijet=0; ijet<jetsp4.size(); ijet++)
        {
	  if(jetsp4[ijet].pt()>leadJet.pt()) leadJet=jetsp4[ijet];
	  jetSyst += jetsp4[ijet];
	  
	  if(jetsp4[ijet].pt()<15 || fabs(jetsp4[ijet].eta())>5)continue;
	  double dphijmet=fabs(deltaPhi(metP4.phi(),jetsp4[ijet].phi()));
	  mindphijmet = min(mindphijmet,dphijmet);
	  ptclosejet=jetsp4[ijet].pt();
	  etaclosejet=jetsp4[ijet].eta();
	}

      Float_t mt = METUtils::transverseMass(gamma,metP4,true);

    

      //reweight to reproduce pt weight in a gamma sample
      std::map<TString, float> qtWeights = gammaEvHandler.getWeights();
      
      //fill control histograms
      Hcutflow->Fill(1,1);
      Hcutflow->Fill(2,weight);
    //   if(nincjets==0) subcat="eq0softjets";
//       TString cats[]={"all",subcat};
//       TString subcats[]={"",phoCat};
//       for(size_t ic=0; ic<sizeof(cats)/sizeof(TString); ic++)
// 	{
      TString ctf="all";
// 	  TString ctf=cats[ic];
// 	  for(size_t isc=0; isc<sizeof(subcats)/sizeof(TString); isc++)
// 	    {
      for(size_t idc=0; idc<dilCats.size(); idc++)
	{
	  //TString pre= subcats[isc]+dilCats[idc];
	  TString pre=dilCats[idc];
	  float iweight=weight;
	  if(isGammaEvent)  iweight*=qtWeights[dilCats[idc]];
	  if(iweight>10) continue;
	  
	  controlHistos.fillHisto(pre+"r9",ctf, r9,iweight);
	  controlHistos.fillHisto(pre+"sietaieta",ctf, sietaieta,iweight);
	  controlHistos.fillHisto(pre+"hoe",ctf, hoe,iweight);
	  
	  if(!passTightGammaId) continue;
	  
	  controlHistos.fillHisto(pre+"nbtags",ctf, nbjets,iweight);
	  if(nbjets) continue;
	  
	  //if(ic==0 && isc==0)  controlHistos.fillHisto(dilCats[idc]+"zmass",ctf,gamma.mass(),iweight);
	  
	  controlHistos.fillHisto(pre+"nvtx",ctf, ev.nvtx,iweight);
	  controlHistos.fillHisto(pre+"qt",ctf, gamma.pt(),iweight);
	  controlHistos.fillHisto(pre+"eta",ctf, fabs(gamma.eta()),iweight);
	  
	  controlHistos.fillHisto(pre+"njets",ctf, njets,iweight);
	  controlHistos.fill2DHisto(pre+"qtvseta",ctf, gamma.pt(), fabs(gamma.eta()),iweight);
	  controlHistos.fill2DHisto(pre+"qtvsnvtx",ctf, gamma.pt(),ev.nvtx,iweight);
	  controlHistos.fillHisto(pre+"ht",ctf, ht,iweight);
	  controlHistos.fillHisto(pre+"htSmear",ctf, htsmear,iweight);
	  controlHistos.fillHisto(pre+"mt",ctf, mt,iweight);
	  controlHistos.fillHisto(pre+"mindphijmet",ctf, fabs(mindphijmet),iweight);
	  controlHistos.fillHisto(pre+"ptclosejet",ctf, ptclosejet,iweight);
	  controlHistos.fillHisto(pre+"dphivmet",ctf, fabs(dphivmet),iweight);
		  
	  for(std::vector<LorentzVector>::iterator jit= jetsp4.begin(); jit!=jetsp4.end(); jit++)
	    controlHistos.fillHisto(pre+"ptjets",ctf, jit->pt(),iweight);
		  
	  for(std::map<TString,LorentzVector>::iterator it = metTypeValues.begin(); it!= metTypeValues.end(); it++)
	    {
	      LorentzVector theMetP4=it->second;
	      TVector2 longi(gamma.px(),gamma.py());
	      longi = longi.Unit();
	      TVector2 perp = longi.Rotate(TMath::Pi()/2);
	      TVector2 met2d(theMetP4.px(),theMetP4.py());
	      double metL = fabs(met2d*longi);
	      double metT = fabs(met2d*perp);
	      controlHistos.fillHisto(   pre+TString("met_") + it->first,        ctf, theMetP4.pt(), iweight,true);
	      controlHistos.fillHisto(   pre+TString("met_") + it->first+TString("L"),        ctf, metL/gamma.pt(), iweight,true);
	      controlHistos.fillHisto(   pre+TString("met_") + it->first+TString("T"),        ctf, metT/gamma.pt(), iweight,true);
	      controlHistos.fill2DHisto( pre+TString("met_") + it->first+"vsht", ctf, ht,     it->second.pt(),  iweight);
	      //		      controlHistos.fill2DHisto( pre+TString("met_") + it->first+"vspu", ctf, ev.ngenITpu,     it->second.pt(),  iweight);
	    }
	    }
    }
      */
    }
  //}    
  //   }

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
  controlHistos.Write();
  ofile->Close();

}  


