//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Apr 16 09:21:17 2015 by ROOT version 5.32/00
// from TTree ZTreeProducer/ZTreeProducer
// found on file: root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/DYJetsMM/allEvts/ZTreeProducer_tree.root
//////////////////////////////////////////////////////////

#ifndef test_allEvt_h
#define test_allEvt_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TLorentzVector.h>
#include "../AnalysisCode/HTransformToHelicityFrame.c"
#include "../AnalysisCode/HTransformToCS.c"
#include "../AnalysisCode/common_stuff.C"

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class test_allEvt {
  public :
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain

  // Declaration of leaf types
  Int_t           nvtx;
  Int_t           evtHasGoodVtx;
  Int_t           Vtx_ndof;
  Int_t           npu;
  Double_t        scalePDF;
  Double_t        parton1_pdgId;
  Double_t        parton1_x;
  Double_t        parton2_pdgId;
  Double_t        parton2_x;
  Double_t        LHE_weight[466];
  Double_t        pfmet;
  Double_t        pfmet_phi;
  Double_t        pfmet_sumEt;
  Double_t        tkmet;
  Double_t        tkmet_phi;
  Double_t        tkmet_sumEt;
  Double_t        gentkmet;
  Double_t        gentkmet_phi;
  Double_t        gentkmet_sumEt;
  Double_t        genpfmet;
  Double_t        genpfmet_phi;
  Double_t        genpfmet_sumEt;
  Double_t        nopumet;
  Double_t        nopumet_phi;
  Double_t        nopumet_sumEt;
  Double_t        pumet;
  Double_t        pumet_phi;
  Double_t        pumet_sumEt;
  Double_t        pucmet;
  Double_t        pucmet_phi;
  Double_t        pucmet_sumEt;
  Double_t        pfMetForRegression;
  Double_t        pfMetForRegression_phi;
  Double_t        pfMetForRegression_sumEt;
  Int_t           njets;
  Int_t           evtHasTrg;
  Int_t           evtZSel;
  Int_t           nMuons;
  Int_t           nTrgMuons;
  Int_t           noTrgExtraMuonsLeadingPt;
  Double_t        Z_pt;
  Double_t        Z_rap;
  Double_t        Z_phi;
  Double_t        Z_mass;
  Double_t        Z_mt;
  Double_t        pt_vis;
  Double_t        phi_vis;
  Double_t        ZGen_pt;
  Double_t        ZGen_rap;
  Double_t        ZGen_phi;
  Double_t        ZGen_mass;
  Double_t        ZGen_PostFSR_pt;
  Double_t        ZGen_PostFSR_rap;
  Double_t        ZGen_PostFSR_phi;
  Double_t        ZGen_PostFSR_mass;
  Double_t        ZGen_mt;
  Double_t        MuPos_pt;
  Double_t        MuPos_eta;
  Double_t        MuPos_phi;
  Double_t        MuPos_mass;
  Double_t        MuPos_charge;
  Double_t        MuPosRelIso;
  Double_t        MuPos_dxy;
  Double_t        MuPos_dz;
  Int_t           MuPosTrg;
  Int_t           MuPosIsTight;
  Double_t        MuPosGen_pt;
  Double_t        MuPosGen_eta;
  Double_t        MuPosGen_phi;
  Double_t        MuPosGen_mass;
  Double_t        MuPosGen_charge;
  Double_t        MuPosGenStatus1_pt;
  Double_t        MuPosGenStatus1_eta;
  Double_t        MuPosGenStatus1_phi;
  Double_t        MuPosGenStatus1_mass;
  Double_t        MuPosGenStatus1_charge;
  Double_t        FSRWeight;
  Double_t        MuNeg_pt;
  Double_t        MuNeg_eta;
  Double_t        MuNeg_phi;
  Double_t        MuNeg_mass;
  Double_t        MuNeg_charge;
  Double_t        MuNegRelIso;
  Double_t        MuNeg_dxy;
  Double_t        MuNeg_dz;
  Int_t           MuNegTrg;
  Int_t           MuNegIsTight;
  Double_t        MuNegGen_pt;
  Double_t        MuNegGen_eta;
  Double_t        MuNegGen_phi;
  Double_t        MuNegGen_mass;
  Double_t        MuNegGen_charge;
  Double_t        MuNegGenStatus1_pt;
  Double_t        MuNegGenStatus1_eta;
  Double_t        MuNegGenStatus1_phi;
  Double_t        MuNegGenStatus1_mass;
  Double_t        MuNegGenStatus1_charge;
  Double_t        MuPosDRGenP;
  Double_t        MuNegDRGenP;
  Double_t        MuPosCovMatrix[9];
  Double_t        MuNegCovMatrix[9];
  Double_t        Jet_leading_pt;
  Double_t        Jet_leading_eta;
  Double_t        Jet_leading_phi;

  // List of branches
  TBranch        *b_nvtx;   //!
  TBranch        *b_evtHasGoodVtx;   //!
  TBranch        *b_Vtx_ndof;   //!
  TBranch        *b_npu;   //!
  TBranch        *b_scalePDF;   //!
  TBranch        *b_parton1_pdgId;   //!
  TBranch        *b_parton1_x;   //!
  TBranch        *b_parton2_pdgId;   //!
  TBranch        *b_parton2_x;   //!
  TBranch        *b_LHE_weight;   //!
  TBranch        *b_pfmet;   //!
  TBranch        *b_pfmet_phi;   //!
  TBranch        *b_pfmet_sumEt;   //!
  TBranch        *b_tkmet;   //!
  TBranch        *b_tkmet_phi;   //!
  TBranch        *b_tkmet_sumEt;   //!
  TBranch        *b_gentkmet;   //!
  TBranch        *b_gentkmet_phi;   //!
  TBranch        *b_gentkmet_sumEt;   //!
  TBranch        *b_genpfmet;   //!
  TBranch        *b_genpfmet_phi;   //!
  TBranch        *b_genpfmet_sumEt;   //!
  TBranch        *b_nopumet;   //!
  TBranch        *b_nopumet_phi;   //!
  TBranch        *b_nopumet_sumEt;   //!
  TBranch        *b_pumet;   //!
  TBranch        *b_pumet_phi;   //!
  TBranch        *b_pumet_sumEt;   //!
  TBranch        *b_pucmet;   //!
  TBranch        *b_pucmet_phi;   //!
  TBranch        *b_pucmet_sumEt;   //!
  TBranch        *b_pfMetForRegression;   //!
  TBranch        *b_pfMetForRegression_phi;   //!
  TBranch        *b_pfMetForRegression_sumEt;   //!
  TBranch        *b_njets;   //!
  TBranch        *b_evtHasTrg;   //!
  TBranch        *b_evtZSel;   //!
  TBranch        *b_nMuons;   //!
  TBranch        *b_nTrgMuons;   //!
  TBranch        *b_noTrgExtraMuonsLeadingPt;   //!
  TBranch        *b_Z_pt;   //!
  TBranch        *b_Z_rap;   //!
  TBranch        *b_Z_phi;   //!
  TBranch        *b_Z_mass;   //!
  TBranch        *b_Z_mt;   //!
  TBranch        *b_pt_vis;   //!
  TBranch        *b_phi_vis;   //!
  TBranch        *b_ZGen_pt;   //!
  TBranch        *b_ZGen_rap;   //!
  TBranch        *b_ZGen_phi;   //!
  TBranch        *b_ZGen_mass;   //!
  TBranch        *b_ZGen_PostFSR_pt;   //!
  TBranch        *b_ZGen_PostFSR_rap;   //!
  TBranch        *b_ZGen_PostFSR_phi;   //!
  TBranch        *b_ZGen_PostFSR_mass;   //!
  TBranch        *b_ZGen_mt;   //!
  TBranch        *b_MuPos_pt;   //!
  TBranch        *b_MuPos_eta;   //!
  TBranch        *b_MuPos_phi;   //!
  TBranch        *b_MuPos_mass;   //!
  TBranch        *b_MuPos_charge;   //!
  TBranch        *b_MuPosRelIso;   //!
  TBranch        *b_MuPos_dxy;   //!
  TBranch        *b_MuPos_dz;   //!
  TBranch        *b_MuPosTrg;   //!
  TBranch        *b_MuPosIsTight;   //!
  TBranch        *b_MuPosGen_pt;   //!
  TBranch        *b_MuPosGen_eta;   //!
  TBranch        *b_MuPosGen_phi;   //!
  TBranch        *b_MuPosGen_mass;   //!
  TBranch        *b_MuPosGen_charge;   //!
  TBranch        *b_MuPosGenStatus1_pt;   //!
  TBranch        *b_MuPosGenStatus1_eta;   //!
  TBranch        *b_MuPosGenStatus1_phi;   //!
  TBranch        *b_MuPosGenStatus1_mass;   //!
  TBranch        *b_MuPosGenStatus1_charge;   //!
  TBranch        *b_FSRWeight;   //!
  TBranch        *b_MuNeg_pt;   //!
  TBranch        *b_MuNeg_eta;   //!
  TBranch        *b_MuNeg_phi;   //!
  TBranch        *b_MuNeg_mass;   //!
  TBranch        *b_MuNeg_charge;   //!
  TBranch        *b_MuNegRelIso;   //!
  TBranch        *b_MuNeg_dxy;   //!
  TBranch        *b_MuNeg_dz;   //!
  TBranch        *b_MuNegTrg;   //!
  TBranch        *b_MuNegIsTight;   //!
  TBranch        *b_MuNegGen_pt;   //!
  TBranch        *b_MuNegGen_eta;   //!
  TBranch        *b_MuNegGen_phi;   //!
  TBranch        *b_MuNegGen_mass;   //!
  TBranch        *b_MuNegGen_charge;   //!
  TBranch        *b_MuNegGenStatus1_pt;   //!
  TBranch        *b_MuNegGenStatus1_eta;   //!
  TBranch        *b_MuNegGenStatus1_phi;   //!
  TBranch        *b_MuNegGenStatus1_mass;   //!
  TBranch        *b_MuNegGenStatus1_charge;   //!
  TBranch        *b_MuPosDRGenP;   //!
  TBranch        *b_MuNegDRGenP;   //!
  TBranch        *b_MuPosCovMatrix;   //!
  TBranch        *b_MuNegCovMatrix;   //!
  TBranch        *b_Jet_leading_pt;   //!
  TBranch        *b_Jet_leading_eta;   //!
  TBranch        *b_Jet_leading_phi;   //!

  test_allEvt(TTree *tree=0);
  virtual ~test_allEvt();
  virtual Int_t    Cut(Long64_t entry);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init(TTree *tree);
  virtual void     Loop();
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);
  void ComputeHXVar(TLorentzVector muPosNoCorr,TLorentzVector muNegNoCorr, bool isGen);
  void ComputeCSVar(TLorentzVector muPosNoCorr,TLorentzVector muNegNoCorr, bool isGen);
  void ComputeAllVarPietro(TLorentzVector lepP,TLorentzVector lepN);
};

#endif

#ifdef test_allEvt_cxx
test_allEvt::test_allEvt(TTree *tree) : fChain(0) 
{
  // if parameter tree is not specified (or zero), connect the file
  // used to generate this class and read the Tree.
  if (tree == 0) {
    TFile *f;
    // = (TFile*)gROOT->GetListOfFiles()->FindObject("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/DYJetsMM/allEvts/ZTreeProducer_tree.root");
    // if (!f || !f->IsOpen()) {
    // }
    f = TFile::Open("root://eoscms//eos/cms/store/group/phys_smp/Wmass/perrozzi/ntuples/ntuples_2014_05_23_53X/DYJetsMM/allEvts/ZTreeProducer_tree.root");
    f->GetObject("ZTreeProducer",tree);

  }
  Init(tree);
}

test_allEvt::~test_allEvt()
{
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

Int_t test_allEvt::GetEntry(Long64_t entry)
{
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}
Long64_t test_allEvt::LoadTree(Long64_t entry)
{
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
    Notify();
  }
  return centry;
}

void test_allEvt::Init(TTree *tree)
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

  // fChain->SetBranchAddress("nvtx", &nvtx, &b_nvtx);
  // fChain->SetBranchAddress("evtHasGoodVtx", &evtHasGoodVtx, &b_evtHasGoodVtx);
  // fChain->SetBranchAddress("Vtx_ndof", &Vtx_ndof, &b_Vtx_ndof);
  // fChain->SetBranchAddress("npu", &npu, &b_npu);
  // fChain->SetBranchAddress("scalePDF", &scalePDF, &b_scalePDF);
  // fChain->SetBranchAddress("parton1_pdgId", &parton1_pdgId, &b_parton1_pdgId);
  // fChain->SetBranchAddress("parton1_x", &parton1_x, &b_parton1_x);
  // fChain->SetBranchAddress("parton2_pdgId", &parton2_pdgId, &b_parton2_pdgId);
  // fChain->SetBranchAddress("parton2_x", &parton2_x, &b_parton2_x);
  // fChain->SetBranchAddress("LHE_weight", LHE_weight, &b_LHE_weight);
  // fChain->SetBranchAddress("pfmet", &pfmet, &b_pfmet);
  // fChain->SetBranchAddress("pfmet_phi", &pfmet_phi, &b_pfmet_phi);
  // fChain->SetBranchAddress("pfmet_sumEt", &pfmet_sumEt, &b_pfmet_sumEt);
  // fChain->SetBranchAddress("tkmet", &tkmet, &b_tkmet);
  // fChain->SetBranchAddress("tkmet_phi", &tkmet_phi, &b_tkmet_phi);
  // fChain->SetBranchAddress("tkmet_sumEt", &tkmet_sumEt, &b_tkmet_sumEt);
  // fChain->SetBranchAddress("gentkmet", &gentkmet, &b_gentkmet);
  // fChain->SetBranchAddress("gentkmet_phi", &gentkmet_phi, &b_gentkmet_phi);
  // fChain->SetBranchAddress("gentkmet_sumEt", &gentkmet_sumEt, &b_gentkmet_sumEt);
  // fChain->SetBranchAddress("genpfmet", &genpfmet, &b_genpfmet);
  // fChain->SetBranchAddress("genpfmet_phi", &genpfmet_phi, &b_genpfmet_phi);
  // fChain->SetBranchAddress("genpfmet_sumEt", &genpfmet_sumEt, &b_genpfmet_sumEt);
  // fChain->SetBranchAddress("nopumet", &nopumet, &b_nopumet);
  // fChain->SetBranchAddress("nopumet_phi", &nopumet_phi, &b_nopumet_phi);
  // fChain->SetBranchAddress("nopumet_sumEt", &nopumet_sumEt, &b_nopumet_sumEt);
  // fChain->SetBranchAddress("pumet", &pumet, &b_pumet);
  // fChain->SetBranchAddress("pumet_phi", &pumet_phi, &b_pumet_phi);
  // fChain->SetBranchAddress("pumet_sumEt", &pumet_sumEt, &b_pumet_sumEt);
  // fChain->SetBranchAddress("pucmet", &pucmet, &b_pucmet);
  // fChain->SetBranchAddress("pucmet_phi", &pucmet_phi, &b_pucmet_phi);
  // fChain->SetBranchAddress("pucmet_sumEt", &pucmet_sumEt, &b_pucmet_sumEt);
  // fChain->SetBranchAddress("pfMetForRegression", &pfMetForRegression, &b_pfMetForRegression);
  // fChain->SetBranchAddress("pfMetForRegression_phi", &pfMetForRegression_phi, &b_pfMetForRegression_phi);
  // fChain->SetBranchAddress("pfMetForRegression_sumEt", &pfMetForRegression_sumEt, &b_pfMetForRegression_sumEt);
  // fChain->SetBranchAddress("njets", &njets, &b_njets);
  // fChain->SetBranchAddress("evtHasTrg", &evtHasTrg, &b_evtHasTrg);
  // fChain->SetBranchAddress("evtZSel", &evtZSel, &b_evtZSel);
  // fChain->SetBranchAddress("nMuons", &nMuons, &b_nMuons);
  // fChain->SetBranchAddress("nTrgMuons", &nTrgMuons, &b_nTrgMuons);
  // fChain->SetBranchAddress("noTrgExtraMuonsLeadingPt", &noTrgExtraMuonsLeadingPt, &b_noTrgExtraMuonsLeadingPt);
  // fChain->SetBranchAddress("Z_pt", &Z_pt, &b_Z_pt);
  // fChain->SetBranchAddress("Z_rap", &Z_rap, &b_Z_rap);
  // fChain->SetBranchAddress("Z_phi", &Z_phi, &b_Z_phi);
  // fChain->SetBranchAddress("Z_mass", &Z_mass, &b_Z_mass);
  // fChain->SetBranchAddress("Z_mt", &Z_mt, &b_Z_mt);
  // fChain->SetBranchAddress("pt_vis", &pt_vis, &b_pt_vis);
  // fChain->SetBranchAddress("phi_vis", &phi_vis, &b_phi_vis);
  // fChain->SetBranchAddress("ZGen_pt", &ZGen_pt, &b_ZGen_pt);
  // fChain->SetBranchAddress("ZGen_rap", &ZGen_rap, &b_ZGen_rap);
  // fChain->SetBranchAddress("ZGen_phi", &ZGen_phi, &b_ZGen_phi);
  // fChain->SetBranchAddress("ZGen_mass", &ZGen_mass, &b_ZGen_mass);
  fChain->SetBranchAddress("ZGen_PostFSR_pt", &ZGen_PostFSR_pt, &b_ZGen_PostFSR_pt);
  fChain->SetBranchAddress("ZGen_PostFSR_rap", &ZGen_PostFSR_rap, &b_ZGen_PostFSR_rap);
  fChain->SetBranchAddress("ZGen_PostFSR_phi", &ZGen_PostFSR_phi, &b_ZGen_PostFSR_phi);
  fChain->SetBranchAddress("ZGen_PostFSR_mass", &ZGen_PostFSR_mass, &b_ZGen_PostFSR_mass);
  // fChain->SetBranchAddress("ZGen_mt", &ZGen_mt, &b_ZGen_mt);
  // fChain->SetBranchAddress("MuPos_pt", &MuPos_pt, &b_MuPos_pt);
  // fChain->SetBranchAddress("MuPos_eta", &MuPos_eta, &b_MuPos_eta);
  // fChain->SetBranchAddress("MuPos_phi", &MuPos_phi, &b_MuPos_phi);
  // fChain->SetBranchAddress("MuPos_mass", &MuPos_mass, &b_MuPos_mass);
  // fChain->SetBranchAddress("MuPos_charge", &MuPos_charge, &b_MuPos_charge);
  // fChain->SetBranchAddress("MuPosRelIso", &MuPosRelIso, &b_MuPosRelIso);
  // fChain->SetBranchAddress("MuPos_dxy", &MuPos_dxy, &b_MuPos_dxy);
  // fChain->SetBranchAddress("MuPos_dz", &MuPos_dz, &b_MuPos_dz);
  // fChain->SetBranchAddress("MuPosTrg", &MuPosTrg, &b_MuPosTrg);
  // fChain->SetBranchAddress("MuPosIsTight", &MuPosIsTight, &b_MuPosIsTight);
  fChain->SetBranchAddress("MuPosGen_pt", &MuPosGen_pt, &b_MuPosGen_pt);
  fChain->SetBranchAddress("MuPosGen_eta", &MuPosGen_eta, &b_MuPosGen_eta);
  fChain->SetBranchAddress("MuPosGen_phi", &MuPosGen_phi, &b_MuPosGen_phi);
  fChain->SetBranchAddress("MuPosGen_mass", &MuPosGen_mass, &b_MuPosGen_mass);
  // fChain->SetBranchAddress("MuPosGen_charge", &MuPosGen_charge, &b_MuPosGen_charge);
  fChain->SetBranchAddress("MuPosGenStatus1_pt", &MuPosGenStatus1_pt, &b_MuPosGenStatus1_pt);
  fChain->SetBranchAddress("MuPosGenStatus1_eta", &MuPosGenStatus1_eta, &b_MuPosGenStatus1_eta);
  fChain->SetBranchAddress("MuPosGenStatus1_phi", &MuPosGenStatus1_phi, &b_MuPosGenStatus1_phi);
  fChain->SetBranchAddress("MuPosGenStatus1_mass", &MuPosGenStatus1_mass, &b_MuPosGenStatus1_mass);
  // fChain->SetBranchAddress("MuPosGenStatus1_charge", &MuPosGenStatus1_charge, &b_MuPosGenStatus1_charge);
  // fChain->SetBranchAddress("FSRWeight", &FSRWeight, &b_FSRWeight);
  // fChain->SetBranchAddress("MuNeg_pt", &MuNeg_pt, &b_MuNeg_pt);
  // fChain->SetBranchAddress("MuNeg_eta", &MuNeg_eta, &b_MuNeg_eta);
  // fChain->SetBranchAddress("MuNeg_phi", &MuNeg_phi, &b_MuNeg_phi);
  // fChain->SetBranchAddress("MuNeg_mass", &MuNeg_mass, &b_MuNeg_mass);
  // fChain->SetBranchAddress("MuNeg_charge", &MuNeg_charge, &b_MuNeg_charge);
  // fChain->SetBranchAddress("MuNegRelIso", &MuNegRelIso, &b_MuNegRelIso);
  // fChain->SetBranchAddress("MuNeg_dxy", &MuNeg_dxy, &b_MuNeg_dxy);
  // fChain->SetBranchAddress("MuNeg_dz", &MuNeg_dz, &b_MuNeg_dz);
  // fChain->SetBranchAddress("MuNegTrg", &MuNegTrg, &b_MuNegTrg);
  // fChain->SetBranchAddress("MuNegIsTight", &MuNegIsTight, &b_MuNegIsTight);
  fChain->SetBranchAddress("MuNegGen_pt", &MuNegGen_pt, &b_MuNegGen_pt);
  fChain->SetBranchAddress("MuNegGen_eta", &MuNegGen_eta, &b_MuNegGen_eta);
  fChain->SetBranchAddress("MuNegGen_phi", &MuNegGen_phi, &b_MuNegGen_phi);
  fChain->SetBranchAddress("MuNegGen_mass", &MuNegGen_mass, &b_MuNegGen_mass);
  // fChain->SetBranchAddress("MuNegGen_charge", &MuNegGen_charge, &b_MuNegGen_charge);
  fChain->SetBranchAddress("MuNegGenStatus1_pt", &MuNegGenStatus1_pt, &b_MuNegGenStatus1_pt);
  fChain->SetBranchAddress("MuNegGenStatus1_eta", &MuNegGenStatus1_eta, &b_MuNegGenStatus1_eta);
  fChain->SetBranchAddress("MuNegGenStatus1_phi", &MuNegGenStatus1_phi, &b_MuNegGenStatus1_phi);
  fChain->SetBranchAddress("MuNegGenStatus1_mass", &MuNegGenStatus1_mass, &b_MuNegGenStatus1_mass);
  // fChain->SetBranchAddress("MuNegGenStatus1_charge", &MuNegGenStatus1_charge, &b_MuNegGenStatus1_charge);
  // fChain->SetBranchAddress("MuPosDRGenP", &MuPosDRGenP, &b_MuPosDRGenP);
  // fChain->SetBranchAddress("MuNegDRGenP", &MuNegDRGenP, &b_MuNegDRGenP);
  // fChain->SetBranchAddress("MuPosCovMatrix", MuPosCovMatrix, &b_MuPosCovMatrix);
  // fChain->SetBranchAddress("MuNegCovMatrix", MuNegCovMatrix, &b_MuNegCovMatrix);
  // fChain->SetBranchAddress("Jet_leading_pt", &Jet_leading_pt, &b_Jet_leading_pt);
  // fChain->SetBranchAddress("Jet_leading_eta", &Jet_leading_eta, &b_Jet_leading_eta);
  // fChain->SetBranchAddress("Jet_leading_phi", &Jet_leading_phi, &b_Jet_leading_phi);
  Notify();
}

Bool_t test_allEvt::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}

void test_allEvt::Show(Long64_t entry)
{
  // Print contents of entry.
  // If entry is not specified, print current entry
  if (!fChain) return;
  fChain->Show(entry);
}
Int_t test_allEvt::Cut(Long64_t entry)
{
  // This function may be called from Loop.
  // returns  1 if entry is accepted.
  // returns -1 otherwise.
  return 1;
}
#endif // #ifdef test_allEvt_cxx
