////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Oct 13 13:57:16 2012 by ROOT version 5.27/06b
// from TTree ZTreeProducer/ZTreeProducer
// found on file: ZTreeProducer_tree.root
//////////////////////////////////////////////////////////

#ifndef Zanalysis_controlplots_h
#define Zanalysis_controlplots_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <iostream>
#include <TSystem.h>
#include <TMath.h>
/*
#include "../includes/common.h"
// #include "rochcor_42X.h"                                                                                                                                     
#include "rochcor_44X_v3.h"
#include "MuScleFitCorrector.h"
#include "RecoilCorrector.h"
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <TGraphAsymmErrors.h>
#include <ctime>
#include <string>
#include <time.h>
*/

using namespace std;

class Zanalysis_controlplots {
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
  TBranch        *b_njets; //!
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

  Zanalysis_controlplots(TString f_str=0, double lumi_scaling_input=1, int useGen=0, TTree *tree=0);
  virtual ~Zanalysis_controlplots();
  virtual Int_t    Cut(Long64_t entry);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init(TTree *tree);
  virtual void     Loop(int IS_MC_CLOSURE_TEST=0, int isMCorDATA=0, TString outputdir=0, int buildTemplates=0, int useMomentumCorr=0, int smearRochCorrByNsigma=0, int useEffSF=0, int usePtSF=0, int useVtxSF=0, int controlplots=0, TString sampleName="", int generated_PDF_set=-1, int generated_PDF_member=-1, int contains_PDF_reweight=-1);
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);

  virtual void plot1D(string title, float xval, double weight, std::map<string, TH1F*> &allhistos,
		      int numbinsx, float xmin, float xmax);
  
  virtual void plot2D(string title, float xval, float yval, double weight, std::map<string, TH2F*> &allhistos,
		      int numbinsx, float xmin, float xmax, int numbinsy, float ymin, float ymax);
  

  virtual float deltaPhi( float phi1 , float phi2 );
  virtual float getMT( float pt1 , float phi1 , float pt2 , float phi2 );
  virtual float getPseudoMET(double pfmet_corr, double pfmet_phi_corr, int type);
  virtual float getPseudoMT(double met, double metphi);

  void calculateU1U2(double fmet , double fMPhi, double fPt1, double fPhi1, double & fU1, double & fU2);

  virtual pair<float,float> getPhiCorrMET( float met, float metphi, int nvtx, bool ismc );

};

#endif

#ifdef Zanalysis_controlplots_cxx
Zanalysis_controlplots::Zanalysis_controlplots(TString f_str, double lumi_scaling_input, int useGen, TTree *tree)
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

Zanalysis_controlplots::~Zanalysis_controlplots()
{
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

Int_t Zanalysis_controlplots::GetEntry(Long64_t entry)
{
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}
Long64_t Zanalysis_controlplots::LoadTree(Long64_t entry)
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

void Zanalysis_controlplots::Init(TTree *tree)
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
  fChain->SetBranchAddress("u1corr", &u1, &b_u1);
  fChain->SetBranchAddress("u2corr", &u2, &b_u2);
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
  }
  Notify();
}

Bool_t Zanalysis_controlplots::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}

void Zanalysis_controlplots::Show(Long64_t entry)
{
  // Print contents of entry.
  // If entry is not specified, print current entry
  if (!fChain) return;
  fChain->Show(entry);
}
Int_t Zanalysis_controlplots::Cut(Long64_t entry)
{
  // This function may be called from Loop.
  // returns  1 if entry is accepted.
  // returns -1 otherwise.
  return 1;
}

void Zanalysis_controlplots::plot1D(string title, float xval, double weight, std::map<string, TH1F*> &allhistos,
                                    int numbinsx, float xmin, float xmax)
{

  std::map<string, TH1F*>::iterator iter= allhistos.find(title);
  if(iter == allhistos.end()) //no histo for this yet, so make a new one                                                                                                                  
    {
      TH1F* currentHisto= new TH1F(title.c_str(), title.c_str(), numbinsx, xmin, xmax);
      currentHisto->Sumw2();
      currentHisto->Fill(xval, weight);
      allhistos.insert(std::pair<string, TH1F*> (title,currentHisto) );
    }
  else // exists already, so just fill it                                                                                                                                                 
    {
      (*iter).second->Fill(xval, weight);
    }
}

void Zanalysis_controlplots::plot2D(string title, float xval, float yval, double weight, std::map<string, TH2F*> &allhistos,
                                    int numbinsx, float xmin, float xmax, int numbinsy, float ymin, float ymax)
{


  std::map<string, TH2F*>::iterator iter= allhistos.find(title);
  if(iter == allhistos.end()) //no histo for this yet, so make a new one                                                                                                                  
    {
      TH2F* currentHisto= new TH2F(title.c_str(), title.c_str(), numbinsx, xmin, xmax, numbinsy, ymin, ymax);
      currentHisto->Fill(xval, yval, weight);
      allhistos.insert(std::pair<string, TH2F*> (title,currentHisto) );
    }
  else // exists already, so just fill it                                                                                                                                                 
    {
      (*iter).second->Fill(xval, yval, weight);
    }

  return;

}

// Delta Phi                                                                                                                                                               
float Zanalysis_controlplots::deltaPhi( float phi1 , float phi2 ) {
  float dphi = fabs( phi1 - phi2 );
  if( dphi > TMath::Pi() ) dphi = TMath::TwoPi() - dphi;
  return dphi;
}

float Zanalysis_controlplots::getMT( float pt1 , float phi1 , float pt2 , float phi2 ){

  float dphi = deltaPhi(phi1, phi2);
  return sqrt( 2 * ( pt1 * pt2 * (1 - cos( dphi ) ) ) );

}

float Zanalysis_controlplots::getPseudoMET(double pfmet_corr, double pfmet_phi_corr, int type)
{

  float metx = pfmet_corr * cos( pfmet_phi_corr );
  float mety = pfmet_corr * sin( pfmet_phi_corr );

  float ptx = MuNeg_pt * cos( MuNeg_phi );
  float pty = MuNeg_pt * sin( MuNeg_phi );

  //recalculate the MET with the negative lepton
  metx += ptx;
  mety += pty;

  float metphicorr_lep    = sqrt(metx*metx + mety*mety);
  float metphicorrphi_lep = atan2( mety , metx );

  if(type==0) return metphicorr_lep;
  if(type==1) return metphicorrphi_lep;

  return 10.;

}

float Zanalysis_controlplots::getPseudoMT(double pfmet_corr, double pfmet_phi_corr)
{

  float metx = pfmet_corr * cos( pfmet_phi_corr );
  float mety = pfmet_corr * sin( pfmet_phi_corr );

  float ptx = MuNeg_pt * cos( MuNeg_phi );
  float pty = MuNeg_pt * sin( MuNeg_phi );

  //recalculate the MET with the negative lepton
  metx += ptx;
  mety += pty;

  float metphicorr_lep    = sqrt(metx*metx + mety*mety);
  float metphicorrphi_lep = atan2( mety , metx );

  //recalculate the MT with the negative lepton                                                                                                                            
  float pseudoMT =  getMT(MuPos_pt, MuPos_phi, metphicorr_lep, metphicorrphi_lep);
  return pseudoMT;

}


pair<float,float> Zanalysis_controlplots::getPhiCorrMET( float met, float metphi, int nvtx, bool ismc ) {                                                 

  //using met phi corrections from C. Veelken (revision 1.6)
  //functions are available here:                                                                                                            
  //http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/JetMETCorrections/Type1MET/python/pfMETsysShiftCorrections_cfi.py                               
  float metx = met * cos( metphi );
  float mety = met * sin( metphi );

  float shiftx = 0.;
  float shifty = 0.;

  //use correction for data vs. mc                                                                                                                  // these numbers are documented in AN2012_333_v5
  shiftx = ismc ? (-0.02245 - 0.0498*nvtx)
    : (-0.1676 + 0.2509*nvtx);
  shifty = ismc ? (0.1158 - 0.3201*nvtx)
    : (-0.2244 - 0.4009*nvtx);

  metx -= shiftx;
  mety -= shifty;

  pair<float, float> phicorrmet = make_pair( sqrt( metx*metx + mety*mety ), atan2( mety , metx ) );
  return phicorrmet;
}

void Zanalysis_controlplots::calculateU1U2(double fMet , double fMPhi, double fPt1, double fPhi1, double & fU1,double & fU2)
{
    double lUX  = fMet*cos(fMPhi) + fPt1*cos(fPhi1);
    double lUY  = fMet*sin(fMPhi) + fPt1*sin(fPhi1);
    double lU   = sqrt(lUX*lUX+lUY*lUY);

    double fZPhi=fPhi1;
    double fZPt=fPt1;

    // rotate of -180 the X and Y component
    double lCos = - (lUX*cos(fZPhi) + lUY*sin(fZPhi))/lU;
    double lSin =   (lUX*sin(fZPhi) - lUY*cos(fZPhi))/lU;
    fU1 = lU*lCos;
    fU2 = lU*lSin;

}



#endif // #ifdef Zanalysis_controlplots_cxx
