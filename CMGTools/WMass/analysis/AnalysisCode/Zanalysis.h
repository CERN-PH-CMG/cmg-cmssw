//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Oct 13 13:57:16 2012 by ROOT version 5.27/06b
// from TTree ZTreeProducer/ZTreeProducer
// found on file: ZTreeProducer_tree.root
//////////////////////////////////////////////////////////

#ifndef Zanalysis_h
#define Zanalysis_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>
#include <TSystem.h>

#include <TLorentzVector.h>

#include "common_stuff.h"

using namespace std;

class Zanalysis {
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
  Int_t           evtZSel;
  Int_t           nMuons;
  Int_t           nTrgMuons;
  Int_t           nNoTrgMuons;
  Int_t           noTrgExtraMuonsLeadingPt;  
  Double_t        pfmet;
  Double_t        pfmet_phi;
  Double_t        pfmet_sumEt;
  Double_t        nopumet;
  Double_t        nopumet_phi;
  Double_t        nopumet_sumEt;
  Double_t        tkmet;
  Double_t        tkmet_phi;
  Double_t        tkmet_sumEt;
  Double_t        pfmetWlikeNeg;
  Double_t        pfmetWlikeNeg_phi;
  Double_t        pfmetWlikePos;
  Double_t        pfmetWlikePos_phi;
  Double_t        WlikePos_pt;
  Double_t        WlikePos_phi;
  Double_t        WlikePos_mt;
  Double_t        WlikeNeg_pt;
  Double_t        WlikeNeg_phi;
  Double_t        WlikeNeg_mt;
  Double_t        Z_pt;
  Double_t        Z_rap;
  Double_t        Z_phi;
  Double_t        Z_mass;
  Double_t        Z_mt;
  Double_t        ZGen_pt;
  Double_t        ZGen_rap;
  Double_t        ZGen_phi;
  Double_t        ZGen_mass;
  Double_t        ZGen_mt;
  Double_t        u1;
  Double_t        u2;
  Double_t        MuPos_pt;
  Double_t        MuPos_eta;
  Double_t        MuPos_phi;
  Double_t        MuPos_mass;
  Double_t        MuPos_charge;
  Double_t        MuPos_dxy;
  Double_t        MuPosRelIso;
  Int_t           MuPosTrg;
  Int_t           MuPosIsTightAndIso;
  Int_t           MuPosIsTight;
  Double_t        MuPosGen_pt;
  Double_t        MuPosGen_eta;
  Double_t        MuPosGen_phi;
  Double_t        MuPosGen_mass;
  Double_t        MuPosGen_charge;
  Double_t        MuPosDRGenP;
  Double_t        MuNeg_pt;
  Double_t        MuNeg_eta;
  Double_t        MuNeg_phi;
  Double_t        MuNeg_mass;
  Double_t        MuNeg_charge;
  Double_t        MuNeg_dxy;
  Double_t        MuNegRelIso;
  Int_t           MuNegTrg;
  Int_t           MuNegIsTightAndIso;
  Int_t           MuNegIsTight;
  Double_t        MuNegGen_pt;
  Double_t        MuNegGen_eta;
  Double_t        MuNegGen_phi;
  Double_t        MuNegGen_mass;
  Double_t        MuNegGen_charge;
  Double_t        MuNegDRGenP;
  Double_t        Jet_leading_pt;
  Double_t        Jet_leading_eta;
  Double_t        Jet_leading_phi;
  Double_t        FSRWeight;
  Double_t        LHE_weight[465];
  Double_t        LHE_ren[200];
  Double_t        LHE_fac[200];
  Double_t        LHE_pdf[200];
  Double_t        ZGen_PostFSR_mass;

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
  TBranch        *b_evtZSel;   //!
  TBranch        *b_nMuons;   //!
  TBranch        *b_nTrgMuons;   //!
  TBranch        *b_nNoTrgMuons;   //!
  TBranch        *b_noTrgExtraMuonsLeadingPt;   //!
  TBranch        *b_pfmet;   //!
  TBranch        *b_pfmet_phi;   //!
  TBranch        *b_pfmet_sumEt;   //!
  TBranch        *b_nopumet;   //!
  TBranch        *b_nopumet_phi;   //!
  TBranch        *b_nopumet_sumEt;   //!
  TBranch        *b_tkmet;   //!
  TBranch        *b_tkmet_phi;   //!
  TBranch        *b_tkmet_sumEt;   //!
  TBranch        *b_pfmetWlikeNeg;   //!
  TBranch        *b_pfmetWlikeNeg_phi;   //!
  TBranch        *b_pfmetWlikePos;   //!
  TBranch        *b_pfmetWlikePos_phi;   //!
  TBranch        *b_WlikePos_pt;   //!
  TBranch        *b_WlikePos_phi;   //!
  TBranch        *b_WlikePos_mt;   //!
  TBranch        *b_WlikeNeg_pt;   //!
  TBranch        *b_WlikeNeg_phi;   //!
  TBranch        *b_WlikeNeg_mt;   //!
  TBranch        *b_Z_pt;   //!
  TBranch        *b_Z_rap;   //!
  TBranch        *b_Z_phi;   //!
  TBranch        *b_Z_mass;   //!
  TBranch        *b_Z_mt;   //!
  TBranch        *b_ZGen_pt;   //!
  TBranch        *b_ZGen_rap;   //!
  TBranch        *b_ZGen_phi;   //!
  TBranch        *b_ZGen_mass;   //!
  TBranch        *b_ZGen_mt;   //!
  TBranch        *b_u1;   //!
  TBranch        *b_u2;   //!
  TBranch        *b_MuPos_pt;   //!
  TBranch        *b_MuPos_eta;   //!
  TBranch        *b_MuPos_phi;   //!
  TBranch        *b_MuPos_mass;   //!
  TBranch        *b_MuPos_charge;   //!
  TBranch        *b_MuPos_dxy;   //!
  TBranch        *b_MuPosRelIso;   //!
  TBranch        *b_MuPosTrg;   //!
  TBranch        *b_MuPosIsTightAndIso;   //!
  TBranch        *b_MuPosIsTight;   //!
  TBranch        *b_MuPosGen_pt;   //!
  TBranch        *b_MuPosGen_eta;   //!
  TBranch        *b_MuPosGen_phi;   //!
  TBranch        *b_MuPosGen_mass;   //!
  TBranch        *b_MuPosGen_charge;   //!
  TBranch        *b_MuPosDRGenP;   //!
  TBranch        *b_MuNeg_pt;   //!
  TBranch        *b_MuNeg_eta;   //!
  TBranch        *b_MuNeg_phi;   //!
  TBranch        *b_MuNeg_mass;   //!
  TBranch        *b_MuNeg_charge;   //!
  TBranch        *b_MuNeg_dxy;   //!
  TBranch        *b_MuNegRelIso;   //!
  TBranch        *b_MuNegTrg;   //!
  TBranch        *b_MuNegIsTightAndIso;   //!
  TBranch        *b_MuNegIsTight;   //!
  TBranch        *b_MuNegGen_pt;   //!
  TBranch        *b_MuNegGen_eta;   //!
  TBranch        *b_MuNegGen_phi;   //!
  TBranch        *b_MuNegGen_mass;   //!
  TBranch        *b_MuNegGen_charge;   //!
  TBranch        *b_MuNegDRGenP;   //!
  TBranch        *b_Jet_leading_pt;   //!
  TBranch        *b_Jet_leading_eta;   //!
  TBranch        *b_Jet_leading_phi;   //!
  TBranch        *b_FSRWeight;   //!
  TBranch        *b_LHE_weight;   //!
  TBranch        *b_LHE_ren;   //!
  TBranch        *b_LHE_fac;   //!
  TBranch        *b_LHE_pdf;   //!
  TBranch        *b_ZGen_PostFSR_mass;   //!


  Zanalysis(TString f_str="", double lumi_scaling_input=1, int useGen=0, TTree *tree=0);
  virtual ~Zanalysis();
  virtual Int_t    Cut(Long64_t entry);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual Long64_t NumEntries();
  virtual void     Init(TTree *tree);
  virtual void     Loop(int chunk=0, int Entry_ini=0, int Entry_fin=0, int IS_MC_CLOSURE_TEST=0, int isMCorDATA=0, TString outputdir="", int buildTemplates=0, int useMomentumCorr=0, int smearRochCorrByNsigma=0, int useEffSF=0, int usePtSF=0, int useVtxSF=0, int controlplots=0, TString sampleName="", int generated_PDF_set=-1, int generated_PDF_member=-1, int contains_PDF_reweight=-1, int usePhiMETCorr=0, int useRecoilCorr=0, int RecoilCorrResolutionNSigmaU1=0, int RecoilCorrScaleNSigmaU1=0, int RecoilCorrResolutionNSigmaU2=0, int use_PForNoPUorTKmet=0, int use_syst_ewk_Alcaraz=0, int gen_mass_value_MeV=0, int contains_LHE_weights=0);
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);
  void ComputeHXVarAndPhiStarEta(TLorentzVector muPosNoCorr,TLorentzVector muNegNoCorr, bool isGen);

  virtual void plotVariables( TLorentzVector met, TLorentzVector ptVis, TLorentzVector Z, TLorentzVector ZGen,double u1_scale, string leptCharge, string cut , bool doCut, bool doneu, std::map<std::string, TH1D*> &h_1d, std::map<std::string, TH2D*> &h_2d,double weight , int jZmass_MeV , TString eta_str );

  virtual void  fillControlPlots(TLorentzVector Zcorr, TLorentzVector met, TLorentzVector muPosCorr, TLorentzVector muNegCorr, std::map<std::string, TH1D*> &h_1d, std::map<std::string, TH2D*> &h_2d, double weight, int jZmass_MeV , TString eta_str, TString SigOrQCD_str);

};

#endif

#ifdef Zanalysis_cxx
Zanalysis::Zanalysis(TString f_str, double lumi_scaling_input, int useGen, TTree *tree)
{
  // if parameter tree is not specified (or zero), connect the file
  // used to generate this class and read the Tree.
  if (tree == 0) {
    // TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("temp/ZTreeProducer_tree.root");
    // if (!f) {
    cout << gSystem->WorkingDirectory() << endl;
    // TFile *f = new TFile(Form("%s",f_str.Data()));
    TFile *f = TFile::Open(Form("%s",f_str.Data()));
    // }
    tree = (TTree*)gDirectory->Get("ZTreeProducer");

  }
  lumi_scaling=lumi_scaling_input;
  useGenVar=useGen;
  Init(tree);
}

Zanalysis::~Zanalysis()
{
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

Int_t Zanalysis::GetEntry(Long64_t entry)
{
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}
Long64_t Zanalysis::LoadTree(Long64_t entry)
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
Long64_t Zanalysis::NumEntries(){

  if (!fChain) return 0;
  return fChain->GetEntries();
};

void Zanalysis::Init(TTree *tree)
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
  fChain->SetBranchAddress("evtZSel", &evtZSel, &b_evtZSel);
  fChain->SetBranchAddress("nMuons", &nMuons, &b_nMuons);
  fChain->SetBranchAddress("nTrgMuons", &nTrgMuons, &b_nTrgMuons);
  fChain->SetBranchAddress("nNoTrgMuons", &nNoTrgMuons, &b_nNoTrgMuons);
  fChain->SetBranchAddress("noTrgExtraMuonsLeadingPt", &noTrgExtraMuonsLeadingPt, &b_noTrgExtraMuonsLeadingPt);
  fChain->SetBranchAddress("pfmet", &pfmet, &b_pfmet);
  fChain->SetBranchAddress("pfmet_phi", &pfmet_phi, &b_pfmet_phi);
  fChain->SetBranchAddress("pfmet_sumEt", &pfmet_sumEt, &b_pfmet_sumEt);
  fChain->SetBranchAddress("nopumet", &nopumet, &b_nopumet);
  fChain->SetBranchAddress("nopumet_phi", &nopumet_phi, &b_nopumet_phi);
  fChain->SetBranchAddress("nopumet_sumEt", &nopumet_sumEt, &b_nopumet_sumEt);
  fChain->SetBranchAddress("tkmet", &tkmet, &b_tkmet);
  fChain->SetBranchAddress("tkmet_phi", &tkmet_phi, &b_tkmet_phi);
  fChain->SetBranchAddress("tkmet_sumEt", &tkmet_sumEt, &b_tkmet_sumEt);
  //  fChain->SetBranchAddress("gentkmet", &tkmet, &b_tkmet);
  //  fChain->SetBranchAddress("gentkmet_phi", &tkmet_phi, &b_tkmet_phi);
  //  fChain->SetBranchAddress("gentkmet_sumEt", &tkmet_sumEt, &b_tkmet_sumEt);
  //  fChain->SetBranchAddress("tkmetABC", &tkmet, &b_tkmet);
  //  fChain->SetBranchAddress("tkmetABC_phi", &tkmet_phi, &b_tkmet_phi);
  //  fChain->SetBranchAddress("tkmetABC_sumEt", &tkmet_sumEt, &b_tkmet_sumEt);
  fChain->SetBranchAddress("pfmetWlikeNeg", &pfmetWlikeNeg, &b_pfmetWlikeNeg);
  fChain->SetBranchAddress("pfmetWlikeNeg_phi", &pfmetWlikeNeg_phi, &b_pfmetWlikeNeg_phi);
  fChain->SetBranchAddress("pfmetWlikePos", &pfmetWlikePos, &b_pfmetWlikePos);
  fChain->SetBranchAddress("pfmetWlikePos_phi", &pfmetWlikePos_phi, &b_pfmetWlikePos_phi);
  fChain->SetBranchAddress("WlikePos_pt", &WlikePos_pt, &b_WlikePos_pt);
  fChain->SetBranchAddress("WlikePos_phi", &WlikePos_phi, &b_WlikePos_phi);
  fChain->SetBranchAddress("WlikePos_mt", &WlikePos_mt, &b_WlikePos_mt);
  fChain->SetBranchAddress("WlikeNeg_pt", &WlikeNeg_pt, &b_WlikeNeg_pt);
  fChain->SetBranchAddress("WlikeNeg_phi", &WlikeNeg_phi, &b_WlikeNeg_phi);
  fChain->SetBranchAddress("WlikeNeg_mt", &WlikeNeg_mt, &b_WlikeNeg_mt);
  fChain->SetBranchAddress("Z_pt", &Z_pt, &b_Z_pt);
  fChain->SetBranchAddress("Z_rap", &Z_rap, &b_Z_rap);
  fChain->SetBranchAddress("Z_phi", &Z_phi, &b_Z_phi);
  fChain->SetBranchAddress("Z_mass", &Z_mass, &b_Z_mass);
  fChain->SetBranchAddress("Z_mt", &Z_mt, &b_Z_mt);
  fChain->SetBranchAddress("u1corr", &u1, &b_u1); // TEMP WITH Jul 2nd NTUPLES!!!!
  fChain->SetBranchAddress("u2corr", &u2, &b_u2); // TEMP WITH Jul 2nd NTUPLES!!!!
  fChain->SetBranchAddress("MuPos_pt", &MuPos_pt, &b_MuPos_pt);
  fChain->SetBranchAddress("MuPos_eta", &MuPos_eta, &b_MuPos_eta);
  fChain->SetBranchAddress("MuPos_phi", &MuPos_phi, &b_MuPos_phi);
  fChain->SetBranchAddress("MuPos_mass", &MuPos_mass, &b_MuPos_mass);
  fChain->SetBranchAddress("MuPos_charge", &MuPos_charge, &b_MuPos_charge);
  fChain->SetBranchAddress("MuPos_dxy", &MuPos_dxy, &b_MuPos_dxy);
  fChain->SetBranchAddress("MuPosRelIso", &MuPosRelIso, &b_MuPosRelIso);
  fChain->SetBranchAddress("MuPosTrg", &MuPosTrg, &b_MuPosTrg);
  fChain->SetBranchAddress("MuPosIsTightAndIso", &MuPosIsTightAndIso, &b_MuPosIsTightAndIso);
  fChain->SetBranchAddress("MuPosIsTight", &MuPosIsTight, &b_MuPosIsTight);
  fChain->SetBranchAddress("MuNeg_pt", &MuNeg_pt, &b_MuNeg_pt);
  fChain->SetBranchAddress("MuNeg_eta", &MuNeg_eta, &b_MuNeg_eta);
  fChain->SetBranchAddress("MuNeg_phi", &MuNeg_phi, &b_MuNeg_phi);
  fChain->SetBranchAddress("MuNeg_mass", &MuNeg_mass, &b_MuNeg_mass);
  fChain->SetBranchAddress("MuNeg_charge", &MuNeg_charge, &b_MuNeg_charge);
  fChain->SetBranchAddress("MuNeg_dxy", &MuNeg_dxy, &b_MuNeg_dxy);
  fChain->SetBranchAddress("MuNegRelIso", &MuNegRelIso, &b_MuNegRelIso);
  fChain->SetBranchAddress("MuNegTrg", &MuNegTrg, &b_MuNegTrg);
  fChain->SetBranchAddress("MuNegIsTightAndIso", &MuNegIsTightAndIso, &b_MuNegIsTightAndIso);
  fChain->SetBranchAddress("MuNegIsTight", &MuNegIsTight, &b_MuNegIsTight);
  fChain->SetBranchAddress("Jet_leading_pt", &Jet_leading_pt, &b_Jet_leading_pt);
  fChain->SetBranchAddress("Jet_leading_eta", &Jet_leading_eta, &b_Jet_leading_eta);
  fChain->SetBranchAddress("Jet_leading_phi", &Jet_leading_phi, &b_Jet_leading_phi);
  fChain->SetBranchAddress("FSRWeight", &FSRWeight, &b_FSRWeight);
  fChain->SetBranchAddress("ZGen_PostFSR_mass", &ZGen_PostFSR_mass, &b_ZGen_PostFSR_mass);

 
  if(useGenVar){
    fChain->SetBranchAddress("ZGen_mass", &ZGen_mass, &b_ZGen_mass);
    fChain->SetBranchAddress("ZGen_pt", &ZGen_pt, &b_ZGen_pt);
    fChain->SetBranchAddress("ZGen_rap", &ZGen_rap, &b_ZGen_rap);
    fChain->SetBranchAddress("ZGen_phi", &ZGen_phi, &b_ZGen_phi);
    fChain->SetBranchAddress("ZGen_mt", &ZGen_mt, &b_ZGen_mt);
    fChain->SetBranchAddress("MuPosGen_pt", &MuPosGen_pt, &b_MuPosGen_pt);
    fChain->SetBranchAddress("MuPosGen_eta", &MuPosGen_eta, &b_MuPosGen_eta);
    fChain->SetBranchAddress("MuPosGen_phi", &MuPosGen_phi, &b_MuPosGen_phi);
    fChain->SetBranchAddress("MuPosGen_mass", &MuPosGen_mass, &b_MuPosGen_mass);
    fChain->SetBranchAddress("MuPosGen_charge", &MuPosGen_charge, &b_MuPosGen_charge);
    fChain->SetBranchAddress("MuPosDRGenP", &MuPosDRGenP, &b_MuPosDRGenP);
    fChain->SetBranchAddress("MuNegGen_pt", &MuNegGen_pt, &b_MuNegGen_pt);
    fChain->SetBranchAddress("MuNegGen_eta", &MuNegGen_eta, &b_MuNegGen_eta);
    fChain->SetBranchAddress("MuNegGen_phi", &MuNegGen_phi, &b_MuNegGen_phi);
    fChain->SetBranchAddress("MuNegGen_mass", &MuNegGen_mass, &b_MuNegGen_mass);
    fChain->SetBranchAddress("MuNegGen_charge", &MuNegGen_charge, &b_MuNegGen_charge);
    fChain->SetBranchAddress("MuNegDRGenP", &MuNegDRGenP, &b_MuNegDRGenP);
    fChain->SetBranchAddress("LHE_weight", LHE_weight, &b_LHE_weight);
    // fChain->SetBranchAddress("LHE_ren", LHE_ren, &b_LHE_ren);
    // fChain->SetBranchAddress("LHE_fac", LHE_fac, &b_LHE_fac);
    // fChain->SetBranchAddress("LHE_pdf", LHE_pdf, &b_LHE_pdf);
 }
  Notify();
}

Bool_t Zanalysis::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}

void Zanalysis::Show(Long64_t entry)
{
  // Print contents of entry.
  // If entry is not specified, print current entry
  if (!fChain) return;
  fChain->Show(entry);
}
Int_t Zanalysis::Cut(Long64_t entry)
{
  // This function may be called from Loop.
  // returns  1 if entry is accepted.
  // returns -1 otherwise.
  return 1;
}


void Zanalysis::plotVariables( TLorentzVector met, TLorentzVector ptVis, TLorentzVector Z, TLorentzVector Zgen, double u1_scale, string leptCharge, string cut , bool doCut, bool doneu, std::map<std::string, TH1D*> &h_1d, std::map<std::string, TH2D*> &h_2d,double weight, int jZmass_MeV , TString eta_str)
{


  double u_parall=-999;
  double u_perp=-999;

  if(doneu) met=met-Z;
  common_stuff::calculateU1U2( met.Pt(), met.Phi(),  ptVis.Pt(), ptVis.Phi() , Z.Pt(), Z.Phi(), u_parall, u_perp );

  double u = fabs(sqrt( u_perp*u_perp + u_parall*u_parall ));
  double unorm = fabs(sqrt( u_perp*u_perp + u_parall*u_parall ))/ptVis.Pt();
  double uPhi=TMath::ATan2(u_perp,-u_parall);

  common_stuff::plot2D("u1_vs_u2"+leptCharge+cut ,  u_perp, u_parall+ptVis.Pt(),   weight, h_2d, 100, -50., 50. , 100, -50., 50.);
  common_stuff::plot2D("u1_vs_u2_scale"+leptCharge+cut ,  u_perp, u_parall+u1_scale,   weight, h_2d, 100, -50., 50. , 100, -50., 50.);
  common_stuff::plot2D("u_vs_Angle"+leptCharge+cut ,  unorm, uPhi ,   weight, h_2d, 100, 0., 10. , 100, -TMath::Pi(), TMath::Pi());

  common_stuff::plot1D("h_ptZ"+leptCharge+cut  , ptVis.Pt()   ,                      weight, h_1d, 100, 0., 50.);

  TString test = cut.c_str();

  if(met.Pt()>0) {

    common_stuff::plot1D("h_met"+leptCharge+cut, met.Pt() ,                 weight, h_1d, 100, 0, 100);
    common_stuff::plot1D("h_metphi"+leptCharge+cut, met.Phi() ,             weight, h_1d, 100, -3.15, 3.15);

    common_stuff::plot1D("h_u"+leptCharge+cut  , u   ,                      weight, h_1d, 100, 0., 20.);
    common_stuff::plot1D("h_u2"+leptCharge+cut  , u_perp   ,                weight, h_1d, 200, -100., 100.);

    common_stuff::plot1D("h_u2Res"+leptCharge+cut  , u_perp/(u_parall)   ,                weight, h_1d, 200, -100., 100.);

    common_stuff::plot1D("h_u1_Residual"+leptCharge+cut  , u_parall+ptVis.Pt(), weight, h_1d, 200, -100., 100.);
    common_stuff::plot1D("h_u1_Pull"+leptCharge+cut  , (u_parall+Z.Pt())/Z.Pt(),  weight, h_1d, 200, -10., 10.);
    common_stuff::plot1D("h_u1_Scale"+leptCharge+cut, u_parall/Z.Pt() ,                weight, h_1d, 200, -100., 100.);
    common_stuff::plot1D("h_u1"+leptCharge+cut, u_parall ,                weight, h_1d, 200, -100., 100.);

    common_stuff::plot1D("h_met_pull_Z"+leptCharge+cut, met.Pt()-Z.Pt() ,   weight, h_1d, 200, -100, 100);

  }

  ////

  common_stuff::plot2D(Form("U1_vs_Zpt_reco_eta%s_%d",eta_str.Data(),jZmass_MeV),
		       Z.Pt(),u_parall, weight,
		       h_2d, 100,0,20,100,-10,10 );
  common_stuff::plot2D(Form("U2_vs_Zpt_reco_eta%s_%d",eta_str.Data(),jZmass_MeV),
		       Z.Pt(),u_perp, weight,
		       h_2d, 100,0,20,100,-10,10 );

  common_stuff::plot2D(Form("U1_vs_Zpt_gen_eta%s_%d",eta_str.Data(),jZmass_MeV),
		       Z.Pt(),u_parall, weight,
		       h_2d, 100,0,20,100,-10,10 );
  common_stuff::plot2D(Form("U2_vs_Zpt_gen_eta%s_%d",eta_str.Data(),jZmass_MeV),
		       Z.Pt(),u_perp, weight,
		       h_2d, 100,0,20,100,-10,10 );

}


void Zanalysis::fillControlPlots(TLorentzVector Zcorr, TLorentzVector met, TLorentzVector muPosCorr, TLorentzVector muNegCorr, std::map<std::string, TH1D*> &h_1d, std::map<std::string, TH2D*> &h_2d, double weight, int jZmass_MeV , TString eta_str, TString SigOrQCD_str)
{

  common_stuff::plot1D(Form("hnvtx_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       nvtx, weight,
		       h_1d, 50,0,50 );
  common_stuff::plot1D(Form("Zmass_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       Zcorr.M(), weight,
		       h_1d, 300,50,200 );
  common_stuff::plot1D(Form("Zmass_zoomed_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       Zcorr.M(), weight,
		       h_1d, 6000,60,120 );
  common_stuff::plot2D(Form("ZmassVsMuPosEta_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       muPosCorr.Eta(),Zcorr.M(), weight,
		       h_2d, 50,-2.5,2.5,200,90,92 );
  // common_stuff::plot2D(Form("PosMuonPtCorrVsMuPosEta_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
  // muPosCorr.Eta(),Zcorr.M(),evt_weight*TRG_TIGHT_ISO_muons_SF,
  // h_2d, 50,-2.5,2.5,300,60,120
  // );
  // common_stuff::plot2D(Form("PosMuonPtCorrVsMuPosPhi_%s_eta%s_%d",WMass::nSigOrQCD_str[0].Data(),eta_str.Data(),jZmass_MeV),
  // muPosCorr.Phi(),Zcorr.M(),evt_weight*TRG_TIGHT_ISO_muons_SF,
  // h_2d, 50,-2*TMath::Pi(),2*TMath::Pi(),300,60,120
  // );
  common_stuff::plot1D(Form("ZlepDeltaEta_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       muPosCorr.Eta()-muNegCorr.Eta(),weight,
		       h_1d, 200,-5,5 );
  common_stuff::plot2D(Form("ZlepEta1VsEta2_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       muPosCorr.Eta(),muNegCorr.Eta(),weight,
		       h_2d, 50,-2.5,2.5,50,-2.5,2.5 );
  common_stuff::plot2D(Form("ZlepDeltaEtaVsEta1_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       muPosCorr.Eta(),muPosCorr.Eta()-muNegCorr.Eta(),weight,
		       h_2d, 50,-2.5,2.5,100,-5,5 );
  common_stuff::plot1D(Form("ZlepDeltaPt_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       muPosCorr.Pt()-muNegCorr.Pt(),weight,
		       h_1d, 100,-50,50 );
  common_stuff::plot2D(Form("ZlepPt1VsPt2_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       muPosCorr.Pt(),muNegCorr.Pt(),weight,
		       h_2d, 200,0,200,200,0,200 );
  common_stuff::plot2D(Form("ZlepDeltaPtVsPt1_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       muPosCorr.Pt(),muPosCorr.Pt()-muNegCorr.Pt(),weight,
		       h_2d, 200,0,200,100,-50,50 );
  common_stuff::plot1D(Form("ZlepDeltaPhi_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       muPosCorr.Phi()-muNegCorr.Phi(),weight,
		       h_1d, 200,-2*TMath::Pi(),2*TMath::Pi() );
  common_stuff::plot2D(Form("ZlepPhi1VsPhi2_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       muPosCorr.Phi(),muNegCorr.Phi(),weight,
		       h_2d, 200,-2*TMath::Pi(),2*TMath::Pi(),200,-2*TMath::Pi(),2*TMath::Pi() );
  common_stuff::plot2D(Form("ZlepDeltaPhiVsPhi1_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       muPosCorr.Phi(),muPosCorr.Phi()-muNegCorr.Phi(),weight,
		       h_2d, 200,-2*TMath::Pi(),2*TMath::Pi(),200,-2*TMath::Pi(),2*TMath::Pi() );
  common_stuff::plot1D(Form("ZlepDeltaR_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       muPosCorr.Eta()-Jet_leading_eta,weight,
		       h_1d, 1000,0,10 );
  common_stuff::plot1D(Form("ZDeltaEtaMuPosJet_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       muPosCorr.Eta()-Jet_leading_eta,weight,
		       h_1d, 200,-5,5 );
  common_stuff::plot1D(Form("ZDeltaPhiMuPosJet_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       muPosCorr.Phi()-Jet_leading_phi,weight,
		       h_1d, 200,-2*TMath::Pi(),2*TMath::Pi() );
  common_stuff::plot1D(Form("ZDeltaRMuPosJet_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       TMath::Hypot(muPosCorr.Phi()-Jet_leading_phi,muPosCorr.Eta()-Jet_leading_eta),weight,
		       h_1d, 1000,0,10 );
  common_stuff::plot1D(Form("ZDeltaEtaMuNegJet_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       muNegCorr.Eta()-Jet_leading_eta,weight,
		       h_1d, 200,-5,5 );
  common_stuff::plot1D(Form("ZDeltaPhiMuNegJet_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       muNegCorr.Phi()-Jet_leading_phi,weight,
		       h_1d, 200,-2*TMath::Pi(),2*TMath::Pi() );
  common_stuff::plot1D(Form("ZDeltaRMuNegJet_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       TMath::Hypot(muNegCorr.Phi()-Jet_leading_phi,muNegCorr.Eta()-Jet_leading_eta),weight,
		       h_1d, 1000,0,10 );
  common_stuff::plot1D(Form("hpfMET_Z_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       met.Pt(),weight,
		       h_1d, 100,0,200 );
  common_stuff::plot1D(Form("hpfMETphi_Z_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       met.Phi(),weight,
		       h_1d, 100,-TMath::Pi(),TMath::Pi() );
  common_stuff::plot1D(Form("hZ_phi_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       Zcorr.Phi(),weight,
		       h_1d, 100,-TMath::Pi(),TMath::Pi() );
  common_stuff::plot1D(Form("hZ_mt_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       Zcorr.Mt(),weight,
		       h_1d, 100,0,200 );
  common_stuff::plot2D(Form("hZ_ptVsmt_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       Zcorr.Mt(),Zcorr.Pt(),weight,
		       h_2d, 150,50,200,100,0,25 );
  common_stuff::plot1D(Form("hWlikePos_pt_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       WlikePos_pt,weight,
		       h_1d, 100,0,25 );
  common_stuff::plot1D(Form("hWlikePos_phi_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       WlikePos_phi,weight,
		       h_1d, 100,-TMath::Pi(),TMath::Pi() );
  common_stuff::plot1D(Form("hWlikePos_mt_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       WlikePos_mt,weight,
		       h_1d, 200,0,200 );
  common_stuff::plot1D(Form("hpfMET_WlikePos_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       pfmetWlikePos,weight,
		       h_1d, 100,0,200 );
  common_stuff::plot1D(Form("hpfMET_WlikePos_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       pfmetWlikePos,weight,
		       h_1d, 100,0,200 );
  common_stuff::plot1D(Form("hpfMETphi_WlikePos_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       pfmetWlikePos_phi,weight,
		       h_1d, 100,-TMath::Pi(),TMath::Pi() );
  common_stuff::plot1D(Form("hMupt_WlikePos_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       muPosCorr.Pt()<200 ? muPosCorr.Pt() : 199.5,weight,
		       h_1d, 200,0,200 );
  common_stuff::plot1D(Form("hMueta_WlikePos_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       muPosCorr.Eta(),weight,
		       h_1d, 100,-2.5,2.5 );
  common_stuff::plot1D(Form("hMuphi_WlikePos_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       muPosCorr.Eta(),weight,
		       h_1d, 100,-TMath::Pi(),TMath::Pi() );
  common_stuff::plot1D(Form("hMuiso_WlikePos_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       MuPosRelIso,weight,
		       h_1d, 1000,0,10 );
  common_stuff::plot1D(Form("hMuMETpt_WlikePos_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       muNegCorr.Pt(),weight,
		       h_1d, 100,0,200 );
  common_stuff::plot1D(Form("hMuMETeta_WlikePos_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       muNegCorr.Eta(),weight,
		       h_1d, 100,-2.5,2.5 );
  common_stuff::plot1D(Form("hMuMETeta_WlikePos_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       muNegCorr.Phi(),weight,
		       h_1d, 100,-TMath::Pi(),TMath::Pi() );
  common_stuff::plot1D(Form("hMuMETiso_WlikePos_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       MuNegRelIso,weight,
		       h_1d, 100,0,10 );
  common_stuff::plot1D(Form("hJetpt_WlikePos_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       Jet_leading_pt,weight,
		       h_1d, 100,0,50 );
  common_stuff::plot1D(Form("hJeteta_WlikePos_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       Jet_leading_eta,weight,
		       h_1d, 100, -2.5,2.5 );
  common_stuff::plot1D(Form("hJeteta_WlikePos_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       Jet_leading_phi,weight,
		       h_1d, 100, -TMath::Pi(),TMath::Pi() );
  common_stuff::plot1D(Form("hu1_WlikePos_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       u1,weight,
		       h_1d, 360,-30,150 );
  common_stuff::plot1D(Form("hu1_Zpt_WlikePos_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       u1-Zcorr.Pt(),weight,
		       h_1d, 360,-30,150 );
  common_stuff::plot1D(Form("hu2_WlikePos_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       u2,weight,
		       h_1d, 250,-50,50 );
  common_stuff::plot2D(Form("hu1vsZpt_WlikePos_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       Zcorr.Pt(),u1,weight,
		       h_2d, 100,0,25,360,-30,150 );
  common_stuff::plot2D(Form("hu2vsZpt_WlikePos_%s_eta%s_%d",SigOrQCD_str.Data(),eta_str.Data(),jZmass_MeV),
		       Zcorr.Pt(),u2,weight,
		       h_2d, 100,0,25,250,-50,50 );

}

#endif // #ifdef Zanalysis_cxx
