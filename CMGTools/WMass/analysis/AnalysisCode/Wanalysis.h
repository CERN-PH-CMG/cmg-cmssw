//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Oct 13 17:11:37 2012 by ROOT version 5.27/06b
// from TTree WTreeProducer/WTreeProducer
// found on file: WTreeProducer_tree.root
//////////////////////////////////////////////////////////

#ifndef Wanalysis_h
#define Wanalysis_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>
#include <TSystem.h>

//#include "common.h"
#include "common_stuff.h"


using namespace std;

class Wanalysis {
  public :
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain
  double          lumi_scaling;
  int             useGenVar;
  
  // Declaration of leaf types
  Double_t        scalePDF;
  Double_t        parton1_pdgId;
  Double_t        parton1_x;
  Double_t        parton2_pdgId;
  Double_t        parton2_x;
  Int_t           run;
  Int_t           lumi;
  Int_t           evt;
  Int_t           nvtx;
  Int_t           npu;
  Int_t           evtHasGoodVtx;
  Int_t           evtHasTrg;
  Int_t           evtWSel;
  Int_t           nMuons;
  Int_t           nTrgMuons;
  Int_t           noTrgMuonsLeadingPt;
  Double_t        tkmet;
  Double_t        tkmet_phi;
  Double_t        nopumet;
  Double_t        nopumet_phi;
  Double_t        pfmet;
  Double_t        pfmet_phi;
  Double_t        pfmet_sumEt;
  Double_t        W_pt;
  Double_t        W_mt;
  Double_t        W_phi;
  Double_t        WGen_pt;
  Double_t        WGen_phi;
  Double_t        WGen_rap;
  Double_t        WGen_m;
  Double_t        WGen_mt;
  Double_t        u1;
  Double_t        u2;
  Double_t        Mu_pt;
  Double_t        Mu_eta;
  Double_t        Mu_phi;
  Double_t        Mu_mass;
  Double_t        Mu_charge;
  Double_t        Mu_dxy;
  Int_t           MuIsTightAndIso;
  Int_t           MuIsTight;
  Double_t        MuRelIso;
  Double_t        MuGen_pt;
  Double_t        MuGen_eta;
  Double_t        MuGen_phi;
  Double_t        MuGen_mass;
  Double_t        MuGen_charge;
  Double_t        MuDRGenP;
  Double_t        NuGen_pt;
  Double_t        NuGen_eta;
  Double_t        NuGen_phi;
  Double_t        NuGen_mass;
  Double_t        NuGen_charge;
  Double_t        Jet_leading_pt;
  Double_t        Jet_leading_eta;
  Double_t        Jet_leading_phi;
  Double_t        FSRWeight;
  Double_t        LHE_weight[465];
  
  // List of branches
  TBranch        *b_scalePDF;   //!
  TBranch        *b_parton1_pdgId;   //!
  TBranch        *b_parton1_x;   //!
  TBranch        *b_parton2_pdgId;   //!
  TBranch        *b_parton2_x;   //!
  TBranch        *b_run;   //!
  TBranch        *b_lumi;   //!
  TBranch        *b_evt;   //!
  TBranch        *b_nvtx;   //!
  TBranch        *b_npu;   //!
  TBranch        *b_evtHasGoodVtx;   //!
  TBranch        *b_evtHasTrg;   //!
  TBranch        *b_evtWSel;   //!
  TBranch        *b_nMuons;   //!
  TBranch        *b_nTrgMuons;   //!
  TBranch        *b_noTrgMuonsLeadingPt;   //!
  TBranch        *b_tkmet;   //!
  TBranch        *b_tkmet_phi;   //!
  TBranch        *b_nopumet;   //!
  TBranch        *b_nopumet_phi;   //!
  TBranch        *b_pfmet;   //!
  TBranch        *b_pfmet_phi;   //!
  TBranch        *b_pfmet_sumEt;   //!
  TBranch        *b_W_pt;   //!
  TBranch        *b_W_mt;   //!
  TBranch        *b_W_phi;   //!
  TBranch        *b_WGen_pt;   //!
  TBranch        *b_WGen_phi;   //!
  TBranch        *b_WGen_rap;   //!
  TBranch        *b_WGen_m;   //!
  TBranch        *b_WGen_mt;   //!
  TBranch        *b_u1;   //!
  TBranch        *b_u2;   //!
  TBranch        *b_Mu_pt;   //!
  TBranch        *b_Mu_eta;   //!
  TBranch        *b_Mu_phi;   //!
  TBranch        *b_Mu_mass;   //!
  TBranch        *b_Mu_charge;   //!
  TBranch        *b_Mu_dxy;   //!
  TBranch        *b_MuIsTightAndIso;   //!
  TBranch        *b_MuIsTight;   //!
  TBranch        *b_MuRelIso;   //!
  TBranch        *b_MuGen_pt;   //!
  TBranch        *b_MuGen_eta;   //!
  TBranch        *b_MuGen_phi;   //!
  TBranch        *b_MuGen_mass;   //!
  TBranch        *b_MuGen_charge;   //!
  TBranch        *b_MuDRGenP;   //!
  TBranch        *b_NuGen_pt;   //!
  TBranch        *b_NuGen_eta;   //!
  TBranch        *b_NuGen_phi;   //!
  TBranch        *b_NuGen_mass;   //!
  TBranch        *b_NuGen_charge;   //!
  TBranch        *b_Jet_leading_pt;   //!
  TBranch        *b_Jet_leading_eta;   //!
  TBranch        *b_Jet_leading_phi;   //!
  TBranch        *b_FSRWeight;   //!
  TBranch        *b_LHE_weight;   //!
  
  Wanalysis(TString f_str=0, double lumi_scaling_input=1, int useGen=0, TTree *tree=0);
  virtual ~Wanalysis();
  virtual Int_t    Cut(Long64_t entry);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual Long64_t NumEntries();
  virtual void     Init(TTree *tree);
  virtual void     Loop(int chunk=0, int Entry_ini=0, int Entry_fin=0, int IS_MC_CLOSURE_TEST=0, int isMCorDATA=0, TString outputdir=0, int useRochCorr=0, int smearRochCorr=0, int useEffSF=0, int usePtSF=0, int useVtxSF=0, int controlplots=0, TString sampleName="", int generated_PDF_set=-1, int generated_PDF_member=-1, int contains_PDF_reweight=-1, int usePhiMETCorr=0, int useRecoilCorr=0, int RecoilCorrResolutionNSigmaU1=0, int RecoilCorrScaleNSigmaU1=0, int RecoilCorrResolutionNSigmaU2=0, int use_PForNoPUorTKmet=0, int use_syst_ewk_Alcaraz=0, int gen_mass_value_MeV=0, int contains_LHE_weights=0);
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef Wanalysis_cxx
Wanalysis::Wanalysis(TString f_str, double lumi_scaling_input, int useGen, TTree *tree)
{
  // if parameter tree is not specified (or zero), connect the file
  // used to generate this class and read the Tree.
  if (tree == 0) {
    // TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("temp/WTreeProducer_tree.root");
    // if (!f) {
    cout << gSystem->WorkingDirectory() << endl;
    // TFile *f = new TFile(Form("%s",f_str.Data()));
    TFile *f = TFile::Open(Form("%s",f_str.Data()));
    // }
    tree = (TTree*)gDirectory->Get("WTreeProducer");

  }
  lumi_scaling=lumi_scaling_input;
  useGenVar=useGen;
  Init(tree);
}

Wanalysis::~Wanalysis()
{
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

Int_t Wanalysis::GetEntry(Long64_t entry)
{
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}
Long64_t Wanalysis::LoadTree(Long64_t entry)
{
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (!fChain->InheritsFrom(TChain::Class()))  return centry;
  TChain *chain = (TChain*)fChain;
  if (chain->GetTreeNumber() != fCurrent) {
    fCurrent = chain->GetTreeNumber();
    Notify();
  }
  return centry;
}
Long64_t Wanalysis::NumEntries(){

  if (!fChain) return 0;
  return fChain->GetEntries();
};

void Wanalysis::Init(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  fChain->SetBranchAddress("scalePDF", &scalePDF, &b_scalePDF);
  fChain->SetBranchAddress("parton1_pdgId", &parton1_pdgId, &b_parton1_pdgId);
  fChain->SetBranchAddress("parton1_x", &parton1_x, &b_parton1_x);
  fChain->SetBranchAddress("parton2_pdgId", &parton2_pdgId, &b_parton2_pdgId);
  fChain->SetBranchAddress("parton2_x", &parton2_x, &b_parton2_x);
  fChain->SetBranchAddress("run", &run, &b_run);
  fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
  fChain->SetBranchAddress("evt", &evt, &b_evt);
  fChain->SetBranchAddress("nvtx", &nvtx, &b_nvtx);
  fChain->SetBranchAddress("npu", &npu, &b_npu);
  fChain->SetBranchAddress("evtHasGoodVtx", &evtHasGoodVtx, &b_evtHasGoodVtx);
  fChain->SetBranchAddress("evtHasTrg", &evtHasTrg, &b_evtHasTrg);
  fChain->SetBranchAddress("evtWSel", &evtWSel, &b_evtWSel);
  fChain->SetBranchAddress("nMuons", &nMuons, &b_nMuons);
  fChain->SetBranchAddress("nTrgMuons", &nTrgMuons, &b_nTrgMuons);
  fChain->SetBranchAddress("noTrgMuonsLeadingPt", &noTrgMuonsLeadingPt, &b_noTrgMuonsLeadingPt);
  fChain->SetBranchAddress("tkmet", &tkmet, &b_tkmet);
  fChain->SetBranchAddress("tkmet_phi", &tkmet_phi, &b_tkmet_phi);
  fChain->SetBranchAddress("nopumet", &nopumet, &b_nopumet);
  fChain->SetBranchAddress("nopumet_phi", &nopumet_phi, &b_nopumet_phi);
  fChain->SetBranchAddress("pfmet", &pfmet, &b_pfmet);
  fChain->SetBranchAddress("pfmet_phi", &pfmet_phi, &b_pfmet_phi);
  fChain->SetBranchAddress("pfmet_sumEt", &pfmet_sumEt, &b_pfmet_sumEt);
  fChain->SetBranchAddress("W_pt", &W_pt, &b_W_pt);
  fChain->SetBranchAddress("W_mt", &W_mt, &b_W_mt);
  fChain->SetBranchAddress("W_phi", &W_phi, &b_W_phi);
  fChain->SetBranchAddress("u1", &u1, &b_u1);
  fChain->SetBranchAddress("u2", &u2, &b_u2);
  fChain->SetBranchAddress("Mu_pt", &Mu_pt, &b_Mu_pt);
  fChain->SetBranchAddress("Mu_eta", &Mu_eta, &b_Mu_eta);
  fChain->SetBranchAddress("Mu_phi", &Mu_phi, &b_Mu_phi);
  fChain->SetBranchAddress("Mu_mass", &Mu_mass, &b_Mu_mass);
  fChain->SetBranchAddress("Mu_charge", &Mu_charge, &b_Mu_charge);
  fChain->SetBranchAddress("Mu_dxy", &Mu_dxy, &b_Mu_dxy);
  fChain->SetBranchAddress("MuIsTightAndIso", &MuIsTightAndIso, &b_MuIsTightAndIso);
  fChain->SetBranchAddress("MuIsTight", &MuIsTight, &b_MuIsTight);
  fChain->SetBranchAddress("MuRelIso", &MuRelIso, &b_MuRelIso);
  fChain->SetBranchAddress("Jet_leading_pt", &Jet_leading_pt, &b_Jet_leading_pt);
  fChain->SetBranchAddress("Jet_leading_eta", &Jet_leading_eta, &b_Jet_leading_eta);
  fChain->SetBranchAddress("Jet_leading_phi", &Jet_leading_phi, &b_Jet_leading_phi);
  fChain->SetBranchAddress("FSRWeight", &FSRWeight, &b_FSRWeight);
  
  if(useGenVar){
    fChain->SetBranchAddress("LHE_weight", LHE_weight, &b_LHE_weight);
    fChain->SetBranchAddress("WGen_m", &WGen_m, &b_WGen_m);
    fChain->SetBranchAddress("WGen_pt", &WGen_pt, &b_WGen_pt);
    fChain->SetBranchAddress("WGen_phi", &WGen_phi, &b_WGen_phi);
    fChain->SetBranchAddress("WGen_rap", &WGen_rap, &b_WGen_rap);
    fChain->SetBranchAddress("WGen_mt", &WGen_mt, &b_WGen_mt);
    fChain->SetBranchAddress("MuGen_pt", &MuGen_pt, &b_MuGen_pt);
    fChain->SetBranchAddress("MuGen_eta", &MuGen_eta, &b_MuGen_eta);
    fChain->SetBranchAddress("MuGen_phi", &MuGen_phi, &b_MuGen_phi);
    fChain->SetBranchAddress("MuGen_mass", &MuGen_mass, &b_MuGen_mass);
    fChain->SetBranchAddress("MuGen_charge", &MuGen_charge, &b_MuGen_charge);
    fChain->SetBranchAddress("MuDRGenP", &MuDRGenP, &b_MuDRGenP);
    fChain->SetBranchAddress("NuGen_pt", &NuGen_pt, &b_NuGen_pt);
    fChain->SetBranchAddress("NuGen_eta", &NuGen_eta, &b_NuGen_eta);
    fChain->SetBranchAddress("NuGen_phi", &NuGen_phi, &b_NuGen_phi);
    fChain->SetBranchAddress("NuGen_mass", &NuGen_mass, &b_NuGen_mass);
    fChain->SetBranchAddress("NuGen_charge", &NuGen_charge, &b_NuGen_charge);
  }
  Notify();
}

Bool_t Wanalysis::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}

void Wanalysis::Show(Long64_t entry)
{
  // Print contents of entry.
  // If entry is not specified, print current entry
  if (!fChain) return;
  fChain->Show(entry);
}
Int_t Wanalysis::Cut(Long64_t entry)
{
  // This function may be called from Loop.
  // returns  1 if entry is accepted.
  // returns -1 otherwise.
  return 1;
}
#endif // #ifdef Wanalysis_cxx
