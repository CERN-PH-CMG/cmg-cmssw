

#include <iostream>
#include <boost/shared_ptr.hpp>

#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"
#include "CMGTools/HtoZZ2l2nu/interface/GammaEventHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/METUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"
#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/SelectionMonitor.h"
#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"
#include "CMGTools/HtoZZ2l2nu/interface/MacroUtils.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

#include "Math/GenVector/Boost.h"

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

  TRandom2 rndGen;

  EventCategory eventClassifComp;
  TString categories[]={"eq0softjets","eq0jets","eq1jets","geq2jets","vbf"};
  
  //configure
  const edm::ParameterSet &runProcess = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("runProcess");
  TString url=runProcess.getParameter<std::string>("input");
  TString outdir=runProcess.getParameter<std::string>("outdir");
  bool isMC = runProcess.getParameter<bool>("isMC");
  int mctruthmode = runProcess.getParameter<int>("mctruthmode");
  int evStart=runProcess.getParameter<int>("evStart");
  int evEnd=runProcess.getParameter<int>("evEnd");
  TString dirname = runProcess.getParameter<std::string>("dirName");

  GammaEventHandler gammaEvHandler(runProcess);
  edm::LumiReWeighting LumiWeights(runProcess.getParameter<std::string>("mcpileup"), runProcess.getParameter<std::string>("datapileup"), "pileup","pileup");
  
  //control Histos
  SelectionMonitor controlHistos;

  std::map<TString,TString> metTypes;
  metTypes["met"                 ]="E_{T}^{miss}";
  metTypes["assocChargedMet"     ]="assoc-E_{T}^{miss}(charged)";
  metTypes["assocMet"            ]="assoc-E_{T}^{miss}";
  //  metTypes["assocOtherVtxMet"    ]="assoc-E_{T}^{miss}(other vtx)";
  //   metTypes["minAssocChargedMet"  ]="min(E_{T}^{miss},assoc-E_{T}^{miss}(charged))";
  //  metTypes["minAssocMet"         ]="min(E_{T}^{miss},assoc-E_{T}^{miss})";
  //  metTypes["minClusteredMet"     ]="min(E_{T}^{miss},clustered-E_{T}^{miss})";
  //  metTypes["min3Met"             ]="min(E_{T}^{miss},assoc-E_{T}^{miss},clustered-E_{T}^{miss})";
  //  metTypes["redAssocMet"         ]="red(E_{T}^{miss},assoc-E_{T}^{miss})";
  metTypes["redClusteredMet"     ]="red(E_{T}^{miss},clustered-E_{T}^{miss})";
  //  metTypes["red3Met"             ]="red(E_{T}^{miss},assoc-E_{T}^{miss},clustered-E_{T}^{miss})";
  std::map<TString,LorentzVector> metTypeValues;

  std::vector<TString> photonSubcats;
  std::vector<TString> dilCats;
  dilCats.push_back("ee");
  dilCats.push_back("mumu");
  for(size_t icat=0; icat<gammaEvHandler.nCategories(); icat++)
    {
      for(size_t idilcat=0; idilcat<dilCats.size(); idilcat++)
	{
	  int triggerThr=gammaEvHandler.category(icat);
	  TString subcat=dilCats[idilcat];
	  if(triggerThr==0) 
	    {
	      controlHistos.addHistogram( new TH1D (subcat+"qt", ";p_{T}^{#gamma} [GeV/c];Events / (2.5 GeV/c)", 200,0,600) );
	      controlHistos.addHistogram( new TH1D (subcat+"eta", ";#eta;Events", 50,0,2.6) );
	      controlHistos.addHistogram( new TH2D (subcat+"qtvseta", ";p_{T}^{#gamma} [GeV/c];#eta;Events / (2.5 GeV/c)", 200,0,600,50,0,2.6) );
	      controlHistos.addHistogram( new TH2D (subcat+"qtvsnvtx", ";p_{T}^{#gamma} [GeV/c];Vertices;Events", 200,0,600,30,0,30) );
	      controlHistos.addHistogram( new TH1D (subcat+"zmass",";M^{ll};Events", 100,91-31,91+31) );
	    }
	  else
	    {
	      subcat ="photon"; subcat+=triggerThr; subcat += dilCats[idilcat];
	    }

	  photonSubcats.push_back(subcat);

	  //these are worth to monitor per sub-category
	  controlHistos.addHistogram( new TH1F (subcat+"hoe", ";H/E;Events", 100,0.,0.1) );
	  controlHistos.addHistogram( new TH1F (subcat+"r9", ";R9;Events", 100,0.8,1) );
	  controlHistos.addHistogram( new TH1F (subcat+"sietaieta", ";#sigma i#eta i#eta;Events", 100,0,0.03) );
	  controlHistos.addHistogram( new TH1F (subcat+"nvtx", ";Vertices;Events", 30,0,30) );
	  controlHistos.addHistogram( new TH1D (subcat+"ptjets", ";p_{T}^{jet} [GeV/c];Jets", 50,0,250) );
	  controlHistos.addHistogram( new TH1D (subcat+"ptclosejet", ";p_{T}(closest jet) [GeV/c];Events", 50,0,250) );
	  controlHistos.addHistogram( new TH1D (subcat+"ht", ";H_{T} [GeV];Events", 100,0,500) );
	  controlHistos.addHistogram( new TH1D (subcat+"mt", ";M_{T} [GeV];Events", 100,0,1000) );
	  controlHistos.addHistogram( new TH1F (subcat+"njets", ";Jets;Events", 6,0,6) );
	  controlHistos.addHistogram( new TH1F (subcat+"nbtags", ";b-tag multiplcity;Events", 5,0,5) );

	  controlHistos.addHistogram( new TH1F (subcat+"minmjv", ";min M(jet,boson);Events", 100,0,500) );
	  controlHistos.addHistogram( new TH1F (subcat+"mindphijmet", ";min #Delta#phi(jet,E_{T}^{miss});Events", 14,0,3.5) );
	  controlHistos.addHistogram( new TH1F (subcat+"mindphijmet50", ";min #Delta#phi(jet,E_{T}^{miss});Events", 14,0,3.5) );
	  controlHistos.addHistogram( new TH1F (subcat+"mindphijmet70", ";min #Delta#phi(jet,E_{T}^{miss});Events", 14,0,3.5) );
	  controlHistos.addHistogram( new TH1F (subcat+"dphivmet", ";#Delta#phi(boson,E_{T}^{miss});Events", 14,0,3.5) );
	  double metAxis[]={0,10,20,30,40,50,60,70,80,90,100,120,140,160,180,200,250,300,400,500};
	  int nMetBins=sizeof(metAxis)/sizeof(double)-1;
	  for(std::map<TString,TString>::iterator it = metTypes.begin(); it!= metTypes.end(); it++)
	    {
	      metTypeValues[it->first]=LorentzVector(0,0,0,0);
	      //controlHistos.addHistogram( new TH1F( subcat+TString("met_") + it->first, ";"+it->second+";Events", 100,0,500) );
	      controlHistos.addHistogram( new TH1F( subcat+TString("met_") + it->first, ";"+it->second+";Events", nMetBins,metAxis) );
	      controlHistos.addHistogram( new TH1F( subcat+TString("met_") + it->first+TString("L"), ";"+it->second+"_{L}/q_{T};Events", 50,0,2) );
	      //controlHistos.addHistogram( new TH1F( subcat+TString("met_") + it->first+TString("T"), ";"+it->second+"_{T}/q_{T};Events", 50,0,2) );
	      //controlHistos.addHistogram( new TH2F( subcat+TString("met_") + it->first+"vspu", ";Pileup events;"+it->second+";Events", 25,0,25,100,0,500) );
	      controlHistos.addHistogram( new TH2F( subcat+TString("met_") + it->first+"vsht", ";H_{T};"+it->second+";Events", 25,0,250,25,0,250) );
	    }
	  
	}
    }

  //replicate plots for other categories
  for(size_t icat=0; icat<sizeof(categories)/sizeof(TString); icat++)  controlHistos.initMonitorForStep(categories[icat]);

  //open the file and get events tree
  TFile *file = TFile::Open(url);
  if(file==0) return -1;
  if(file->IsZombie()) return -1;
  ZZ2l2nuSummaryHandler evSummaryHandler;
  cout << url << endl;
  if( !evSummaryHandler.attachToTree( (TTree *)file->Get(dirname) ) ) 
    {
      file->Close();
      return -1;
    }

  //check run range
  float rescaleFactor( evEnd>0 ?  float(evSummaryHandler.getEntries())/float(evEnd-evStart) : -1 );
  if(evEnd<0 || evEnd>evSummaryHandler.getEntries() ) evEnd=evSummaryHandler.getEntries();
  if(evStart > evEnd ) 
    {
      file->Close();
      return -1;
    }


  //run the analysis
  std::set<int> trigList;
  for( int iev=evStart; iev<evEnd; iev++)
    {
      if(iev%1000==0) printf("\r [ %d/100 ] ",int(100*float(iev-evStart)/float(evEnd)));
      evSummaryHandler.getEntry(iev);

      ZZ2l2nuSummary_t &ev=evSummaryHandler.getEvent();
      PhysicsEvent_t phys=getPhysicsEventFrom(ev);
      int eventCategory = eventClassifComp.Get(phys);
      TString subcat    = eventClassifComp.GetLabel(eventCategory);
      bool isGammaEvent = gammaEvHandler.isGood(phys);
      if(mctruthmode==22 && !isGammaEvent) continue;
      if(mctruthmode==1 && isGammaEvent) continue;
      if(!isGammaEvent && ev.cat != EE && ev.cat !=MUMU) continue;

      float weight = 1.0;
      if(isMC && !isGammaEvent)	{ weight = LumiWeights.weight( ev.ngenITpu ); }

      //event categories
      std::vector<TString> dilCats;
      LorentzVector gamma(0,0,0,0);
      int triggerThr(0);
      float r9(0),ensf(1.0),sietaieta(0),hoe(0);
      bool passTightGammaId(false);
      if(isGammaEvent)
	{
	  dilCats.push_back("ee");
	  dilCats.push_back("mumu");
	  dilCats.push_back("ll");
	  r9         = phys.gammas[0].r9*(isMC ? 1.005 : 1.0); 
	  sietaieta  = phys.gammas[0].sihih;
	  hoe        = phys.gammas[0].hoe;
	  ensf       = ev.g_ecorr[0];
	  gamma      = gammaEvHandler.massiveGamma("ll");
	  gamma *= ensf;
	  triggerThr = gammaEvHandler.triggerThr();
	  if(r9>0.94 && sietaieta<0.0106 && sietaieta>0.) passTightGammaId=true;
	  //if(sietaieta>0.) passTightGammaId=true;
	}
      else
	{
	  passTightGammaId=true;
	  gamma=phys.leptons[0]+phys.leptons[1];
	  if(fabs(gamma.mass()-91)>15) continue;
	  if(ev.cat==MUMU) dilCats.push_back("mumu");
	  if(ev.cat==EE)   dilCats.push_back("ee");
	  dilCats.push_back("ll");
	  triggerThr=gammaEvHandler.findTriggerCategoryFor( gamma.pt() );
	}
      
      //minimum threshold
      if(gamma.pt()<20) continue;
      
      TString phoCat("photon");
      phoCat += triggerThr;

      //select jets
      int njets=eventClassifComp.GetVBFJetCount();//CentralJetCount();
      int nincjets(0);
      int nbjets(0);
      std::vector<LorentzVector> jetsp4;
      float minmjv(9999999.),ht(0.);
      for(size_t ijet=0; ijet<phys.jets.size(); ijet++)
	{

	  bool passTCHEL(phys.jets[ijet].btag1>2);
	  double dr=deltaR(phys.jets[ijet],gamma);
	  LorentzVector jv=phys.jets[ijet]+gamma;
	  float mjv=jv.mass();
	  if(dr<0.1) continue;
	  jetsp4.push_back( phys.jets[ijet] );
	  nincjets   += (phys.jets[ijet].pt()>15 && fabs(phys.jets[ijet].eta())<5.0);
	  if(phys.jets[ijet].pt()>30 && fabs(phys.jets[ijet].eta())<2.5) nbjets += passTCHEL;
	  ht += phys.jets[ijet].pt();
	  if(mjv<minmjv) minmjv=mjv;
	}
      if(ev.ln>0) continue;

      //met
      LorentzVector metP4=phys.met[0];
      LorentzVector assocChargedMetP4=phys.met[1];
      LorentzVector assocMetP4=phys.met[3];
      LorentzVector assocMetOtherVtxP4=phys.met[4];
      LorentzVector centralMetP4=phys.met[5];
      LorentzVector cleanMetP4=phys.met[6];
      LorentzVector assocOtherVertexMetP4=phys.met[7];
      LorentzVector assocFwdMetP4=phys.met[8];
      LorentzVector assocMet5P4=phys.met[9];
      LorentzVector assocFwdMet5P4=phys.met[10];
      LorentzVector assocMet10P4=phys.met[11];
      LorentzVector assocFwdMet10P4=phys.met[12];
      LorentzVector clusteredMetP4 = -1*gamma;  for(unsigned int i=0;i<jetsp4.size();i++){clusteredMetP4 -= jetsp4[i];}
      if(isGammaEvent)
	{
	  assocChargedMetP4 -= gamma;
	  if(!phys.gammas[0].isConv) assocMetP4 -= gamma;
	}

      METUtils::stRedMET redMetInfo;
      LorentzVector nullP4   = LorentzVector(0,0,0,0);
      LorentzVector lep1     = gamma;
      LorentzVector lep2     = nullP4;
      //      LorentzVector rTMetP4  = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocChargedMetP4  , metP4                  , true);
      //      LorentzVector rAMetP4  = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocMetP4         , metP4                  , true);
      LorentzVector rCMetP4  = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, clusteredMetP4     , metP4                  , true);
      //      LorentzVector rTAMetP4 = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocChargedMetP4  , assocMetP4             , true);
      //      LorentzVector rTCMetP4 = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocChargedMetP4  , clusteredMetP4         , true);
      //      LorentzVector rACMetP4 = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocMetP4         , clusteredMetP4         , true);
      //      LorentzVector r3MetP4  = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocMetP4         , clusteredMetP4, metP4  , true);
      //      LorentzVector rmAMetP4 = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, min(metP4,assocMetP4), clusteredMetP4, metP4, true);
      //      LorentzVector redMetP4 = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, jetsp4             , metP4                  , true, &redMetInfo);
      //      double redMet = redMetP4.pt();   double redMetL = redMetInfo.redMET_l; double redMetT = redMetInfo.redMET_t;

      //met control
      metTypeValues["met"]                 = metP4;
      metTypeValues["assocChargedMet"]     = assocChargedMetP4;
      metTypeValues["assocMet"]            = assocMetP4;
      //      metTypeValues["clusteredMet"]        = clusteredMetP4;
      //      metTypeValues["minAssocChargedMet"]  = min(metP4,assocChargedMetP4);
      //      metTypeValues["minAssocMet"]         = min(metP4,assocMetP4);
      //      metTypeValues["assocOtherVtxMet"]    = assocMetOtherVtxP4;
      //      metTypeValues["minClusteredMet"]     = min(metP4,clusteredMetP4);
      //      metTypeValues["minTAssocMet"]        = min(assocChargedMetP4,assocMetP4);
      //      metTypeValues["minTClusteredMet"]    = min(assocChargedMetP4,clusteredMetP4);
      //      metTypeValues["minAClusteredMet"]    = min(assocMetP4,clusteredMetP4);
      //      metTypeValues["min3Met"]             = min(metP4, min(assocMetP4,clusteredMetP4));
      //      metTypeValues["min4Met"]             = min(min(metP4,assocChargedMetP4), min(assocMetP4,clusteredMetP4));
      //      metTypeValues["redMet"]              = redMetP4;
      //      metTypeValues["redAssocChargedMet"]  = rTMetP4;
      //      metTypeValues["redAssocMet"]         = rAMetP4;
      metTypeValues["redClusteredMet"]     = rCMetP4;
      //      metTypeValues["redTAssocMet"]        = rTAMetP4;
      //      metTypeValues["redTClusteredMet"]    = rTCMetP4;
      //      metTypeValues["redAClusteredMet"]    = rACMetP4;
      //      metTypeValues["red3Met"]             = r3MetP4;
      //      metTypeValues["redminAssocMet"]      = rmAMetP4;
     

      double dphivmet(deltaPhi(metP4.phi(),gamma.phi()));
      //if(isGammaEvent && dphivmet>2.8) continue;

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
      if(nincjets==0) subcat="eq0softjets";
      TString cats[]={"all",subcat};
      TString subcats[]={"",phoCat};
      for(size_t ic=0; ic<sizeof(cats)/sizeof(TString); ic++)
	{
	  TString ctf=cats[ic];
	  for(size_t isc=0; isc<sizeof(subcats)/sizeof(TString); isc++)
	    {
	      for(size_t idc=0; idc<dilCats.size(); idc++)
		{
		  TString pre= subcats[isc]+dilCats[idc];
		  float iweight=weight;
		  if(isGammaEvent)  iweight*=qtWeights[dilCats[idc]];
		  if(iweight>6) continue;

		  controlHistos.fillHisto(pre+"r9",ctf, r9,iweight);
		  controlHistos.fillHisto(pre+"sietaieta",ctf, sietaieta,iweight);
		  controlHistos.fillHisto(pre+"hoe",ctf, hoe,iweight);
		  
		  if(!passTightGammaId) continue;

		  controlHistos.fillHisto(pre+"nbtags",ctf, nbjets,iweight);
		  if(nbjets) continue;
		  
		  if(ic==0 && isc==0)  controlHistos.fillHisto(dilCats[idc]+"zmass",ctf,gamma.mass(),iweight);

		  controlHistos.fillHisto(pre+"nvtx",ctf, ev.nvtx,iweight);
		  controlHistos.fillHisto(pre+"qt",ctf, gamma.pt(),iweight);
		  controlHistos.fillHisto(pre+"eta",ctf, fabs(gamma.eta()),iweight);
		  
		  controlHistos.fillHisto(pre+"njets",ctf, njets,iweight);
		  controlHistos.fill2DHisto(pre+"qtvseta",ctf, gamma.pt(), fabs(gamma.eta()),iweight);
		  controlHistos.fill2DHisto(pre+"qtvsnvtx",ctf, gamma.pt(),ev.nvtx,iweight);
		  controlHistos.fillHisto(pre+"ht",ctf, ht,iweight);
		  controlHistos.fillHisto(pre+"mt",ctf, mt,iweight);
		  //if(ht>0) 
		    {
		      controlHistos.fillHisto(pre+"minmjv",ctf, minmjv,iweight);
		      controlHistos.fillHisto(pre+"mindphijmet",ctf, fabs(mindphijmet),iweight);
		      if(fabs(metP4.pt())>50) controlHistos.fillHisto(pre+"mindphijmet50",ctf, fabs(mindphijmet),iweight);
		      if(fabs(metP4.pt())>70) controlHistos.fillHisto(pre+"mindphijmet70",ctf, fabs(mindphijmet),iweight);
		      controlHistos.fillHisto(pre+"ptclosejet",ctf, ptclosejet,iweight);
		    }
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
	}    
    }
  
  //MC normalization (to 1/pb)
  float cnorm=1.0;
  if(isMC)
    {
      TH1F *cutflowH = (TH1F *) file->Get("evAnalyzer/h2zz/cutflow");
      if(cutflowH==0) cutflowH = (TH1F *) file->Get("evAnalyzer/h2zz/cutflow");
      if(cutflowH)    cnorm=cutflowH->GetBinContent(1);
      cout << "The initial number of events was " << cnorm << endl;
      if(rescaleFactor>0) 
	{
	  cout << "Warning: rescaling by " << cnorm/rescaleFactor << " because we have run partially on this sample" << endl;
	  cnorm /= rescaleFactor;
	}
    }
  
  for(std::set<int>::iterator it = trigList.begin();
      it != trigList.end();
      it++)
    cout << *it << " ";
  cout << endl;
  cout << "Sample treated as MC? " << isMC << endl;

  //detach all histograms from the file
  SelectionMonitor::StepMonitor_t &mons=controlHistos.getAllMonitors();
  for(SelectionMonitor::StepMonitor_t::iterator it =mons.begin(); it!= mons.end(); it++)
    {
      for(SelectionMonitor::Monitor_t::iterator hit=it->second.begin(); hit!= it->second.end(); hit++)
	{
	  hit->second->SetDirectory(0);
	}
    }
  
  //all done with the events file
  file->Close();
  //save to file
  TString outUrl( outdir );
  gSystem->Exec("mkdir -p " + outUrl);
  outUrl += "/";
  outUrl += gSystem->BaseName(url);
  
  TFile *ofile=TFile::Open(outUrl, "recreate");
  TDirectory *baseOutDir=ofile->mkdir("localAnalysis");
  std::map<TString, TDirectory *> outDirs;
  for(SelectionMonitor::StepMonitor_t::iterator it =mons.begin(); it!= mons.end(); it++)
    {
      TString moncat=it->first;
      if(outDirs.find(moncat)==outDirs.end()) outDirs[moncat]=baseOutDir->mkdir(moncat);
      outDirs[moncat]->cd();
      for(SelectionMonitor::Monitor_t::iterator hit=it->second.begin(); hit!= it->second.end(); hit++)
	{
	  if( !((TClass*)hit->second->IsA())->InheritsFrom("TH1") 
	      && !((TClass*)hit->second->IsA())->InheritsFrom("TH2") 
	      && !((TClass*)hit->second->IsA())->InheritsFrom("TGraph") ) 
	    continue;
	  
	  if(isMC && !((TClass*)hit->second->IsA())->InheritsFrom("TGraph") && cnorm>0)
	      hit->second->Scale(1./cnorm);
	  if( !((TClass*)hit->second->IsA())->InheritsFrom("TH2") && !((TClass*)hit->second->IsA())->InheritsFrom("TGraph") )
	    fixExtremities(hit->second,true,true);
	  
	  hit->second->Write();
	}
    }
  ofile->Close();
}  


