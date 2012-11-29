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
  
TTree *hzz_tree     = new TTree("hzz_tree","Train and Test Tree");
double hzz_lept1_PT;
double hzz_lept2_PT;
double hzz_lept1_ETA;
double hzz_lept2_ETA;
double hzz_lept1_PHI;
double hzz_lept2_PHI;
double hzz_lept1_DetIso;
double hzz_lept2_DetIso;
double hzz_lept1_PFIso2012;
double hzz_lept2_PFIso2012;
double hzz_lept1_PFIsoDbeta;
double hzz_lept2_PFIsoDbeta;
double hzz_Z_Mass;
double hzz_Z_PT;
double hzz_Z_Eta;
double hzz_Z_Phi;
double PUWeight;
double hzz_PFMET;
double hzz_MET_Phi;
double hzz_TransMass_Higgs;
double hzz_Jet_PT;
double hzz_Jet_ETA;
double hzz_Jet_PHI;
double hzz_Jet_Energy;
int ev_category;
int hzz_N_Jets;
double hzz_dPhi_JetMet;
int hzz_lept1_ID;
int hzz_lept2_ID;
double event_weight;
int ev_num;
int run_num;
int lumi_num;
bool vbf_flag;
double hzz_Jet_SumPT;
double hzz_lept1_gIso;
double hzz_lept2_gIso;
double hzz_lept1_chIso;
double hzz_lept2_chIso;
double hzz_lept1_nhIso;
double hzz_lept2_nhIso;
double hzz_lept1_nIso;
double hzz_lept2_nIso;
double hzz_rho;
double lineshape_weight;
double hzz_BTag;
double hzz_RedMet;
double hzz_RedMetL;
double hzz_RedMetT;
double hzz_RawMet;
double hzz_MVAMet;
double hzz_vbfdetajj;
double hzz_vbfmjj;
double hzz_vbfdphijj;
float hzz_weight_PUup;
float hzz_weight_PUdown;
double hzz_TransMass_Higgs_JERup;
double hzz_TransMass_Higgs_JERdown;
double hzz_TransMass_Higgs_JESup;
double hzz_TransMass_Higgs_JESdown;
double hzz_TransMass_Higgs_UMETup;
double hzz_TransMass_Higgs_UMETdown;
double hzz_TransMass_Higgs_LESup;
double hzz_TransMass_Higgs_LESdown;
double hzz_RawMet_JERup;
double hzz_RawMet_JERdown;
double hzz_RawMet_JESup;
double hzz_RawMet_JESdown;
double hzz_RawMet_UMETup;
double hzz_RawMet_UMETdown;
double hzz_RawMet_LESup;
double hzz_RawMet_LESdown;
double hzz_dPhi_JetMet_JERup;
double hzz_dPhi_JetMet_JERdown;
double hzz_dPhi_JetMet_JESup;
double hzz_dPhi_JetMet_JESdown;
double hzz_dPhi_JetMet_UMETup;
double hzz_dPhi_JetMet_UMETdown;
double hzz_dPhi_JetMet_LESup;
double hzz_dPhi_JetMet_LESdown;


TBranch *b_hzz_Jet_SumPT = hzz_tree->Branch("hzz_Jet_SumPT",&hzz_Jet_SumPT,"hzz_Jet_SumPT/D");
TBranch *b_hzz_lept1_ID = hzz_tree->Branch("hzz_lept1_ID", &hzz_lept1_ID ,"hzz_lept1_ID/I");
TBranch *b_hzz_lept2_ID = hzz_tree->Branch("hzz_lept2_ID", &hzz_lept2_ID ,"hzz_lept2_ID/I");
TBranch *b_hzz_lept1_PT = hzz_tree->Branch("hzz_lept1_PT", &hzz_lept1_PT ,"hzz_lept1_PT/D");
TBranch *b_hzz_lept2_PT = hzz_tree->Branch("hzz_lept2_PT", &hzz_lept2_PT ,"hzz_lept2_PT/D");
TBranch *b_hzz_lept1_ETA = hzz_tree->Branch("hzz_lept1_ETA", &hzz_lept1_ETA ,"hzz_lept1_ETA/D");
TBranch *b_hzz_lept2_ETA = hzz_tree->Branch("hzz_lept2_ETA", &hzz_lept2_ETA ,"hzz_lept2_ETA/D");
TBranch *b_hzz_lept1_PHI = hzz_tree->Branch("hzz_lept1_PHI", &hzz_lept1_PHI ,"hzz_lept1_PHI/D");
TBranch *b_hzz_lept2_PHI = hzz_tree->Branch("hzz_lept2_PHI", &hzz_lept2_PHI ,"hzz_lept2_PHI/D");
TBranch *b_hzz_lept1_DetIso = hzz_tree->Branch("hzz_lept1_DetIso", &hzz_lept1_DetIso ,"hzz_lept1_DetIso/D");
TBranch *b_hzz_lept2_DetIso = hzz_tree->Branch("hzz_lept2_DetIso", &hzz_lept2_DetIso ,"hzz_lept2_DetIso/D");
TBranch *b_hzz_lept1_PFIso2012 = hzz_tree->Branch("hzz_lept1_PFIso2012", &hzz_lept1_PFIso2012 ,"hzz_lept1_PFIso2012/D");
TBranch *b_hzz_lept2_PFIso2012 = hzz_tree->Branch("hzz_lept2_PFIso2012", &hzz_lept2_PFIso2012 ,"hzz_lept2_PFIso2012/D");
TBranch *b_hzz_lept1_PFIsoDbeta = hzz_tree->Branch("hzz_lept1_PFIsoDbeta", &hzz_lept1_PFIsoDbeta ,"hzz_lept1_PFIsoDbeta/D");
TBranch *b_hzz_lept2_PFIsoDbeta = hzz_tree->Branch("hzz_lept2_PFIsoDbeta", &hzz_lept2_PFIsoDbeta ,"hzz_lept2_PFIsoDbeta/D");
TBranch *b_hzz_Z_Mass = hzz_tree->Branch("hzz_Z_Mass", &hzz_Z_Mass ,"hzz_Z_Mass/D");
TBranch *b_hzz_Z_PT = hzz_tree->Branch("hzz_Z_PT", &hzz_Z_PT ,"hzz_Z_PT/D");
TBranch *b_hzz_Z_Eta = hzz_tree->Branch("hzz_Z_Eta", &hzz_Z_Eta ,"hzz_Z_Eta/D");
TBranch *b_hzz_Z_Phi = hzz_tree->Branch("hzz_Z_Phi", &hzz_Z_Phi ,"hzz_Z_Phi/D");
TBranch *b_hzz_PFMET = hzz_tree->Branch("hzz_PFMET", &hzz_PFMET ,"hzz_PFMET/D");
TBranch *b_hzz_MET_Phi = hzz_tree->Branch("hzz_MET_Phi", &hzz_MET_Phi ,"hzz_MET_Phi/D");
TBranch *b_PUWeight = hzz_tree->Branch("PUWeight", &PUWeight ,"PUWeight/D");
TBranch *b_hzz_TransMass_Higgs = hzz_tree->Branch("hzz_TransMass_Higgs", &hzz_TransMass_Higgs ,"hzz_TransMass_Higgs/D");
TBranch *b_hzz_Jet_PT = hzz_tree->Branch("hzz_Jet_PT", &hzz_Jet_PT ,"hzz_Jet_PT/D");
TBranch *b_hzz_Jet_ETA = hzz_tree->Branch("hzz_Jet_ETA", &hzz_Jet_ETA ,"hzz_Jet_ETA/D");
TBranch *b_hzz_Jet_PHI = hzz_tree->Branch("hzz_Jet_PHI", &hzz_Jet_PHI ,"hzz_Jet_PHI/D");
TBranch *b_hzz_Jet_Energy = hzz_tree->Branch("hzz_Jet_Energy", &hzz_Jet_Energy ,"hzz_Jet_Energy/D");
TBranch *b_ev_category = hzz_tree->Branch("ev_category", &ev_category ,"ev_category/I");
TBranch *b_hzz_N_Jets = hzz_tree->Branch("hzz_N_Jets", &hzz_N_Jets , "hzz_N_Jets/I");
TBranch *b_hzz_dPhi_JetMet = hzz_tree->Branch("hzz_dPhi_JetMet", &hzz_dPhi_JetMet , "hzz_dPhi_JetMet/D");
TBranch *b_event_weight = hzz_tree->Branch("event_weight", &event_weight ,"event_weight/D");
TBranch *b_ev_num = hzz_tree->Branch("ev_num", &ev_num , "ev_num/I");
TBranch *b_run_num = hzz_tree->Branch("run_num", &run_num , "run_num/I");
TBranch *b_lumi_num = hzz_tree->Branch("lumi_num", &lumi_num , "lumi_num/I");
TBranch *b_vbf_flag = hzz_tree->Branch("vbf_flag", &vbf_flag , "vbf_flag/B");
TBranch *b_hzz_lept1_gIso = hzz_tree->Branch("hzz_lept1_gIso", &hzz_lept1_gIso , "hzz_lept1_gIso/D");
TBranch *b_hzz_lept2_gIso = hzz_tree->Branch("hzz_lept2_gIso", &hzz_lept2_gIso , "hzz_lept2_gIso/D");
TBranch *b_hzz_lept1_chIso = hzz_tree->Branch("hzz_lept1_chIso", &hzz_lept1_chIso , "hzz_lept1_chIso/D");
TBranch *b_hzz_lept2_chIso = hzz_tree->Branch("hzz_lept2_chIso", &hzz_lept2_chIso , "hzz_lept2_chIso/D");
TBranch *b_hzz_lept1_nhIso = hzz_tree->Branch("hzz_lept1_nhIso", &hzz_lept1_nhIso , "hzz_lept1_nhIso/D");
TBranch *b_hzz_lept2_nhIso = hzz_tree->Branch("hzz_lept2_nhIso", &hzz_lept2_nhIso , "hzz_lept2_nhIso/D");
TBranch *b_hzz_lept1_nIso = hzz_tree->Branch("hzz_lept1_nIso", &hzz_lept1_nIso , "hzz_lept1_nIso/D");
TBranch *b_hzz_lept2_nIso = hzz_tree->Branch("hzz_lept2_nIso", &hzz_lept2_nIso , "hzz_lept2_nIso/D");
TBranch *b_hzz_rho = hzz_tree->Branch("hzz_rho", &hzz_rho , "hzz_rho/D");
TBranch *b_lineshape_weight = hzz_tree->Branch("lineshape_weight", &lineshape_weight , "lineshape_weight/D");
TBranch *b_hzz_BTag = hzz_tree->Branch("hzz_BTag", &hzz_BTag , "hzz_BTag/D");
TBranch *b_hzz_RedMet = hzz_tree->Branch("hzz_RedMet", &hzz_RedMet , "hzz_RedMet/D");
TBranch *b_hzz_RedMetL = hzz_tree->Branch("hzz_RedMetL", &hzz_RedMetL , "hzz_RedMetL/D");
TBranch *b_hzz_RedMetT = hzz_tree->Branch("hzz_RedMetT", &hzz_RedMetT , "hzz_RedMetT/D");
TBranch *b_hzz_RawMet = hzz_tree->Branch("hzz_RawMet", &hzz_RawMet , "hzz_RawMet/D");
TBranch *b_hzz_MVAMet = hzz_tree->Branch("hzz_MVAMet", &hzz_MVAMet , "hzz_MVAMet/D");
TBranch *b_hzz_vbfdetajj = hzz_tree->Branch("hzz_vbfdetajj", &hzz_vbfdetajj , "hzz_vbfdetajj/D");
TBranch *b_hzz_vbfdphijj = hzz_tree->Branch("hzz_vbfdphijj", &hzz_vbfdphijj , "hzz_vbfdphijj/D");
TBranch *b_hzz_vbfmjj = hzz_tree->Branch("hzz_vbfmjj", &hzz_vbfmjj , "hzz_vbfmjj/D");
TBranch *b_hzz_weight_PUup = hzz_tree->Branch("hzz_weight_PUup", &hzz_weight_PUup, "hzz_weight_PUup/F");
TBranch *b_hzz_weight_PUdown = hzz_tree->Branch("hzz_weight_PUdown", &hzz_weight_PUdown, "hzz_weight_PUdown/F");
TBranch *b_hzz_TransMass_Higgs_JERup = hzz_tree->Branch("hzz_TransMass_Higgs_JERup", &hzz_TransMass_Higgs_JERup ,"hzz_TransMass_Higgs_JERup/D");
TBranch *b_hzz_TransMass_Higgs_JERdown = hzz_tree->Branch("hzz_TransMass_Higgs_JERdown", &hzz_TransMass_Higgs_JERdown ,"hzz_TransMass_Higgs_JERdown/D");
TBranch *b_hzz_TransMass_Higgs_JESup = hzz_tree->Branch("hzz_TransMass_Higgs_JESup", &hzz_TransMass_Higgs_JESup ,"hzz_TransMass_Higgs_JESup/D");
TBranch *b_hzz_TransMass_Higgs_JESdown = hzz_tree->Branch("hzz_TransMass_Higgs_JESdown", &hzz_TransMass_Higgs_JESdown ,"hzz_TransMass_Higgs_JESdown/D");
TBranch *b_hzz_TransMass_Higgs_UMETup = hzz_tree->Branch("hzz_TransMass_Higgs_UMETup", &hzz_TransMass_Higgs_UMETup ,"hzz_TransMass_Higgs_UMETup/D");
TBranch *b_hzz_TransMass_Higgs_UMETdown = hzz_tree->Branch("hzz_TransMass_Higgs_UMETdown", &hzz_TransMass_Higgs_UMETdown ,"hzz_TransMass_Higgs_UMETdown/D");
TBranch *b_hzz_TransMass_Higgs_LESup = hzz_tree->Branch("hzz_TransMass_Higgs_LESup", &hzz_TransMass_Higgs_LESup ,"hzz_TransMass_Higgs_LESup/D");
TBranch *b_hzz_TransMass_Higgs_LESdown = hzz_tree->Branch("hzz_TransMass_Higgs_LESdown", &hzz_TransMass_Higgs_LESdown ,"hzz_TransMass_Higgs_LESdown/D");
TBranch *b_hzz_RawMet_JERup = hzz_tree->Branch("hzz_RawMet_JERup", &hzz_RawMet_JERup , "hzz_RawMet_JERup/D");
TBranch *b_hzz_RawMet_JERdown = hzz_tree->Branch("hzz_RawMet_JERdown", &hzz_RawMet_JERdown , "hzz_RawMet_JERdown/D");
TBranch *b_hzz_RawMet_JESup = hzz_tree->Branch("hzz_RawMet_JESup", &hzz_RawMet_JESup , "hzz_RawMet_JESup/D");
TBranch *b_hzz_RawMet_JESdown = hzz_tree->Branch("hzz_RawMet_JESdown", &hzz_RawMet_JESdown , "hzz_RawMet_JESdown/D");
TBranch *b_hzz_RawMet_UMETup = hzz_tree->Branch("hzz_RawMet_UMETup", &hzz_RawMet_UMETup , "hzz_RawMet_UMETup/D");
TBranch *b_hzz_RawMet_UMETdown = hzz_tree->Branch("hzz_RawMet_UMETdown", &hzz_RawMet_UMETdown , "hzz_RawMet_UMETdown/D");
TBranch *b_hzz_RawMet_LESup = hzz_tree->Branch("hzz_RawMet_LESup", &hzz_RawMet_LESup , "hzz_RawMet_LESup/D");
TBranch *b_hzz_RawMet_LESdown = hzz_tree->Branch("hzz_RawMet_LESdown", &hzz_RawMet_LESdown , "hzz_RawMet_LESdown/D");
TBranch *b_hzz_dPhi_JetMet_JERup = hzz_tree->Branch("hzz_dPhi_JetMet_JERup", &hzz_dPhi_JetMet_JERup , "hzz_dPhi_JetMet_JERup/D");
TBranch *b_hzz_dPhi_JetMet_JERdown = hzz_tree->Branch("hzz_dPhi_JetMet_JERdown", &hzz_dPhi_JetMet_JERdown , "hzz_dPhi_JetMet_JERdown/D");
TBranch *b_hzz_dPhi_JetMet_JESup = hzz_tree->Branch("hzz_dPhi_JetMet_JESup", &hzz_dPhi_JetMet_JESup , "hzz_dPhi_JetMet_JESup/D");
TBranch *b_hzz_dPhi_JetMet_JESdown = hzz_tree->Branch("hzz_dPhi_JetMet_JESdown", &hzz_dPhi_JetMet_JESdown , "hzz_dPhi_JetMet_JESdown/D");
TBranch *b_hzz_dPhi_JetMet_UMETup = hzz_tree->Branch("hzz_dPhi_JetMet_UMETup", &hzz_dPhi_JetMet_UMETup , "hzz_dPhi_JetMet_UMETup/D");
TBranch *b_hzz_dPhi_JetMet_UMETdown = hzz_tree->Branch("hzz_dPhi_JetMet_UMETdown", &hzz_dPhi_JetMet_UMETdown , "hzz_dPhi_JetMet_UMETdown/D");
TBranch *b_hzz_dPhi_JetMet_LESup = hzz_tree->Branch("hzz_dPhi_JetMet_LESup", &hzz_dPhi_JetMet_LESup , "hzz_dPhi_JetMet_LESup/D");
TBranch *b_hzz_dPhi_JetMet_LESdown = hzz_tree->Branch("hzz_dPhi_JetMet_LESdown", &hzz_dPhi_JetMet_LESdown , "hzz_dPhi_JetMet_LESdown/D");


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

  bool isMC = runProcess.getParameter<bool>("isMC");
  bool runBlinded = runProcess.getParameter<bool>("runBlinded");
  int mctruthmode = runProcess.getParameter<int>("mctruthmode");

  double xsec=runProcess.getParameter<double>("xsec");

std::cout << "xsec =  " << xsec << std::endl;

  TString url=runProcess.getParameter<std::string>("input");
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
  //##############################################

  //add last year cut


  //##############################################
  //######## CONTROL PLOTS FOR SELECTION #########
  //##############################################
  
  //  mon.addHistogram( new TH1D( "qt",        ";p_{T}^{#gamma} [GeV/c];Events / (2.5 GeV/c)", 100,0,500) );
  Float_t qtaxis[100];
  for(size_t i=0; i<40; i++)  qtaxis[i]=2.5*i;       //0-97.5
  for(size_t i=0; i<20; i++)  qtaxis[40+i]=100+5*i;  //100-195
  for(size_t i=0; i<15; i++)  qtaxis[60+i]=200+10*i; //200-340
  for(size_t i=0; i<25; i++)  qtaxis[75+i]=350+25*i; //350-976
   
  //jet id efficiencies
  TString jetRegs[]={"TK","HEin","HEout","HF"};
  const size_t nJetRegs=sizeof(jetRegs)/sizeof(TString);
  TString jetIds[]={"pfloose","pftight","minloose","minmedium"};
  const size_t nJetIds=sizeof(jetIds)/sizeof(TString);
  Double_t jetPtBins[]={0,15,20,25,30,40,50,60,70,80,100,200,300,400,500,600,700,1000};
  Int_t nJetPtBins=sizeof(jetPtBins)/sizeof(Double_t)-1;
  Double_t jetEtaBins[]={0,0.25,0.5,0.75,1.0,1.25,1.5,1.75,2.0,2.25,2.625,2.75,2.875,3.0,3.5,4.0,4.5,5.0};
  Double_t nJetEtaBins=sizeof(jetEtaBins)/sizeof(Double_t)-1;
  
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

      bool isGammaEvent    = gammaEvHandler.isGood(phys,use2011Id);
      if(!isMC && duplicatesChecker.isDuplicate(ev.run,ev.lumi, ev.event)) {  NumberOfDuplicated++; continue; }

      //check which event type is required to use (dilepton/or photon)
      if(mctruthmode==22 && !isGammaEvent ) continue;
      if(mctruthmode==1  && isGammaEvent) continue;
      if(!isGammaEvent && ev.cat != EE && ev.cat !=MUMU) continue;
      std::vector<TString> dilCats;
      
      //build the gamma candidate
      LorentzVector gamma(0,0,0,0);
      float r9(0),sietaieta(0);
      bool hasTrkVeto(false),/*isConv(false),*/hasElectronVeto(false),hasConvUnsafeElectronVeto(false);
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
	  //isConv=(phys.gammas[0].isConv);
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
	  if(fabs(gamma.mass()-90)>30) continue;
	  if(!isMC && ev.cat==MUMU && !hasMMtrigger) continue;
	  if(!isMC && ev.cat==EE && !hasEEtrigger) continue;
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
	  else if(fabs(phys.leptons[ilep].id)==11)
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
      //LorentzVector metP4(phys.met[0]);
      LorentzVector metP4(phys.met[2]);

      LorentzVectorCollection zvvs;
      std::vector<PhysicsObjectJetCollection> jets;
      METUtils::computeVariation(phys.ajets, phys.leptons, metP4, jets, zvvs, &jecUnc);
      metP4=zvvs[0];
      PhysicsObjectJetCollection & jetsToUse=jets[0];
      if(disableJERSmear) jetsToUse=phys.ajets;
     
      //count the jets
      int nbtags(0),npfjets30(0),njets30(0),njets15(0);
      double ht(0);
      double mindphijmet(9999.),mindphijmet15(9999.);
      PhysicsObjectJetCollection selJets,recoilJets;
      LorentzVector clusteredMet(gamma);  clusteredMet *= -1;
      LorentzVector mht(0,0,0,0),unclusteredMet(0,0,0,0);
      for(size_t ijet=0; ijet<jetsToUse.size(); ijet++)
        {
	  LorentzVector ijetP4=jetsToUse[ijet];
	  if(isGammaEvent && ijetP4.pt()>30 && deltaR(ijetP4,gamma)<0.1) continue;

	  //dphi(jet,MET)
	  double idphijmet( fabs(deltaPhi(ijetP4.phi(),metP4.phi()) ) );	  
	  if(idphijmet<mindphijmet15) mindphijmet15=idphijmet;
	  if(ijetP4.pt()>30) if(idphijmet<mindphijmet) mindphijmet = idphijmet;
	  if( fabs(deltaPhi(ijetP4.phi(),gamma.phi())>2 ) ) recoilJets.push_back( jetsToUse[ijet] );
	  
	  //base jet id
	  bool isGoodPFJet=hasObjectId(jetsToUse[ijet].pid,JETID_LOOSE);
	  if(isGoodPFJet && ijetP4.pt()>30) npfjets30++;

	  //tighten id
	  bool isGoodJet=hasObjectId(jetsToUse[ijet].pid,JETID_CUTBASED_LOOSE);
	  if(!isGoodJet) continue;
	  selJets.push_back(jetsToUse[ijet]);
	  clusteredMet -= ijetP4;
	  mht -= ijetP4;
	  ht += ijetP4.pt();
	  if(ijetP4.pt()>15)  njets15++; 
	  if(ijetP4.pt()<30) continue;
	  if(fabs(ijetP4.eta())<2.5) nbtags += (jetsToUse[ijet].btag2>0.244);
	  njets30++;
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
      

      //event weight
      float weight = 1.0;
      float pre_weight = 1.0;  
      float pu_weight = 1.0;
      if(isMC)                  { pu_weight = LumiWeights.weight( ev.ngenITpu ); }
      if(!isMC && isGammaEvent && !use2011Id) { pre_weight *= ev.gn_prescale;  }
cout << "pre_weight = " << pre_weight << endl;
      //  
      // EVENT SELECTION
      //
      bool passMultiplicityVetoes (nextraleptons==0);
      bool passKinematics         (gamma.pt()>55);
      if(isGammaEvent)
	{
	  passKinematics &= (fabs(gamma.eta())<2.5); 
	  passKinematics &= (r9>0.9 && r9<1.0);
	  if(!isMC)    passKinematics &= (gamma.pt()>gammaEvHandler.triggerThr());
	  //passKinematics &= (njets15>0);
	}
      bool passBveto              (nbtags==0);
      bool passMinDphiJmet        (mindphijmet>0.5);
      if(njets30==0) passMinDphiJmet=(mindphijmet15>0.5);
      
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
      else if(tag_subcat=="vbf") {
	subcats.push_back(tag_subcat);
	TString tag_subcatVBF(tag_subcat);
	if(fabs(jetsToUse[0].eta())<2.1 && fabs(jetsToUse[1].eta())<2.1)      { tag_subcatVBF+="2"; }
	else if(fabs(jetsToUse[0].eta())<2.1 || fabs(jetsToUse[1].eta())<2.1) { tag_subcatVBF+="1"; }
	else                                                                  { tag_subcatVBF+="0"; }
	subcats.push_back(tag_subcatVBF);
      }
      else if(njets30==2)        { subcats.push_back("eq2jets");  tag_subcat="eq2jets"; }
      else if(njets30>2)         { subcats.push_back("geq3jets"); tag_subcat="geq3jets"; }
      

      //now do the control plots
      std::map<TString, float> qtWeights;
      if(isGammaEvent) qtWeights = gammaEvHandler.getWeights(phys,tag_subcat);

      //now do the control plots
      if(!passMultiplicityVetoes) continue;
      if(!passKinematics) continue;
//cout << "dilCats.size() = " << dilCats.size() <<endl; 
cout << "*********************************************************" << endl;
//cout << "ev.category" << ev.cat << endl; 
cout << "qtWeights.size() = " << qtWeights.size() << endl;
cout << "qtWeights[dilCats[0]] = " << qtWeights[dilCats[0]] << endl;
cout << "qtWeights[dilCats[1]] = " << qtWeights[dilCats[1]] << endl;

      for(size_t idc=0; idc<dilCats.size(); idc++)
	{
	  LorentzVector iboson(isGammaEvent ? gammaEvHandler.massiveGamma(dilCats[idc]) : gamma);
	  float zmass=iboson.mass();
	  Float_t mt( METUtils::transverseMass(iboson,metP4,true) );
cout << "zmass = " << zmass << endl;
cout << "mt = " << mt << endl;
cout << "gamma PT = " << iboson.pt() << endl;
	  float iweight=weight;
	  if(isGammaEvent) iweight*=qtWeights[dilCats[idc]];
//cout << " qtWeights[dilCats[idc]] = " << qtWeights[dilCats[idc]] << " " << "dilCats[idc] = " << dilCats[idc] << endl;
	  for(size_t isc=0; isc<subcats.size(); isc++)
	    {
	      TString ctf=dilCats[idc]+subcats[isc];	      
	  
	      if(hasElectronVeto) continue;
	      if(!passBveto) continue;
	      
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
		}  

	      if(passMinDphiJmet)
		{
	  //VBF monitoring
		  //float dphijj(-1);
		  float hardpt(-1);
		  if(njets30>=2 && selJets.size()>=2)
		    {
		      LorentzVector vbfSyst=selJets[0]+selJets[1];
		      LorentzVector hardSyst=vbfSyst+metP4+gamma;
		      hardpt=hardSyst.pt();
		      //dphijj=deltaPhi(selJets[0].phi(),selJets[1].phi());
		      double maxEta=max(selJets[0].eta(),selJets[1].eta());
		      double minEta=min(selJets[0].eta(),selJets[1].eta());
		      float avgEtajj=0.5*(maxEta+minEta);
		      float detajj=maxEta-minEta;
		      int ncjv(0);
		      float htcjv(0);
		      for(size_t iotherjet=2; iotherjet<selJets.size(); iotherjet++)
			{
			  if(selJets[iotherjet].pt()<30 || selJets[iotherjet].eta()<minEta || selJets[iotherjet].eta()>maxEta) continue;
			  htcjv+= selJets[iotherjet].pt();
			  ncjv++;
			}
		    }

		  bool blind(false);
		  if(!tag_subcat.Contains("vbf")) blind = (mt>250);
		  else                            blind=(metP4.pt()>70);
		 
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
hzz_tree->Write();
  ofile->Close();
}  


