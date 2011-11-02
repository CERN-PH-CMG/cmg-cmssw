#include <iostream>
#include <boost/shared_ptr.hpp>

#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"
#include "CMGTools/HtoZZ2l2nu/interface/ReducedMETComputer.h"
#include "CMGTools/HtoZZ2l2nu/interface/TransverseMassComputer.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"
#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/SelectionMonitor.h"
#include "CMGTools/HtoZZ2l2nu/interface/JetEnergyUncertaintyComputer.h"
#include "CondFormats/JetMETObjects/interface/JetResolution.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

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

TH1F *getZMassHisto();

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

  TransverseMassComputer mtComp;
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

  TString gammaPtWeightsFile =  runProcess.getParameter<std::string>("weightsFile"); gSystem->ExpandPathName(gammaPtWeightsFile);
  
  //control Histos
  SelectionMonitor controlHistos;
  TH1F *zmassH=getZMassHisto();

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

  float minJetPt(15);
  bool doInclusiveEventCategories(true);
  if(doInclusiveEventCategories) minJetPt=30;
  Int_t photoncats[]={0,20,30,50,75,125};
  //Int_t photoncats[]={0,20,30,50,75,90,125,135,200};
  const size_t nPhotonCats=sizeof(photoncats)/sizeof(Int_t);
  std::vector<TString> photonSubcats;
  std::vector<TString> dilCats;
  dilCats.push_back("ee");
  dilCats.push_back("mumu");
  for(size_t icat=0; icat<nPhotonCats; icat++)
    {
      for(size_t idilcat=0; idilcat<dilCats.size(); idilcat++)
	{
	  TString subcat("photon"); subcat+=photoncats[icat]; 
	  if(photoncats[icat]==0) subcat="";
	  subcat+=dilCats[idilcat];
	  photonSubcats.push_back(subcat);
	  controlHistos.addHistogram( new TH1F (subcat+"nvtx", ";Vertices;Events", 30,0,30) );
	  controlHistos.addHistogram( new TH1D (subcat+"eta", ";#eta;Events", 100,0,2.6) );
	  controlHistos.addHistogram( new TH1D (subcat+"qt", ";p_{T}^{#gamma} [GeV/c];Events / (2.5 GeV/c)", 200,0,600) );
	  controlHistos.addHistogram( new TH1D (subcat+"ptjets", ";p_{T}^{jet} [GeV/c];Jets / (2.5 GeV/c)", 200,0,600) );
	  controlHistos.addHistogram( new TH1D (subcat+"ht", ";H_{T} [GeV];Events", 100,0,500) );
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

  std::map<TString,TH1 *> wgtsH;
  bool weight2D(false);
  TFile *fwgt=TFile::Open(gammaPtWeightsFile);
  if(fwgt)
    {
      TString wgtName = gammaPtWeightsFile;
      wgtName=gSystem->BaseName(wgtName.ReplaceAll(".root",""));
      wgtName=((TObjString *)(wgtName.Tokenize("_")->At(1)))->GetString();
      for(size_t icat=0; icat<sizeof(categories)/sizeof(TString); icat++)
	{
	  for(size_t isubcat=0; isubcat<photonSubcats.size(); isubcat++)
	    {
	      TString key=categories[icat]+photonSubcats[isubcat];
	      TH1 *h=(TH1 *)fwgt->Get(key+wgtName);
	      if(h==0) continue;
	      wgtsH[key] = h;
	      wgtsH[key]->SetDirectory(0);
	      weight2D = ((TClass*)wgtsH[key]->IsA())->InheritsFrom("TH2");
	      cout << "Adding weight for category: " << key << endl;
	    }
	}
      fwgt->Close();
      if(wgtsH.size()) cout << "Gamma Pt will be reweighted using " 
			    << (weight2D ? "2d" : "1d") 
			    << " distribution found @ " 
			    << gammaPtWeightsFile << endl;
    }



  //replicate plots for other categories
  for(size_t icat=0; icat<sizeof(categories)/sizeof(TString); icat++)  controlHistos.initMonitorForStep(categories[icat]);

  //start the met computers
  ReducedMETComputer rmetComp(1., 1., 1., 1., 1.);
  ReducedMETComputer rTComp(1., 1., 1., 1., 1.);
  ReducedMETComputer rAComp(1., 1., 1., 1., 1.);
  ReducedMETComputer rCComp(1., 1., 1., 1., 1.);
  ReducedMETComputer rTAComp(1., 1., 1., 1., 1.);
  ReducedMETComputer rTCComp(1., 1., 1., 1., 1.);
  ReducedMETComputer rACComp(1., 1., 1., 1., 1.);
  ReducedMETComputer r3Comp(1., 1., 1., 1., 1.);
  ReducedMETComputer rmAComp(1., 1., 1., 1., 1.);

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

      bool isGammaEvent(ev.cat>3);
      int eventCategory = eventClassifComp.Get(phys,0,isGammaEvent);
      TString subcat    = eventClassifComp.GetLabel(eventCategory);
      
      float weight=ev.weight;
      
      //event categories
      std::vector<TString> dilCats;
      LorentzVector gamma(0,0,0,0);
      int triggerThr(0);
      float r9(0);
      if(isGammaEvent)
	{
	  //cross check: only 1 prompt gamma allowed
	  if(ev.gn!=1) continue;
	  gamma=phys.gammas[0];
	  if(gamma.pt()<1) continue;
	  if(fabs(gamma.eta())>1.4442 && fabs(gamma.eta())<1.566) continue;

	  //get the trigger threshold
	  triggerThr=(ev.cat-22)/1000;
	  if(triggerThr==0) continue;
	  trigList.insert(triggerThr);

	  //check if pt is within the trigger threshold range 
	  bool rejectEvent(false);
	  for(size_t icat=0; icat<nPhotonCats-1; icat++)
	    {
	      if(photoncats[icat]!=triggerThr) continue;
	      if(gamma.pt()<photoncats[icat] || gamma.pt()>=photoncats[icat+1]) rejectEvent=true;
	    }
	  if(rejectEvent) continue;

	  r9=phys.gammas[0].r9;
	  gamma *= ev.g_ecorr[0];

	  //generate random mass from Z line shape
	  float mass(0);
	  while(fabs(mass-91)>15) mass = zmassH->GetRandom();
	  gamma=LorentzVector(gamma.px(),gamma.py(),gamma.pz(),sqrt(pow(mass,2)+pow(gamma.energy(),2)));
	  
	  dilCats.push_back("ee");
	  dilCats.push_back("mumu");
	}
      else
	{
	  gamma=phys.leptons[0]+phys.leptons[1];
	  if(fabs(gamma.mass()-91)>15) continue;

	  if(ev.cat != dilepton::MUMU && ev.cat!=dilepton::EE) continue;
	  if(ev.cat==dilepton::MUMU) dilCats.push_back("mumu");
	  if(ev.cat==dilepton::EE)   dilCats.push_back("ee");

	  //find the trigger - threshold category (assume 100% efficiency...) 
	  if(gamma.pt()>=photoncats[nPhotonCats-1]) triggerThr=photoncats[nPhotonCats-1];
	  else
	    {
	      size_t icat=0;
	      for(; icat<nPhotonCats-1; icat++)	
		if(gamma.pt()>=photoncats[icat] && gamma.pt()<photoncats[icat+1]) break;
	      triggerThr=photoncats[icat];
	    }
	}
      
      //minimum threshold
      if(gamma.pt()<20 || gamma.pt()>500) continue;
      if(fabs(gamma.eta())>1.4442) continue;

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

	  njets   += (phys.jets[ijet].pt()>minJetPt && fabs(phys.jets[ijet].eta())<2.5);
	  if(phys.jets[ijet].pt()>30) nbjets += (passJBPL || passSSVHEM); 
	  jetsp4.push_back( phys.jets[ijet] );
	  ht += phys.jets[ijet].pt();
	  if(mjv<minmjv) minmjv=mjv;
	}
      if(ev.ln>0 || nbjets>0) continue;

      //revert to inclusive event categories
      if(doInclusiveEventCategories && eventCategory != EventCategory::VBF)
	{
	  if(njets==0)
	    {
	      eventCategory = EventCategory::EQ0JETS;
	      subcat    = "eq0jets";
	    }
	  else if(njets==1)
	    {
	      eventCategory = EventCategory::EQ1JETS;
	      subcat    = "eq1jets";
	    }
	  else if(njets>=2) 
	    {
	      eventCategory = EventCategory::GEQ2JETS;
	      subcat    = "geq2jets";
	    }
	}


      //reweight to reproduce pt weight
      std::map<TString, float> qtWeights;
      if(isGammaEvent)
 	{

	  for(size_t idcat=0; idcat<dilCats.size(); idcat++)
	    {
	      float qtweight(1.0);
	      qtWeights[dilCats[idcat]]=qtweight;

	      TString wgtKey=subcat+phoCat+dilCats[idcat];
	      if( wgtsH.find(wgtKey)== wgtsH.end()) continue;

	      //take the last bin weight if pT>max available
	      TH1 *theH = wgtsH[wgtKey];
	      for(int ibin=1; ibin<=theH->GetXaxis()->GetNbins(); ibin++)
		{
		  if(gamma.pt()<theH->GetXaxis()->GetBinLowEdge(ibin) ) break;
		  if(weight2D)
		    {
		      if(gammaPtWeightsFile.Contains("eta"))        qtweight = theH->GetBinContent(ibin,theH->GetYaxis()->FindBin(fabs(gamma.eta())));
		      else if(gammaPtWeightsFile.Contains("njets")) qtweight = theH->GetBinContent(ibin,theH->GetYaxis()->FindBin(njets));
		      else                                          qtweight = theH->GetBinContent(ibin,theH->GetYaxis()->FindBin(ht));
		    }
		  else
		    {
		      qtweight = theH->GetBinContent(ibin);
		    }
		}
	      if(qtweight>100) continue;
	      qtWeights[dilCats[idcat]]=qtweight;
	    }
	}

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

      Float_t met = metP4.pt();
      LorentzVector nullP4(0,0,0,0);
      rmetComp.compute(gamma,0,nullP4,0, jetsp4, metP4, true );
      double redmet  = rmetComp.reducedMET(ReducedMETComputer::INDEPENDENTLYMINIMIZED);
      double redmetL = rmetComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).second;
      double redmetT = rmetComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).first;
      Float_t assocChargedMet  = assocChargedMetP4.pt();
      Float_t assocMet         = assocMetP4.pt();
      Float_t assocMetOtherVtx = assocMetOtherVtxP4.pt();
      Float_t minAssocMet      = min(assocMet,met);

      //redmet
      rmetComp.compute(gamma,0,nullP4,0, jetsp4, metP4, true );
      Float_t redMet_d0  = rmetComp.reducedMET(ReducedMETComputer::D0);
      Float_t redMetL_d0  = rmetComp.reducedMETComponents(ReducedMETComputer::D0).second;
      Float_t redMetT_d0  = rmetComp.reducedMETComponents(ReducedMETComputer::D0).first;
      Float_t redMetX_d0  = rmetComp.reducedMETcartesian(ReducedMETComputer::D0).X();
      Float_t redMetY_d0  = rmetComp.reducedMETcartesian(ReducedMETComputer::D0).Y();
      Float_t redMet         = rmetComp.reducedMET(ReducedMETComputer::INDEPENDENTLYMINIMIZED);
      Float_t redMetL        = rmetComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).second;
      Float_t redMetT        = rmetComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).first;
      Float_t redMetX        = rmetComp.reducedMETcartesian(ReducedMETComputer::INDEPENDENTLYMINIMIZED).X();
      Float_t redMetY        = rmetComp.reducedMETcartesian(ReducedMETComputer::INDEPENDENTLYMINIMIZED).Y();

      //cross-check that the second computer is giving same result as the first one
      rTComp.compute(gamma,0,nullP4,0,assocChargedMetP4, metP4, metP4,true );
      Float_t rTMet         = rTComp.reducedMET(ReducedMETComputer::INDEPENDENTLYMINIMIZED);
      Float_t rTMetL        = rTComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).second;
      Float_t rTMetT        = rTComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).first;
      Float_t rTMetX        = rTComp.reducedMETcartesian(ReducedMETComputer::INDEPENDENTLYMINIMIZED).X();
      Float_t rTMetY        = rTComp.reducedMETcartesian(ReducedMETComputer::INDEPENDENTLYMINIMIZED).Y();

      rAComp.compute(gamma,0,nullP4,0, assocMetP4, metP4, metP4,true );
      Float_t rAMet         = rAComp.reducedMET(ReducedMETComputer::INDEPENDENTLYMINIMIZED);
      Float_t rAMetL        = rAComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).second;
      Float_t rAMetT        = rAComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).first;
      Float_t rAMetX        = rAComp.reducedMETcartesian(ReducedMETComputer::INDEPENDENTLYMINIMIZED).X();
      Float_t rAMetY        = rAComp.reducedMETcartesian(ReducedMETComputer::INDEPENDENTLYMINIMIZED).Y();

      rCComp.compute(gamma,0,nullP4,0, clusteredMetP4, metP4, metP4, true);
      Float_t rCMet         = rCComp.reducedMET(ReducedMETComputer::INDEPENDENTLYMINIMIZED);
      Float_t rCMetL        = rCComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).second;
      Float_t rCMetT        = rCComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).first;
      Float_t rCMetX        = rCComp.reducedMETcartesian(ReducedMETComputer::INDEPENDENTLYMINIMIZED).X();
      Float_t rCMetY        = rCComp.reducedMETcartesian(ReducedMETComputer::INDEPENDENTLYMINIMIZED).Y();
     //printf("---> %f %f\n",rCMetX,rCMetY);

      rTAComp.compute(gamma,0,nullP4,0, assocChargedMetP4, assocMetP4, assocMetP4 ,true );
      Float_t rTAMet         = rTAComp.reducedMET(ReducedMETComputer::INDEPENDENTLYMINIMIZED);
      Float_t rTAMetL        = rTAComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).second;
      Float_t rTAMetT        = rTAComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).first;
      Float_t rTAMetX        = rTAComp.reducedMETcartesian(ReducedMETComputer::INDEPENDENTLYMINIMIZED).X();
      Float_t rTAMetY        = rTAComp.reducedMETcartesian(ReducedMETComputer::INDEPENDENTLYMINIMIZED).Y();

      rTCComp.compute(gamma,0,nullP4,0, assocChargedMetP4, clusteredMetP4, clusteredMetP4,true );
      Float_t rTCMet         = rTCComp.reducedMET(ReducedMETComputer::INDEPENDENTLYMINIMIZED);
      Float_t rTCMetL        = rTCComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).second;
      Float_t rTCMetT        = rTCComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).first;
      Float_t rTCMetX        = rTCComp.reducedMETcartesian(ReducedMETComputer::INDEPENDENTLYMINIMIZED).X();
      Float_t rTCMetY        = rTCComp.reducedMETcartesian(ReducedMETComputer::INDEPENDENTLYMINIMIZED).Y();

      rACComp.compute(gamma,0,nullP4,0, assocMetP4, clusteredMetP4, clusteredMetP4, true );
      Float_t rACMet         = rACComp.reducedMET(ReducedMETComputer::INDEPENDENTLYMINIMIZED);
      Float_t rACMetL        = rACComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).second;
      Float_t rACMetT        = rACComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).first;
      Float_t rACMetX        = rACComp.reducedMETcartesian(ReducedMETComputer::INDEPENDENTLYMINIMIZED).X();
      Float_t rACMetY        = rACComp.reducedMETcartesian(ReducedMETComputer::INDEPENDENTLYMINIMIZED).Y();

      r3Comp.compute(gamma,0,nullP4, 0, metP4, assocMetP4, clusteredMetP4,true );
      Float_t r3Met         = r3Comp.reducedMET(ReducedMETComputer::INDEPENDENTLYMINIMIZED);
      Float_t r3MetL        = r3Comp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).second;
      Float_t r3MetT        = r3Comp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).first;
      Float_t r3MetX        = r3Comp.reducedMETcartesian(ReducedMETComputer::INDEPENDENTLYMINIMIZED).X();
      Float_t r3MetY        = r3Comp.reducedMETcartesian(ReducedMETComputer::INDEPENDENTLYMINIMIZED).Y();

      rmAComp.compute(gamma,0,nullP4, 0, min(metP4,assocMetP4), clusteredMetP4, clusteredMetP4,true );
      Float_t rmAMet         = rmAComp.reducedMET(ReducedMETComputer::INDEPENDENTLYMINIMIZED);
      Float_t rmAMetL        = rmAComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).second;
      Float_t rmAMetT        = rmAComp.reducedMETComponents(ReducedMETComputer::INDEPENDENTLYMINIMIZED).first;
      Float_t rmAMetX        = rmAComp.reducedMETcartesian(ReducedMETComputer::INDEPENDENTLYMINIMIZED).X();
      Float_t rmAMetY        = rmAComp.reducedMETcartesian(ReducedMETComputer::INDEPENDENTLYMINIMIZED).Y();

      Float_t cleanMet            = cleanMetP4.pt();
      Float_t centralMet          = centralMetP4.pt();

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
      metTypeValues["redMet"]              = LorentzVector(redMetX,redMetY,0,redMet); 
      metTypeValues["redAssocChargedMet"]  = LorentzVector(rTMetX,rTMetY,0,rTMet);
      metTypeValues["redAssocMet"]         = LorentzVector(rAMetX ,rAMetY ,0,rAMet );
      metTypeValues["redClusteredMet"]     = LorentzVector(rCMetX ,rCMetY ,0,rCMet );
      metTypeValues["redTAssocMet"]        = LorentzVector(rTAMetX ,rTAMetY ,0,rTAMet );
      metTypeValues["redTClusteredMet"]    = LorentzVector(rTCMetX ,rTCMetY ,0,rTCMet );
      metTypeValues["redAClusteredMet"]    = LorentzVector(rACMetX  ,rACMetY  ,0,rACMet  );
      metTypeValues["red3Met"]             = LorentzVector(r3MetX   ,r3MetY   ,0,r3Met   );
      metTypeValues["redminAssocMet"]      = LorentzVector(rmAMetX  ,rmAMetY  ,0,rmAMet  );
      
      double dphivmet(deltaPhi(metP4.phi(),gamma.phi()));

      double mindphijmet(9999.);
      for(size_t ijet=0; ijet<jetsp4.size(); ijet++)
        {
	  if(jetsp4[ijet].pt()<minJetPt || fabs(jetsp4[ijet].eta())>2.5)continue;
	  double dphijmet=fabs(deltaPhi(metP4.phi(),jetsp4[ijet].phi()));
	  mindphijmet = min(mindphijmet,dphijmet);
	}

      Float_t mt         = mtComp.compute(gamma,metP4,true);

      bool pass250( fabs(mindphijmet)>0.47 && metP4.pt()>59  && mt>222 && mt<272);
      bool pass300( fabs(mindphijmet)>0.33 && metP4.pt()>76  && mt>264 && mt<331);
      bool pass350( fabs(mindphijmet)>0.21 && metP4.pt()>95  && mt>298 && mt<393);
      bool pass400( fabs(mindphijmet)>0.12 && metP4.pt()>115 && mt>327 && mt<460);
      bool pass450( fabs(mindphijmet)>0.   && metP4.pt()>133 && mt>354 && mt<531);
      bool pass500( fabs(mindphijmet)>0.   && metP4.pt()>148 && mt>382 && mt<605);
      bool pass550( fabs(mindphijmet)>0.   && metP4.pt()>157 && mt>413 && mt<684);
      bool pass600( fabs(mindphijmet)>0.   && metP4.pt()>159 && mt>452 && mt<767);

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
		  if(isGammaEvent) iweight*=qtWeights[dilCats[idc]];

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



TH1F *getZMassHisto()
{
  TH1F *h7 = new TH1F("h7","Z/#gamma^{*}+jets#rightarrow ll",100,60,122);
  h7->SetBinContent(2,224.5647);
  h7->SetBinContent(3,554.0167);
  h7->SetBinContent(4,566.5224);
  h7->SetBinContent(5,552.7701);
  h7->SetBinContent(6,587.4919);
  h7->SetBinContent(7,610.8146);
  h7->SetBinContent(8,616.3866);
  h7->SetBinContent(9,616.8841);
  h7->SetBinContent(10,621.0645);
  h7->SetBinContent(11,655.2755);
  h7->SetBinContent(12,662.3901);
  h7->SetBinContent(13,691.3835);
  h7->SetBinContent(14,718.0921);
  h7->SetBinContent(15,710.9708);
  h7->SetBinContent(16,748.7878);
  h7->SetBinContent(17,768.9978);
  h7->SetBinContent(18,828.2835);
  h7->SetBinContent(19,855.2431);
  h7->SetBinContent(20,889.053);
  h7->SetBinContent(21,917.1881);
  h7->SetBinContent(22,970.9272);
  h7->SetBinContent(23,1031.813);
  h7->SetBinContent(24,1046.726);
  h7->SetBinContent(25,1126.527);
  h7->SetBinContent(26,1207.563);
  h7->SetBinContent(27,1273.453);
  h7->SetBinContent(28,1425.611);
  h7->SetBinContent(29,1544.184);
  h7->SetBinContent(30,1618.689);
  h7->SetBinContent(31,1746.936);
  h7->SetBinContent(32,1953.294);
  h7->SetBinContent(33,2162.93);
  h7->SetBinContent(34,2427.241);
  h7->SetBinContent(35,2699.189);
  h7->SetBinContent(36,3063.189);
  h7->SetBinContent(37,3487.09);
  h7->SetBinContent(38,4049.625);
  h7->SetBinContent(39,4698.98);
  h7->SetBinContent(40,5681.619);
  h7->SetBinContent(41,6748.15);
  h7->SetBinContent(42,8294.227);
  h7->SetBinContent(43,10273.09);
  h7->SetBinContent(44,13067.44);
  h7->SetBinContent(45,16761.06);
  h7->SetBinContent(46,22010.14);
  h7->SetBinContent(47,28787.83);
  h7->SetBinContent(48,37092.34);
  h7->SetBinContent(49,45319.52);
  h7->SetBinContent(50,50338.48);
  h7->SetBinContent(51,50795.45);
  h7->SetBinContent(52,45510.67);
  h7->SetBinContent(53,37119.49);
  h7->SetBinContent(54,28424.62);
  h7->SetBinContent(55,20781.35);
  h7->SetBinContent(56,15177.01);
  h7->SetBinContent(57,11095.01);
  h7->SetBinContent(58,8337.408);
  h7->SetBinContent(59,6395.614);
  h7->SetBinContent(60,4982.944);
  h7->SetBinContent(61,4076.928);
  h7->SetBinContent(62,3297.828);
  h7->SetBinContent(63,2729.356);
  h7->SetBinContent(64,2307.547);
  h7->SetBinContent(65,1985.329);
  h7->SetBinContent(66,1668.291);
  h7->SetBinContent(67,1501.595);
  h7->SetBinContent(68,1293.719);
  h7->SetBinContent(69,1140.973);
  h7->SetBinContent(70,1012.907);
  h7->SetBinContent(71,933.1086);
  h7->SetBinContent(72,814.4987);
  h7->SetBinContent(73,723.5501);
  h7->SetBinContent(74,711.6543);
  h7->SetBinContent(75,615.81);
  h7->SetBinContent(76,576.3617);
  h7->SetBinContent(77,550.0873);
  h7->SetBinContent(78,476.4019);
  h7->SetBinContent(79,479.9992);
  h7->SetBinContent(80,462.6855);
  h7->SetBinContent(81,398.6108);
  h7->SetBinContent(82,381.3881);
  h7->SetBinContent(83,356.9314);
  h7->SetBinContent(84,347.377);
  h7->SetBinContent(85,315.4952);
  h7->SetBinContent(86,285.8653);
  h7->SetBinContent(87,284.8145);
  h7->SetBinContent(88,269.4411);
  h7->SetBinContent(89,259.0076);
  h7->SetBinContent(90,235.7017);
  h7->SetBinContent(91,212.6829);
  h7->SetBinContent(92,210.0757);
  h7->SetBinContent(93,220.422);
  h7->SetBinContent(94,201.8417);
  h7->SetBinContent(95,191.1922);
  h7->SetBinContent(96,201.9856);
  h7->SetBinContent(97,164.9426);
  h7->SetBinContent(98,173.8608);
  h7->SetBinContent(99,58.11536);
  return h7;
}
