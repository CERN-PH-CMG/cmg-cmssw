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
  TString categories[]={"eq0jets","eq1jets","geq2jets","vbf"};
  
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
  std::string puWeightFile = runProcess.getParameter<std::string>("puWeightFile");
  if(puWeightFile.size()==0)  LumiWeights.weight3D_init();
  else                        LumiWeights.weight3D_init(puWeightFile);

  //control Histos
  SelectionMonitor controlHistos;

  std::map<TString,TString> metTypes;
  metTypes["met"                 ]="E_{T}^{miss}";
  metTypes["assocChargedMet"     ]="assoc-E_{T}^{miss}(charged)";
  metTypes["assocMet"            ]="assoc-E_{T}^{miss}";
  metTypes["assocOtherVtxMet"    ]="assoc-E_{T}^{miss}(other vtx)";
  metTypes["minAssocChargedMet"  ]="min(E_{T}^{miss},assoc-E_{T}^{miss}(charged))";
  metTypes["minAssocMet"         ]="min(E_{T}^{miss},assoc-E_{T}^{miss})";
  metTypes["minClusteredMet"     ]="min(E_{T}^{miss},clustered-E_{T}^{miss})";
  metTypes["min3Met"             ]="min(E_{T}^{miss},assoc-E_{T}^{miss},clustered-E_{T}^{miss})";
  metTypes["redAssocMet"         ]="red(E_{T}^{miss},assoc-E_{T}^{miss})";
  metTypes["redClusteredMet"     ]="red(E_{T}^{miss},clustered-E_{T}^{miss})";
  metTypes["red3Met"             ]="red(E_{T}^{miss},assoc-E_{T}^{miss},clustered-E_{T}^{miss})";
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
	  TString subcat("photon"); subcat+=triggerThr;
	  if(triggerThr==0) subcat="";
	  subcat+=dilCats[idilcat];
	  photonSubcats.push_back(subcat);
	  controlHistos.addHistogram( new TH1F (subcat+"nvtx", ";Vertices;Events", 30,0,30) );
	  controlHistos.addHistogram( new TH1D (subcat+"eta", ";#eta;Events", 100,0,2.6) );
	  controlHistos.addHistogram( new TH1D (subcat+"qt", ";p_{T}^{#gamma} [GeV/c];Events / (2.5 GeV/c)", 200,0,600) );
	  controlHistos.addHistogram( new TH1D (subcat+"ptjets", ";p_{T}^{jet} [GeV/c];Jets / (2.5 GeV/c)", 200,0,600) );
	  controlHistos.addHistogram( new TH1D (subcat+"ht", ";H_{T} [GeV];Events", 100,0,500) );
	  if(icat==0) controlHistos.addHistogram( new TH1D (dilCats[idilcat]+"zmass",";M^{ll};Events", 100,91-31,91+31) );
	  controlHistos.addHistogram( new TH1D (subcat+"mt", ";M_{T} [GeV];Events", 100,0,1000) );
	  controlHistos.addHistogram( new TH1D (subcat+"dphill", ";#Delta#phi(l^{1},l^{2})[rad];Events", 100,0,3.2) );
	  controlHistos.addHistogram( new TH1F (subcat+"njets", ";Jets;Events", 6,0,6) );
	  controlHistos.addHistogram( new TH2D (subcat+"qtvseta", ";p_{T}^{#gamma} [GeV/c];#eta;Events / (2.5 GeV/c)", 200,0,600,50,0,2.6) );
	  controlHistos.addHistogram( new TH2D (subcat+"qtvsht", ";p_{T}^{#gamma} [GeV/c];H_{T} [GeV/c];Events / (2.5 GeV/c)", 200,0,600,200,0,1000) );
	  controlHistos.addHistogram( new TH2D (subcat+"metvsr9", ";E_{T}^{miss};R_{9}", 100,0,250,100,0,1.0) );
	  controlHistos.addHistogram( new TH2D (subcat+"qtvsnjets", ";p_{T}^{#gamma} [GeV/c];N_{jets};Events / (2 GeV/c)", 300,0,600,5,0,5) );
	  controlHistos.addHistogram( new TH1F (subcat+"minmjv", ";min M(jet,V);Events", 100,0,500) );
	  controlHistos.addHistogram( new TH1F (subcat+"mindphijmet", ";min #Delta#phi(jet,E_{T}^{miss});Events", 100,0,3.2) );
	  controlHistos.addHistogram( new TH1F (subcat+"dphivmet", ";#Delta#phi(boson,E_{T}^{miss});Events", 100,0,3.2) );
	  for(std::map<TString,TString>::iterator it = metTypes.begin(); it!= metTypes.end(); it++)
	    {
	      metTypeValues[it->first]=LorentzVector(0,0,0,0);
	      controlHistos.addHistogram( new TH1F( subcat+TString("met_") + it->first, ";"+it->second+";Events", 200,0,500) );
	      controlHistos.addHistogram( new TH1F( subcat+TString("met_") + it->first+TString("overqt"), ";"+it->second+";Events", 200,0,20) );
	      controlHistos.addHistogram( new TH2F( subcat+TString("met_") + it->first+"vspu", ";Pileup events;"+it->second+";Events", 25,0,25,200,0,500) );
	    }
	  
	  controlHistos.addHistogram( new TH1F (subcat+"evtctr", ";Mass;Events", 8,0,8) );
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
      if(!isGammaEvent && ev.cat != EE && ev.cat !=MUMU) continue;

      //float weight=ev.weight;
      float weight = LumiWeights.weight3D( ev.ngenOOTpu/2, ev.ngenITpu, ev.ngenOOTpu/2 );

      //event categories
      std::vector<TString> dilCats;
      LorentzVector gamma(0,0,0,0);
      int triggerThr(0);
      float r9(0),ensf(1.0);
      if(isGammaEvent)
	{
	  dilCats.push_back("ee");
	  dilCats.push_back("mumu");
	  dilCats.push_back("ll");
	  r9         = phys.gammas[0].r9;
	  ensf       = ev.g_ecorr[0];
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
	}
      
      //minimum threshold
      if(gamma.pt()<20 || gamma.pt()>500) continue;
      
      TString phoCat("photon");
      phoCat += triggerThr;

      //select jets
      int njets(0), nbjets(0);
      std::vector<LorentzVector> jetsp4;
      float minmjv(9999999.),ht(0.);
      for(size_t ijet=0; ijet<phys.jets.size(); ijet++)
	{

	  bool passJBPL(phys.jets[ijet].btag2>1.33);
	  bool passSSVHEM(phys.jets[ijet].btag3>1.74);
	  double dr=deltaR(phys.jets[ijet],gamma);
	  LorentzVector jv=phys.jets[ijet]+gamma;
	  float mjv=jv.mass();
	  if(dr<0.1) continue;
	  //if(mjv<91) continue;

	  njets   += (phys.jets[ijet].pt()>30 && fabs(phys.jets[ijet].eta())<2.5);
	  if(phys.jets[ijet].pt()>30) nbjets += (passJBPL || passSSVHEM); 
	  jetsp4.push_back( phys.jets[ijet] );
	  ht += phys.jets[ijet].pt();
	  if(mjv<minmjv) minmjv=mjv;
	}
      if(ev.ln>0 || nbjets>0) continue;

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
      LorentzVector rTMetP4  = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocChargedMetP4  , metP4                  , true);
      LorentzVector rAMetP4  = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocMetP4         , metP4                  , true);
      LorentzVector rCMetP4  = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, clusteredMetP4     , metP4                  , true);
      LorentzVector rTAMetP4 = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocChargedMetP4  , assocMetP4             , true);
      LorentzVector rTCMetP4 = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocChargedMetP4  , clusteredMetP4         , true);
      LorentzVector rACMetP4 = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocMetP4         , clusteredMetP4         , true);
      LorentzVector r3MetP4  = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, assocMetP4         , clusteredMetP4, metP4  , true);
      LorentzVector rmAMetP4 = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, min(metP4,assocMetP4), clusteredMetP4, metP4, true);
      LorentzVector redMetP4 = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, jetsp4             , metP4                  , true, &redMetInfo);
      double redMet = redMetP4.pt();   double redMetL = redMetInfo.redMET_l; double redMetT = redMetInfo.redMET_t;

      //met control
      metTypeValues["met"]                 = metP4;
      metTypeValues["assocChargedMet"]     = assocChargedMetP4;
      metTypeValues["assocMet"]            = assocMetP4;
      metTypeValues["clusteredMet"]        = clusteredMetP4;
      metTypeValues["minAssocChargedMet"]  = min(metP4,assocChargedMetP4);
      metTypeValues["minAssocMet"]         = min(metP4,assocMetP4);
      metTypeValues["assocOtherVtxMet"]    = assocMetOtherVtxP4;
      metTypeValues["minClusteredMet"]     = min(metP4,clusteredMetP4);
      metTypeValues["minTAssocMet"]        = min(assocChargedMetP4,assocMetP4);
      metTypeValues["minTClusteredMet"]    = min(assocChargedMetP4,clusteredMetP4);
      metTypeValues["minAClusteredMet"]    = min(assocMetP4,clusteredMetP4);
      metTypeValues["min3Met"]             = min(metP4, min(assocMetP4,clusteredMetP4));
      metTypeValues["min4Met"]             = min(min(metP4,assocChargedMetP4), min(assocMetP4,clusteredMetP4));
      metTypeValues["redMet"]              = redMetP4;
      metTypeValues["redAssocChargedMet"]  = rTMetP4;
      metTypeValues["redAssocMet"]         = rAMetP4;
      metTypeValues["redClusteredMet"]     = rCMetP4;
      metTypeValues["redTAssocMet"]        = rTAMetP4;
      metTypeValues["redTClusteredMet"]    = rTCMetP4;
      metTypeValues["redAClusteredMet"]    = rACMetP4;
      metTypeValues["red3Met"]             = r3MetP4;
      metTypeValues["redminAssocMet"]      = rmAMetP4;
      



      double dphivmet(deltaPhi(metP4.phi(),gamma.phi()));

      double mindphijmet(9999.);
      for(size_t ijet=0; ijet<jetsp4.size(); ijet++)
        {
	  if(jetsp4[ijet].pt()<30 || fabs(jetsp4[ijet].eta())>2.5)continue;
	  double dphijmet=fabs(deltaPhi(metP4.phi(),jetsp4[ijet].phi()));
	  mindphijmet = min(mindphijmet,dphijmet);
	}

      Float_t mt = METUtils::transverseMass(gamma,metP4,true);

      bool pass250( fabs(mindphijmet)>0.47 && metP4.pt()>59  && mt>222 && mt<272);
      bool pass300( fabs(mindphijmet)>0.33 && metP4.pt()>76  && mt>264 && mt<331);
      bool pass350( fabs(mindphijmet)>0.21 && metP4.pt()>95  && mt>298 && mt<393);
      bool pass400( fabs(mindphijmet)>0.12 && metP4.pt()>115 && mt>327 && mt<460);
      bool pass450( fabs(mindphijmet)>0.   && metP4.pt()>133 && mt>354 && mt<531);
      bool pass500( fabs(mindphijmet)>0.   && metP4.pt()>148 && mt>382 && mt<605);
      bool pass550( fabs(mindphijmet)>0.   && metP4.pt()>157 && mt>413 && mt<684);
      bool pass600( fabs(mindphijmet)>0.   && metP4.pt()>159 && mt>452 && mt<767);

      //reweight to reproduce pt weight in a gamma sample
      std::map<TString, float> qtWeights = gammaEvHandler.getWeights();

      //fill control histograms
      TString cats[]={"all",subcat};
      TString subcats[]={"",phoCat};
      for(size_t ic=0; ic<sizeof(cats)/sizeof(TString); ic++)
	{
	  TString ctf=cats[ic];
	  for(size_t isc=0; isc<sizeof(subcats)/sizeof(TString); isc++)
	    {
	      for(size_t idc=0; idc<dilCats.size(); idc++)
		{
		  float iweight=weight;
		  if(isGammaEvent) 
		    {
		      iweight*=qtWeights[dilCats[idc]];
		    }
		  if(ic==0 && isc==0)  controlHistos.fillHisto(dilCats[idc]+"zmass",ctf,gamma.mass(),iweight);

		  TString pre= subcats[isc]+dilCats[idc];
		  controlHistos.fillHisto(pre+"nvtx",ctf, ev.nvtx,iweight);
		  controlHistos.fillHisto(pre+"qt",ctf, gamma.pt(),iweight);
		  controlHistos.fillHisto(pre+"eta",ctf, fabs(gamma.eta()),iweight);
		  
		  controlHistos.fillHisto(pre+"njets",ctf, njets,iweight);
		  controlHistos.fill2DHisto(pre+"qtvseta",ctf, gamma.pt(), fabs(gamma.eta()),iweight);
		  controlHistos.fill2DHisto(pre+"metvsr9",ctf, metTypeValues["met"].pt(),r9,iweight);
		  controlHistos.fill2DHisto(pre+"qtvsnjets",ctf, gamma.pt(), (njets>4 ? 4:njets),iweight);
		  controlHistos.fill2DHisto(pre+"qtvsht",ctf, gamma.pt(), ht,iweight);
		  controlHistos.fillHisto(pre+"ht",ctf, ht,iweight);
		  controlHistos.fillHisto(pre+"mt",ctf, mt,iweight);
		  if(ht>0) 
		    {
		      controlHistos.fillHisto(pre+"minmjv",ctf, minmjv,iweight);
		      controlHistos.fillHisto(pre+"mindphijmet",ctf, fabs(mindphijmet),iweight);
		    }
		  controlHistos.fillHisto(pre+"dphivmet",ctf, fabs(dphivmet),iweight);
		  
		  for(std::vector<LorentzVector>::iterator jit= jetsp4.begin(); jit!=jetsp4.end(); jit++)
		    controlHistos.fillHisto(pre+"ptjets",ctf, jit->pt(),iweight);
		  
		  for(std::map<TString,LorentzVector>::iterator it = metTypeValues.begin(); it!= metTypeValues.end(); it++)
		    {
		      controlHistos.fillHisto(   pre+TString("met_") + it->first,        ctf, it->second.pt(), iweight);
		      controlHistos.fillHisto(   pre+TString("met_") + it->first+"overqt",        ctf, it->second.pt()/gamma.pt(), iweight);
		      controlHistos.fill2DHisto( pre+TString("met_") + it->first+"vspu", ctf, ev.ngenITpu,     it->second.pt(),  iweight);
		    }
		  
		  //event counters
		  if(pass250) controlHistos.fillHisto( pre+"evtctr",ctf,0,iweight);
		  if(pass300) controlHistos.fillHisto( pre+"evtctr",ctf,1,iweight);
		  if(pass350) controlHistos.fillHisto( pre+"evtctr",ctf,2,iweight);
		  if(pass400) controlHistos.fillHisto( pre+"evtctr",ctf,3,iweight);
		  if(pass450) controlHistos.fillHisto( pre+"evtctr",ctf,4,iweight);
		  if(pass500) controlHistos.fillHisto( pre+"evtctr",ctf,5,iweight);
		  if(pass550) controlHistos.fillHisto( pre+"evtctr",ctf,6,iweight);
		  if(pass600) controlHistos.fillHisto( pre+"evtctr",ctf,7,iweight);
		}
	    }
	}    
    }
  
  //MC normalization (to 1/pb)
  float cnorm=1.0;
  if(isMC)
    {
      TH1F *cutflowH = (TH1F *) file->Get("evAnalyzer/h2zz/photon/photon_cutflow");
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
      //       moncat.ReplaceAll("eq0jets","");
      //       moncat.ReplaceAll("eq1jets","");
      //       moncat.ReplaceAll("geq2jets","");
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


