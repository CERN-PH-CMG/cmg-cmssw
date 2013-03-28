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
  bool useCHS(true);
  bool nodphisoftjet(true);

  TString url=runProcess.getParameter<std::string>("input");

  bool isMC = runProcess.getParameter<bool>("isMC");
 //bool runBlinded = runProcess.getParameter<bool>("runBlinded");
  int mctruthmode = runProcess.getParameter<int>("mctruthmode");
  bool isV0JetsMC(isMC && url.Contains("0Jets"));

  TString outFileUrl(gSystem->BaseName(url));
  outFileUrl.ReplaceAll(".root","");
  if(mctruthmode!=0) { outFileUrl += "_filt"; outFileUrl += mctruthmode; }
  TString outdir=runProcess.getParameter<std::string>("outdir");
  TString dirname = runProcess.getParameter<std::string>("dirName");
  TString uncFile =  runProcess.getParameter<std::string>("jesUncFileName"); gSystem->ExpandPathName(uncFile);
  JetCorrectionUncertainty jecUnc(uncFile.Data());

  TRandom2 rndGen;
  EventCategory eventCategoryInst(3);
  GammaEventHandler gammaEvHandler(runProcess);  

  //book histograms
  SmartSelectionMonitor mon;

  mon.addHistogram(  new TProfile("metvsrun"    ,      ";Run number;<MET>",     500, 190000,200000) ) ;
  mon.addHistogram(  new TProfile("metvsavginstlumi",  ";Avg. inst lumi;<MET>", 50,  0,5000));
  mon.addHistogram(  new TProfile("nvtxvsrun",         ";Run number;<Vertices>",     500, 190000,200000) ) ;
  mon.addHistogram(  new TProfile("nvtxvsavginstlumi", ";Avg. inst lumi;<Vertices>", 50,  0,5000));

  TH1F* Hcutflow     = (TH1F*) mon.addHistogram(  new TH1F ("cutflow"    , "cutflow"    ,6,0,6) ) ;

  //##############################################
  //######## STUFF FOR CUTS OPTIMIZATION  ########
  //##############################################

   std::vector<double> optim_Cuts1_met; 
   std::vector<double> optim_Cuts1_mtmin;
   std::vector<double> optim_Cuts1_mtmax;
   for(double met=50;met<140;met+=5.0){
     optim_Cuts1_met    .push_back(met);
     optim_Cuts1_mtmin  .push_back(0);
     optim_Cuts1_mtmax  .push_back(9999);
   }
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

  mon.addHistogram( new TH1F ("jetpt",     ";Jet p_{T};Jets",50,0,250) );
  mon.addHistogram( new TH1F ("ht",     ";H_{T};Events",50,0,500) );
  mon.addHistogram( new TH1F ("ht50",     ";H_{T};Events",50,0,500) );
  mon.addHistogram( new TH1F ("clusteredmet",     ";Clustered E_{T}^{miss} [GeV];Events",50,0,250) );
  mon.addHistogram( new TH1F ("unclusteredmet",     ";Unclustered E_{T}^{miss} [GeV];Events",50,0,250) );
  mon.addHistogram( new TH1F ("mht",     ";MHT [GeV];Events",50,0,250) );
  mon.addHistogram( new TH1F ("njets",     ";Jet multiplicity (p_{T}>30 GeV/c);Events",5,0,5) );
  mon.addHistogram( new TProfile( "njetsvspu", ";Vertices;N_{jets}",50,0,50) );
  mon.addHistogram( new TH1F ("nsoftjets", ";Jet multiplicity (p_{T}>15 GeV/c);Events",5,0,5) );
  mon.addHistogram( new TH1F ("nbtags",    ";b-tag multiplicity; Events", 5,0,5) );
  mon.addHistogram( new TH1F ("npfjetsbtagsJPNM1",    ";b-tag multiplicity; Events", 5,0,5) );
  for(size_t ibin=1; ibin<=5; ibin++){
    TString label("");
    if(ibin==5) label +="#geq";
    else        label +="=";
    label += (ibin-1);
    mon.getHisto("njets")->GetXaxis()->SetBinLabel(ibin,label);
    mon.getHisto("nbtags")->GetXaxis()->SetBinLabel(ibin,label);
    mon.getHisto("npfjetsbtagsJPNM1")->GetXaxis()->SetBinLabel(ibin,label);
  }
  
  mon.addHistogram( new TH1F ("r9", ";R9;Events", 100,0.8,1) );
  mon.addHistogram( new TH1F ("sietaieta", ";#sigma i#eta i#eta;Events", 100,0,0.03) );
  mon.addHistogram( new TH1F ("iso", ";Isolation [GeV];Events", 100,0,10.) );
  mon.addHistogram( new TProfile( "mindphijmetvspu", ";Vertices;min #Delta#phi(jet,E_{T}^{miss});Events",50,0,50) );
  mon.addHistogram( new TH1F( "mindphijmet", ";min #Delta#phi(jet,E_{T}^{miss});Events",40,0,4) );
  mon.addHistogram( new TH1F( "mindphijmetNM1", ";min #Delta#phi(jet,E_{T}^{miss});Events",40,0,4) );
  mon.addHistogram( new TH1F( "mindphijmet_0", ";min #Delta#phi(jet,E_{T}^{miss});Events",40,0,4) );
  mon.addHistogram( new TH1F( "mindphijmet_25", ";min #Delta#phi(jet,E_{T}^{miss}>25);Events",40,0,4) );
  mon.addHistogram( new TH1F( "mindphijmet_50", ";min #Delta#phi(jet,E_{T}^{miss}>50);Events",40,0,4) );
  mon.addHistogram( new TH1F( "dphibmet", ";#Delta#phi(boson,E_{T}^{miss});Events",40,0,4) );
  mon.addHistogram( new TH1D( "balance", ";E_{T}^{miss}/q_{T};Events", 25,0,2.5) );
  
//   Float_t qtaxis[100];
//   for(size_t i=0; i<40; i++)  qtaxis[i]=2.5*i;       //0-97.5
//   for(size_t i=0; i<20; i++)  qtaxis[40+i]=100+5*i;  //100-195
//   for(size_t i=0; i<15; i++)  qtaxis[60+i]=200+10*i; //200-340
//   for(size_t i=0; i<25; i++)  qtaxis[75+i]=350+25*i; //350-976
//   mon.addHistogram( new TH1D( "qtraw"           , ";p_{T}^{#gamma} [GeV/c];Events / (2.5 GeV/c)",99,qtaxis));
//   mon.addHistogram( new TH1D( "qt"              , ";p_{T}^{#gamma} [GeV/c];Events / (2.5 GeV/c)",99,qtaxis));
  mon.addHistogram( new TH1F( "qt",        ";p_{T}^{#gamma} [GeV/c];Events / (2.5 GeV/c)",1500,0,1500));
  mon.addHistogram( new TH1F( "qtraw",        ";p_{T}^{#gamma} [GeV/c];Events / (2.5 GeV/c)",1500,0,1500));
  mon.addHistogram( new TH1D( "metoverqt"       , ";E_{T}^{miss}/p_{T}^{#gamma} [GeV/c];Events", 25,0,2.5) );
  mon.addHistogram( new TH1D( "eta"             , ";#eta;Events", 50,0,2.6) );  
  mon.addHistogram( new TH1F( "nvtx"            , ";Vertices;Events", 50,0,50) );  
  mon.addHistogram( new TH1F( "rho"             , ";#rho;Events", 50,0,25) );  
  mon.addHistogram( new TH1F( "mt"  , ";M_{T};Events", 100,0,1000) );
  mon.addHistogram( new TH1F( "mtNM1"  , ";M_{T};Events", 100,0,1000) );
  mon.addHistogram( new TH1F( "mt_blind"  , ";M_{T};Events", 100,0,1000) );
  mon.addHistogram( new TH1F( "mt_unroll"  , ";M_{T};Events",       50,150,2650) );
  mon.addHistogram( new TH1F( "mt_unroll_NM1"  , ";M_{T};Events",   50,150,2650) );
  mon.addHistogram( new TH1F( "mt_unroll_blind"  , ";M_{T};Events", 50,150,2650) );
  mon.addHistogram( new TH1F( "met_phi"         , ";#phi [rad];Events", 50,0,3.5) );
  mon.addHistogram( new TH1F( "met_met"         , ";E_{T}^{miss};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_metNM1"      , ";E_{T}^{miss};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_met_blind"   , ";E_{T}^{miss};Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_redMet"      ,  ";red(E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_redMetNM1"   ,  ";red(E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_redMet_blind", ";red(E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,500) );
  mon.addHistogram( new TH1F( "met_redMetL"     , ";red(E_{T}^{miss},clustered-E_{T}^{miss}) - longi.;Events", 50,-250,250) );
  mon.addHistogram( new TH1F( "met_redMetT"     , ";red(E_{T}^{miss},clustered-E_{T}^{miss}) - perp.;Events", 50,-250,250) );
  mon.addHistogram( new TH1F( "qmass"           , ";M^{ll};Events", 15,76,106) );
  mon.addHistogram( new TH2F( "met_met_vspu"    , ";Vertices;E_{T}^{miss};Events", 50,0,50,50,0,500) );
  mon.addHistogram( new TH2F( "met_redMet_vspu" , ";Vertices;red(E_{T}^{miss},clustered-E_{T}^{miss});Events", 50,0,50,50,0,500) );
  mon.addHistogram( new TH2F ("mt_shapes"       , ";cut index;M_{T} [GeV/c^{2}];",nOptimCuts,0,nOptimCuts, 160,150,1750) );  
  mon.addHistogram( new TH2F ("met_shapes",       ";cut index;MET [GeV/c^{2}];#events (/10GeV)",nOptimCuts,0,nOptimCuts,100 ,0,500) );
  
  //jet id efficiencies
  TString jetRegs[]={"TK","HEin","HEout","HF"};
  const size_t nJetRegs=sizeof(jetRegs)/sizeof(TString);
  TString jetIds[]={"pfloose","pftight","minloose","minmedium"};
  const size_t nJetIds=sizeof(jetIds)/sizeof(TString);
  Double_t jetPtBins[]={0,15,20,25,30,40,50,60,70,80,100,200,300,400,500,600,700,1000};
  Int_t nJetPtBins=sizeof(jetPtBins)/sizeof(Double_t)-1;
  Double_t jetEtaBins[]={0,0.25,0.5,0.75,1.0,1.25,1.5,1.75,2.0,2.25,2.625,2.75,2.875,3.0,3.5,4.0,4.5,5.0};
  Double_t nJetEtaBins=sizeof(jetEtaBins)/sizeof(Double_t)-1;
  mon.addHistogram( new TH1F("pfjetflavor",";Flavor;Events",10,0,10) );
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
  mon.addHistogram( new TH1F("pfvbfcandjeteta"      , ";#eta;Jets",50,0,5) );
  mon.addHistogram( new TH1F("pfvbfcandjetdeta"     , ";|#Delta #eta|;Jets",50,0,10) );
  mon.addHistogram( new TH1F("pfvbfcandjetpt"       , ";p_{T} [GeV/c];Jets",50,0,250) );
  mon.addHistogram( new TH1F("pfvbfhardpt"          , ";Hard p_{T} [GeV/c];Events",25,0,250) );
  mon.addHistogram( new TH1F("pfvbfhardptfinal"     , ";Hard p_{T} [GeV/c];Events",25,0,250) );
  TH1 *h=mon.addHistogram( new TH1F("pfvbfcjv"      , ";Central jet count;Events",3,0,3) );
  h->GetXaxis()->SetBinLabel(1,"=0 jets");
  h->GetXaxis()->SetBinLabel(2,"=1 jets");
  h->GetXaxis()->SetBinLabel(3,"#geq 2 jets");
  mon.addHistogram( new TH1F("pfvbfhtcjv"           , ";Central jet H_{T} [GeV/c];Events",50,0,250) );
  mon.addHistogram( new TH1F("pfvbfpremjj"          , ";M(jet_{1},jet_{2}) [GeV/c^{2}];Events",40,0,2000) );
  mon.addHistogram( new TH1F("pfvbfmjj"             , ";M(jet_{1},jet_{2}) [GeV/c^{2}];Events",40,0,2000) );
  mon.addHistogram( new TH1F("pfvbfcandjetdphi"     , ";#Delta#phi;Events",20,0,3.5) );  

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
  bool useObservedPU(true);//use2011Id);
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
      if( !gammaEvHandler.isGood(phys,use2011Id) ) continue;
      if( !isMC && duplicatesChecker.isDuplicate(ev.run,ev.lumi, ev.event)) {  NumberOfDuplicated++; continue; }

      if((mctruthmode==22||mctruthmode==111))
	{
	  if(isMC)
	    {
	      int npromptGammas = ((ev.mccat>>28)&0xf) ;      
	      if(mctruthmode==22  && npromptGammas<1) continue;
	      if(mctruthmode==111 && npromptGammas>0) continue;
	    }
	}
      else continue;
      if(isV0JetsMC && ev.mc_nup>5)                          continue;

      std::vector<TString> dilCats;
      dilCats.push_back("ee");
      dilCats.push_back("mumu");
      
      //build the gamma candidate
      LorentzVector gamma            = phys.gammas[0];
      float r9(0),sietaieta(0);
      bool hasTrkVeto(false),hasElectronVeto(false);
      r9               = phys.gammas[0].r9*(isMC ? 1.005 : 1.0);
      sietaieta        = phys.gammas[0].sihih;
      hasElectronVeto  = phys.gammas[0].hasElectronVeto;
      hasTrkVeto       = phys.gammas[0].hasCtfTrkVeto;
      float gIso=(TMath::Max(phys.gammas[0].iso2+phys.gammas[0].iso3-pow(0.4,2)*TMath::Pi()*ev.rho,0.)+phys.gammas[0].iso1);

      //
      // EXTRA LEPTONS
      //
      int nextraleptons(0);
      for(size_t ilep=0; ilep<phys.leptons.size(); ilep++)
	{
	  int lpid=phys.leptons[ilep].pid;

	  //FIXME: looks like sometimes the gamma comes double counted as a soft-id electron ???
	  if(fabs(phys.leptons[ilep].id)==11)
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
	  else if(fabs(phys.leptons[ilep].id)==11)
	    {
	      if(use2011Id)
		{
		  if( hasObjectId(ev.en_idbits[lpid],EID_VBTF2011) && phys.leptons[ilep].relIsoRho(ev.rho)<0.1 && phys.leptons[ilep].pt()>10) nextraleptons++;
		}
	      else
		{
		  if( hasObjectId(ev.en_idbits[lpid],EID_VETO) && phys.leptons[ilep].ePFRelIsoCorrected2012(ev.rho,ev.en_sceta[lpid])<0.15 && phys.leptons[ilep].pt()>10) nextraleptons++;
		}
	    }
	}
      
      //
      // JET/MET
      //
      //LorentzVector metP4(phys.met[0]);
      LorentzVector metP4(phys.met[2]);
      if(use2011Id) metP4=phys.met[0];
      bool passLMetVeto(true);

      LorentzVectorCollection zvvs;
      std::vector<PhysicsObjectJetCollection> jets;
      if(!use2011Id)
	{
	  if(!useCHS) METUtils::computeVariation(phys.jets, phys.leptons, metP4, jets, zvvs, &jecUnc);
	  else        METUtils::computeVariation(phys.ajets,  phys.leptons, metP4, jets, zvvs, &jecUnc);
	}
      else
	{
	  METUtils::computeVariation(phys.ajets,  phys.leptons, metP4, jets, zvvs, &jecUnc);
	}
      metP4=zvvs[0];
      PhysicsObjectJetCollection & jetsToUse=jets[0];
      if(disableJERSmear && useCHS)  jetsToUse=phys.ajets;
      if(disableJERSmear && !useCHS) jetsToUse=phys.jets;
      
      //count the jets
      int nbtags(0),njets30(0),njets15(0);
      double ht(0);
      double mindphijmet(9999.),mindphijmet15(9999.);
      PhysicsObjectJetCollection selJets,recoilJets;
      LorentzVector clusteredMet(gamma);  clusteredMet *= -1;
      LorentzVector mht(0,0,0,0),unclusteredMet(0,0,0,0);
      for(size_t ijet=0; ijet<jetsToUse.size(); ijet++)
        {
	  LorentzVector ijetP4=jetsToUse[ijet];
	  if(ijetP4.pt()>30 && deltaR(ijetP4,gamma)<0.1) continue;

	  //base jet id
	  //	  bool isGoodPFJet=hasObjectId(jetsToUse[ijet].pid,JETID_LOOSE);
	  // if(!isGoodPFJet) continue; //this is redundant...
	  
	  //tighten id
	  bool isGoodJet=(hasObjectId(jetsToUse[ijet].pid,JETID_CUTBASED_LOOSE) && fabs(jetsToUse[ijet].eta())<4.7);
	  if(!isGoodJet) continue;

	  //dphi(jet,MET)
	  double idphijmet( fabs(deltaPhi(ijetP4.phi(),metP4.phi()) ) );	  
	  if(ijetP4.pt()>15) if(idphijmet<mindphijmet15) mindphijmet15=idphijmet;
	  if(ijetP4.pt()>30) if(idphijmet<mindphijmet)   mindphijmet = idphijmet;
	  if( fabs(deltaPhi(ijetP4.phi(),gamma.phi())>2 ) ) recoilJets.push_back( jetsToUse[ijet] );
	  
	  selJets.push_back(jetsToUse[ijet]);
	  clusteredMet -= ijetP4;
	  mht -= ijetP4;
	  ht += ijetP4.pt();
	  if(ijetP4.pt()>15)  njets15++; 
	  if(ijetP4.pt()<30) continue;
	  if(fabs(ijetP4.eta())<2.5) nbtags += (jetsToUse[ijet].btag3>0.275);
	  njets30++;
	}
      
      //other mets
      unclusteredMet = metP4-clusteredMet;
      LorentzVector nullP4(0,0,0,0);
      //LorentzVector assocMetP4 = phys.met[1];
      //LorentzVector min3Met=min(metP4, min(assocMetP4,clusteredMet)) ;
      METUtils::stRedMET redMetOut;
      LorentzVector redMet(METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, gamma, 0, nullP4, 0, clusteredMet, metP4,true,&redMetOut));
      double redMetL=redMetOut.redMET_l;
      double redMetT=redMetOut.redMET_t;
      

      //event weight
      float weight = 1.0;  
      if(isMC)                { weight = LumiWeights.weight( ev.ngenITpu ); }
      if(!isMC && !use2011Id) { weight *= gammaEvHandler.triggerWgt_; }
      Hcutflow->Fill(1,1);
      Hcutflow->Fill(2,weight);
      Hcutflow->Fill(3,weight);
      Hcutflow->Fill(4,weight);
      Hcutflow->Fill(5,1);
        
      //  
      // EVENT SELECTION
      //
      bool passMultiplicityVetoes (nextraleptons==0);
      bool passKinematics         (gamma.pt()>55);
      passKinematics &= (fabs(gamma.eta())<1.4442); 
      passKinematics &= (r9>0.9 && r9<1.0);
      //      passKinematics &= (gIso<0.5);
      if(!isMC)    passKinematics &= (gamma.pt()>gammaEvHandler.triggerThr());
      passKinematics &= !hasElectronVeto; 
	
      bool passBveto              (nbtags==0);
      if(nodphisoftjet)  mindphijmet15=99999.;
      bool passMinDphiJmet        (mindphijmet>0.5);
      if(njets30==0) passMinDphiJmet=(mindphijmet15>0.5);
      if(use2011Id && njets15==0) passMinDphiJmet=false;

      //event category
      int eventSubCat    = eventCategoryInst.Get(phys,&selJets);
      TString tag_subcat = eventCategoryInst.GetLabel(eventSubCat); 
      std::vector<TString> subcats;
      subcats.push_back("");
      if(tag_subcat=="vbf") {
	subcats.push_back(tag_subcat);
	// 	TString tag_subcatVBF(tag_subcat);
	// 	if(fabs(jetsToUse[0].eta())<2.1 && fabs(jetsToUse[1].eta())<2.1)      { tag_subcatVBF+="2"; }
	// 	else if(fabs(jetsToUse[0].eta())<2.1 || fabs(jetsToUse[1].eta())<2.1) { tag_subcatVBF+="1"; }
	// 	else                                                                  { tag_subcatVBF+="0"; }
	//	subcats.push_back(tag_subcatVBF);
      }
      else if(njets30==0)
	{
	  subcats.push_back("eq0jets");
	} 
      else { subcats.push_back("geq1jets");  tag_subcat="geq1jets"; }

      //now do the control plots
      std::map<TString, float> qtWeights;
      TString photonSubCat("eq");    photonSubCat+=min(njets30,3); photonSubCat += "jets";
      if(photonSubCat.Contains("3")) photonSubCat = "g"+photonSubCat;
      if(tag_subcat.Contains("vbf")) photonSubCat = "vbf";
      
      qtWeights = gammaEvHandler.getWeights(phys,photonSubCat);
     
      //now do the control plots
      if(passKinematics && passMultiplicityVetoes)
	{
	  for(size_t idc=0; idc<dilCats.size(); idc++)
	    {
	      LorentzVector iboson(gammaEvHandler.massiveGamma(dilCats[idc]));
	      float zmass = iboson.mass();
	      Float_t mt( METUtils::transverseMass(iboson,metP4,true) );
	      double  mt_unroll(mt);
	      if(mt>1250)         mt_unroll = 1240;
	      if(passMinDphiJmet) mt_unroll += 1250;


	      float iweight=weight*qtWeights[dilCats[idc]];
	     
	      for(size_t isc=0; isc<subcats.size(); isc++)
		{
		  TString ctf=dilCats[idc]+subcats[isc];	      
		  mon.fillHisto("r9",        ctf, r9,         iweight);
		  mon.fillHisto("sietaieta", ctf, sietaieta,  iweight);
		  mon.fillHisto("iso",    ctf, gIso,  iweight);
		  mon.fillHisto("trkveto",   ctf, hasTrkVeto, iweight);
		  		  
		  mon.fillHisto("nbtags",ctf, nbtags,iweight);
		  if(passMinDphiJmet && metP4.pt()>70)  mon.fillHisto("npfjetsbtagsJPNM1", ctf, nbtags ,iweight);
		  if(passBveto)
		    {
		      mon.fillHisto("eta",ctf, fabs(gamma.eta()),iweight);
		      
		      //TString photonCtf=ctf;
		      TString photonCtf=dilCats[idc]+photonSubCat;
		      mon.fillHisto("qtraw", photonCtf, gamma.pt(), iweight/weight, true);
		      mon.fillHisto("qt",    photonCtf, gamma.pt(), iweight,        true);

		      mon.fillHisto("nvtx",ctf, ev.nvtx,iweight);
		      mon.fillHisto("rho",ctf, ev.rho,iweight);
		      mon.fillHisto("qmass",ctf, zmass,iweight);
		      mon.fillHisto("njets",ctf, njets30,iweight);
		      mon.fillProfile("njetsvspu",ctf,ev.nvtx,njets30,iweight);	      	      
		      mon.fillHisto("ht",ctf, ht,iweight);
		      for(size_t ijet=0; ijet<selJets.size(); ijet++) mon.fillHisto("jetpt",ctf, selJets[ijet].pt(),iweight);
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
			      mon.fillHisto("pfjetflavor",ctf,fabs(recoilJets[0].flavid),iweight,true);
			      for(size_t iids=0; iids<passIds.size(); iids++)
				{
				  mon.fillHisto(etaReg+passIds[iids]+"pfjetpt",ctf,recoilJets[0].pt(),iweight,true);
				  mon.fillHisto(passIds[iids]+"pfjeteta",ctf,fabs(recoilJets[0].eta()),iweight,true);
				}
			    }
			}
   
		      mon.fillHisto("mindphijmet_0",ctf,njets30==0? mindphijmet15:mindphijmet,iweight);	      
		      if(metP4.pt()>25) mon.fillHisto("mindphijmet_25",ctf,njets30==0? mindphijmet15:mindphijmet,iweight);	      
		      if(metP4.pt()>50) 
			{
			  mon.fillHisto("mindphijmet_50",ctf,njets30==0? mindphijmet15:mindphijmet,iweight);	      
			  mon.fillHisto("mindphijmet",ctf,njets30==0? mindphijmet15:mindphijmet,iweight);
			  mon.fillProfile("mindphijmetvspu",ctf,ev.nvtx,njets30==0? mindphijmet15:mindphijmet,iweight);	      	      
			  mon.fillHisto("dphibmet",ctf,fabs(deltaPhi(gamma.phi(),metP4.phi())),iweight);	      
			  mon.fillHisto("ht50",ctf,ht,iweight);	      
			}
		      
		      if(metP4.pt()>70) mon.fillHisto("mindphijmetNM1",  ctf,njets30==0? mindphijmet15:mindphijmet,iweight);

		      bool blind(false);
		      if(passMinDphiJmet)
			{
			  //VBF monitoring
			  float dphijj(-1),hardpt(-1);
			  if(njets30>=2 && selJets.size()>=2)
			    {
			      LorentzVector vbfSyst=selJets[0]+selJets[1];
			      LorentzVector hardSyst=vbfSyst+metP4+gamma;
			      hardpt=hardSyst.pt();
			      dphijj=deltaPhi(selJets[0].phi(),selJets[1].phi());
			      double maxEta=max(selJets[0].eta(),selJets[1].eta());
			      double minEta=min(selJets[0].eta(),selJets[1].eta());
			      float detajj=maxEta-minEta;
			      mon.fillHisto("pfvbfcandjetpt",      ctf, fabs(selJets[0].pt()),iweight);
			      mon.fillHisto("pfvbfcandjetpt",      ctf, fabs(selJets[1].pt()),iweight);
			      if(selJets[0].pt()>30 && selJets[1].pt()>30)
				{
				  mon.fillHisto("pfvbfcandjeteta",     ctf, fabs(minEta),iweight);
				  mon.fillHisto("pfvbfcandjetdeta",    ctf, fabs(detajj),iweight);
				  mon.fillHisto("pfvbfcandjeteta",     ctf, fabs(maxEta),iweight);
				  mon.fillHisto("pfvbfpremjj",         ctf, vbfSyst.mass(),iweight);
				  if(fabs(detajj)>4.0)
				    {
				      mon.fillHisto("pfvbfmjj",         ctf, vbfSyst.mass(),iweight);
				      
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
			    }
			  
			  mon.fillHisto("met_phi",         ctf, fabs(phys.met[0].phi()),iweight);
			  
			  if(passLMetVeto)
			    {
			      if(!tag_subcat.Contains("vbf")) blind = (mt>250);
			      else                            blind=(metP4.pt()>70);
			      if(!blind){
				mon.fillHisto("met_met_blind",         ctf, metP4.pt(),iweight);
				mon.fillHisto("met_redMet_blind",      ctf, redMet.pt(),iweight);
				mon.fillHisto("mt_blind",              ctf, mt,iweight);
			      }
			      mon.fillHisto("metoverqt",       ctf, metP4.pt()/gamma.pt(),iweight);
			      mon.fillHisto("met_met",         ctf, metP4.pt(),iweight);
			      mon.fillHisto("met_metNM1",ctf,metP4.pt(),iweight);
			      mon.fillHisto("met_met_vspu",    ctf, ev.nvtx,metP4.pt(),iweight);
			      mon.fillHisto("met_redMet",      ctf, redMet.pt(),iweight);
			      mon.fillHisto("met_redMetNM1",   ctf, redMet.pt(),iweight);
			      mon.fillHisto("met_redMet_vspu", ctf, ev.nvtx,redMet.pt(),iweight);
			      mon.fillHisto("met_redMetT",     ctf, redMetL,iweight);
			      mon.fillHisto("met_redMetL",     ctf, redMetT,iweight);
			      mon.fillHisto("mt",              ctf, mt,iweight);
			      

			      if(metP4.pt()>70) 
				{
				  mon.fillHisto("mtNM1",              ctf, mt,iweight);
				  mon.fillHisto("pfvbfcandjetdphi",    ctf, fabs(dphijj),iweight);
				}
			      mon.fillProfile("metvsrun",          ctf, ev.run,            metP4.pt(), iweight);
			      mon.fillProfile("metvsavginstlumi",  ctf, ev.curAvgInstLumi, metP4.pt(), iweight);
			      mon.fillProfile("nvtxvsrun",         ctf, ev.run,            ev.nvtx,      iweight);
			      mon.fillProfile("nvtxvsavginstlumi", ctf, ev.curAvgInstLumi, ev.nvtx,      iweight);  
		  
			      for(unsigned int index=0;index<nOptimCuts;index++){
				if ( index<nOptimCuts-1 && metP4.pt()>optim_Cuts1_met[index] && mt>optim_Cuts1_mtmin[index] && mt<optim_Cuts1_mtmax[index])
				  {
				    mon.fillHisto("mt_shapes",ctf,index, mt,iweight);
				    mon.fillHisto("met_shapes",ctf,index, metP4.pt(),iweight);
				  }
			      } 
			    }
			}
		    }
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
  outUrl += outFileUrl + ".root";
  printf("Results save in %s\n", outUrl.Data());

  TFile *ofile=TFile::Open(outUrl, "recreate");
  mon.Write();
  ofile->Close();
}  


