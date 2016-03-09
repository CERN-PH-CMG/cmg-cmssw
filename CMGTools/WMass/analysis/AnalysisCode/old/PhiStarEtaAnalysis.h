//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Oct 13 13:57:16 2012 by ROOT version 5.27/06b
// from TTree ZTreeProducer/ZTreeProducer
// found on file: ZTreeProducer_tree.root
//////////////////////////////////////////////////////////

#ifndef PhiStarEtaAnalysis_h
#define PhiStarEtaAnalysis_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>
#include <TSystem.h>
#include <TLorentzVector.h>

using namespace std;

class PhiStarEtaAnalysis {
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
  Int_t           njets;
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
  Double_t        pfmetraw;
  Double_t        pfmetraw_phi;
  Double_t        nopumet;
  Double_t        nopumet_phi;
  Double_t        pucmet;
  Double_t        pucmet_phi;
  Double_t        pfMetForRegression;
  Double_t        pfMetForRegression_phi;
  Double_t        pumet;
  Double_t        pumet_phi;
  Double_t        tkmet;
  Double_t        tkmet_phi;
  Double_t        pfmet_sumEt;
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
  Int_t           MuPosMatchCMGmuon;
  Double_t        MuPos_dz;
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
  Int_t           MuNegMatchCMGmuon;
  Double_t        MuNeg_dz;
  Double_t        FSRWeight;
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
  Double_t        MuNegDRGenP;
  Double_t        Jet_leading_pt;
  Double_t        Jet_leading_eta;
  Double_t        Jet_leading_phi;
  Int_t           cmgjets_number;
  Double_t        cmgjets_pt[10];
  Double_t        cmgjets_eta[10];
  Double_t        cmgjets_phi[10];
  Int_t           cmgmuons_number;
  Double_t        cmgmuons_charge[10];
  Double_t        cmgmuons_ID[10];
  Double_t        cmgmuons_ID_8TeV[10];
  Double_t        cmgmuons_Iso[10];
  Double_t        cmgmuons_IsPromt[10];
  Double_t        cmgmuons_IsTrig[10];
  Double_t        cmgmuons_pt[10];
  Double_t        cmgmuons_eta[10];
  Double_t        cmgmuons_phi[10];
  Int_t           cmgelectrons_number;
  Double_t        cmgelectrons_pt[10];
  Double_t        cmgelectrons_eta[10];
  Double_t        cmgelectrons_phi[10];
  Double_t        cmgelectrons_TightID[10];
  Double_t        cmgelectrons_TightIso[10];
  Double_t        cmgelectrons_charge[10];
  Double_t        cmgelectrons_IsPromt[10];

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
  TBranch        *b_njets;   //!
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
  TBranch        *b_pfmetraw;   //!
  TBranch        *b_pfmetraw_phi;   //!
  TBranch        *b_nopumet;   //!
  TBranch        *b_nopumet_phi;   //!
  TBranch        *b_pucmet;   //!
  TBranch        *b_pucmet_phi;   //!
  TBranch        *b_pfMetForRegression;   //!
  TBranch        *b_pfMetForRegression_phi;   //!
  TBranch        *b_pumet;   //!
  TBranch        *b_pumet_phi;   //!
  TBranch        *b_tkmet;   //!
  TBranch        *b_tkmet_phi;   //!
  TBranch        *b_pfmet_sumEt;   //!
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
  TBranch        *b_MuPosMatchCMGmuon;   //!
  TBranch        *b_MuPos_dz;   //!
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
  TBranch        *b_MuNegMatchCMGmuon;   //!
  TBranch        *b_MuNeg_dz;   //!
  TBranch        *b_FSRWeight;   //!
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
  TBranch        *b_MuNegDRGenP;   //!
  TBranch        *b_Jet_leading_pt;   //!
  TBranch        *b_Jet_leading_eta;   //!
  TBranch        *b_Jet_leading_phi;   //!
  TBranch        *b_cmgjets_number;   //!
  TBranch        *b_cmgjets_pt;   //!
  TBranch        *b_cmgjets_eta;   //!
  TBranch        *b_cmgjets_phi;   //!
  TBranch        *b_cmgmuons_number;   //!
  TBranch        *b_cmgmuons_charge;   //!
  TBranch        *b_cmgmuons_ID;   //!
  TBranch        *b_cmgmuons_Iso;   //!
  TBranch        *b_cmgmuons_IsPromt;   //!
  TBranch        *b_cmgmuons_IsTrig;   //!
  TBranch        *b_cmgmuons_pt;   //!
  TBranch        *b_cmgmuons_eta;   //!
  TBranch        *b_cmgmuons_phi;   //!
  TBranch        *b_cmgelectrons_number;   //!
  TBranch        *b_cmgelectrons_pt;   //!
  TBranch        *b_cmgelectrons_eta;   //!
  TBranch        *b_cmgelectrons_phi;   //!
  TBranch        *b_cmgelectrons_TightID;   //!
  TBranch        *b_cmgelectrons_TightIso;   //!
  TBranch        *b_cmgelectrons_charge;   //!
  TBranch        *b_cmgelectrons_IsPromt;   //!

  PhiStarEtaAnalysis(TString f_str=0, double lumi_scaling_input=1, int useGen=0, TTree *tree=0);
  virtual ~PhiStarEtaAnalysis();
  virtual Int_t    Cut(Long64_t entry);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual Long64_t NumEntries();
  virtual void     Init(TTree *tree);
  virtual void     Loop(int chunk=0, int Entry_ini=0, int Entry_fin=0, int IS_MC_CLOSURE_TEST=0, int isMCorDATA=0, TString outputdir=0, int buildTemplates=0, int useMomentumCorr=0, int smearRochCorrByNsigma=0, int useEffSF=0, int usePtSF=0, int useVtxSF=0, int controlplots=0, TString sampleName="", int generated_PDF_set=-1, int generated_PDF_member=-1, int contains_PDF_reweight=-1);
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);
  void SetMuPosNegZ(const Int_t& iPos, const Int_t& iNeg);
  bool LeadMuIdTrg(const Int_t& i);
  bool LeadMuIdTrg(const Double_t& pt, const Double_t& eta, const Int_t& trg, const Double_t& iso, const Int_t& id);
  bool SecondMuId(const Int_t& i);
  bool SecondMuId(const Double_t& pt, const Double_t& eta,const Double_t& iso, const Int_t& id);
  void AssignRecoLeadingAndSubleadingMuonLabels(int useMomentumCorr, int IS_MC_CLOSURE_TEST, int isMCorDATA, int smearRochCorrByNsigma);
  void ComputeHXVarAndPhiStarEta();
  void SelectBestMuPosNegPair();
  bool ElectronTest(const Int_t& i);
  bool SelectBestElMuPair();
  TLorentzVector HZ_lv, HMuPos_lv, HMuNeg_lv;
  int HMuPosIsPrompt, HMuNegIsPrompt;
  void my_handler(int s);

};

#endif

#ifdef PhiStarEtaAnalysis_cxx
PhiStarEtaAnalysis::PhiStarEtaAnalysis(TString f_str, double lumi_scaling_input, int useGen, TTree *tree)
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

PhiStarEtaAnalysis::~PhiStarEtaAnalysis()
{
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

Int_t PhiStarEtaAnalysis::GetEntry(Long64_t entry)
{
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}
Long64_t PhiStarEtaAnalysis::LoadTree(Long64_t entry)
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

Long64_t PhiStarEtaAnalysis::NumEntries(){
  return fChain->GetEntries();
};


void PhiStarEtaAnalysis::Init(TTree *tree)
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
  fChain->SetBranchAddress("njets", &njets, &b_njets);
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
  fChain->SetBranchAddress("pfmetraw", &pfmetraw, &b_pfmetraw);
  fChain->SetBranchAddress("pfmetraw_phi", &pfmetraw_phi, &b_pfmetraw_phi);
  fChain->SetBranchAddress("nopumet", &nopumet, &b_nopumet);
  fChain->SetBranchAddress("nopumet_phi", &nopumet_phi, &b_nopumet_phi);
  fChain->SetBranchAddress("pucmet", &pucmet, &b_pucmet);
  fChain->SetBranchAddress("pucmet_phi", &pucmet_phi, &b_pucmet_phi);
  fChain->SetBranchAddress("pfMetForRegression", &pfMetForRegression, &b_pfMetForRegression);
  fChain->SetBranchAddress("pfMetForRegression_phi", &pfMetForRegression_phi, &b_pfMetForRegression_phi);
  fChain->SetBranchAddress("pumet", &pumet, &b_pumet);
  fChain->SetBranchAddress("pumet_phi", &pumet_phi, &b_pumet_phi);
  fChain->SetBranchAddress("tkmet", &tkmet, &b_tkmet);
  fChain->SetBranchAddress("tkmet_phi", &tkmet_phi, &b_tkmet_phi);
  fChain->SetBranchAddress("pfmet_sumEt", &pfmet_sumEt, &b_pfmet_sumEt);
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
  fChain->SetBranchAddress("MuPosMatchCMGmuon", &MuPosMatchCMGmuon, &b_MuPosMatchCMGmuon);
  fChain->SetBranchAddress("MuPos_dz", &MuPos_dz, &b_MuPos_dz);
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
  fChain->SetBranchAddress("MuNegMatchCMGmuon", &MuNegMatchCMGmuon, &b_MuNegMatchCMGmuon);
  fChain->SetBranchAddress("MuNeg_dz", &MuNeg_dz, &b_MuNeg_dz);
  fChain->SetBranchAddress("Jet_leading_pt", &Jet_leading_pt, &b_Jet_leading_pt);
  fChain->SetBranchAddress("Jet_leading_eta", &Jet_leading_eta, &b_Jet_leading_eta);
  fChain->SetBranchAddress("Jet_leading_phi", &Jet_leading_phi, &b_Jet_leading_phi);
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
    fChain->SetBranchAddress("MuPosGenStatus1_pt", &MuPosGenStatus1_pt, &b_MuPosGenStatus1_pt);
    fChain->SetBranchAddress("MuPosGenStatus1_eta", &MuPosGenStatus1_eta, &b_MuPosGenStatus1_eta);
    fChain->SetBranchAddress("MuPosGenStatus1_phi", &MuPosGenStatus1_phi, &b_MuPosGenStatus1_phi);
    fChain->SetBranchAddress("MuPosGenStatus1_mass", &MuPosGenStatus1_mass, &b_MuPosGenStatus1_mass);
    fChain->SetBranchAddress("MuPosGenStatus1_charge", &MuPosGenStatus1_charge, &b_MuPosGenStatus1_charge);
    fChain->SetBranchAddress("MuPosDRGenP", &MuPosDRGenP, &b_MuPosDRGenP);
    fChain->SetBranchAddress("MuNegGen_pt", &MuNegGen_pt, &b_MuNegGen_pt);
    fChain->SetBranchAddress("MuNegGen_eta", &MuNegGen_eta, &b_MuNegGen_eta);
    fChain->SetBranchAddress("MuNegGen_phi", &MuNegGen_phi, &b_MuNegGen_phi);
    fChain->SetBranchAddress("MuNegGen_mass", &MuNegGen_mass, &b_MuNegGen_mass);
    fChain->SetBranchAddress("MuNegGen_charge", &MuNegGen_charge, &b_MuNegGen_charge);
    fChain->SetBranchAddress("MuNegGenStatus1_pt", &MuNegGenStatus1_pt, &b_MuNegGenStatus1_pt);
    fChain->SetBranchAddress("MuNegGenStatus1_eta", &MuNegGenStatus1_eta, &b_MuNegGenStatus1_eta);
    fChain->SetBranchAddress("MuNegGenStatus1_phi", &MuNegGenStatus1_phi, &b_MuNegGenStatus1_phi);
    fChain->SetBranchAddress("MuNegGenStatus1_mass", &MuNegGenStatus1_mass, &b_MuNegGenStatus1_mass);
    fChain->SetBranchAddress("MuNegGenStatus1_charge", &MuNegGenStatus1_charge, &b_MuNegGenStatus1_charge);
    fChain->SetBranchAddress("MuNegDRGenP", &MuNegDRGenP, &b_MuNegDRGenP);
  }
  fChain->SetBranchAddress("cmgjets_number", &cmgjets_number, &b_cmgjets_number);
  fChain->SetBranchAddress("cmgjets_pt", cmgjets_pt, &b_cmgjets_pt);
  fChain->SetBranchAddress("cmgjets_eta", cmgjets_eta, &b_cmgjets_eta);
  fChain->SetBranchAddress("cmgjets_phi", cmgjets_phi, &b_cmgjets_phi);
  fChain->SetBranchAddress("cmgmuons_number", &cmgmuons_number, &b_cmgmuons_number);
  fChain->SetBranchAddress("cmgmuons_charge", cmgmuons_charge, &b_cmgmuons_charge);
  fChain->SetBranchAddress("cmgmuons_ID", cmgmuons_ID, &b_cmgmuons_ID);
  fChain->SetBranchAddress("cmgmuons_Iso", cmgmuons_Iso, &b_cmgmuons_Iso);
  fChain->SetBranchAddress("cmgmuons_IsPromt", cmgmuons_IsPromt, &b_cmgmuons_IsPromt);
  fChain->SetBranchAddress("cmgmuons_IsTrig", cmgmuons_IsTrig, &b_cmgmuons_IsTrig);
  fChain->SetBranchAddress("cmgmuons_pt", cmgmuons_pt, &b_cmgmuons_pt);
  fChain->SetBranchAddress("cmgmuons_eta", cmgmuons_eta, &b_cmgmuons_eta);
  fChain->SetBranchAddress("cmgmuons_phi", cmgmuons_phi, &b_cmgmuons_phi);
  fChain->SetBranchAddress("cmgelectrons_number", &cmgelectrons_number, &b_cmgelectrons_number);
  fChain->SetBranchAddress("cmgelectrons_pt", cmgelectrons_pt, &b_cmgelectrons_pt);
  fChain->SetBranchAddress("cmgelectrons_eta", cmgelectrons_eta, &b_cmgelectrons_eta);
  fChain->SetBranchAddress("cmgelectrons_phi", cmgelectrons_phi, &b_cmgelectrons_phi);
  fChain->SetBranchAddress("cmgelectrons_TightID", cmgelectrons_TightID, &b_cmgelectrons_TightID);
  fChain->SetBranchAddress("cmgelectrons_TightIso", cmgelectrons_TightIso, &b_cmgelectrons_TightIso);
  fChain->SetBranchAddress("cmgelectrons_charge", cmgelectrons_charge, &b_cmgelectrons_charge);
  fChain->SetBranchAddress("cmgelectrons_IsPromt", cmgelectrons_IsPromt, &b_cmgelectrons_IsPromt);
  Notify();
}

Bool_t PhiStarEtaAnalysis::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}

void PhiStarEtaAnalysis::Show(Long64_t entry)
{
  // Print contents of entry.
  // If entry is not specified, print current entry
  if (!fChain) return;
  fChain->Show(entry);
}
Int_t PhiStarEtaAnalysis::Cut(Long64_t entry)
{
  // This function may be called from Loop.
  // returns  1 if entry is accepted.
  // returns -1 otherwise.
  return 1;
}


#endif // #ifdef PhiStarEtaAnalysis_cxx
