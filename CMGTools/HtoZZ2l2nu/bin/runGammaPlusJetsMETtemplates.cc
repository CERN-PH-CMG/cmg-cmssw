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

TString getJetRegion(float eta)
{
  TString reg("TK");
  if(fabs(eta)>2.5)  reg="HEin";
  if(fabs(eta)>2.75) reg="HEout";
  if(fabs(eta)>3)    reg="HF";
  return reg;
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

  bool use2011Id = runProcess.getParameter<bool>("is2011");
  cout << "Note: will apply " << (use2011Id ? 2011 : 2012) << " version of the id's" << endl;

  TString url=runProcess.getParameter<std::string>("input");
  TString outdir=runProcess.getParameter<std::string>("outdir");
  bool isMC = runProcess.getParameter<bool>("isMC");
  bool runBlinded = runProcess.getParameter<bool>("runBlinded");
  int mctruthmode = runProcess.getParameter<int>("mctruthmode");
  TString dirname = runProcess.getParameter<std::string>("dirName");
  TString uncFile =  runProcess.getParameter<std::string>("jesUncFileName"); gSystem->ExpandPathName(uncFile);
  JetCorrectionUncertainty jecUnc(uncFile.Data());

  TRandom2 rndGen;
  EventCategory eventCategoryInst(3);
  GammaEventHandler gammaEvHandler(runProcess);  

  //book histograms
  SmartSelectionMonitor mon;

  TH1F* Hcutflow     = (TH1F*) mon.addHistogram(  new TH1F ("cutflow"    , "cutflow"    ,6,0,6) ) ;

  //##############################################
  //######## STUFF FOR CUTS OPTIMIZATION  ########
  //##############################################

   std::vector<double> optim_Cuts1_met; 
   std::vector<double> optim_Cuts1_mtmin;
   std::vector<double> optim_Cuts1_mtmax;
   for(double met=50;met<140;met+=2.5){
//       if(met>100 && int(met)%5!=0)continue;
//         for(double mtmin_=140;mtmin_<550;mtmin_+=10){
//            double mtmin = mtmin_;
//            if(mtmin<=140)mtmin=0;
//          if(mtmin>350 && int(mtmin)%20!=0)continue;
//            for(double mtmax=mtmin+50;mtmax<mtmin+450;mtmax+=10){
//               if(mtmax>=mtmin+445)mtmax=3000;
//               if(mtmin==0 && mtmax!=3000)continue;
//             if(mtmin>350 && int(mtmax)%20!=0)continue;
//               if(mtmax-mtmin>200 && int(mtmax)%50!=0)continue;
               optim_Cuts1_met    .push_back(met);
               optim_Cuts1_mtmin  .push_back(0);//mtmin);
               optim_Cuts1_mtmax  .push_back(9999);//mtmax);
//            }
//      }
  }
  //add last year cut
/*  optim_Cuts1_met.push_back( 70); optim_Cuts1_mtmin.push_back(229); optim_Cuts1_mtmax.push_back(258);
  optim_Cuts1_met.push_back( 77); optim_Cuts1_mtmin.push_back(245); optim_Cuts1_mtmax.push_back(293);
  optim_Cuts1_met.push_back( 84); optim_Cuts1_mtmin.push_back(260); optim_Cuts1_mtmax.push_back(328);
  optim_Cuts1_met.push_back( 91); optim_Cuts1_mtmin.push_back(276); optim_Cuts1_mtmax.push_back(364);
  optim_Cuts1_met.push_back( 98); optim_Cuts1_mtmin.push_back(292); optim_Cuts1_mtmax.push_back(399);
  optim_Cuts1_met.push_back(105); optim_Cuts1_mtmin.push_back(308); optim_Cuts1_mtmax.push_back(434);
  optim_Cuts1_met.push_back(112); optim_Cuts1_mtmin.push_back(323); optim_Cuts1_mtmax.push_back(470);
  optim_Cuts1_met.push_back(119); optim_Cuts1_mtmin.push_back(339); optim_Cuts1_mtmax.push_back(505);
  optim_Cuts1_met.push_back(126); optim_Cuts1_mtmin.push_back(355); optim_Cuts1_mtmax.push_back(540);
  optim_Cuts1_met.push_back(133); optim_Cuts1_mtmin.push_back(370); optim_Cuts1_mtmax.push_back(576);
  optim_Cuts1_met.push_back(140); optim_Cuts1_mtmin.push_back(386); optim_Cuts1_mtmax.push_back(611);
  optim_Cuts1_met.push_back(147); optim_Cuts1_mtmin.push_back(402); optim_Cuts1_mtmax.push_back(646);
  optim_Cuts1_met.push_back(154); optim_Cuts1_mtmin.push_back(417); optim_Cuts1_mtmax.push_back(682);
  optim_Cuts1_met.push_back(161); optim_Cuts1_mtmin.push_back(433); optim_Cuts1_mtmax.push_back(717);
  optim_Cuts1_met.push_back(168); optim_Cuts1_mtmin.push_back(449); optim_Cuts1_mtmax.push_back(752);
*/
   
   TH1F* Hoptim_cuts1_met     =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_met"    , ";cut index;met"    ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
   TH1F* Hoptim_cuts1_mtmin   =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_mtmin"  , ";cut index;mtmin"  ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
   TH1F* Hoptim_cuts1_mtmax   =  (TH1F*) mon.addHistogram( new TH1F ("optim_cut1_mtmax"  , ";cut index;mtmax"  ,optim_Cuts1_met.size(),0,optim_Cuts1_met.size()) ) ;
   const size_t nOptimCuts=optim_Cuts1_met.size();
   for(unsigned int index=0;index<nOptimCuts;index++){
      Hoptim_cuts1_met    ->Fill(index, optim_Cuts1_met[index]);    
      Hoptim_cuts1_mtmin  ->Fill(index, optim_Cuts1_mtmin[index]);
      Hoptim_cuts1_mtmax  ->Fill(index, optim_Cuts1_mtmax[index]);
   }


  //##############################################
  //######## CONTROL PLOTS FOR SELECTION #########
  //##############################################

  mon.addHistogram( new TH1F ("ht",     ";H_{T};Events",50,0,500) );
  mon.addHistogram( new TH1F ("ht50",     ";H_{T};Events",50,0,500) );
  mon.addHistogram( new TH1F ("clusteredmet",     ";Clustered E_{T}^{miss} [GeV];Events",50,0,250) );
  mon.addHistogram( new TH1F ("unclusteredmet",     ";Unclustered E_{T}^{miss} [GeV];Events",50,0,250) );
  mon.addHistogram( new TH1F ("mht",     ";MHT [GeV];Events",50,0,250) );
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
  
  mon.addHistogram( new TH1F ("r9", ";R9;Events", 100,0.8,1) );
  mon.addHistogram( new TH1F ("sietaieta", ";#sigma i#eta i#eta;Events", 100,0,0.03) );
  mon.addHistogram( new TH1F( "mindphijmet", ";min #Delta#phi(jet,E_{T}^{miss});Events",40,0,4) );
  mon.addHistogram( new TH1F( "dphibmet", ";min #Delta#phi(boson,E_{T}^{miss});Events",40,0,4) );
  mon.addHistogram( new TH1D( "balance", ";E_{T}^{miss}/q_{T};Events", 25,0,2.5) );
  mon.addHistogram( new TH1F ("trkveto", ";Track veto;Events", 2,0,2) );
  
  //  mon.addHistogram( new TH1D( "qt",        ";p_{T}^{#gamma} [GeV/c];Events / (2.5 GeV/c)", 100,0,500) );
  Float_t qtaxis[100];
  for(size_t i=0; i<40; i++)  qtaxis[i]=2.5*i;       //0-97.5
  for(size_t i=0; i<20; i++)  qtaxis[40+i]=100+5*i;  //100-195
  for(size_t i=0; i<15; i++)  qtaxis[60+i]=200+10*i; //200-340
  for(size_t i=0; i<25; i++)  qtaxis[75+i]=350+25*i; //350-976
  mon.addHistogram( new TH1D( "qt",        ";p_{T}^{#gamma} [GeV/c];Events / (2.5 GeV/c)",99,qtaxis));
  mon.addHistogram( new TH1D( "metoverqt", ";E_{T}^{miss}/p_{T}^{#gamma} [GeV/c];Events", 25,0,2.5) );
  mon.addHistogram( new TH1D( "eta",       ";#eta;Events", 50,0,2.6) );  
  mon.addHistogram( new TH1F( "nvtx", ";Vertices;Events", 50,0,50) );  
  mon.addHistogram( new TH1F( "rho", ";#rho;Events", 50,0,25) );  
  mon.addHistogram( new TH1F( "mt"  , ";M_{T};Events", 100,0,1000) );
  mon.addHistogram( new TH1F( "mtevetounsafe"  , ";M_{T};Events", 100,0,1000) );
  mon.addHistogram( new TH1F( "met_phi"  , ";#phi [rad];Events", 50,0,3.5) );
  mon.addHistogram( new TH1F( "met_rawmet"  , ";E_{T}^{miss} (raw);Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_met"  , ";E_{T}^{miss};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_min3Met"  , ";min(E_{T}^{miss},assoc-E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_redMet"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_redMetL"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss}) - longi.;Events", 50,-250,250) );
  mon.addHistogram( new TH1F( "met_redMetT"  , ";red(E_{T}^{miss},clustered-E_{T}^{miss}) - perp.;Events", 50,-250,250) );
  mon.addHistogram( new TH1F( "zmass", ";M^{ll};Events", 15,76,106) );
  mon.addHistogram( new TH2F( "met_met_vspu"       , ";Vertices;E_{T}^{miss};Events", 50,0,50,50,0,500) );
  mon.addHistogram( new TH2F( "met_min3Met_vspu"       , ";Vertices;min(E_{T}^{miss},assoc-E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,50,50,0,500) );
  mon.addHistogram( new TH2F( "met_redMet_vspu"       , ";Vertices;red(E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,50,50,0,500) );
  
  mon.addHistogram( new TH2F ("mt_shapes", ";cut index;M_{T} [GeV/c^{2}];",nOptimCuts,0,nOptimCuts, 160,150,950) );  
  

  //jet id efficiencies
  TString jetRegs[]={"TK","HEin","HEout","HF"};
  const size_t nJetRegs=sizeof(jetRegs)/sizeof(TString);
  TString jetIds[]={"pfloose","pftight","minloose","minmedium"};
  const size_t nJetIds=sizeof(jetIds)/sizeof(TString);
  Double_t jetPtBins[]={0,15,20,25,30,40,50,60,70,80,100,200,300,400,500,600,700,1000};
  Int_t nJetPtBins=sizeof(jetPtBins)/sizeof(Double_t)-1;
  Double_t jetEtaBins[]={0,0.25,0.5,0.75,1.0,1.25,1.5,1.75,2.0,2.25,2.625,2.75,2.875,3.0,3.5,4.0,4.5,5.0};
  Double_t nJetEtaBins=sizeof(jetEtaBins)/sizeof(Double_t)-1;
  for(size_t ireg=0; ireg<nJetRegs; ireg++)
    {
      mon.addHistogram( new TH1F(jetRegs[ireg]+"pfjetbeta"    , ";#beta;Events",50,0,1) );
      mon.addHistogram( new TH1F(jetRegs[ireg]+"pfjetbetastar", ";#beta *;Events",50,0,1) );
      mon.addHistogram( new TH1F(jetRegs[ireg]+"pfjetdrmean"  , ";<#Delta R>;Events",50,0,0.5) );
      mon.addHistogram( new TH1F(jetRegs[ireg]+"pfjetptd"     , ";p_{T}D [GeV/c];Events",50,0,1) );
      mon.addHistogram( new TH1F(jetRegs[ireg]+"pfjetptrms"   , ";RMS p_{T} [GeV/c];Events",50,0,0.5) );
      mon.addHistogram( new TH1F(jetRegs[ireg]+"pfjetmva"     , ";MVA;Events",50,-1,1) );
      mon.addHistogram( new TH1F(jetRegs[ireg]+"pfjetbalance", ";Jet p_{T} / Boson p_{T};Events", 25,0,2.5) );
      if(ireg==0) mon.addHistogram( new TH1F("pfjetbalance", ";Jet p_{T} / Boson p_{T};Events", 25,0,2.5) );
      for(size_t iid=0; iid<nJetIds; iid++)
	{
	  mon.addHistogram( new TH1F( jetRegs[ireg]+jetIds[iid]+"pfjetpt", ";Jet p_{T} [GeV/c];Jets", nJetPtBins,jetPtBins) );
	  mon.addHistogram( new TH1F( jetRegs[ireg]+"true"+jetIds[iid]+"pfjetpt", ";Jet p_{T} [GeV/c];Jets", nJetPtBins,jetPtBins) );
	  if(ireg==0)
	    {
	      mon.addHistogram( new TH1F( jetIds[iid]+"pfjeteta", ";Jet |#eta|;Jets", nJetEtaBins, jetEtaBins) );
	      mon.addHistogram( new TH1F( "true"+jetIds[iid]+"pfjeteta", ";Jet |#eta|;Jets", nJetEtaBins, jetEtaBins) );
	    }
	}
    }
  
  //VBF control
  mon.addHistogram( new TH1F("pfvbfcandjeteta"       , ";#eta;Jets",50,0,5) );
  mon.addHistogram( new TH1F("pfvbfcandjetdeta"       , ";|#Delta #eta|;Jets",50,0,10) );
  mon.addHistogram( new TH1F("pfvbfcandjetpt"       , ";p_{T} [GeV/c];Jets",50,0,250) );
  mon.addHistogram( new TH1F("pfvbfcandzeppenfeld"       , ";|Zeppenfeld variable|;Events",50,0,5) );
  mon.addHistogram( new TH1F("pfvbfhardpt"       , ";Hard p_{T} [GeV/c];Events",25,0,250) );
  mon.addHistogram( new TH1F("pfvbfhardptfinal"       , ";Hard p_{T} [GeV/c];Events",25,0,250) );
  TH1 *h=mon.addHistogram( new TH1F("pfvbfcjv"       , ";Central jet count;Events",3,0,3) );
  h->GetXaxis()->SetBinLabel(1,"=0 jets");
  h->GetXaxis()->SetBinLabel(2,"=1 jets");
  h->GetXaxis()->SetBinLabel(3,"#geq 2 jets");
  mon.addHistogram( new TH1F("pfvbfhtcjv"       , ";Central jet H_{T} [GeV/c];Events",50,0,250) );
  mon.addHistogram( new TH1F("pfvbfpremjj"       , ";M(jet_{1},jet_{2}) [GeV/c^{2}];Events",40,0,2000) );
  mon.addHistogram( new TH1F("pfvbfmjj"       , ";M(jet_{1},jet_{2}) [GeV/c^{2}];Events",40,0,2000) );
  
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

  //duplicate checker
  DuplicatesChecker duplicatesChecker;
  int NumberOfDuplicated(0);

  //pileup reweighting
  bool disableJERSmear(false);
  std::vector<double> dataPileupDistributionDouble = runProcess.getParameter< std::vector<double> >("datapileup");
  std::vector<float> dataPileupDistribution; 
  for(unsigned int i=0;i<dataPileupDistributionDouble.size();i++){dataPileupDistribution.push_back(dataPileupDistributionDouble[i]);}
  std::vector<float> mcPileupDistribution;
  bool useObservedPU(use2011Id);
  if(isMC)
    {
      TString puDist("evAnalyzer/h2zz/pileuptrue");
      if(useObservedPU) puDist="evAnalyzer/h2zz/pileup";
      TH1F* histo = (TH1F *) file->Get(puDist);
      if(!histo)std::cout<<"pileup histogram is null!!!\n";
      for(int i=1;i<=histo->GetNbinsX();i++){mcPileupDistribution.push_back(histo->GetBinContent(i));}
      delete histo;
      if(dataPileupDistribution.size()==0) 
	{ 
	  dataPileupDistribution=mcPileupDistribution; disableJERSmear=true; 
	  cout << "No PU reweighting or JER smearing will be applied" << endl;
	}
    }
  else mcPileupDistribution=dataPileupDistribution;
  while(mcPileupDistribution.size()<dataPileupDistribution.size())  mcPileupDistribution.push_back(0.0);
  while(mcPileupDistribution.size()>dataPileupDistribution.size())  dataPileupDistribution.push_back(0.0);

  gROOT->cd();  //THIS LINE IS NEEDED TO MAKE SURE THAT HISTOGRAM INTERNALLY PRODUCED IN LumiReWeighting ARE NOT DESTROYED WHEN CLOSING THE FILE
  edm::LumiReWeighting LumiWeights(mcPileupDistribution,dataPileupDistribution);
   
  //run the analysis
  int evStart(0),evEnd(evSummaryHandler.getEntries());
  for( int iev=0; iev<evEnd; iev++)
    {
      if(iev%1000==0) printf("\r [ %d/100 ] ",int(100*float(iev-evStart)/float(evEnd)));
      evSummaryHandler.getEntry(iev);
      
      //interpret event
      ZZ2l2nuSummary_t &ev = evSummaryHandler.getEvent();
      PhysicsEvent_t phys  = getPhysicsEventFrom(ev);
      bool hasEEtrigger = ev.triggerType & 0x1;
      bool hasMMtrigger = (ev.triggerType >> 1 ) & 0x1;

      bool isGammaEvent    = gammaEvHandler.isGood(phys);
      if(duplicatesChecker.isDuplicate(ev.run,ev.lumi, ev.event)) {  NumberOfDuplicated++; continue; }

      //event weight
      float weight = 1.0;  
      if(isMC)                  { weight = LumiWeights.weight( ev.ngenITpu ); }
      //if(!isMC && isGammaEvent) { weight = gammaEvHandler.getTriggerPrescaleWeight(); }
      Hcutflow->Fill(1,1);
      Hcutflow->Fill(2,weight);
      Hcutflow->Fill(3,weight);
      Hcutflow->Fill(4,weight);
      Hcutflow->Fill(5,1);

      //check which event type is required to use (dilepton/or photon)
      if(mctruthmode==22 && !isGammaEvent ) continue;
      if(mctruthmode==1  && isGammaEvent) continue;
      if(!isGammaEvent && ev.cat != EE && ev.cat !=MUMU) continue;
      std::vector<TString> dilCats;
      
      //build the gamma candidate
      LorentzVector gamma(0,0,0,0);
      float r9(0),sietaieta(0);
      bool hasTrkVeto(false),isConv(false),hasElectronVeto(false),hasConvUnsafeElectronVeto(false);
      if(isGammaEvent)
	{
	  dilCats.push_back("ee");
	  dilCats.push_back("mumu");
	  r9               = phys.gammas[0].r9*(isMC ? 1.005 : 1.0);
	  sietaieta        = phys.gammas[0].sihih;
	  hasElectronVeto  = phys.gammas[0].hasElectronVeto;
	  hasConvUnsafeElectronVeto = phys.gammas[0].hasConvUnsafeElectronVeto;
	  hasTrkVeto       = phys.gammas[0].hasCtfTrkVeto;
	  gamma            = phys.gammas[0];
	  isConv=(phys.gammas[0].isConv);
	}
      else
	{
	  //check if dilepton is good
	  bool isGood(true);
	  for(size_t ilep=0; ilep<2; ilep++)
	    {
	      gamma += phys.leptons[ilep];
	      int lpid=phys.leptons[ilep].pid;
	      if(fabs(phys.leptons[ilep].id)==13)
		{
		  if(use2011Id)
		    {
		      float relIso=phys.leptons[ilep].relIsoRho(ev.rho);
		      if( !hasObjectId(ev.mn_idbits[lpid], MID_VBTF2011) || relIso>0.15) isGood=false;
		    }
		  else
		    {
		      float relIso=phys.leptons[ilep].pfRelIsoDbeta();
		      if( !hasObjectId(ev.mn_idbits[lpid], MID_LOOSE) || relIso>0.2) isGood=false;
		    }
		}
	      if(fabs(phys.leptons[ilep].id)==11)
		{
		  if(use2011Id)
		    {
		      float relIso=phys.leptons[ilep].relIsoRho(ev.rho);
		      if(!hasObjectId(ev.en_idbits[lpid], EID_VBTF2011) || relIso>0.1) isGood=false;
		    }
		  else
		    {
		      float relIso=phys.leptons[ilep].ePFRelIsoCorrected2012(ev.rho);
		      if(!hasObjectId(ev.en_idbits[lpid],EID_MEDIUM) || relIso>0.15) isGood=false;
		    }
		}
	    }
	  if(!isGood) continue;
	  if(fabs(gamma.mass()-91)>15) continue;
	  if(ev.cat==MUMU) dilCats.push_back("mumu");
	  if(ev.cat==EE)   dilCats.push_back("ee");
	}

      //
      // EXTRA LEPTONS
      //
      int nextraleptons(0);
      for(size_t ilep=(isGammaEvent ? 0 : 2); ilep<phys.leptons.size(); ilep++)
	{
	  int lpid=phys.leptons[ilep].pid;

	  //FIXME: looks like sometimes the gamma comes double counted as a soft-id electron ???
	  if(isGammaEvent && fabs(phys.leptons[ilep].id)==11)
	    {
	      double dr( deltaR(phys.leptons[ilep],gamma) );
	      if(dr<0.1) continue;
	    }

	  if(fabs(phys.leptons[ilep].id)==13)
	    {
	      if(use2011Id)
		{
		  if( (hasObjectId(ev.mn_idbits[lpid], MID_VBTF2011) && phys.leptons[ilep].relIsoRho(ev.rho)<0.15 && phys.leptons[ilep].pt()>10)
		      || (hasObjectId(ev.mn_idbits[lpid], MID_SOFT2011) && phys.leptons[ilep].pt()>3) )
		    nextraleptons++;
		}
	      else
		{
		  if(  (hasObjectId(ev.mn_idbits[lpid], MID_LOOSE) && phys.leptons[ilep].pfRelIsoDbeta()<0.2)
		       || (hasObjectId(ev.mn_idbits[lpid], MID_SOFT) && phys.leptons[ilep].pt()>3) )
		    nextraleptons++;
		}
	    }
	  if(fabs(phys.leptons[ilep].id)==11)
	    {
	      if(use2011Id)
		{
		  if( hasObjectId(ev.en_idbits[lpid],EID_VBTF2011) && phys.leptons[ilep].relIsoRho(ev.rho)<0.1 && phys.leptons[ilep].pt()>10) nextraleptons++;
		}
	      else
		{
		  if( hasObjectId(ev.en_idbits[lpid],EID_VETO) && phys.leptons[ilep].ePFRelIsoCorrected2012(ev.rho)<0.15 && phys.leptons[ilep].pt()>10) nextraleptons++;
		}
	    }
	}
      
      //
      // JET/MET
      //
      LorentzVector metP4(phys.met[0]);
      LorentzVectorCollection zvvs;
      std::vector<PhysicsObjectJetCollection> jets;
      METUtils::computeVariation(phys.ajets, phys.leptons, metP4, jets, zvvs, &jecUnc);
      metP4=zvvs[0];
      
      //count the jets
      int nbtags(0),njets30(0),njets15(0);
      double ht(0);
      double mindphijmet(9999.),mindphijmet15(9999.);
      PhysicsObjectJetCollection selJets,recoilJets;
      LorentzVector clusteredMet(gamma);  clusteredMet *= -1;
      LorentzVector mht(0,0,0,0),unclusteredMet(0,0,0,0);
      PhysicsObjectJetCollection & jetsToUse=jets[0];
      if(disableJERSmear) jetsToUse=phys.ajets;
      for(size_t ijet=0; ijet<jetsToUse.size(); ijet++)
        {
	  LorentzVector ijetP4=jetsToUse[ijet];
	  if(ijetP4.pt()<15) continue;
	  if(isGammaEvent && deltaR(ijetP4,gamma)<0.2) continue;
	  
	  //condition for recoil jet
	  if( fabs(deltaPhi(ijetP4.phi(),gamma.phi())>2 ) ) recoilJets.push_back( jetsToUse[ijet] );
	  
	  njets15++;
	  selJets.push_back(ijetP4);
	  clusteredMet -= ijetP4;
	  mht -= ijetP4;
	  ht += ijetP4.pt();

	  //dphi(jet,MET)
	  double idphijmet=fabs(deltaPhi(metP4.phi(),ijetP4.phi()));	  
	  mindphijmet15=min(idphijmet,mindphijmet15);
	  
	  if(ijetP4.pt()<30) continue;
	  njets30++;
	  mindphijmet=min(idphijmet,mindphijmet);
	  
	  //b-tag
	  if(fabs(ijetP4.eta())<2.5) nbtags += (jetsToUse[ijet].btag3>0.275);

// 	  TString reg=getJetRegion(jets[0][ijet].eta());
// 	  mon.fillHisto(reg+"pfjetbeta",     tags_full,jets[0][ijet].beta,     weight);
// 	  mon.fillHisto(reg+"pfjetbetastar", tags_full,jets[0][ijet].betaStar, weight);
// 	  mon.fillHisto(reg+"pfjetdrmean",   tags_full,jets[0][ijet].dRMean,   weight);
// 	  mon.fillHisto(reg+"pfjetptd",      tags_full,jets[0][ijet].ptD,      weight);
// 	  mon.fillHisto(reg+"pfjetptrms",    tags_full,jets[0][ijet].ptRMS,    weight);
// 	  mon.fillHisto(reg+"pfjetmva",      tags_full,jets[0][ijet].pumva,    weight);
	}
            
      //other mets
      unclusteredMet = metP4-clusteredMet;
      LorentzVector nullP4(0,0,0,0);
      LorentzVector assocMetP4 = phys.met[1];
      LorentzVector min3Met=min(metP4, min(assocMetP4,clusteredMet)) ;
      METUtils::stRedMET redMetOut;
      LorentzVector redMet(METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, gamma, 0, nullP4, 0, clusteredMet, metP4,true,&redMetOut));
      double redMetL=redMetOut.redMET_l;
      double redMetT=redMetOut.redMET_t;
        
      //  
      // EVENT SELECTION
      //
      bool passMultiplicityVetoes (isGammaEvent ? (nextraleptons==0 /*&& phys.gammas.size()==1*/) : (nextraleptons==0) );
      bool passKinematics         (gamma.pt()>55); //30);
      if(isGammaEvent && !isMC)    passKinematics &= (gamma.pt()>gammaEvHandler. triggerThr());
      bool passEB                 (!isGammaEvent || fabs(gamma.eta())<1.4442); // (fabs(gamma.eta())<1.4442); 
      bool passR9                 (!isGammaEvent || r9<1.0);
      bool passR9tight            (true); //!isGammaEvent || r9>0.85); 
      bool passBveto              (nbtags==0);
      bool passMinDphiJmet        (mindphijmet>0.5);
      if(njets30==0)  passMinDphiJmet=(mindphijmet15>0.5);
      bool passSMZZpreSel(njets30==0 && passBveto && passMinDphiJmet);  
      bool passSMZZredMet(redMet.pt()>50);
      bool passSMZZbalance(metP4.pt()/gamma.pt() > 0.4 && metP4.pt()/gamma.pt() < 1.8);
      
      //event category
      int eventSubCat    = eventCategoryInst.Get(phys,&selJets);
      TString tag_subcat = eventCategoryInst.GetLabel(eventSubCat); 
      std::vector<TString> subcats;
      subcats.push_back("");
      if(njets30==0)
	{
	  subcats.push_back("eq0jets");
	  if(njets15==0)         subcats.push_back("eq0softjets");
	  else                   subcats.push_back("eq0hardjets");
	} 
      else if(njets30==1)        subcats.push_back("eq1jets");
      else if(tag_subcat=="vbf") subcats.push_back("vbf");
      else if(njets30==2)        { subcats.push_back("eq2jets");  tag_subcat="eq2jets"; }
      else if(njets30>2)         { subcats.push_back("geq3jets"); tag_subcat="geq3jets"; }
      
      //add blinding (dilepton only)
      bool mustBlind = (!isMC && !isGammaEvent && runBlinded && evSummaryHandler.hasSpoilerAlert(!isMC));
      bool hasVbfBlinding(!isMC && !isGammaEvent && runBlinded && tag_subcat=="vbf" && zvvs[0].pt()>70);
      mustBlind |= hasVbfBlinding;

      //now do the control plots
      std::map<TString, float> qtWeights;
      if(isGammaEvent) qtWeights = gammaEvHandler.getWeights(phys,tag_subcat);
     
      //now do the control plots
      if(!passMultiplicityVetoes) continue;
      if(!passEB || !passR9 /*|| isConv*/) continue;
      if(!passKinematics) continue;
      for(size_t idc=0; idc<dilCats.size(); idc++)
	{
	  LorentzVector iboson(isGammaEvent ? gammaEvHandler.massiveGamma(dilCats[idc]) : gamma);
	  float zmass=iboson.mass();
	  Float_t mt( METUtils::transverseMass(iboson,metP4,true) );

	  float iweight=weight;
	  if(isGammaEvent) iweight*=qtWeights[dilCats[idc]];
	  for(size_t isc=0; isc<subcats.size(); isc++)
	    {
	      TString ctf=dilCats[idc]+subcats[isc];	      
	  
	      if(isGammaEvent)  
		{
		  mon.fillHisto("r9",        ctf, r9,         iweight);
		  mon.fillHisto("sietaieta", ctf, sietaieta,  iweight);
		  mon.fillHisto("trkveto",   ctf, hasTrkVeto, iweight);
		  if(!hasConvUnsafeElectronVeto) mon.fillHisto("mtevetounsafe", ctf, mt,iweight);
		}
	      
	      if(hasElectronVeto || !passR9tight) continue;
	      //if(hasElectronVeto || hasTrkVeto || !passR9tight) continue;
	      
	      mon.fillHisto("nbtags",ctf, nbtags,iweight);
	      if(!passBveto) continue;

	      mon.fillHisto("eta",ctf, fabs(gamma.eta()),iweight);
	      // mon.fillHisto("qt",ctf, gamma.pt(),iweight);
	      mon.fillHisto("qt",ctf, gamma.pt(),iweight,true);
	      mon.fillHisto("nvtx",ctf, ev.nvtx,iweight);
	      mon.fillHisto("rho",ctf, ev.rho,iweight);
	      mon.fillHisto("zmass",ctf, zmass,iweight);

	      mon.fillHisto("njets",ctf, njets30,iweight);
	      mon.fillHisto("ht",ctf, ht,iweight);
	      mon.fillHisto("clusteredmet",ctf,clusteredMet.pt(),iweight);
	      mon.fillHisto("mht",ctf,mht.pt(),iweight);
	      mon.fillHisto("unclusteredmet",ctf,unclusteredMet.pt(),iweight);
	      mon.fillHisto("nsoftjets",ctf, njets15,iweight);
	      
	      //measure jet selection efficiency 
	      if(ctf.Contains("eq1jets") && recoilJets.size()==1)
		{
		  double balance = recoilJets[0].pt()/gamma.pt();
		  bool passBalanceCut(balance>0.5 && balance<1.5);
		  TString etaReg = getJetRegion(recoilJets[0].eta());
		  bool isMatched(isMC && recoilJets[0].genPt>0);
		  std::vector<TString> passIds;
		  if( hasObjectId(recoilJets[0].pid,JETID_LOOSE) )      { passIds.push_back("pfloose");   if(isMatched) passIds.push_back("truepfloose"); }
		  if( hasObjectId(recoilJets[0].pid,JETID_TIGHT) )      { passIds.push_back("pftight");   if(isMatched) passIds.push_back("truepftight"); }
		  if( hasObjectId(recoilJets[0].pid,JETID_MIN_LOOSE) )  { passIds.push_back("minloose");  if(isMatched) passIds.push_back("trueminloose"); }
		  if( hasObjectId(recoilJets[0].pid,JETID_MIN_MEDIUM) ) { passIds.push_back("minmedium"); if(isMatched) passIds.push_back("trueminmedium"); }
		  mon.fillHisto("pfjetbalance",ctf, balance,iweight);
		  mon.fillHisto(etaReg+"pfjetbalance",ctf, balance,iweight);
		  if(passBalanceCut)
		    {
		      for(size_t iids=0; iids<passIds.size(); iids++)
			{
			  mon.fillHisto(etaReg+passIds[iids]+"pfjetpt",ctf,recoilJets[0].pt(),iweight,true);
			  mon.fillHisto(passIds[iids]+"pfjeteta",ctf,fabs(recoilJets[0].eta()),iweight,true);
			}
		    }
		}

	      if(metP4.pt()>50) 
		{
		  mon.fillHisto("mindphijmet",ctf,njets30==0? mindphijmet15:mindphijmet,iweight);	      
		  mon.fillHisto("dphibmet",ctf,fabs(deltaPhi(gamma.phi(),metP4.phi())),iweight);	      
		  mon.fillHisto("ht50",ctf,ht,iweight);	      
		}
		  
	      if(passMinDphiJmet)
		{

		  //VBF monitoring
		  float dphijj(-1),hardpt(-1);
		  if(njets30>=2)
		    {
		      LorentzVector vbfSyst=selJets[0]+selJets[1];
		      LorentzVector hardSyst=vbfSyst+metP4+gamma;
		      hardpt=hardSyst.pt();
		      dphijj=deltaPhi(selJets[0].phi(),selJets[1].phi());
		      double maxEta=max(selJets[0].eta(),selJets[1].eta());
		      double minEta=min(selJets[0].eta(),selJets[1].eta());
		      float avgEtajj=0.5*(maxEta+minEta);
		      float detajj=maxEta-minEta;
		      mon.fillHisto("pfvbfcandjetpt",      ctf, fabs(selJets[0].pt()),iweight);
		      mon.fillHisto("pfvbfcandjetpt",      ctf, fabs(selJets[1].pt()),iweight);
		      mon.fillHisto("pfvbfcandjeteta",     ctf, fabs(maxEta),iweight);
		      mon.fillHisto("pfvbfcandjeteta",     ctf, fabs(minEta),iweight);
		      mon.fillHisto("pfvbfcandjetdeta",    ctf, fabs(detajj),iweight);
		      mon.fillHisto("pfvbfcandzeppenfeld", ctf, fabs(maxEta-avgEtajj)/fabs(detajj),iweight);
		      mon.fillHisto("pfvbfcandzeppenfeld", ctf, fabs(minEta-avgEtajj)/fabs(detajj),iweight);
		      mon.fillHisto("pfvbfpremjj",         ctf, vbfSyst.mass(),iweight);
		      if(fabs(detajj)>4.0)
			{
			  mon.fillHisto("pfvbfmjj",         ctf, vbfSyst.mass(),iweight);
			  mon.fillHisto("pfvbfmjjvsdeta",   ctf, vbfSyst.mass(),fabs(detajj),iweight);
			  mon.fillHisto("pfvbfmjjvshardpt", ctf, vbfSyst.mass(),hardpt,iweight);
			  if(vbfSyst.mass()>500) 
			    {
			      mon.fillHisto("pfvbfhardpt",  ctf, hardpt,iweight);
			      int ncjv(0);
			      float htcjv(0);
			      for(size_t iotherjet=2; iotherjet<selJets.size(); iotherjet++)
				{
				  if(selJets[iotherjet].pt()<30 || selJets[iotherjet].eta()<minEta || selJets[iotherjet].eta()>maxEta) continue;
				  htcjv+= selJets[iotherjet].pt();
				  ncjv++;
				}
			      mon.fillHisto("pfvbfcjv",   ctf, ncjv,iweight);
			      mon.fillHisto("pfvbfhtcjv", ctf, htcjv,iweight);
			    }
			}
		    }
		
		  mon.fillHisto("met_phi",         ctf, fabs(phys.met[0].phi()),iweight);
		  if(mustBlind) continue;  
		  mon.fillHisto("met_rawmet",      ctf, phys.met[0].pt(),iweight);
		  mon.fillHisto("metoverqt",       ctf, metP4.pt()/gamma.pt(),iweight);
		  mon.fillHisto("met_met",         ctf, metP4.pt(),iweight);
		  mon.fillHisto("met_met_vspu",    ctf, ev.nvtx,metP4.pt(),iweight);
		  mon.fillHisto("met_min3Met",     ctf, min3Met.pt(),iweight);
		  mon.fillHisto("met_min3Met_vspu",ctf, ev.nvtx,min3Met.pt(),iweight);
		  mon.fillHisto("met_redMet",      ctf, redMet.pt(),iweight);
		  mon.fillHisto("met_redMet_vspu", ctf, ev.nvtx,redMet.pt(),iweight);
		  mon.fillHisto("met_redMetT",     ctf, redMetL,iweight);
		  mon.fillHisto("met_redMetL",     ctf, redMetT,iweight);
		  mon.fillHisto("mt",              ctf, mt,iweight);
		}
	      
	      if(passSMZZpreSel && !mustBlind)
		{
		  if(passSMZZredMet) mon.fillHisto("metoverqt","ZZ"+ctf, metP4.pt()/gamma.pt(),iweight);
		  if(passSMZZbalance)
		    {
		      mon.fillHisto("met_met","ZZ"+ctf, metP4.pt(),iweight);
		      mon.fillHisto("met_redMet","ZZ"+ctf, redMet.pt(),iweight);
		      if(passSMZZredMet)
			{
			  mon.fillHisto("met_redMetT","ZZ"+ctf, redMetL,iweight);
			  mon.fillHisto("met_redMetL","ZZ"+ctf, redMetT,iweight);
			}
		    }
		}

	      for(unsigned int index=0;index<nOptimCuts;index++){
		if ( index<nOptimCuts-1 && zvvs[0].pt()>optim_Cuts1_met[index] && mt>optim_Cuts1_mtmin[index] && mt<optim_Cuts1_mtmax[index])
		  mon.fillHisto("mt_shapes",ctf,index, mt,iweight);
	      }
	    }
	}
    }

  //all done with the events file
  file->Close();
  cout << "Sample treated as MC? " << isMC << endl;

  //save histograms to file
  TString outUrl( outdir );
  gSystem->Exec("mkdir -p " + outUrl);
  outUrl += "/";
  outUrl += gSystem->BaseName(url);
  printf("Results save in %s\n", outUrl.Data());

  TFile *ofile=TFile::Open(outUrl, "recreate");
  mon.Write();
  ofile->Close();
}  


