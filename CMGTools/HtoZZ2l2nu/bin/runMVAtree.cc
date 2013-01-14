#include <iostream>
#include <boost/shared_ptr.hpp>
#include "Math/GenVector/Boost.h"

#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuSummaryHandler.h"
#include "CMGTools/HtoZZ2l2nu/interface/ZZ2l2nuPhysicsEvent.h"
#include "CMGTools/HtoZZ2l2nu/interface/METUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/setStyle.h"
#include "CMGTools/HtoZZ2l2nu/interface/plotter.h"
#include "CMGTools/HtoZZ2l2nu/interface/ObjectFilters.h"
#include "CMGTools/HtoZZ2l2nu/interface/TMVAUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/MacroUtils.h"
#include "CMGTools/HtoZZ2l2nu/interface/EventCategory.h"

#include "CondFormats/JetMETObjects/interface/JetResolution.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

#include "CMGTools/HtoZZ2l2nu/interface/EfficiencyMap.h"
#include "CMGTools/HtoZZ2l2nu/interface/SmartSelectionMonitor.h"


#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TEventList.h"
#include "TROOT.h"
#include <vector> 
using namespace std;


int main(int argc, char* argv[])
{
  //##############################################
  //########    GLOBAL INITIALIZATION     ########
  //##############################################

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
double lineshape_weight_up;
double lineshape_weight_down;


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
TBranch *b_lineshape_weight_up = hzz_tree->Branch("lineshape_weight_up", &lineshape_weight_up , "lineshape_weight_up/D");
TBranch *b_lineshape_weight_down = hzz_tree->Branch("lineshape_weight_down", &lineshape_weight_down , "lineshape_weight_down/D");



  // check arguments
  if(argc<2){ std::cout << "Usage : " << argv[0] << " parameters_cfg.py" << std::endl; exit(0); }
  
  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  // configure the process
  const edm::ParameterSet &runProcess = edm::readPSetsFrom(argv[1])->getParameter<edm::ParameterSet>("runProcess");

  bool use2011Id = runProcess.getParameter<bool>("is2011");
  cout << "Note: will apply " << (use2011Id ? 2011 : 2012) << " version of the id's" << endl;

  double xsec=runProcess.getParameter<double>("xsec");

std::cout << "xsec =  " << xsec << std::endl;

  bool isMC = runProcess.getParameter<bool>("isMC");
  bool runBlinded = runProcess.getParameter<bool>("runBlinded");
  int mctruthmode=runProcess.getParameter<int>("mctruthmode");

  TString url=runProcess.getParameter<std::string>("input");
  TString outFileUrl(gSystem->BaseName(url));
  outFileUrl.ReplaceAll(".root","");
  if(mctruthmode!=0) { outFileUrl += "_filt"; outFileUrl += mctruthmode; }
  TString outdir=runProcess.getParameter<std::string>("outdir");
  TString outUrl( outdir );
  gSystem->Exec("mkdir -p " + outUrl);
  int fType(0);
  if(url.Contains("DoubleEle")) fType=EE;
  if(url.Contains("DoubleMu"))  fType=MUMU;
  if(url.Contains("MuEG"))      fType=EMU;
  if(url.Contains("SingleMu"))  fType=MUMU;
  bool isSingleMuPD(!isMC && url.Contains("SingleMu"));
  
  TString outTxtUrl= outUrl + "/" + outFileUrl + ".txt";
  FILE* outTxtFile = NULL;
  //if(!isMC)
  outTxtFile = fopen(outTxtUrl.Data(), "w");
  printf("TextFile URL = %s\n",outTxtUrl.Data());
  
  //tree info
  int evStart=runProcess.getParameter<int>("evStart");
  int evEnd=runProcess.getParameter<int>("evEnd");
  TString dirname = runProcess.getParameter<std::string>("dirName");
  

  //jet energy scale uncertainties
  TString uncFile =  runProcess.getParameter<std::string>("jesUncFileName"); gSystem->ExpandPathName(uncFile);
  JetCorrectionUncertainty jecUnc(uncFile.Data());
 bool runSystematics = runProcess.getParameter<bool>("runSystematics");
  TString varNames[]={"",
                      "_jerup","_jerdown",
                      "_jesup","_jesdown",
                      "_umetup","_umetdown",
                      "_lesup","_lesdown",
                      "_puup","_pudown",
                      "_renup","_rendown",
                      "_factup","_factdown",
                      "_btagup","_btagdown",
                      "_lshapeup","_lshapedown"};

  size_t nvarsToInclude(1);
  if(runSystematics) 
    { 
      cout << "Systematics will be computed for this analysis" << endl;
      nvarsToInclude=sizeof(varNames)/sizeof(TString);
    }
 


  // this is disabled for the moment
  double HiggsMass=0; string VBFString = ""; string GGString("");
  bool isMC_GG  = isMC && ( string(url.Data()).find("GG" )  != string::npos);
  bool isMC_VBF = isMC && ( string(url.Data()).find("VBF")  != string::npos);
  TFile *fin;
  int cmEnergy(8);
  if(url.Contains("7TeV")) cmEnergy=7;
  std::vector<TGraph *> hWeightsGrVec,hLineShapeGrVec;

  if(isMC_GG){  
    size_t GGStringpos =  string(url.Data()).find("GG");
    string StringMass = string(url.Data()).substr(GGStringpos+5,4);  sscanf(StringMass.c_str(),"%lf",&HiggsMass);
    GGString = string(url.Data()).substr(GGStringpos);
  
    //H pT
    if(cmEnergy==7){
      TString hqtWeightsFileURL = runProcess.getParameter<std::vector<std::string> >("hqtWeightsFile")[0]; gSystem->ExpandPathName(hqtWeightsFileURL);
      fin=TFile::Open(hqtWeightsFileURL);
      if(fin){
	cout << "HpT weights (and uncertainties) will be applied from: " << hqtWeightsFileURL << endl;
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
    }    
  }else if(isMC_VBF){
    size_t VBFStringpos =  string(url.Data()).find("VBF");
    string StringMass = string(url.Data()).substr(VBFStringpos+6,4);  sscanf(StringMass.c_str(),"%lf",&HiggsMass);
    VBFString = string(url.Data()).substr(VBFStringpos);
  }


  //LINE SHAPE WEIGHTS
  TString lineShapeWeightsFileURL = runProcess.getParameter<std::vector<std::string> >("hqtWeightsFile")[1]; gSystem->ExpandPathName(lineShapeWeightsFileURL);
  fin=0;
  std::vector<TString> wgts;
  if(isMC_VBF)
    {
      char buf[100]; sprintf(buf,"H%d/",int(HiggsMass));
      wgts.push_back(buf+TString("cpsWgt"));
      wgts.push_back(buf+TString("cpsUpWgt"));
      wgts.push_back(buf+TString("cpsDownWgt"));
      wgts.push_back(buf+TString("cpsWgt"));
      lineShapeWeightsFileURL.ReplaceAll("LineShapeWeights","VBFLineShapeWeights");
      fin=TFile::Open(lineShapeWeightsFileURL);
    }
  else
    {
      char buf[100]; sprintf(buf,"Higgs%d_%dTeV/",int(HiggsMass),cmEnergy);
      wgts.push_back(buf+TString("rwgtpint"));
      wgts.push_back(buf+TString("rwgtpint_up"));
      wgts.push_back(buf+TString("rwgtpint_down"));
      wgts.push_back(buf+TString("rwgt"));
      fin=TFile::Open(lineShapeWeightsFileURL);
    }
  if(fin)
    {
      cout << "Line shape weights (and uncertainties) will be applied from " << fin->GetName() << endl;
      for(size_t i=0; i<wgts.size(); i++)
        {
          TGraph *gr= (TGraph *) fin->Get(wgts[i]);
          if(gr) hLineShapeGrVec.push_back((TGraph *)gr->Clone());
        }
      fin->Close();
      delete fin;
    }

  SmartSelectionMonitor mon;
 TH1F* Hoptim_systs     =  (TH1F*) mon.addHistogram( new TH1F ("optim_systs"    , ";syst;", nvarsToInclude,0,nvarsToInclude) ) ;


   for(size_t ivar=0; ivar<nvarsToInclude; ivar++) {
     Hoptim_systs->GetXaxis()->SetBinLabel(ivar+1, varNames[ivar]);
}


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


  //pileup weighting: based on vtx for now...
  std::vector<double> dataPileupDistributionDouble = runProcess.getParameter< std::vector<double> >("datapileup");
  std::vector<float> dataPileupDistribution; for(unsigned int i=0;i<dataPileupDistributionDouble.size();i++){dataPileupDistribution.push_back(dataPileupDistributionDouble[i]);}
  std::vector<float> mcPileupDistribution;
  bool useObservedPU(true);
  //bool useObservedPU(use2011Id);
  if(!use2011Id && url.Contains("toZZto2L")) useObservedPU=true;
  if(isMC){
    TString puDist("evAnalyzer/h2zz/pileuptrue");
    if(useObservedPU) puDist="evAnalyzer/h2zz/pileup";
    TH1F* histo = (TH1F *) file->Get(puDist);
    if(!histo)std::cout<<"pileup histogram is null!!!\n";
    for(int i=1;i<=histo->GetNbinsX();i++){mcPileupDistribution.push_back(histo->GetBinContent(i));}
    delete histo;
    if(dataPileupDistribution.size()==0) dataPileupDistribution=mcPileupDistribution;
  }
  while(mcPileupDistribution.size()<dataPileupDistribution.size())  mcPileupDistribution.push_back(0.0);
  while(mcPileupDistribution.size()>dataPileupDistribution.size())dataPileupDistribution.push_back(0.0);

  gROOT->cd();  //THIS LINE IS NEEDED TO MAKE SURE THAT HISTOGRAM INTERNALLY PRODUCED IN LumiReWeighting ARE NOT DESTROYED WHEN CLOSING THE FILE
  edm::LumiReWeighting *LumiWeights=0;
  PuShifter_t PuShifters;
 //   reweight::PoissonMeanShifter *PShiftUp=0, *PShiftDown=0;
  if(isMC){
      LumiWeights= new edm::LumiReWeighting(mcPileupDistribution,dataPileupDistribution);
      PuShifters=getPUshifters(dataPileupDistribution,0.05);
   //         PShiftUp = new reweight::PoissonMeanShifter(+0.8);
     //       PShiftDown = new reweight::PoissonMeanShifter(-0.8);
  }



  //event Categorizer
//  EventCategory eventCategoryInst(0); //inclusive analysis
  EventCategory eventCategoryInst(2); //VBF, no VBF

  //##############################################
  //########           EVENT LOOP         ########
  //##############################################
  //loop on all the events
  printf("Progressing Bar     :0%%       20%%       40%%       60%%       80%%       100%%\n");
  printf("Scanning the ntuple :");
  int treeStep = (evEnd-evStart)/50;if(treeStep==0)treeStep=1;
  DuplicatesChecker duplicatesChecker;
  int nDuplicates(0);

  for( int iev=evStart; iev<evEnd; iev++){
      if((iev-evStart)%treeStep==0){printf(".");fflush(stdout);}

bool mustBlind(false);

      //##############################################   EVENT LOOP STARTS   ##############################################
   
      //load the event content from tree
      evSummaryHandler.getEntry(iev);
//      if(runBlinded && !isMC && evSummaryHandler.hasSpoilerAlert(!isMC)) continue;
      ZZ2l2nuSummary_t &ev=evSummaryHandler.getEvent();
      if(!isMC && duplicatesChecker.isDuplicate( ev.run, ev.lumi, ev.event) ) { nDuplicates++; continue; }
      PhysicsEvent_t phys=getPhysicsEventFrom(ev);
//       mustBlind = (!isMC && runBlinded && evSummaryHandler.hasSpoilerAlert(!isMC));
 

    
event_weight = xsec/cnorm;
//std::cout <<"event weight = " << event_weight << std::endl;      
//std::cout << ev.event << " "<< ev.run << " " << ev.lumi << std::endl;
      //categorize events
//      bool vbfFlag(false);
      bool vbfFlag(false);
      TString tag_cat;
      switch(ev.cat){
      case EMU : tag_cat = "emu";   break;
      case MUMU: tag_cat = "mumu";  break;
      case EE  : tag_cat = "ee";    break;
      default  : continue;
      }
      if(isMC && mctruthmode==1 && !isDYToLL(ev.mccat) ) continue;
      if(isMC && mctruthmode==2 && !isDYToTauTau(ev.mccat) ) continue;


     //require compatibilitiy of the event with the PD
      bool hasEEtrigger = ev.triggerType & 0x1;
      bool hasMMtrigger = (ev.triggerType >> 1 ) & 0x1;
      bool hasEMtrigger = (ev.triggerType >> 2 ) & 0x1;
      bool hasMtrigger  = (ev.triggerType >> 3 ) & 0x1;
      if(!isMC){
	  if(ev.cat!=fType) continue;

	  if(ev.cat==EE   && !hasEEtrigger) continue;
	  if(ev.cat==MUMU && !(hasMMtrigger||hasMtrigger) ) continue;
	  if(ev.cat==EMU  && !hasEMtrigger) continue;

	  //this is a safety veto for the single mu PD
	  if(isSingleMuPD) {
	    if(!hasMtrigger) continue;
	    if(hasMtrigger && hasMMtrigger) continue;
	  }
      }

      
      int eventSubCat  = eventCategoryInst.Get(phys,&phys.ajets);
      TString tag_subcat = eventCategoryInst.GetLabel(eventSubCat);

      //prepare the tag's vectors for histo filling
      std::vector<TString> tags_full;
      tags_full.push_back("all");
      tags_full.push_back(tag_cat);
     if(tag_subcat=="vbf") vbfFlag = true;


//if(vbfFlag == 1) {
//cout <<"vbf flag = " << vbfFlag << endl;
//cout << "********************************************************"<<endl;
//}

      //pileup weight
      float weight = 1.0;
      float weight_lsup = 1.0;
      float weight_lsdown = 1.0;
      float pu_weight = 1.0;
      float noLShapeWeight=1.0;
      float signalWeight=1.0;
      double TotalWeight_plus = 1.0;
      double TotalWeight_minus = 1.0;
      float lShapeWeights[4]={1.0,1.0,1.0,1.0};



      if(isMC){

       pu_weight            = LumiWeights->weight(useObservedPU ? ev.ngenITpu : ev.ngenTruepu);
       TotalWeight_plus  = PuShifters[PUUP]->Eval(useObservedPU ? ev.ngenITpu : ev.ngenTruepu);
       TotalWeight_minus = PuShifters[PUDOWN]->Eval(useObservedPU ? ev.ngenITpu : ev.ngenTruepu);

  //      if(isMC_VBF || isMC_GG)mon.fillHisto("higgsMass_0raw",tags_inc, phys.genhiggs[0].mass(), weight);
        //if(isMC_VBF){ signalWeight = weightVBF(VBFString,HiggsMass, phys.genhiggs[0].mass() );  weight*=signalWeight; }
  //      if(isMC_VBF || isMC_GG)mon.fillHisto("higgsMass_1vbf",tags_inc, phys.genhiggs[0].mass(), weight);

        //if(isMC_GG) {
        for(size_t iwgt=0; iwgt<hWeightsGrVec.size(); iwgt++)
          ev.hptWeights[iwgt] = hWeightsGrVec[iwgt]->Eval(phys.genhiggs[0].pt());
        weight *= ev.hptWeights[0];
          //}
 //       if(isMC_VBF || isMC_GG)mon.fillHisto("higgsMass_2qt" ,tags_inc, phys.genhiggs[0].mass(), weight);

        for(size_t iwgt=0; iwgt<hLineShapeGrVec.size(); iwgt++)
          lShapeWeights[iwgt]=hLineShapeGrVec[iwgt]->Eval(phys.genhiggs[0].mass());
        noLShapeWeight=weight;
        weight *= lShapeWeights[0];
        weight_lsup *= lShapeWeights[1];
        weight_lsdown *= lShapeWeights[2];

        //printf("lsw=%f \n",lShapeWeights[0]);
//cout << "lShapeWeights[0] = " << lShapeWeights[0] << endl;
//cout << "lShapeWeights[1] = " << lShapeWeights[1] << endl;
//cout << "lShapeWeights[2] = " << lShapeWeights[2] << endl;
//cout << "lShapeWeights[3] = " << lShapeWeights[3] << endl;
}

//cout << "*******************************************************************"<<endl;

//=================================================== Lepton Analysis ===================================================

      bool passIds(true);
      bool pass3dLeptonVeto(true);
      int nextraleptons(0);
      double llScaleFactor(1.0),llTriggerEfficiency(1.0);
      LorentzVector lep1=phys.leptons[0];
      int lpid=phys.leptons[0].pid;
      TString lepStr1( fabs(phys.leptons[0].id)==13 ? "mu" : "e");

         float relIso1 = (lepStr1=="mu") ?
          phys.leptons[0].pfRelIsoDbeta(): //muPFRelIsoCorrected2012(ev.rho25Neut):
	   phys.leptons[0].ePFRelIsoCorrected2012(ev.rho,ev.en_sceta[lpid]);


//      float relIso1 = phys.leptons[0].relIsoRho(ev.rho);
//cout << "relIso1 = " << relIso1 << endl;
//cout << "particle id = " << phys.leptons[0].id << endl;
	 if(fabs(phys.leptons[0].id)==13)      passIds &= hasObjectId(ev.mn_idbits[lpid], MID_TIGHT);
      else if(fabs(phys.leptons[0].id)==11) passIds &= hasObjectId(ev.en_idbits[lpid], EID_MEDIUM);
//      else if(fabs(phys.leptons[0].id)==11) passIds &= hasObjectId(ev.en_idbits[lpid], EID_VBTF2011);


      LorentzVector lep2=phys.leptons[1];
      lpid=phys.leptons[1].pid;
  //    float relIso2 = phys.leptons[1].relIsoRho(ev.rho);
      TString lepStr2( fabs(phys.leptons[1].id)==13 ? "mu" : "e");
          float relIso2 = (lepStr2=="mu") ?
            phys.leptons[1].pfRelIsoDbeta(): //muPFRelIsoCorrected2012(ev.rho25Neut):
            phys.leptons[1].ePFRelIsoCorrected2012(ev.rho,ev.en_sceta[lpid]);




      if(fabs(phys.leptons[1].id)==13)      passIds &= hasObjectId(ev.mn_idbits[lpid], MID_TIGHT);
      else if(fabs(phys.leptons[1].id)==11) passIds &= hasObjectId(ev.en_idbits[lpid], EID_MEDIUM); 
//      else if(fabs(phys.leptons[1].id)==11) passIds &= hasObjectId(ev.en_idbits[lpid], EID_VBTF2011); 

//cout << "relIso1 = " << relIso1 << " " << "relIso2 = " << relIso2 << endl;


if(fabs(phys.leptons[0].id)==13) 
{
if(!use2011Id)
   llTriggerEfficiency *= muonTriggerEfficiency(phys.leptons[0].pt(),fabs(phys.leptons[0].eta()),2012);
}

if(fabs(phys.leptons[0].id)==11)
{
if(!use2011Id)
 //  llTriggerEfficiency *= muonTriggerEfficiency(phys.leptons[0].pt(),fabs(phys.leptons[0].eta()),2012);
  llTriggerEfficiency *= electronTriggerEfficiency(phys.leptons[0].pt(),fabs(phys.leptons[0].eta()),2012);

}

if(fabs(phys.leptons[1].id)==13)  
{
if(!use2011Id)
   llTriggerEfficiency *= muonTriggerEfficiency(phys.leptons[1].pt(),fabs(phys.leptons[1].eta()),2012);
}

if(fabs(phys.leptons[1].id)==11)
{
if(!use2011Id)
 //  llTriggerEfficiency *= muonTriggerEfficiency(phys.leptons[0].pt(),fabs(phys.leptons[0].eta()),2012);
  llTriggerEfficiency *= electronTriggerEfficiency(phys.leptons[1].pt(),fabs(phys.leptons[1].eta()),2012);

}

//cout << " llTriggerEfficiency = " << llTriggerEfficiency <<  endl;


     LorentzVector zll=lep1+lep2;

// 3rd LEPTON ANALYSIS
	      //
	      for(size_t ilep=2; ilep<phys.leptons.size(); ilep++){
		  //lepton type
		  bool isGood(false);
		  int lpid=phys.leptons[ilep].pid;
		  if(fabs(phys.leptons[ilep].id)==13){
		      if(!use2011Id){
			  isGood = (hasObjectId(ev.mn_idbits[lpid], MID_LOOSE) && phys.leptons[ilep].pfRelIsoDbeta()<0.2);
			  isGood |= (hasObjectId(ev.mn_idbits[lpid], MID_SOFT) && phys.leptons[ilep].pt()>3);
		      }else{
			  isGood = (hasObjectId(ev.mn_idbits[lpid], MID_VBTF2011) && phys.leptons[ilep].relIsoRho(ev.rho)<0.15 && phys.leptons[ilep].pt()>10);
			  isGood |= (hasObjectId(ev.mn_idbits[lpid], MID_SOFT2011) && phys.leptons[ilep].pt()>3);
		      }
	          }else{
		      if(!use2011Id){
			isGood = ( hasObjectId(ev.en_idbits[lpid],EID_VETO) && phys.leptons[ilep].ePFRelIsoCorrected2012(ev.rho,ev.en_sceta[lpid])<0.15 && phys.leptons[ilep].pt()>10);
		      }else{
			  isGood = ( hasObjectId(ev.en_idbits[lpid],EID_VBTF2011) && phys.leptons[ilep].relIsoRho(ev.rho)<0.1 && phys.leptons[ilep].pt()>10);
		      }
		  }
		  nextraleptons += isGood;
		  
		  if(!isGood) continue;
	      }
	      
	      pass3dLeptonVeto=(nextraleptons==0);
      
      bool pass3rdLeptonVeto(ev.ln==0);

//cout << " pass3dLeptonVeto = " << pass3dLeptonVeto << endl;


//========================================= JET/MET ANALYSIS ===================================================================

      //MET variables
      LorentzVector rawMetP4=phys.met[2];
      LorentzVector fullTypeIMetP4=phys.met[0];
      LorentzVector typeIMetP4=rawMetP4;
      for(size_t ijet=0; ijet<phys.ajets.size();ijet++)
        {
          LorentzVector jetP4=phys.ajets[ijet];
          LorentzVector rawJetP4=jetP4*phys.ajets[ijet].rawCorrFact;
          typeIMetP4 -= (jetP4-rawJetP4);
        }
      LorentzVector mvaMetP4=phys.met[7];

      //apply JER base corrections to jets (and compute associated variations on the MET variable)
      std::vector<PhysicsObjectJetCollection> variedAJets;
      LorentzVectorCollection zvvs;
      METUtils::computeVariation(phys.ajets, phys.leptons, rawMetP4, variedAJets, zvvs, &jecUnc);
                  //std PF
                  PhysicsObjectJetCollection aJets= variedAJets[0];
                  PhysicsObjectJetCollection aGoodIdJets;
                  LorentzVector aClusteredMetP4(zll); aClusteredMetP4 *= -1;
                  int nAJetsLoose(0), nAJetsTight(0), nAJetsPUIdLoose(0), nAJetsPUIdMedium(0), nAJetsGood30(0);
                  int nABtags[3]={0,0,0};
		  double sumet(0);
                  float mindphijmet(999999.),mindphijmet15(999999.);
                  PhysicsObjectJetCollection recoilJets;
                  for(size_t ijet=0; ijet<aJets.size(); ijet++)
                    {
  // 		cout << "aJets[ijet].pt() = " << aJets[ijet].pt() << endl;
                      float idphijmet( fabs(deltaPhi(aJets[ijet].phi(),zvvs[0].phi()) ) );
//cout << "idphijmet = " << idphijmet << endl;
                      if(idphijmet<mindphijmet15)  mindphijmet15=idphijmet;
                      if(aJets[ijet].pt()>30) if(idphijmet<mindphijmet)  mindphijmet=idphijmet;
                      if(fabs(deltaPhi(aJets[ijet].phi(),zll.phi()))>2) recoilJets.push_back( aJets[ijet] );

//cout << "mindphijmet = " << mindphijmet << endl;

                      //bool isGoodJet    =hasObjectId(aJets[ijet].pid,JETID_LOOSE);//TIGHT);
                      bool isGoodJet    =hasObjectId(aJets[ijet].pid,JETID_CUTBASED_LOOSE);
                      if(isGoodJet)
                        {
                          if(aJets[ijet].pt()>30)nAJetsGood30++;
                          aClusteredMetP4 -= aJets[ijet];
                          aGoodIdJets.push_back(aJets[ijet]);
			  sumet+=aJets[ijet].pt();
                        }

                      if(aJets[ijet].pt()<30) continue;

                      if(fabs(aJets[ijet].eta())<2.5)
                        {
                          nABtags[0] += (aJets[ijet].btag1>2.0);
                          nABtags[1] += (aJets[ijet].btag2>0.244);
                          nABtags[2] += (aJets[ijet].btag3>0.275);
                          Float_t ijetbtags[]={aJets[ijet].btag1, aJets[ijet].btag2, aJets[ijet].btag3};
                        }
                      nAJetsLoose      += hasObjectId(aJets[ijet].pid,JETID_LOOSE);
                      nAJetsTight      += hasObjectId(aJets[ijet].pid,JETID_TIGHT);
                      nAJetsPUIdLoose  += hasObjectId(aJets[ijet].pid,JETID_OPT_LOOSE);
                      nAJetsPUIdMedium += hasObjectId(aJets[ijet].pid,JETID_OPT_MEDIUM);

                    } // for(size_t ijet=0; ijet<aJets.size(); ijet++)


    bool passBveto(nABtags[2]==0);


//VBF monitoring
                          double dphijj(-1),hardpt(-1), vbfmjj(0.), vbfdetajj(-1.), vbfdphijj(-1.);
                          if(aGoodIdJets.size()>=2)
                          {
                              LorentzVector vbfSyst=aGoodIdJets[0]+aGoodIdJets[1];
                              LorentzVector hardSyst=vbfSyst+zvvs[0]+zll;
                              hardpt=hardSyst.pt();
                              dphijj=deltaPhi(aGoodIdJets[0].phi(),aGoodIdJets[1].phi());
                              double maxEta=max(aGoodIdJets[0].eta(),aGoodIdJets[1].eta());
                              double minEta=min(aGoodIdJets[0].eta(),aGoodIdJets[1].eta());
                              float avgEtajj=0.5*(maxEta+minEta);
                              float detajj=maxEta-minEta;
                              if(aGoodIdJets[0].pt()>30 && aGoodIdJets[1].pt()>30){
                                vbfdetajj =   fabs(detajj);
                                 vbfmjj =  vbfSyst.mass(); 
				 vbfdphijj = dphijj;} 
}

//cout << "mindphijmet = " << mindphijmet << endl; 

//cout <<"aJets.size() = " << aJets.size() << endl;
//cout <<" nAJetsGood30 =  " << nAJetsGood30 << endl;
//cout <<"aGoodIdJets.size() = " << aGoodIdJets.size() << endl; 
//cout <<" nABtags[2] = " << nABtags[2] << endl;
//cout << " passBveto = " << passBveto << endl;
//cout << "****************************************************************" <<endl; 


//      bool passBveto(nbtags==0);

//cout << " passBveto = " << passBveto << endl;

                METUtils::stRedMET aRedMetOut;
                LorentzVector aRedMet=METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, lep1, 0, lep2, 0, aClusteredMetP4, zvvs[0],false,&aRedMetOut);
                double aRedMetL=aRedMetOut.redMET_l;
                double aRedMetT=aRedMetOut.redMET_t;
                double aMT=METUtils::transverseMass(zll,zvvs[0],true);
                double amvaMT=METUtils::transverseMass(zll,mvaMetP4,true);


//cout << "reduced MET = " << aRedMet.pt() << endl;
//cout << "zvvs[0].pt() = " << zvvs[0].pt() << endl;
//cout << "Type 1 MET = " << zvvs[0].pt() << endl;
//cout << "rawMetP4.pt() = " << rawMetP4.pt() << endl;
//cout << "zvvs[2].pt() = " << zvvs[2].pt() << endl;
//cout << "typeIMetP4v = " <<typeIMetP4.pt() << endl;
//cout << "***************************************************************" <<endl;

//cout << "MVA MET = " << mvaMetP4.pt() << endl;

//cout << "size of zvvs = " << zvvs.size() << endl;
/*
cout << "zvvs[0] = " << zvvs[0].pt() << endl;
cout << "zvvs[1] = " << zvvs[1].pt() << endl;
cout << "zvvs[2] = " << zvvs[2].pt() << endl;
cout << "zvvs[3] = " << zvvs[3].pt() << endl;
cout << "zvvs[4] = " << zvvs[4].pt() << endl;
cout << "zvvs[5] = " << zvvs[5].pt() << endl;
cout << "zvvs[6] = " << zvvs[6].pt() << endl;
cout << "zvvs[7] = " << zvvs[7].pt() << endl;
cout << "zvvs[8] = " << zvvs[8].pt() << endl;
*/



//if(mustBlind == 1) {cout << "mustBlind = " << mustBlind << endl;      }

//===================================================================== Systematics ========================================================

//cout << " nvarsToInclude = " << nvarsToInclude << endl;

        float weight_PUup = 1.0;
        float weight_PUdown = 1.0;
	std::vector<double> mtvar;
	std::vector<double> metvar;
	std::vector<double> dphijetmetvar;

      for(size_t ivar=0; ivar<nvarsToInclude; ivar++){
        float iweight = weight;                                               //nominal
        if(ivar==9)                         weight_PUup = TotalWeight_plus;        //pu up
        if(ivar==10)                        weight_PUdown = TotalWeight_minus;       //pu down
        if(ivar<=14 && ivar>=11 && isMC_GG) iweight *=ev.hptWeights[ivar-10]/ev.hptWeights[0];   //ren/fact     scales   
        if((ivar==17 || ivar==18) && isMC_GG) iweight *= (lShapeWeights[0]!=0?lShapeWeights[ivar-16]/lShapeWeights[0] : 1.0); //lineshape weights

        //recompute MET/MT if JES/JER was varied
        LorentzVector zvv    = zvvs[ivar>8 ? 0 : ivar];
        //float mt3(0);
        //      if(nextraleptons==1) mt3 = METUtils::transverseMass(extraLeptonsP4[0],zvv,false);
        PhysicsObjectJetCollection &varJets=variedAJets[ivar>4 ? 0  : ivar];
        PhysicsObjectJetCollection tightVarJets;
        LorentzVector clusteredMetP4(zll); clusteredMetP4 *= -1;
        bool passLocalBveto(passBveto);
	float mindphijmet_var(999999.);
        for(size_t ijet=0; ijet<varJets.size(); ijet++){
            if(!hasObjectId(varJets[ijet].pid,JETID_LOOSE)) continue;
            clusteredMetP4 -= varJets[ijet];
            tightVarJets.push_back( varJets[ijet] );
            float idphijmet_var( fabs(deltaPhi(varJets[ijet].phi(),zvv.phi()) ) );
            if(varJets[ijet].pt()>30) if(idphijmet_var<mindphijmet_var)  mindphijmet_var=idphijmet_var;
            if(varJets[ijet].pt()<30 || fabs(varJets[ijet].eta())>2.5)continue;
            if(ivar==15)      passLocalBveto &= (varJets[ijet].btag3<0.285);
            else if(ivar==16) passLocalBveto &= (varJets[ijet].btag3<0.265);
//cout << " ivar = " << ivar << endl;
        }

	dphijetmetvar.push_back(mindphijmet_var);
//cout << "mindphijmet_var = " << mindphijmet_var <<endl;
         mtvar.push_back(METUtils::transverseMass(zll,zvv,true));
        LorentzVector nullP4(0,0,0,0);
        LorentzVector redMet = METUtils::redMET(METUtils::INDEPENDENTLYMINIMIZED, zll, 0, nullP4, 0, clusteredMetP4, zvv,true);

//if(ivar < 9 && ivar > 0) {


//cout << " ivar = " << ivar << endl;
//cout << "ivar = " << ivar << " mt = " << (METUtils::transverseMass(zll,zvv,true)) << endl;
//cout << "weight_PUup = " << weight_PUup << " weight_PUdown =  " << weight_PUdown << endl;

}

//cout << " hzz_TransMass_Higgs_JERup = " << hzz_TransMass_Higgs_JERup << endl;
//cout << "***************************************************************************** " <<endl;
//============================================================ FILLING OF BRANCHES===========================================================


	if(passIds && pass3rdLeptonVeto && passBveto) {
//      if(passIds && pass3rdLeptonVeto &&  mustBlind == 0) {
//if(passIds && pass3rdLeptonVeto) {

hzz_weight_PUup = weight_PUup;
hzz_weight_PUdown = weight_PUdown;

hzz_TransMass_Higgs_JERup = mtvar[1];
hzz_TransMass_Higgs_JERdown = mtvar[2];
hzz_TransMass_Higgs_JESup = mtvar[3];
hzz_TransMass_Higgs_JESdown = mtvar[4];
hzz_TransMass_Higgs_UMETup = mtvar[5];
hzz_TransMass_Higgs_UMETdown = mtvar[6];
hzz_TransMass_Higgs_LESup = mtvar[7];
hzz_TransMass_Higgs_LESdown = mtvar[8];

hzz_RawMet_JERup = zvvs[1].pt();
hzz_RawMet_JERdown = zvvs[2].pt();
hzz_RawMet_JESup = zvvs[3].pt();
hzz_RawMet_JESdown = zvvs[4].pt();
hzz_RawMet_UMETup = zvvs[5].pt();
hzz_RawMet_UMETdown = zvvs[6].pt();
hzz_RawMet_LESup = zvvs[7].pt();
hzz_RawMet_LESdown = zvvs[8].pt();

hzz_dPhi_JetMet_JERup = dphijetmetvar[1];
hzz_dPhi_JetMet_JERdown = dphijetmetvar[2];
hzz_dPhi_JetMet_JESup = dphijetmetvar[3];
hzz_dPhi_JetMet_JESdown = dphijetmetvar[4];
hzz_dPhi_JetMet_UMETup = dphijetmetvar[5];
hzz_dPhi_JetMet_UMETdown = dphijetmetvar[6];
hzz_dPhi_JetMet_LESup = dphijetmetvar[7];
hzz_dPhi_JetMet_LESdown = dphijetmetvar[8];


//cout << "weight_PUup = " << hzz_weight_PUup << " weight_PUdown =  " << hzz_weight_PUdown << endl;

hzz_vbfdetajj = vbfdetajj;
hzz_vbfmjj = vbfmjj;
hzz_vbfdphijj = vbfdphijj;
hzz_RedMet = aRedMet.pt();
hzz_RedMetL = aRedMetL;
hzz_RedMetT = aRedMetT;
hzz_RawMet = rawMetP4.pt();
hzz_MVAMet = mvaMetP4.pt();
//hzz_BTag = btag_discrm;
lineshape_weight = weight;
lineshape_weight_up =  weight_lsup;
lineshape_weight_down =  weight_lsdown;
hzz_lept1_gIso = phys.leptons[0].gIso;
hzz_lept2_gIso = phys.leptons[1].gIso;
hzz_lept1_chIso = phys.leptons[0].chIso;
hzz_lept2_chIso = phys.leptons[1].chIso;
hzz_lept1_nhIso = phys.leptons[0].nhIso;
hzz_lept2_nhIso = phys.leptons[1].nhIso;
hzz_rho = ev.rho25;

	vbf_flag = vbfFlag;
	ev_num = ev.event;
	run_num = ev.run;
	lumi_num = ev.lumi;
      hzz_lept1_ID = fabs(phys.leptons[0].id);
      hzz_lept1_PT = lep1.pt();
      hzz_lept1_ETA = lep1.eta();
      hzz_lept1_PHI = lep1.phi();
      hzz_lept1_DetIso = phys.leptons[0].relIsoRho(ev.rho);
      hzz_lept1_PFIsoDbeta = phys.leptons[0].pfRelIsoDbeta();
//      hzz_lept1_PFIso2012 = phys.leptons[0].ePFRelIsoCorrected2012(ev.rho);
      hzz_lept1_PFIso2012 = relIso1;

      hzz_lept2_ID = fabs(phys.leptons[1].id);
      hzz_lept2_PT = lep2.pt();
      hzz_lept2_ETA = lep2.eta();
      hzz_lept2_PHI = lep2.phi();
      hzz_lept2_DetIso = phys.leptons[1].relIsoRho(ev.rho);
      hzz_lept2_PFIsoDbeta = phys.leptons[1].pfRelIsoDbeta();
//      hzz_lept2_PFIso2012 = phys.leptons[1].ePFRelIsoCorrected2012(ev.rho);
      hzz_lept2_PFIso2012 = relIso2;

      hzz_Z_Mass = zll.mass();
      hzz_Z_PT = zll.pt();
      hzz_Z_Eta = zll.eta();
      hzz_Z_Phi = zll.phi();
      PUWeight = pu_weight;
      hzz_PFMET = zvvs[0].pt();
      hzz_MET_Phi = zvvs[0].phi();
//      hzz_TransMass_Higgs = mts[0];
      hzz_TransMass_Higgs = aMT;
      ev_category = ev.cat;

      //cout << " Z mass =  " << hzz_Z_Mass << endl;
      //cout << " PU Weight =  " << weight << endl;
      //cout << " PFMET = " << zvvs[0].pt() << endl;
      //cout << " lepton 1 PT =  " << lep1.pt() << "Particle ID = " << phys.leptons[0].pid << " lepton 1 PT =  " << phys.leptons[0].pt()<< endl;
//      cout << " Event category =  " << ev.cat << endl;

//cout << " hzz_lept1_PFIso2012 = " << hzz_lept1_PFIso2012 << " " << "hzz_lept2_PFIso2012 = " << hzz_lept2_PFIso2012 << endl;
//cout << " hzz_lept1_PFIsoDbeta = " << hzz_lept1_PFIsoDbeta << " " << "hzz_lept2_PFIsoDbeta = " << hzz_lept2_PFIsoDbeta << endl;
//cout <<"****************************************************************************************************" <<endl;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Jets Filling ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//PhysicsObjectJetCollection correctedJets = phys.ajets;
//cout << " Number of Jets = " << correctedJets.size() << endl;
//cout<< "sumet = " << sumet <<endl;
//cout << " Number of Jets = " << hzz_N_Jets << endl;
//cout << " hzz_TransMass_Higgs = " << hzz_TransMass_Higgs << endl;
//cout << "***************************************************************" <<endl;
//hzz_Jet_PT = high_PT;
//hzz_N_Jets = njets;
if(aGoodIdJets.size() !=0){
hzz_Jet_PT = aGoodIdJets[0].pt();
hzz_Jet_ETA = aGoodIdJets[0].eta();
hzz_Jet_PHI = aGoodIdJets[0].phi();
hzz_Jet_Energy = aGoodIdJets[0].energy();
}

hzz_N_Jets = nAJetsGood30;
//cout << " Leading Jet PT = " << hzz_Jet_PT << endl;
//cout << " Number of Jets = " << hzz_N_Jets << endl;
hzz_dPhi_JetMet = mindphijmet;
hzz_Jet_SumPT = sumet;
//if(ev_num == 587401) {
//cout << "ev_num = " << ev_num << " hzz_dPhi_JetMet = " << hzz_dPhi_JetMet << endl; }

//cout <<"hzz_weight_PUup = " << hzz_weight_PUup << endl;
//cout <<"hzz_weight_PUdown = " << hzz_weight_PUdown << endl;
//cout <<"hzz_weight_PU = " << PUWeight << endl;


//cout << "***********************************************************************************************************" << endl;
      hzz_tree->Fill();
}//
  }
  
  printf("\n"); 
  file->Close();

  outUrl += "/";
  outUrl += gSystem->BaseName(url);
  printf("Results save in %s\n", outUrl.Data());

  //save all to the file
  TFile *ofile=TFile::Open(outUrl, "recreate");
//  mon.Write();
hzz_tree->Write();
Hoptim_systs->Write();
  ofile->Close();

  if(outTxtFile)fclose(outTxtFile);
}  
