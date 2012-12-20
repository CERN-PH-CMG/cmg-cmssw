//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Dec 19 14:14:52 2012 by ROOT version 5.32/00
// from TTree ttHLepTreeProducerBase/ttHLepTreeProducerBase
// found on file: ../../../cfg/TREES_171212/TTH/ttHLepTreeProducerBase/ttHLepTreeProducerBase_tree.root
//////////////////////////////////////////////////////////

#ifndef TTHTree_h
#define TTHTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class TTHTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Double_t        nVert;
   Int_t           nLepLoose;
   Int_t           nLepGood;
   Int_t           nJet25;
   Int_t           nJet30;
   Double_t        LepLoose1_pt;
   Double_t        LepLoose1_eta;
   Double_t        LepLoose1_phi;
   Double_t        LepLoose1_mass;
   Double_t        LepLoose1_energy;
   Int_t           LepLoose1_pdgId;
   Int_t           LepLoose1_charge;
   Double_t        LepLoose1_sip3d;
   Double_t        LepLoose1_sip3ds;
   Double_t        LepLoose1_ip3ds;
   Double_t        LepLoose1_dxy;
   Double_t        LepLoose1_dz;
   Double_t        LepLoose1_relIso;
   Double_t        LepLoose1_chargedIso;
   Double_t        LepLoose1_drBJet;
   Double_t        LepLoose1_jetPtRatio;
   Double_t        LepLoose1_jetBTagCSV;
   Double_t        LepLoose1_jetBTagTCHE;
   Double_t        LepLoose1_ptRelJet;
   Double_t        LepLoose1_jetDR;
   Int_t           LepLoose1_jetSelf;
   Int_t           LepLoose1_innerHits;
   Double_t        LepLoose1_mvaId;
   Double_t        LepLoose1_tightId;
   Double_t        LepLoose1_mva;
   Int_t           LepLoose1_tightCharge;
   Double_t        LepGood1_pt;
   Double_t        LepGood1_eta;
   Double_t        LepGood1_phi;
   Double_t        LepGood1_mass;
   Double_t        LepGood1_energy;
   Int_t           LepGood1_pdgId;
   Int_t           LepGood1_charge;
   Double_t        LepGood1_sip3d;
   Double_t        LepGood1_sip3ds;
   Double_t        LepGood1_ip3ds;
   Double_t        LepGood1_dxy;
   Double_t        LepGood1_dz;
   Double_t        LepGood1_relIso;
   Double_t        LepGood1_chargedIso;
   Double_t        LepGood1_drBJet;
   Double_t        LepGood1_jetPtRatio;
   Double_t        LepGood1_jetBTagCSV;
   Double_t        LepGood1_jetBTagTCHE;
   Double_t        LepGood1_ptRelJet;
   Double_t        LepGood1_jetDR;
   Int_t           LepGood1_jetSelf;
   Int_t           LepGood1_innerHits;
   Double_t        LepGood1_mvaId;
   Double_t        LepGood1_tightId;
   Double_t        LepGood1_mva;
   Int_t           LepGood1_tightCharge;
   Int_t           LepGood1_mcMatchId;
   Int_t           LepGood1_mcMatchAny;
   Double_t        LepGood1_mcDeltaRB;
   Double_t        LepLoose2_pt;
   Double_t        LepLoose2_eta;
   Double_t        LepLoose2_phi;
   Double_t        LepLoose2_mass;
   Double_t        LepLoose2_energy;
   Int_t           LepLoose2_pdgId;
   Int_t           LepLoose2_charge;
   Double_t        LepLoose2_sip3d;
   Double_t        LepLoose2_sip3ds;
   Double_t        LepLoose2_ip3ds;
   Double_t        LepLoose2_dxy;
   Double_t        LepLoose2_dz;
   Double_t        LepLoose2_relIso;
   Double_t        LepLoose2_chargedIso;
   Double_t        LepLoose2_drBJet;
   Double_t        LepLoose2_jetPtRatio;
   Double_t        LepLoose2_jetBTagCSV;
   Double_t        LepLoose2_jetBTagTCHE;
   Double_t        LepLoose2_ptRelJet;
   Double_t        LepLoose2_jetDR;
   Int_t           LepLoose2_jetSelf;
   Int_t           LepLoose2_innerHits;
   Double_t        LepLoose2_mvaId;
   Double_t        LepLoose2_tightId;
   Double_t        LepLoose2_mva;
   Int_t           LepLoose2_tightCharge;
   Double_t        LepGood2_pt;
   Double_t        LepGood2_eta;
   Double_t        LepGood2_phi;
   Double_t        LepGood2_mass;
   Double_t        LepGood2_energy;
   Int_t           LepGood2_pdgId;
   Int_t           LepGood2_charge;
   Double_t        LepGood2_sip3d;
   Double_t        LepGood2_sip3ds;
   Double_t        LepGood2_ip3ds;
   Double_t        LepGood2_dxy;
   Double_t        LepGood2_dz;
   Double_t        LepGood2_relIso;
   Double_t        LepGood2_chargedIso;
   Double_t        LepGood2_drBJet;
   Double_t        LepGood2_jetPtRatio;
   Double_t        LepGood2_jetBTagCSV;
   Double_t        LepGood2_jetBTagTCHE;
   Double_t        LepGood2_ptRelJet;
   Double_t        LepGood2_jetDR;
   Int_t           LepGood2_jetSelf;
   Int_t           LepGood2_innerHits;
   Double_t        LepGood2_mvaId;
   Double_t        LepGood2_tightId;
   Double_t        LepGood2_mva;
   Int_t           LepGood2_tightCharge;
   Int_t           LepGood2_mcMatchId;
   Int_t           LepGood2_mcMatchAny;
   Double_t        LepGood2_mcDeltaRB;
   Double_t        LepLoose3_pt;
   Double_t        LepLoose3_eta;
   Double_t        LepLoose3_phi;
   Double_t        LepLoose3_mass;
   Double_t        LepLoose3_energy;
   Int_t           LepLoose3_pdgId;
   Int_t           LepLoose3_charge;
   Double_t        LepLoose3_sip3d;
   Double_t        LepLoose3_sip3ds;
   Double_t        LepLoose3_ip3ds;
   Double_t        LepLoose3_dxy;
   Double_t        LepLoose3_dz;
   Double_t        LepLoose3_relIso;
   Double_t        LepLoose3_chargedIso;
   Double_t        LepLoose3_drBJet;
   Double_t        LepLoose3_jetPtRatio;
   Double_t        LepLoose3_jetBTagCSV;
   Double_t        LepLoose3_jetBTagTCHE;
   Double_t        LepLoose3_ptRelJet;
   Double_t        LepLoose3_jetDR;
   Int_t           LepLoose3_jetSelf;
   Int_t           LepLoose3_innerHits;
   Double_t        LepLoose3_mvaId;
   Double_t        LepLoose3_tightId;
   Double_t        LepLoose3_mva;
   Int_t           LepLoose3_tightCharge;
   Double_t        LepGood3_pt;
   Double_t        LepGood3_eta;
   Double_t        LepGood3_phi;
   Double_t        LepGood3_mass;
   Double_t        LepGood3_energy;
   Int_t           LepGood3_pdgId;
   Int_t           LepGood3_charge;
   Double_t        LepGood3_sip3d;
   Double_t        LepGood3_sip3ds;
   Double_t        LepGood3_ip3ds;
   Double_t        LepGood3_dxy;
   Double_t        LepGood3_dz;
   Double_t        LepGood3_relIso;
   Double_t        LepGood3_chargedIso;
   Double_t        LepGood3_drBJet;
   Double_t        LepGood3_jetPtRatio;
   Double_t        LepGood3_jetBTagCSV;
   Double_t        LepGood3_jetBTagTCHE;
   Double_t        LepGood3_ptRelJet;
   Double_t        LepGood3_jetDR;
   Int_t           LepGood3_jetSelf;
   Int_t           LepGood3_innerHits;
   Double_t        LepGood3_mvaId;
   Double_t        LepGood3_tightId;
   Double_t        LepGood3_mva;
   Int_t           LepGood3_tightCharge;
   Int_t           LepGood3_mcMatchId;
   Int_t           LepGood3_mcMatchAny;
   Double_t        LepGood3_mcDeltaRB;
   Double_t        LepLoose4_pt;
   Double_t        LepLoose4_eta;
   Double_t        LepLoose4_phi;
   Double_t        LepLoose4_mass;
   Double_t        LepLoose4_energy;
   Int_t           LepLoose4_pdgId;
   Int_t           LepLoose4_charge;
   Double_t        LepLoose4_sip3d;
   Double_t        LepLoose4_sip3ds;
   Double_t        LepLoose4_ip3ds;
   Double_t        LepLoose4_dxy;
   Double_t        LepLoose4_dz;
   Double_t        LepLoose4_relIso;
   Double_t        LepLoose4_chargedIso;
   Double_t        LepLoose4_drBJet;
   Double_t        LepLoose4_jetPtRatio;
   Double_t        LepLoose4_jetBTagCSV;
   Double_t        LepLoose4_jetBTagTCHE;
   Double_t        LepLoose4_ptRelJet;
   Double_t        LepLoose4_jetDR;
   Int_t           LepLoose4_jetSelf;
   Int_t           LepLoose4_innerHits;
   Double_t        LepLoose4_mvaId;
   Double_t        LepLoose4_tightId;
   Double_t        LepLoose4_mva;
   Int_t           LepLoose4_tightCharge;
   Double_t        LepGood4_pt;
   Double_t        LepGood4_eta;
   Double_t        LepGood4_phi;
   Double_t        LepGood4_mass;
   Double_t        LepGood4_energy;
   Int_t           LepGood4_pdgId;
   Int_t           LepGood4_charge;
   Double_t        LepGood4_sip3d;
   Double_t        LepGood4_sip3ds;
   Double_t        LepGood4_ip3ds;
   Double_t        LepGood4_dxy;
   Double_t        LepGood4_dz;
   Double_t        LepGood4_relIso;
   Double_t        LepGood4_chargedIso;
   Double_t        LepGood4_drBJet;
   Double_t        LepGood4_jetPtRatio;
   Double_t        LepGood4_jetBTagCSV;
   Double_t        LepGood4_jetBTagTCHE;
   Double_t        LepGood4_ptRelJet;
   Double_t        LepGood4_jetDR;
   Int_t           LepGood4_jetSelf;
   Int_t           LepGood4_innerHits;
   Double_t        LepGood4_mvaId;
   Double_t        LepGood4_tightId;
   Double_t        LepGood4_mva;
   Int_t           LepGood4_tightCharge;
   Int_t           LepGood4_mcMatchId;
   Int_t           LepGood4_mcMatchAny;
   Double_t        LepGood4_mcDeltaRB;
   Double_t        LepLoose5_pt;
   Double_t        LepLoose5_eta;
   Double_t        LepLoose5_phi;
   Double_t        LepLoose5_mass;
   Double_t        LepLoose5_energy;
   Int_t           LepLoose5_pdgId;
   Int_t           LepLoose5_charge;
   Double_t        LepLoose5_sip3d;
   Double_t        LepLoose5_sip3ds;
   Double_t        LepLoose5_ip3ds;
   Double_t        LepLoose5_dxy;
   Double_t        LepLoose5_dz;
   Double_t        LepLoose5_relIso;
   Double_t        LepLoose5_chargedIso;
   Double_t        LepLoose5_drBJet;
   Double_t        LepLoose5_jetPtRatio;
   Double_t        LepLoose5_jetBTagCSV;
   Double_t        LepLoose5_jetBTagTCHE;
   Double_t        LepLoose5_ptRelJet;
   Double_t        LepLoose5_jetDR;
   Int_t           LepLoose5_jetSelf;
   Int_t           LepLoose5_innerHits;
   Double_t        LepLoose5_mvaId;
   Double_t        LepLoose5_tightId;
   Double_t        LepLoose5_mva;
   Int_t           LepLoose5_tightCharge;
   Double_t        LepGood5_pt;
   Double_t        LepGood5_eta;
   Double_t        LepGood5_phi;
   Double_t        LepGood5_mass;
   Double_t        LepGood5_energy;
   Int_t           LepGood5_pdgId;
   Int_t           LepGood5_charge;
   Double_t        LepGood5_sip3d;
   Double_t        LepGood5_sip3ds;
   Double_t        LepGood5_ip3ds;
   Double_t        LepGood5_dxy;
   Double_t        LepGood5_dz;
   Double_t        LepGood5_relIso;
   Double_t        LepGood5_chargedIso;
   Double_t        LepGood5_drBJet;
   Double_t        LepGood5_jetPtRatio;
   Double_t        LepGood5_jetBTagCSV;
   Double_t        LepGood5_jetBTagTCHE;
   Double_t        LepGood5_ptRelJet;
   Double_t        LepGood5_jetDR;
   Int_t           LepGood5_jetSelf;
   Int_t           LepGood5_innerHits;
   Double_t        LepGood5_mvaId;
   Double_t        LepGood5_tightId;
   Double_t        LepGood5_mva;
   Int_t           LepGood5_tightCharge;
   Int_t           LepGood5_mcMatchId;
   Int_t           LepGood5_mcMatchAny;
   Double_t        LepGood5_mcDeltaRB;
   Double_t        LepLoose6_pt;
   Double_t        LepLoose6_eta;
   Double_t        LepLoose6_phi;
   Double_t        LepLoose6_mass;
   Double_t        LepLoose6_energy;
   Int_t           LepLoose6_pdgId;
   Int_t           LepLoose6_charge;
   Double_t        LepLoose6_sip3d;
   Double_t        LepLoose6_sip3ds;
   Double_t        LepLoose6_ip3ds;
   Double_t        LepLoose6_dxy;
   Double_t        LepLoose6_dz;
   Double_t        LepLoose6_relIso;
   Double_t        LepLoose6_chargedIso;
   Double_t        LepLoose6_drBJet;
   Double_t        LepLoose6_jetPtRatio;
   Double_t        LepLoose6_jetBTagCSV;
   Double_t        LepLoose6_jetBTagTCHE;
   Double_t        LepLoose6_ptRelJet;
   Double_t        LepLoose6_jetDR;
   Int_t           LepLoose6_jetSelf;
   Int_t           LepLoose6_innerHits;
   Double_t        LepLoose6_mvaId;
   Double_t        LepLoose6_tightId;
   Double_t        LepLoose6_mva;
   Int_t           LepLoose6_tightCharge;
   Double_t        LepGood6_pt;
   Double_t        LepGood6_eta;
   Double_t        LepGood6_phi;
   Double_t        LepGood6_mass;
   Double_t        LepGood6_energy;
   Int_t           LepGood6_pdgId;
   Int_t           LepGood6_charge;
   Double_t        LepGood6_sip3d;
   Double_t        LepGood6_sip3ds;
   Double_t        LepGood6_ip3ds;
   Double_t        LepGood6_dxy;
   Double_t        LepGood6_dz;
   Double_t        LepGood6_relIso;
   Double_t        LepGood6_chargedIso;
   Double_t        LepGood6_drBJet;
   Double_t        LepGood6_jetPtRatio;
   Double_t        LepGood6_jetBTagCSV;
   Double_t        LepGood6_jetBTagTCHE;
   Double_t        LepGood6_ptRelJet;
   Double_t        LepGood6_jetDR;
   Int_t           LepGood6_jetSelf;
   Int_t           LepGood6_innerHits;
   Double_t        LepGood6_mvaId;
   Double_t        LepGood6_tightId;
   Double_t        LepGood6_mva;
   Int_t           LepGood6_tightCharge;
   Int_t           LepGood6_mcMatchId;
   Int_t           LepGood6_mcMatchAny;
   Double_t        LepGood6_mcDeltaRB;
   Double_t        LepLoose7_pt;
   Double_t        LepLoose7_eta;
   Double_t        LepLoose7_phi;
   Double_t        LepLoose7_mass;
   Double_t        LepLoose7_energy;
   Int_t           LepLoose7_pdgId;
   Int_t           LepLoose7_charge;
   Double_t        LepLoose7_sip3d;
   Double_t        LepLoose7_sip3ds;
   Double_t        LepLoose7_ip3ds;
   Double_t        LepLoose7_dxy;
   Double_t        LepLoose7_dz;
   Double_t        LepLoose7_relIso;
   Double_t        LepLoose7_chargedIso;
   Double_t        LepLoose7_drBJet;
   Double_t        LepLoose7_jetPtRatio;
   Double_t        LepLoose7_jetBTagCSV;
   Double_t        LepLoose7_jetBTagTCHE;
   Double_t        LepLoose7_ptRelJet;
   Double_t        LepLoose7_jetDR;
   Int_t           LepLoose7_jetSelf;
   Int_t           LepLoose7_innerHits;
   Double_t        LepLoose7_mvaId;
   Double_t        LepLoose7_tightId;
   Double_t        LepLoose7_mva;
   Int_t           LepLoose7_tightCharge;
   Double_t        LepGood7_pt;
   Double_t        LepGood7_eta;
   Double_t        LepGood7_phi;
   Double_t        LepGood7_mass;
   Double_t        LepGood7_energy;
   Int_t           LepGood7_pdgId;
   Int_t           LepGood7_charge;
   Double_t        LepGood7_sip3d;
   Double_t        LepGood7_sip3ds;
   Double_t        LepGood7_ip3ds;
   Double_t        LepGood7_dxy;
   Double_t        LepGood7_dz;
   Double_t        LepGood7_relIso;
   Double_t        LepGood7_chargedIso;
   Double_t        LepGood7_drBJet;
   Double_t        LepGood7_jetPtRatio;
   Double_t        LepGood7_jetBTagCSV;
   Double_t        LepGood7_jetBTagTCHE;
   Double_t        LepGood7_ptRelJet;
   Double_t        LepGood7_jetDR;
   Int_t           LepGood7_jetSelf;
   Int_t           LepGood7_innerHits;
   Double_t        LepGood7_mvaId;
   Double_t        LepGood7_tightId;
   Double_t        LepGood7_mva;
   Int_t           LepGood7_tightCharge;
   Int_t           LepGood7_mcMatchId;
   Int_t           LepGood7_mcMatchAny;
   Double_t        LepGood7_mcDeltaRB;
   Double_t        LepLoose8_pt;
   Double_t        LepLoose8_eta;
   Double_t        LepLoose8_phi;
   Double_t        LepLoose8_mass;
   Double_t        LepLoose8_energy;
   Int_t           LepLoose8_pdgId;
   Int_t           LepLoose8_charge;
   Double_t        LepLoose8_sip3d;
   Double_t        LepLoose8_sip3ds;
   Double_t        LepLoose8_ip3ds;
   Double_t        LepLoose8_dxy;
   Double_t        LepLoose8_dz;
   Double_t        LepLoose8_relIso;
   Double_t        LepLoose8_chargedIso;
   Double_t        LepLoose8_drBJet;
   Double_t        LepLoose8_jetPtRatio;
   Double_t        LepLoose8_jetBTagCSV;
   Double_t        LepLoose8_jetBTagTCHE;
   Double_t        LepLoose8_ptRelJet;
   Double_t        LepLoose8_jetDR;
   Int_t           LepLoose8_jetSelf;
   Int_t           LepLoose8_innerHits;
   Double_t        LepLoose8_mvaId;
   Double_t        LepLoose8_tightId;
   Double_t        LepLoose8_mva;
   Int_t           LepLoose8_tightCharge;
   Double_t        LepGood8_pt;
   Double_t        LepGood8_eta;
   Double_t        LepGood8_phi;
   Double_t        LepGood8_mass;
   Double_t        LepGood8_energy;
   Int_t           LepGood8_pdgId;
   Int_t           LepGood8_charge;
   Double_t        LepGood8_sip3d;
   Double_t        LepGood8_sip3ds;
   Double_t        LepGood8_ip3ds;
   Double_t        LepGood8_dxy;
   Double_t        LepGood8_dz;
   Double_t        LepGood8_relIso;
   Double_t        LepGood8_chargedIso;
   Double_t        LepGood8_drBJet;
   Double_t        LepGood8_jetPtRatio;
   Double_t        LepGood8_jetBTagCSV;
   Double_t        LepGood8_jetBTagTCHE;
   Double_t        LepGood8_ptRelJet;
   Double_t        LepGood8_jetDR;
   Int_t           LepGood8_jetSelf;
   Int_t           LepGood8_innerHits;
   Double_t        LepGood8_mvaId;
   Double_t        LepGood8_tightId;
   Double_t        LepGood8_mva;
   Int_t           LepGood8_tightCharge;
   Int_t           LepGood8_mcMatchId;
   Int_t           LepGood8_mcMatchAny;
   Double_t        LepGood8_mcDeltaRB;
   Double_t        Jet1_pt;
   Double_t        Jet1_eta;
   Double_t        Jet1_phi;
   Double_t        Jet1_mass;
   Double_t        Jet1_energy;
   Double_t        Jet1_btagCSV;
   Double_t        Jet1_leg_pt;
   Double_t        Jet1_leg_eta;
   Double_t        Jet1_leg_phi;
   Double_t        Jet1_leg_mass;
   Double_t        Jet1_leg_energy;
   Int_t           Jet1_mcMatchId;
   Int_t           Jet1_mcMatchFlav;
   Double_t        Jet2_pt;
   Double_t        Jet2_eta;
   Double_t        Jet2_phi;
   Double_t        Jet2_mass;
   Double_t        Jet2_energy;
   Double_t        Jet2_btagCSV;
   Double_t        Jet2_leg_pt;
   Double_t        Jet2_leg_eta;
   Double_t        Jet2_leg_phi;
   Double_t        Jet2_leg_mass;
   Double_t        Jet2_leg_energy;
   Int_t           Jet2_mcMatchId;
   Int_t           Jet2_mcMatchFlav;
   Double_t        Jet3_pt;
   Double_t        Jet3_eta;
   Double_t        Jet3_phi;
   Double_t        Jet3_mass;
   Double_t        Jet3_energy;
   Double_t        Jet3_btagCSV;
   Double_t        Jet3_leg_pt;
   Double_t        Jet3_leg_eta;
   Double_t        Jet3_leg_phi;
   Double_t        Jet3_leg_mass;
   Double_t        Jet3_leg_energy;
   Int_t           Jet3_mcMatchId;
   Int_t           Jet3_mcMatchFlav;
   Double_t        Jet4_pt;
   Double_t        Jet4_eta;
   Double_t        Jet4_phi;
   Double_t        Jet4_mass;
   Double_t        Jet4_energy;
   Double_t        Jet4_btagCSV;
   Double_t        Jet4_leg_pt;
   Double_t        Jet4_leg_eta;
   Double_t        Jet4_leg_phi;
   Double_t        Jet4_leg_mass;
   Double_t        Jet4_leg_energy;
   Int_t           Jet4_mcMatchId;
   Int_t           Jet4_mcMatchFlav;
   Double_t        Jet5_pt;
   Double_t        Jet5_eta;
   Double_t        Jet5_phi;
   Double_t        Jet5_mass;
   Double_t        Jet5_energy;
   Double_t        Jet5_btagCSV;
   Double_t        Jet5_leg_pt;
   Double_t        Jet5_leg_eta;
   Double_t        Jet5_leg_phi;
   Double_t        Jet5_leg_mass;
   Double_t        Jet5_leg_energy;
   Int_t           Jet5_mcMatchId;
   Int_t           Jet5_mcMatchFlav;
   Double_t        Jet6_pt;
   Double_t        Jet6_eta;
   Double_t        Jet6_phi;
   Double_t        Jet6_mass;
   Double_t        Jet6_energy;
   Double_t        Jet6_btagCSV;
   Double_t        Jet6_leg_pt;
   Double_t        Jet6_leg_eta;
   Double_t        Jet6_leg_phi;
   Double_t        Jet6_leg_mass;
   Double_t        Jet6_leg_energy;
   Int_t           Jet6_mcMatchId;
   Int_t           Jet6_mcMatchFlav;
   Double_t        Jet7_pt;
   Double_t        Jet7_eta;
   Double_t        Jet7_phi;
   Double_t        Jet7_mass;
   Double_t        Jet7_energy;
   Double_t        Jet7_btagCSV;
   Double_t        Jet7_leg_pt;
   Double_t        Jet7_leg_eta;
   Double_t        Jet7_leg_phi;
   Double_t        Jet7_leg_mass;
   Double_t        Jet7_leg_energy;
   Int_t           Jet7_mcMatchId;
   Int_t           Jet7_mcMatchFlav;
   Double_t        Jet8_pt;
   Double_t        Jet8_eta;
   Double_t        Jet8_phi;
   Double_t        Jet8_mass;
   Double_t        Jet8_energy;
   Double_t        Jet8_btagCSV;
   Double_t        Jet8_leg_pt;
   Double_t        Jet8_leg_eta;
   Double_t        Jet8_leg_phi;
   Double_t        Jet8_leg_mass;
   Double_t        Jet8_leg_energy;
   Int_t           Jet8_mcMatchId;
   Int_t           Jet8_mcMatchFlav;
   Int_t           nBJetLoose25;
   Int_t           nBJetMedium25;
   Int_t           nBJetLoose30;
   Int_t           nBJetMedium30;
   Double_t        met;
   Double_t        metSignificance;
   Double_t        projMetAll1S;
   Double_t        projMetAll2S;
   Double_t        projMetJet1S;
   Double_t        projMetJet2S;
   Double_t        htJet30;
   Double_t        htJet25;
   Double_t        mhtJet30;
   Double_t        mhtJet25;
   Double_t        mZ1;
   Double_t        mZ2;
   Double_t        minMllSFOS;
   Double_t        minMllAFOS;
   Double_t        minMllAFAS;
   Int_t           GenHiggsDecayMode;
   Double_t        GenHiggs_pt;
   Double_t        GenHiggs_eta;
   Double_t        GenHiggs_phi;
   Double_t        GenHiggs_mass;
   Double_t        GenHiggs_energy;
   Int_t           GenHiggs_pdgId;
   Int_t           GenHiggs_charge;
   Double_t        GenTop1_pt;
   Double_t        GenTop1_eta;
   Double_t        GenTop1_phi;
   Double_t        GenTop1_mass;
   Double_t        GenTop1_energy;
   Int_t           GenTop1_pdgId;
   Int_t           GenTop1_charge;
   Double_t        GenTop2_pt;
   Double_t        GenTop2_eta;
   Double_t        GenTop2_phi;
   Double_t        GenTop2_mass;
   Double_t        GenTop2_energy;
   Int_t           GenTop2_pdgId;
   Int_t           GenTop2_charge;
   Int_t           nGenLeps;
   Int_t           nGenLepsFromTau;
   Int_t           nGenQuarks;
   Int_t           nGenBQuarks;
   Double_t        GenLep1_pt;
   Double_t        GenLep1_eta;
   Double_t        GenLep1_phi;
   Double_t        GenLep1_mass;
   Double_t        GenLep1_energy;
   Int_t           GenLep1_pdgId;
   Int_t           GenLep1_charge;
   Int_t           GenLep1_sourceId;
   Double_t        GenLepFromTau1_pt;
   Double_t        GenLepFromTau1_eta;
   Double_t        GenLepFromTau1_phi;
   Double_t        GenLepFromTau1_mass;
   Double_t        GenLepFromTau1_energy;
   Int_t           GenLepFromTau1_pdgId;
   Int_t           GenLepFromTau1_charge;
   Int_t           GenLepFromTau1_sourceId;
   Double_t        GenQuark1_pt;
   Double_t        GenQuark1_eta;
   Double_t        GenQuark1_phi;
   Double_t        GenQuark1_mass;
   Double_t        GenQuark1_energy;
   Int_t           GenQuark1_pdgId;
   Int_t           GenQuark1_charge;
   Int_t           GenQuark1_sourceId;
   Double_t        GenLep2_pt;
   Double_t        GenLep2_eta;
   Double_t        GenLep2_phi;
   Double_t        GenLep2_mass;
   Double_t        GenLep2_energy;
   Int_t           GenLep2_pdgId;
   Int_t           GenLep2_charge;
   Int_t           GenLep2_sourceId;
   Double_t        GenLepFromTau2_pt;
   Double_t        GenLepFromTau2_eta;
   Double_t        GenLepFromTau2_phi;
   Double_t        GenLepFromTau2_mass;
   Double_t        GenLepFromTau2_energy;
   Int_t           GenLepFromTau2_pdgId;
   Int_t           GenLepFromTau2_charge;
   Int_t           GenLepFromTau2_sourceId;
   Double_t        GenQuark2_pt;
   Double_t        GenQuark2_eta;
   Double_t        GenQuark2_phi;
   Double_t        GenQuark2_mass;
   Double_t        GenQuark2_energy;
   Int_t           GenQuark2_pdgId;
   Int_t           GenQuark2_charge;
   Int_t           GenQuark2_sourceId;
   Double_t        GenLep3_pt;
   Double_t        GenLep3_eta;
   Double_t        GenLep3_phi;
   Double_t        GenLep3_mass;
   Double_t        GenLep3_energy;
   Int_t           GenLep3_pdgId;
   Int_t           GenLep3_charge;
   Int_t           GenLep3_sourceId;
   Double_t        GenLepFromTau3_pt;
   Double_t        GenLepFromTau3_eta;
   Double_t        GenLepFromTau3_phi;
   Double_t        GenLepFromTau3_mass;
   Double_t        GenLepFromTau3_energy;
   Int_t           GenLepFromTau3_pdgId;
   Int_t           GenLepFromTau3_charge;
   Int_t           GenLepFromTau3_sourceId;
   Double_t        GenQuark3_pt;
   Double_t        GenQuark3_eta;
   Double_t        GenQuark3_phi;
   Double_t        GenQuark3_mass;
   Double_t        GenQuark3_energy;
   Int_t           GenQuark3_pdgId;
   Int_t           GenQuark3_charge;
   Int_t           GenQuark3_sourceId;
   Double_t        GenLep4_pt;
   Double_t        GenLep4_eta;
   Double_t        GenLep4_phi;
   Double_t        GenLep4_mass;
   Double_t        GenLep4_energy;
   Int_t           GenLep4_pdgId;
   Int_t           GenLep4_charge;
   Int_t           GenLep4_sourceId;
   Double_t        GenLepFromTau4_pt;
   Double_t        GenLepFromTau4_eta;
   Double_t        GenLepFromTau4_phi;
   Double_t        GenLepFromTau4_mass;
   Double_t        GenLepFromTau4_energy;
   Int_t           GenLepFromTau4_pdgId;
   Int_t           GenLepFromTau4_charge;
   Int_t           GenLepFromTau4_sourceId;
   Double_t        GenQuark4_pt;
   Double_t        GenQuark4_eta;
   Double_t        GenQuark4_phi;
   Double_t        GenQuark4_mass;
   Double_t        GenQuark4_energy;
   Int_t           GenQuark4_pdgId;
   Int_t           GenQuark4_charge;
   Int_t           GenQuark4_sourceId;
   Double_t        GenLep5_pt;
   Double_t        GenLep5_eta;
   Double_t        GenLep5_phi;
   Double_t        GenLep5_mass;
   Double_t        GenLep5_energy;
   Int_t           GenLep5_pdgId;
   Int_t           GenLep5_charge;
   Int_t           GenLep5_sourceId;
   Double_t        GenLepFromTau5_pt;
   Double_t        GenLepFromTau5_eta;
   Double_t        GenLepFromTau5_phi;
   Double_t        GenLepFromTau5_mass;
   Double_t        GenLepFromTau5_energy;
   Int_t           GenLepFromTau5_pdgId;
   Int_t           GenLepFromTau5_charge;
   Int_t           GenLepFromTau5_sourceId;
   Double_t        GenQuark5_pt;
   Double_t        GenQuark5_eta;
   Double_t        GenQuark5_phi;
   Double_t        GenQuark5_mass;
   Double_t        GenQuark5_energy;
   Int_t           GenQuark5_pdgId;
   Int_t           GenQuark5_charge;
   Int_t           GenQuark5_sourceId;
   Double_t        GenLep6_pt;
   Double_t        GenLep6_eta;
   Double_t        GenLep6_phi;
   Double_t        GenLep6_mass;
   Double_t        GenLep6_energy;
   Int_t           GenLep6_pdgId;
   Int_t           GenLep6_charge;
   Int_t           GenLep6_sourceId;
   Double_t        GenLepFromTau6_pt;
   Double_t        GenLepFromTau6_eta;
   Double_t        GenLepFromTau6_phi;
   Double_t        GenLepFromTau6_mass;
   Double_t        GenLepFromTau6_energy;
   Int_t           GenLepFromTau6_pdgId;
   Int_t           GenLepFromTau6_charge;
   Int_t           GenLepFromTau6_sourceId;
   Double_t        GenQuark6_pt;
   Double_t        GenQuark6_eta;
   Double_t        GenQuark6_phi;
   Double_t        GenQuark6_mass;
   Double_t        GenQuark6_energy;
   Int_t           GenQuark6_pdgId;
   Int_t           GenQuark6_charge;
   Int_t           GenQuark6_sourceId;
   Double_t        GenBQuark1_pt;
   Double_t        GenBQuark1_eta;
   Double_t        GenBQuark1_phi;
   Double_t        GenBQuark1_mass;
   Double_t        GenBQuark1_energy;
   Int_t           GenBQuark1_pdgId;
   Int_t           GenBQuark1_charge;
   Double_t        GenBQuark2_pt;
   Double_t        GenBQuark2_eta;
   Double_t        GenBQuark2_phi;
   Double_t        GenBQuark2_mass;
   Double_t        GenBQuark2_energy;
   Int_t           GenBQuark2_pdgId;
   Int_t           GenBQuark2_charge;
   Int_t           nGoodLepsMatchId;
   Int_t           nGoodLepsMatchAny;

   // List of branches
   TBranch        *b_nVert;   //!
   TBranch        *b_nLepLoose;   //!
   TBranch        *b_nLepGood;   //!
   TBranch        *b_nJet25;   //!
   TBranch        *b_nJet30;   //!
   TBranch        *b_LepLoose1_pt;   //!
   TBranch        *b_LepLoose1_eta;   //!
   TBranch        *b_LepLoose1_phi;   //!
   TBranch        *b_LepLoose1_mass;   //!
   TBranch        *b_LepLoose1_energy;   //!
   TBranch        *b_LepLoose1_pdgId;   //!
   TBranch        *b_LepLoose1_charge;   //!
   TBranch        *b_LepLoose1_sip3d;   //!
   TBranch        *b_LepLoose1_sip3ds;   //!
   TBranch        *b_LepLoose1_ip3ds;   //!
   TBranch        *b_LepLoose1_dxy;   //!
   TBranch        *b_LepLoose1_dz;   //!
   TBranch        *b_LepLoose1_relIso;   //!
   TBranch        *b_LepLoose1_chargedIso;   //!
   TBranch        *b_LepLoose1_drBJet;   //!
   TBranch        *b_LepLoose1_jetPtRatio;   //!
   TBranch        *b_LepLoose1_jetBTagCSV;   //!
   TBranch        *b_LepLoose1_jetBTagTCHE;   //!
   TBranch        *b_LepLoose1_ptRelJet;   //!
   TBranch        *b_LepLoose1_jetDR;   //!
   TBranch        *b_LepLoose1_jetSelf;   //!
   TBranch        *b_LepLoose1_innerHits;   //!
   TBranch        *b_LepLoose1_mvaId;   //!
   TBranch        *b_LepLoose1_tightId;   //!
   TBranch        *b_LepLoose1_mva;   //!
   TBranch        *b_LepLoose1_tightCharge;   //!
   TBranch        *b_LepGood1_pt;   //!
   TBranch        *b_LepGood1_eta;   //!
   TBranch        *b_LepGood1_phi;   //!
   TBranch        *b_LepGood1_mass;   //!
   TBranch        *b_LepGood1_energy;   //!
   TBranch        *b_LepGood1_pdgId;   //!
   TBranch        *b_LepGood1_charge;   //!
   TBranch        *b_LepGood1_sip3d;   //!
   TBranch        *b_LepGood1_sip3ds;   //!
   TBranch        *b_LepGood1_ip3ds;   //!
   TBranch        *b_LepGood1_dxy;   //!
   TBranch        *b_LepGood1_dz;   //!
   TBranch        *b_LepGood1_relIso;   //!
   TBranch        *b_LepGood1_chargedIso;   //!
   TBranch        *b_LepGood1_drBJet;   //!
   TBranch        *b_LepGood1_jetPtRatio;   //!
   TBranch        *b_LepGood1_jetBTagCSV;   //!
   TBranch        *b_LepGood1_jetBTagTCHE;   //!
   TBranch        *b_LepGood1_ptRelJet;   //!
   TBranch        *b_LepGood1_jetDR;   //!
   TBranch        *b_LepGood1_jetSelf;   //!
   TBranch        *b_LepGood1_innerHits;   //!
   TBranch        *b_LepGood1_mvaId;   //!
   TBranch        *b_LepGood1_tightId;   //!
   TBranch        *b_LepGood1_mva;   //!
   TBranch        *b_LepGood1_tightCharge;   //!
   TBranch        *b_LepGood1_mcMatchId;   //!
   TBranch        *b_LepGood1_mcMatchAny;   //!
   TBranch        *b_LepGood1_mcDeltaRB;   //!
   TBranch        *b_LepLoose2_pt;   //!
   TBranch        *b_LepLoose2_eta;   //!
   TBranch        *b_LepLoose2_phi;   //!
   TBranch        *b_LepLoose2_mass;   //!
   TBranch        *b_LepLoose2_energy;   //!
   TBranch        *b_LepLoose2_pdgId;   //!
   TBranch        *b_LepLoose2_charge;   //!
   TBranch        *b_LepLoose2_sip3d;   //!
   TBranch        *b_LepLoose2_sip3ds;   //!
   TBranch        *b_LepLoose2_ip3ds;   //!
   TBranch        *b_LepLoose2_dxy;   //!
   TBranch        *b_LepLoose2_dz;   //!
   TBranch        *b_LepLoose2_relIso;   //!
   TBranch        *b_LepLoose2_chargedIso;   //!
   TBranch        *b_LepLoose2_drBJet;   //!
   TBranch        *b_LepLoose2_jetPtRatio;   //!
   TBranch        *b_LepLoose2_jetBTagCSV;   //!
   TBranch        *b_LepLoose2_jetBTagTCHE;   //!
   TBranch        *b_LepLoose2_ptRelJet;   //!
   TBranch        *b_LepLoose2_jetDR;   //!
   TBranch        *b_LepLoose2_jetSelf;   //!
   TBranch        *b_LepLoose2_innerHits;   //!
   TBranch        *b_LepLoose2_mvaId;   //!
   TBranch        *b_LepLoose2_tightId;   //!
   TBranch        *b_LepLoose2_mva;   //!
   TBranch        *b_LepLoose2_tightCharge;   //!
   TBranch        *b_LepGood2_pt;   //!
   TBranch        *b_LepGood2_eta;   //!
   TBranch        *b_LepGood2_phi;   //!
   TBranch        *b_LepGood2_mass;   //!
   TBranch        *b_LepGood2_energy;   //!
   TBranch        *b_LepGood2_pdgId;   //!
   TBranch        *b_LepGood2_charge;   //!
   TBranch        *b_LepGood2_sip3d;   //!
   TBranch        *b_LepGood2_sip3ds;   //!
   TBranch        *b_LepGood2_ip3ds;   //!
   TBranch        *b_LepGood2_dxy;   //!
   TBranch        *b_LepGood2_dz;   //!
   TBranch        *b_LepGood2_relIso;   //!
   TBranch        *b_LepGood2_chargedIso;   //!
   TBranch        *b_LepGood2_drBJet;   //!
   TBranch        *b_LepGood2_jetPtRatio;   //!
   TBranch        *b_LepGood2_jetBTagCSV;   //!
   TBranch        *b_LepGood2_jetBTagTCHE;   //!
   TBranch        *b_LepGood2_ptRelJet;   //!
   TBranch        *b_LepGood2_jetDR;   //!
   TBranch        *b_LepGood2_jetSelf;   //!
   TBranch        *b_LepGood2_innerHits;   //!
   TBranch        *b_LepGood2_mvaId;   //!
   TBranch        *b_LepGood2_tightId;   //!
   TBranch        *b_LepGood2_mva;   //!
   TBranch        *b_LepGood2_tightCharge;   //!
   TBranch        *b_LepGood2_mcMatchId;   //!
   TBranch        *b_LepGood2_mcMatchAny;   //!
   TBranch        *b_LepGood2_mcDeltaRB;   //!
   TBranch        *b_LepLoose3_pt;   //!
   TBranch        *b_LepLoose3_eta;   //!
   TBranch        *b_LepLoose3_phi;   //!
   TBranch        *b_LepLoose3_mass;   //!
   TBranch        *b_LepLoose3_energy;   //!
   TBranch        *b_LepLoose3_pdgId;   //!
   TBranch        *b_LepLoose3_charge;   //!
   TBranch        *b_LepLoose3_sip3d;   //!
   TBranch        *b_LepLoose3_sip3ds;   //!
   TBranch        *b_LepLoose3_ip3ds;   //!
   TBranch        *b_LepLoose3_dxy;   //!
   TBranch        *b_LepLoose3_dz;   //!
   TBranch        *b_LepLoose3_relIso;   //!
   TBranch        *b_LepLoose3_chargedIso;   //!
   TBranch        *b_LepLoose3_drBJet;   //!
   TBranch        *b_LepLoose3_jetPtRatio;   //!
   TBranch        *b_LepLoose3_jetBTagCSV;   //!
   TBranch        *b_LepLoose3_jetBTagTCHE;   //!
   TBranch        *b_LepLoose3_ptRelJet;   //!
   TBranch        *b_LepLoose3_jetDR;   //!
   TBranch        *b_LepLoose3_jetSelf;   //!
   TBranch        *b_LepLoose3_innerHits;   //!
   TBranch        *b_LepLoose3_mvaId;   //!
   TBranch        *b_LepLoose3_tightId;   //!
   TBranch        *b_LepLoose3_mva;   //!
   TBranch        *b_LepLoose3_tightCharge;   //!
   TBranch        *b_LepGood3_pt;   //!
   TBranch        *b_LepGood3_eta;   //!
   TBranch        *b_LepGood3_phi;   //!
   TBranch        *b_LepGood3_mass;   //!
   TBranch        *b_LepGood3_energy;   //!
   TBranch        *b_LepGood3_pdgId;   //!
   TBranch        *b_LepGood3_charge;   //!
   TBranch        *b_LepGood3_sip3d;   //!
   TBranch        *b_LepGood3_sip3ds;   //!
   TBranch        *b_LepGood3_ip3ds;   //!
   TBranch        *b_LepGood3_dxy;   //!
   TBranch        *b_LepGood3_dz;   //!
   TBranch        *b_LepGood3_relIso;   //!
   TBranch        *b_LepGood3_chargedIso;   //!
   TBranch        *b_LepGood3_drBJet;   //!
   TBranch        *b_LepGood3_jetPtRatio;   //!
   TBranch        *b_LepGood3_jetBTagCSV;   //!
   TBranch        *b_LepGood3_jetBTagTCHE;   //!
   TBranch        *b_LepGood3_ptRelJet;   //!
   TBranch        *b_LepGood3_jetDR;   //!
   TBranch        *b_LepGood3_jetSelf;   //!
   TBranch        *b_LepGood3_innerHits;   //!
   TBranch        *b_LepGood3_mvaId;   //!
   TBranch        *b_LepGood3_tightId;   //!
   TBranch        *b_LepGood3_mva;   //!
   TBranch        *b_LepGood3_tightCharge;   //!
   TBranch        *b_LepGood3_mcMatchId;   //!
   TBranch        *b_LepGood3_mcMatchAny;   //!
   TBranch        *b_LepGood3_mcDeltaRB;   //!
   TBranch        *b_LepLoose4_pt;   //!
   TBranch        *b_LepLoose4_eta;   //!
   TBranch        *b_LepLoose4_phi;   //!
   TBranch        *b_LepLoose4_mass;   //!
   TBranch        *b_LepLoose4_energy;   //!
   TBranch        *b_LepLoose4_pdgId;   //!
   TBranch        *b_LepLoose4_charge;   //!
   TBranch        *b_LepLoose4_sip3d;   //!
   TBranch        *b_LepLoose4_sip3ds;   //!
   TBranch        *b_LepLoose4_ip3ds;   //!
   TBranch        *b_LepLoose4_dxy;   //!
   TBranch        *b_LepLoose4_dz;   //!
   TBranch        *b_LepLoose4_relIso;   //!
   TBranch        *b_LepLoose4_chargedIso;   //!
   TBranch        *b_LepLoose4_drBJet;   //!
   TBranch        *b_LepLoose4_jetPtRatio;   //!
   TBranch        *b_LepLoose4_jetBTagCSV;   //!
   TBranch        *b_LepLoose4_jetBTagTCHE;   //!
   TBranch        *b_LepLoose4_ptRelJet;   //!
   TBranch        *b_LepLoose4_jetDR;   //!
   TBranch        *b_LepLoose4_jetSelf;   //!
   TBranch        *b_LepLoose4_innerHits;   //!
   TBranch        *b_LepLoose4_mvaId;   //!
   TBranch        *b_LepLoose4_tightId;   //!
   TBranch        *b_LepLoose4_mva;   //!
   TBranch        *b_LepLoose4_tightCharge;   //!
   TBranch        *b_LepGood4_pt;   //!
   TBranch        *b_LepGood4_eta;   //!
   TBranch        *b_LepGood4_phi;   //!
   TBranch        *b_LepGood4_mass;   //!
   TBranch        *b_LepGood4_energy;   //!
   TBranch        *b_LepGood4_pdgId;   //!
   TBranch        *b_LepGood4_charge;   //!
   TBranch        *b_LepGood4_sip3d;   //!
   TBranch        *b_LepGood4_sip3ds;   //!
   TBranch        *b_LepGood4_ip3ds;   //!
   TBranch        *b_LepGood4_dxy;   //!
   TBranch        *b_LepGood4_dz;   //!
   TBranch        *b_LepGood4_relIso;   //!
   TBranch        *b_LepGood4_chargedIso;   //!
   TBranch        *b_LepGood4_drBJet;   //!
   TBranch        *b_LepGood4_jetPtRatio;   //!
   TBranch        *b_LepGood4_jetBTagCSV;   //!
   TBranch        *b_LepGood4_jetBTagTCHE;   //!
   TBranch        *b_LepGood4_ptRelJet;   //!
   TBranch        *b_LepGood4_jetDR;   //!
   TBranch        *b_LepGood4_jetSelf;   //!
   TBranch        *b_LepGood4_innerHits;   //!
   TBranch        *b_LepGood4_mvaId;   //!
   TBranch        *b_LepGood4_tightId;   //!
   TBranch        *b_LepGood4_mva;   //!
   TBranch        *b_LepGood4_tightCharge;   //!
   TBranch        *b_LepGood4_mcMatchId;   //!
   TBranch        *b_LepGood4_mcMatchAny;   //!
   TBranch        *b_LepGood4_mcDeltaRB;   //!
   TBranch        *b_LepLoose5_pt;   //!
   TBranch        *b_LepLoose5_eta;   //!
   TBranch        *b_LepLoose5_phi;   //!
   TBranch        *b_LepLoose5_mass;   //!
   TBranch        *b_LepLoose5_energy;   //!
   TBranch        *b_LepLoose5_pdgId;   //!
   TBranch        *b_LepLoose5_charge;   //!
   TBranch        *b_LepLoose5_sip3d;   //!
   TBranch        *b_LepLoose5_sip3ds;   //!
   TBranch        *b_LepLoose5_ip3ds;   //!
   TBranch        *b_LepLoose5_dxy;   //!
   TBranch        *b_LepLoose5_dz;   //!
   TBranch        *b_LepLoose5_relIso;   //!
   TBranch        *b_LepLoose5_chargedIso;   //!
   TBranch        *b_LepLoose5_drBJet;   //!
   TBranch        *b_LepLoose5_jetPtRatio;   //!
   TBranch        *b_LepLoose5_jetBTagCSV;   //!
   TBranch        *b_LepLoose5_jetBTagTCHE;   //!
   TBranch        *b_LepLoose5_ptRelJet;   //!
   TBranch        *b_LepLoose5_jetDR;   //!
   TBranch        *b_LepLoose5_jetSelf;   //!
   TBranch        *b_LepLoose5_innerHits;   //!
   TBranch        *b_LepLoose5_mvaId;   //!
   TBranch        *b_LepLoose5_tightId;   //!
   TBranch        *b_LepLoose5_mva;   //!
   TBranch        *b_LepLoose5_tightCharge;   //!
   TBranch        *b_LepGood5_pt;   //!
   TBranch        *b_LepGood5_eta;   //!
   TBranch        *b_LepGood5_phi;   //!
   TBranch        *b_LepGood5_mass;   //!
   TBranch        *b_LepGood5_energy;   //!
   TBranch        *b_LepGood5_pdgId;   //!
   TBranch        *b_LepGood5_charge;   //!
   TBranch        *b_LepGood5_sip3d;   //!
   TBranch        *b_LepGood5_sip3ds;   //!
   TBranch        *b_LepGood5_ip3ds;   //!
   TBranch        *b_LepGood5_dxy;   //!
   TBranch        *b_LepGood5_dz;   //!
   TBranch        *b_LepGood5_relIso;   //!
   TBranch        *b_LepGood5_chargedIso;   //!
   TBranch        *b_LepGood5_drBJet;   //!
   TBranch        *b_LepGood5_jetPtRatio;   //!
   TBranch        *b_LepGood5_jetBTagCSV;   //!
   TBranch        *b_LepGood5_jetBTagTCHE;   //!
   TBranch        *b_LepGood5_ptRelJet;   //!
   TBranch        *b_LepGood5_jetDR;   //!
   TBranch        *b_LepGood5_jetSelf;   //!
   TBranch        *b_LepGood5_innerHits;   //!
   TBranch        *b_LepGood5_mvaId;   //!
   TBranch        *b_LepGood5_tightId;   //!
   TBranch        *b_LepGood5_mva;   //!
   TBranch        *b_LepGood5_tightCharge;   //!
   TBranch        *b_LepGood5_mcMatchId;   //!
   TBranch        *b_LepGood5_mcMatchAny;   //!
   TBranch        *b_LepGood5_mcDeltaRB;   //!
   TBranch        *b_LepLoose6_pt;   //!
   TBranch        *b_LepLoose6_eta;   //!
   TBranch        *b_LepLoose6_phi;   //!
   TBranch        *b_LepLoose6_mass;   //!
   TBranch        *b_LepLoose6_energy;   //!
   TBranch        *b_LepLoose6_pdgId;   //!
   TBranch        *b_LepLoose6_charge;   //!
   TBranch        *b_LepLoose6_sip3d;   //!
   TBranch        *b_LepLoose6_sip3ds;   //!
   TBranch        *b_LepLoose6_ip3ds;   //!
   TBranch        *b_LepLoose6_dxy;   //!
   TBranch        *b_LepLoose6_dz;   //!
   TBranch        *b_LepLoose6_relIso;   //!
   TBranch        *b_LepLoose6_chargedIso;   //!
   TBranch        *b_LepLoose6_drBJet;   //!
   TBranch        *b_LepLoose6_jetPtRatio;   //!
   TBranch        *b_LepLoose6_jetBTagCSV;   //!
   TBranch        *b_LepLoose6_jetBTagTCHE;   //!
   TBranch        *b_LepLoose6_ptRelJet;   //!
   TBranch        *b_LepLoose6_jetDR;   //!
   TBranch        *b_LepLoose6_jetSelf;   //!
   TBranch        *b_LepLoose6_innerHits;   //!
   TBranch        *b_LepLoose6_mvaId;   //!
   TBranch        *b_LepLoose6_tightId;   //!
   TBranch        *b_LepLoose6_mva;   //!
   TBranch        *b_LepLoose6_tightCharge;   //!
   TBranch        *b_LepGood6_pt;   //!
   TBranch        *b_LepGood6_eta;   //!
   TBranch        *b_LepGood6_phi;   //!
   TBranch        *b_LepGood6_mass;   //!
   TBranch        *b_LepGood6_energy;   //!
   TBranch        *b_LepGood6_pdgId;   //!
   TBranch        *b_LepGood6_charge;   //!
   TBranch        *b_LepGood6_sip3d;   //!
   TBranch        *b_LepGood6_sip3ds;   //!
   TBranch        *b_LepGood6_ip3ds;   //!
   TBranch        *b_LepGood6_dxy;   //!
   TBranch        *b_LepGood6_dz;   //!
   TBranch        *b_LepGood6_relIso;   //!
   TBranch        *b_LepGood6_chargedIso;   //!
   TBranch        *b_LepGood6_drBJet;   //!
   TBranch        *b_LepGood6_jetPtRatio;   //!
   TBranch        *b_LepGood6_jetBTagCSV;   //!
   TBranch        *b_LepGood6_jetBTagTCHE;   //!
   TBranch        *b_LepGood6_ptRelJet;   //!
   TBranch        *b_LepGood6_jetDR;   //!
   TBranch        *b_LepGood6_jetSelf;   //!
   TBranch        *b_LepGood6_innerHits;   //!
   TBranch        *b_LepGood6_mvaId;   //!
   TBranch        *b_LepGood6_tightId;   //!
   TBranch        *b_LepGood6_mva;   //!
   TBranch        *b_LepGood6_tightCharge;   //!
   TBranch        *b_LepGood6_mcMatchId;   //!
   TBranch        *b_LepGood6_mcMatchAny;   //!
   TBranch        *b_LepGood6_mcDeltaRB;   //!
   TBranch        *b_LepLoose7_pt;   //!
   TBranch        *b_LepLoose7_eta;   //!
   TBranch        *b_LepLoose7_phi;   //!
   TBranch        *b_LepLoose7_mass;   //!
   TBranch        *b_LepLoose7_energy;   //!
   TBranch        *b_LepLoose7_pdgId;   //!
   TBranch        *b_LepLoose7_charge;   //!
   TBranch        *b_LepLoose7_sip3d;   //!
   TBranch        *b_LepLoose7_sip3ds;   //!
   TBranch        *b_LepLoose7_ip3ds;   //!
   TBranch        *b_LepLoose7_dxy;   //!
   TBranch        *b_LepLoose7_dz;   //!
   TBranch        *b_LepLoose7_relIso;   //!
   TBranch        *b_LepLoose7_chargedIso;   //!
   TBranch        *b_LepLoose7_drBJet;   //!
   TBranch        *b_LepLoose7_jetPtRatio;   //!
   TBranch        *b_LepLoose7_jetBTagCSV;   //!
   TBranch        *b_LepLoose7_jetBTagTCHE;   //!
   TBranch        *b_LepLoose7_ptRelJet;   //!
   TBranch        *b_LepLoose7_jetDR;   //!
   TBranch        *b_LepLoose7_jetSelf;   //!
   TBranch        *b_LepLoose7_innerHits;   //!
   TBranch        *b_LepLoose7_mvaId;   //!
   TBranch        *b_LepLoose7_tightId;   //!
   TBranch        *b_LepLoose7_mva;   //!
   TBranch        *b_LepLoose7_tightCharge;   //!
   TBranch        *b_LepGood7_pt;   //!
   TBranch        *b_LepGood7_eta;   //!
   TBranch        *b_LepGood7_phi;   //!
   TBranch        *b_LepGood7_mass;   //!
   TBranch        *b_LepGood7_energy;   //!
   TBranch        *b_LepGood7_pdgId;   //!
   TBranch        *b_LepGood7_charge;   //!
   TBranch        *b_LepGood7_sip3d;   //!
   TBranch        *b_LepGood7_sip3ds;   //!
   TBranch        *b_LepGood7_ip3ds;   //!
   TBranch        *b_LepGood7_dxy;   //!
   TBranch        *b_LepGood7_dz;   //!
   TBranch        *b_LepGood7_relIso;   //!
   TBranch        *b_LepGood7_chargedIso;   //!
   TBranch        *b_LepGood7_drBJet;   //!
   TBranch        *b_LepGood7_jetPtRatio;   //!
   TBranch        *b_LepGood7_jetBTagCSV;   //!
   TBranch        *b_LepGood7_jetBTagTCHE;   //!
   TBranch        *b_LepGood7_ptRelJet;   //!
   TBranch        *b_LepGood7_jetDR;   //!
   TBranch        *b_LepGood7_jetSelf;   //!
   TBranch        *b_LepGood7_innerHits;   //!
   TBranch        *b_LepGood7_mvaId;   //!
   TBranch        *b_LepGood7_tightId;   //!
   TBranch        *b_LepGood7_mva;   //!
   TBranch        *b_LepGood7_tightCharge;   //!
   TBranch        *b_LepGood7_mcMatchId;   //!
   TBranch        *b_LepGood7_mcMatchAny;   //!
   TBranch        *b_LepGood7_mcDeltaRB;   //!
   TBranch        *b_LepLoose8_pt;   //!
   TBranch        *b_LepLoose8_eta;   //!
   TBranch        *b_LepLoose8_phi;   //!
   TBranch        *b_LepLoose8_mass;   //!
   TBranch        *b_LepLoose8_energy;   //!
   TBranch        *b_LepLoose8_pdgId;   //!
   TBranch        *b_LepLoose8_charge;   //!
   TBranch        *b_LepLoose8_sip3d;   //!
   TBranch        *b_LepLoose8_sip3ds;   //!
   TBranch        *b_LepLoose8_ip3ds;   //!
   TBranch        *b_LepLoose8_dxy;   //!
   TBranch        *b_LepLoose8_dz;   //!
   TBranch        *b_LepLoose8_relIso;   //!
   TBranch        *b_LepLoose8_chargedIso;   //!
   TBranch        *b_LepLoose8_drBJet;   //!
   TBranch        *b_LepLoose8_jetPtRatio;   //!
   TBranch        *b_LepLoose8_jetBTagCSV;   //!
   TBranch        *b_LepLoose8_jetBTagTCHE;   //!
   TBranch        *b_LepLoose8_ptRelJet;   //!
   TBranch        *b_LepLoose8_jetDR;   //!
   TBranch        *b_LepLoose8_jetSelf;   //!
   TBranch        *b_LepLoose8_innerHits;   //!
   TBranch        *b_LepLoose8_mvaId;   //!
   TBranch        *b_LepLoose8_tightId;   //!
   TBranch        *b_LepLoose8_mva;   //!
   TBranch        *b_LepLoose8_tightCharge;   //!
   TBranch        *b_LepGood8_pt;   //!
   TBranch        *b_LepGood8_eta;   //!
   TBranch        *b_LepGood8_phi;   //!
   TBranch        *b_LepGood8_mass;   //!
   TBranch        *b_LepGood8_energy;   //!
   TBranch        *b_LepGood8_pdgId;   //!
   TBranch        *b_LepGood8_charge;   //!
   TBranch        *b_LepGood8_sip3d;   //!
   TBranch        *b_LepGood8_sip3ds;   //!
   TBranch        *b_LepGood8_ip3ds;   //!
   TBranch        *b_LepGood8_dxy;   //!
   TBranch        *b_LepGood8_dz;   //!
   TBranch        *b_LepGood8_relIso;   //!
   TBranch        *b_LepGood8_chargedIso;   //!
   TBranch        *b_LepGood8_drBJet;   //!
   TBranch        *b_LepGood8_jetPtRatio;   //!
   TBranch        *b_LepGood8_jetBTagCSV;   //!
   TBranch        *b_LepGood8_jetBTagTCHE;   //!
   TBranch        *b_LepGood8_ptRelJet;   //!
   TBranch        *b_LepGood8_jetDR;   //!
   TBranch        *b_LepGood8_jetSelf;   //!
   TBranch        *b_LepGood8_innerHits;   //!
   TBranch        *b_LepGood8_mvaId;   //!
   TBranch        *b_LepGood8_tightId;   //!
   TBranch        *b_LepGood8_mva;   //!
   TBranch        *b_LepGood8_tightCharge;   //!
   TBranch        *b_LepGood8_mcMatchId;   //!
   TBranch        *b_LepGood8_mcMatchAny;   //!
   TBranch        *b_LepGood8_mcDeltaRB;   //!
   TBranch        *b_Jet1_pt;   //!
   TBranch        *b_Jet1_eta;   //!
   TBranch        *b_Jet1_phi;   //!
   TBranch        *b_Jet1_mass;   //!
   TBranch        *b_Jet1_energy;   //!
   TBranch        *b_Jet1_btagCSV;   //!
   TBranch        *b_Jet1_leg_pt;   //!
   TBranch        *b_Jet1_leg_eta;   //!
   TBranch        *b_Jet1_leg_phi;   //!
   TBranch        *b_Jet1_leg_mass;   //!
   TBranch        *b_Jet1_leg_energy;   //!
   TBranch        *b_Jet1_mcMatchId;   //!
   TBranch        *b_Jet1_mcMatchFlav;   //!
   TBranch        *b_Jet2_pt;   //!
   TBranch        *b_Jet2_eta;   //!
   TBranch        *b_Jet2_phi;   //!
   TBranch        *b_Jet2_mass;   //!
   TBranch        *b_Jet2_energy;   //!
   TBranch        *b_Jet2_btagCSV;   //!
   TBranch        *b_Jet2_leg_pt;   //!
   TBranch        *b_Jet2_leg_eta;   //!
   TBranch        *b_Jet2_leg_phi;   //!
   TBranch        *b_Jet2_leg_mass;   //!
   TBranch        *b_Jet2_leg_energy;   //!
   TBranch        *b_Jet2_mcMatchId;   //!
   TBranch        *b_Jet2_mcMatchFlav;   //!
   TBranch        *b_Jet3_pt;   //!
   TBranch        *b_Jet3_eta;   //!
   TBranch        *b_Jet3_phi;   //!
   TBranch        *b_Jet3_mass;   //!
   TBranch        *b_Jet3_energy;   //!
   TBranch        *b_Jet3_btagCSV;   //!
   TBranch        *b_Jet3_leg_pt;   //!
   TBranch        *b_Jet3_leg_eta;   //!
   TBranch        *b_Jet3_leg_phi;   //!
   TBranch        *b_Jet3_leg_mass;   //!
   TBranch        *b_Jet3_leg_energy;   //!
   TBranch        *b_Jet3_mcMatchId;   //!
   TBranch        *b_Jet3_mcMatchFlav;   //!
   TBranch        *b_Jet4_pt;   //!
   TBranch        *b_Jet4_eta;   //!
   TBranch        *b_Jet4_phi;   //!
   TBranch        *b_Jet4_mass;   //!
   TBranch        *b_Jet4_energy;   //!
   TBranch        *b_Jet4_btagCSV;   //!
   TBranch        *b_Jet4_leg_pt;   //!
   TBranch        *b_Jet4_leg_eta;   //!
   TBranch        *b_Jet4_leg_phi;   //!
   TBranch        *b_Jet4_leg_mass;   //!
   TBranch        *b_Jet4_leg_energy;   //!
   TBranch        *b_Jet4_mcMatchId;   //!
   TBranch        *b_Jet4_mcMatchFlav;   //!
   TBranch        *b_Jet5_pt;   //!
   TBranch        *b_Jet5_eta;   //!
   TBranch        *b_Jet5_phi;   //!
   TBranch        *b_Jet5_mass;   //!
   TBranch        *b_Jet5_energy;   //!
   TBranch        *b_Jet5_btagCSV;   //!
   TBranch        *b_Jet5_leg_pt;   //!
   TBranch        *b_Jet5_leg_eta;   //!
   TBranch        *b_Jet5_leg_phi;   //!
   TBranch        *b_Jet5_leg_mass;   //!
   TBranch        *b_Jet5_leg_energy;   //!
   TBranch        *b_Jet5_mcMatchId;   //!
   TBranch        *b_Jet5_mcMatchFlav;   //!
   TBranch        *b_Jet6_pt;   //!
   TBranch        *b_Jet6_eta;   //!
   TBranch        *b_Jet6_phi;   //!
   TBranch        *b_Jet6_mass;   //!
   TBranch        *b_Jet6_energy;   //!
   TBranch        *b_Jet6_btagCSV;   //!
   TBranch        *b_Jet6_leg_pt;   //!
   TBranch        *b_Jet6_leg_eta;   //!
   TBranch        *b_Jet6_leg_phi;   //!
   TBranch        *b_Jet6_leg_mass;   //!
   TBranch        *b_Jet6_leg_energy;   //!
   TBranch        *b_Jet6_mcMatchId;   //!
   TBranch        *b_Jet6_mcMatchFlav;   //!
   TBranch        *b_Jet7_pt;   //!
   TBranch        *b_Jet7_eta;   //!
   TBranch        *b_Jet7_phi;   //!
   TBranch        *b_Jet7_mass;   //!
   TBranch        *b_Jet7_energy;   //!
   TBranch        *b_Jet7_btagCSV;   //!
   TBranch        *b_Jet7_leg_pt;   //!
   TBranch        *b_Jet7_leg_eta;   //!
   TBranch        *b_Jet7_leg_phi;   //!
   TBranch        *b_Jet7_leg_mass;   //!
   TBranch        *b_Jet7_leg_energy;   //!
   TBranch        *b_Jet7_mcMatchId;   //!
   TBranch        *b_Jet7_mcMatchFlav;   //!
   TBranch        *b_Jet8_pt;   //!
   TBranch        *b_Jet8_eta;   //!
   TBranch        *b_Jet8_phi;   //!
   TBranch        *b_Jet8_mass;   //!
   TBranch        *b_Jet8_energy;   //!
   TBranch        *b_Jet8_btagCSV;   //!
   TBranch        *b_Jet8_leg_pt;   //!
   TBranch        *b_Jet8_leg_eta;   //!
   TBranch        *b_Jet8_leg_phi;   //!
   TBranch        *b_Jet8_leg_mass;   //!
   TBranch        *b_Jet8_leg_energy;   //!
   TBranch        *b_Jet8_mcMatchId;   //!
   TBranch        *b_Jet8_mcMatchFlav;   //!
   TBranch        *b_nBJetLoose25;   //!
   TBranch        *b_nBJetMedium25;   //!
   TBranch        *b_nBJetLoose30;   //!
   TBranch        *b_nBJetMedium30;   //!
   TBranch        *b_met;   //!
   TBranch        *b_metSignificance;   //!
   TBranch        *b_projMetAll1S;   //!
   TBranch        *b_projMetAll2S;   //!
   TBranch        *b_projMetJet1S;   //!
   TBranch        *b_projMetJet2S;   //!
   TBranch        *b_htJet30;   //!
   TBranch        *b_htJet25;   //!
   TBranch        *b_mhtJet30;   //!
   TBranch        *b_mhtJet25;   //!
   TBranch        *b_mZ1;   //!
   TBranch        *b_mZ2;   //!
   TBranch        *b_minMllSFOS;   //!
   TBranch        *b_minMllAFOS;   //!
   TBranch        *b_minMllAFAS;   //!
   TBranch        *b_GenHiggsDecayMode;   //!
   TBranch        *b_GenHiggs_pt;   //!
   TBranch        *b_GenHiggs_eta;   //!
   TBranch        *b_GenHiggs_phi;   //!
   TBranch        *b_GenHiggs_mass;   //!
   TBranch        *b_GenHiggs_energy;   //!
   TBranch        *b_GenHiggs_pdgId;   //!
   TBranch        *b_GenHiggs_charge;   //!
   TBranch        *b_GenTop1_pt;   //!
   TBranch        *b_GenTop1_eta;   //!
   TBranch        *b_GenTop1_phi;   //!
   TBranch        *b_GenTop1_mass;   //!
   TBranch        *b_GenTop1_energy;   //!
   TBranch        *b_GenTop1_pdgId;   //!
   TBranch        *b_GenTop1_charge;   //!
   TBranch        *b_GenTop2_pt;   //!
   TBranch        *b_GenTop2_eta;   //!
   TBranch        *b_GenTop2_phi;   //!
   TBranch        *b_GenTop2_mass;   //!
   TBranch        *b_GenTop2_energy;   //!
   TBranch        *b_GenTop2_pdgId;   //!
   TBranch        *b_GenTop2_charge;   //!
   TBranch        *b_nGenLeps;   //!
   TBranch        *b_nGenLepsFromTau;   //!
   TBranch        *b_nGenQuarks;   //!
   TBranch        *b_nGenBQuarks;   //!
   TBranch        *b_GenLep1_pt;   //!
   TBranch        *b_GenLep1_eta;   //!
   TBranch        *b_GenLep1_phi;   //!
   TBranch        *b_GenLep1_mass;   //!
   TBranch        *b_GenLep1_energy;   //!
   TBranch        *b_GenLep1_pdgId;   //!
   TBranch        *b_GenLep1_charge;   //!
   TBranch        *b_GenLep1_sourceId;   //!
   TBranch        *b_GenLepFromTau1_pt;   //!
   TBranch        *b_GenLepFromTau1_eta;   //!
   TBranch        *b_GenLepFromTau1_phi;   //!
   TBranch        *b_GenLepFromTau1_mass;   //!
   TBranch        *b_GenLepFromTau1_energy;   //!
   TBranch        *b_GenLepFromTau1_pdgId;   //!
   TBranch        *b_GenLepFromTau1_charge;   //!
   TBranch        *b_GenLepFromTau1_sourceId;   //!
   TBranch        *b_GenQuark1_pt;   //!
   TBranch        *b_GenQuark1_eta;   //!
   TBranch        *b_GenQuark1_phi;   //!
   TBranch        *b_GenQuark1_mass;   //!
   TBranch        *b_GenQuark1_energy;   //!
   TBranch        *b_GenQuark1_pdgId;   //!
   TBranch        *b_GenQuark1_charge;   //!
   TBranch        *b_GenQuark1_sourceId;   //!
   TBranch        *b_GenLep2_pt;   //!
   TBranch        *b_GenLep2_eta;   //!
   TBranch        *b_GenLep2_phi;   //!
   TBranch        *b_GenLep2_mass;   //!
   TBranch        *b_GenLep2_energy;   //!
   TBranch        *b_GenLep2_pdgId;   //!
   TBranch        *b_GenLep2_charge;   //!
   TBranch        *b_GenLep2_sourceId;   //!
   TBranch        *b_GenLepFromTau2_pt;   //!
   TBranch        *b_GenLepFromTau2_eta;   //!
   TBranch        *b_GenLepFromTau2_phi;   //!
   TBranch        *b_GenLepFromTau2_mass;   //!
   TBranch        *b_GenLepFromTau2_energy;   //!
   TBranch        *b_GenLepFromTau2_pdgId;   //!
   TBranch        *b_GenLepFromTau2_charge;   //!
   TBranch        *b_GenLepFromTau2_sourceId;   //!
   TBranch        *b_GenQuark2_pt;   //!
   TBranch        *b_GenQuark2_eta;   //!
   TBranch        *b_GenQuark2_phi;   //!
   TBranch        *b_GenQuark2_mass;   //!
   TBranch        *b_GenQuark2_energy;   //!
   TBranch        *b_GenQuark2_pdgId;   //!
   TBranch        *b_GenQuark2_charge;   //!
   TBranch        *b_GenQuark2_sourceId;   //!
   TBranch        *b_GenLep3_pt;   //!
   TBranch        *b_GenLep3_eta;   //!
   TBranch        *b_GenLep3_phi;   //!
   TBranch        *b_GenLep3_mass;   //!
   TBranch        *b_GenLep3_energy;   //!
   TBranch        *b_GenLep3_pdgId;   //!
   TBranch        *b_GenLep3_charge;   //!
   TBranch        *b_GenLep3_sourceId;   //!
   TBranch        *b_GenLepFromTau3_pt;   //!
   TBranch        *b_GenLepFromTau3_eta;   //!
   TBranch        *b_GenLepFromTau3_phi;   //!
   TBranch        *b_GenLepFromTau3_mass;   //!
   TBranch        *b_GenLepFromTau3_energy;   //!
   TBranch        *b_GenLepFromTau3_pdgId;   //!
   TBranch        *b_GenLepFromTau3_charge;   //!
   TBranch        *b_GenLepFromTau3_sourceId;   //!
   TBranch        *b_GenQuark3_pt;   //!
   TBranch        *b_GenQuark3_eta;   //!
   TBranch        *b_GenQuark3_phi;   //!
   TBranch        *b_GenQuark3_mass;   //!
   TBranch        *b_GenQuark3_energy;   //!
   TBranch        *b_GenQuark3_pdgId;   //!
   TBranch        *b_GenQuark3_charge;   //!
   TBranch        *b_GenQuark3_sourceId;   //!
   TBranch        *b_GenLep4_pt;   //!
   TBranch        *b_GenLep4_eta;   //!
   TBranch        *b_GenLep4_phi;   //!
   TBranch        *b_GenLep4_mass;   //!
   TBranch        *b_GenLep4_energy;   //!
   TBranch        *b_GenLep4_pdgId;   //!
   TBranch        *b_GenLep4_charge;   //!
   TBranch        *b_GenLep4_sourceId;   //!
   TBranch        *b_GenLepFromTau4_pt;   //!
   TBranch        *b_GenLepFromTau4_eta;   //!
   TBranch        *b_GenLepFromTau4_phi;   //!
   TBranch        *b_GenLepFromTau4_mass;   //!
   TBranch        *b_GenLepFromTau4_energy;   //!
   TBranch        *b_GenLepFromTau4_pdgId;   //!
   TBranch        *b_GenLepFromTau4_charge;   //!
   TBranch        *b_GenLepFromTau4_sourceId;   //!
   TBranch        *b_GenQuark4_pt;   //!
   TBranch        *b_GenQuark4_eta;   //!
   TBranch        *b_GenQuark4_phi;   //!
   TBranch        *b_GenQuark4_mass;   //!
   TBranch        *b_GenQuark4_energy;   //!
   TBranch        *b_GenQuark4_pdgId;   //!
   TBranch        *b_GenQuark4_charge;   //!
   TBranch        *b_GenQuark4_sourceId;   //!
   TBranch        *b_GenLep5_pt;   //!
   TBranch        *b_GenLep5_eta;   //!
   TBranch        *b_GenLep5_phi;   //!
   TBranch        *b_GenLep5_mass;   //!
   TBranch        *b_GenLep5_energy;   //!
   TBranch        *b_GenLep5_pdgId;   //!
   TBranch        *b_GenLep5_charge;   //!
   TBranch        *b_GenLep5_sourceId;   //!
   TBranch        *b_GenLepFromTau5_pt;   //!
   TBranch        *b_GenLepFromTau5_eta;   //!
   TBranch        *b_GenLepFromTau5_phi;   //!
   TBranch        *b_GenLepFromTau5_mass;   //!
   TBranch        *b_GenLepFromTau5_energy;   //!
   TBranch        *b_GenLepFromTau5_pdgId;   //!
   TBranch        *b_GenLepFromTau5_charge;   //!
   TBranch        *b_GenLepFromTau5_sourceId;   //!
   TBranch        *b_GenQuark5_pt;   //!
   TBranch        *b_GenQuark5_eta;   //!
   TBranch        *b_GenQuark5_phi;   //!
   TBranch        *b_GenQuark5_mass;   //!
   TBranch        *b_GenQuark5_energy;   //!
   TBranch        *b_GenQuark5_pdgId;   //!
   TBranch        *b_GenQuark5_charge;   //!
   TBranch        *b_GenQuark5_sourceId;   //!
   TBranch        *b_GenLep6_pt;   //!
   TBranch        *b_GenLep6_eta;   //!
   TBranch        *b_GenLep6_phi;   //!
   TBranch        *b_GenLep6_mass;   //!
   TBranch        *b_GenLep6_energy;   //!
   TBranch        *b_GenLep6_pdgId;   //!
   TBranch        *b_GenLep6_charge;   //!
   TBranch        *b_GenLep6_sourceId;   //!
   TBranch        *b_GenLepFromTau6_pt;   //!
   TBranch        *b_GenLepFromTau6_eta;   //!
   TBranch        *b_GenLepFromTau6_phi;   //!
   TBranch        *b_GenLepFromTau6_mass;   //!
   TBranch        *b_GenLepFromTau6_energy;   //!
   TBranch        *b_GenLepFromTau6_pdgId;   //!
   TBranch        *b_GenLepFromTau6_charge;   //!
   TBranch        *b_GenLepFromTau6_sourceId;   //!
   TBranch        *b_GenQuark6_pt;   //!
   TBranch        *b_GenQuark6_eta;   //!
   TBranch        *b_GenQuark6_phi;   //!
   TBranch        *b_GenQuark6_mass;   //!
   TBranch        *b_GenQuark6_energy;   //!
   TBranch        *b_GenQuark6_pdgId;   //!
   TBranch        *b_GenQuark6_charge;   //!
   TBranch        *b_GenQuark6_sourceId;   //!
   TBranch        *b_GenBQuark1_pt;   //!
   TBranch        *b_GenBQuark1_eta;   //!
   TBranch        *b_GenBQuark1_phi;   //!
   TBranch        *b_GenBQuark1_mass;   //!
   TBranch        *b_GenBQuark1_energy;   //!
   TBranch        *b_GenBQuark1_pdgId;   //!
   TBranch        *b_GenBQuark1_charge;   //!
   TBranch        *b_GenBQuark2_pt;   //!
   TBranch        *b_GenBQuark2_eta;   //!
   TBranch        *b_GenBQuark2_phi;   //!
   TBranch        *b_GenBQuark2_mass;   //!
   TBranch        *b_GenBQuark2_energy;   //!
   TBranch        *b_GenBQuark2_pdgId;   //!
   TBranch        *b_GenBQuark2_charge;   //!
   TBranch        *b_nGoodLepsMatchId;   //!
   TBranch        *b_nGoodLepsMatchAny;   //!

   TTHTree(TTree *tree=0);
   virtual ~TTHTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef TTHTree_cxx
TTHTree::TTHTree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../../../cfg/TREES_171212/TTH/ttHLepTreeProducerBase/ttHLepTreeProducerBase_tree.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("../../../cfg/TREES_171212/TTH/ttHLepTreeProducerBase/ttHLepTreeProducerBase_tree.root");
      }
      f->GetObject("ttHLepTreeProducerBase",tree);

   }
   Init(tree);
}

TTHTree::~TTHTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t TTHTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t TTHTree::LoadTree(Long64_t entry)
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

void TTHTree::Init(TTree *tree)
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

   fChain->SetBranchAddress("nVert", &nVert, &b_nVert);
   fChain->SetBranchAddress("nLepLoose", &nLepLoose, &b_nLepLoose);
   fChain->SetBranchAddress("nLepGood", &nLepGood, &b_nLepGood);
   fChain->SetBranchAddress("nJet25", &nJet25, &b_nJet25);
   fChain->SetBranchAddress("nJet30", &nJet30, &b_nJet30);
   fChain->SetBranchAddress("LepLoose1_pt", &LepLoose1_pt, &b_LepLoose1_pt);
   fChain->SetBranchAddress("LepLoose1_eta", &LepLoose1_eta, &b_LepLoose1_eta);
   fChain->SetBranchAddress("LepLoose1_phi", &LepLoose1_phi, &b_LepLoose1_phi);
   fChain->SetBranchAddress("LepLoose1_mass", &LepLoose1_mass, &b_LepLoose1_mass);
   fChain->SetBranchAddress("LepLoose1_energy", &LepLoose1_energy, &b_LepLoose1_energy);
   fChain->SetBranchAddress("LepLoose1_pdgId", &LepLoose1_pdgId, &b_LepLoose1_pdgId);
   fChain->SetBranchAddress("LepLoose1_charge", &LepLoose1_charge, &b_LepLoose1_charge);
   fChain->SetBranchAddress("LepLoose1_sip3d", &LepLoose1_sip3d, &b_LepLoose1_sip3d);
   fChain->SetBranchAddress("LepLoose1_sip3ds", &LepLoose1_sip3ds, &b_LepLoose1_sip3ds);
   fChain->SetBranchAddress("LepLoose1_ip3ds", &LepLoose1_ip3ds, &b_LepLoose1_ip3ds);
   fChain->SetBranchAddress("LepLoose1_dxy", &LepLoose1_dxy, &b_LepLoose1_dxy);
   fChain->SetBranchAddress("LepLoose1_dz", &LepLoose1_dz, &b_LepLoose1_dz);
   fChain->SetBranchAddress("LepLoose1_relIso", &LepLoose1_relIso, &b_LepLoose1_relIso);
   fChain->SetBranchAddress("LepLoose1_chargedIso", &LepLoose1_chargedIso, &b_LepLoose1_chargedIso);
   fChain->SetBranchAddress("LepLoose1_drBJet", &LepLoose1_drBJet, &b_LepLoose1_drBJet);
   fChain->SetBranchAddress("LepLoose1_jetPtRatio", &LepLoose1_jetPtRatio, &b_LepLoose1_jetPtRatio);
   fChain->SetBranchAddress("LepLoose1_jetBTagCSV", &LepLoose1_jetBTagCSV, &b_LepLoose1_jetBTagCSV);
   fChain->SetBranchAddress("LepLoose1_jetBTagTCHE", &LepLoose1_jetBTagTCHE, &b_LepLoose1_jetBTagTCHE);
   fChain->SetBranchAddress("LepLoose1_ptRelJet", &LepLoose1_ptRelJet, &b_LepLoose1_ptRelJet);
   fChain->SetBranchAddress("LepLoose1_jetDR", &LepLoose1_jetDR, &b_LepLoose1_jetDR);
   fChain->SetBranchAddress("LepLoose1_jetSelf", &LepLoose1_jetSelf, &b_LepLoose1_jetSelf);
   fChain->SetBranchAddress("LepLoose1_innerHits", &LepLoose1_innerHits, &b_LepLoose1_innerHits);
   fChain->SetBranchAddress("LepLoose1_mvaId", &LepLoose1_mvaId, &b_LepLoose1_mvaId);
   fChain->SetBranchAddress("LepLoose1_tightId", &LepLoose1_tightId, &b_LepLoose1_tightId);
   fChain->SetBranchAddress("LepLoose1_mva", &LepLoose1_mva, &b_LepLoose1_mva);
   fChain->SetBranchAddress("LepLoose1_tightCharge", &LepLoose1_tightCharge, &b_LepLoose1_tightCharge);
   fChain->SetBranchAddress("LepGood1_pt", &LepGood1_pt, &b_LepGood1_pt);
   fChain->SetBranchAddress("LepGood1_eta", &LepGood1_eta, &b_LepGood1_eta);
   fChain->SetBranchAddress("LepGood1_phi", &LepGood1_phi, &b_LepGood1_phi);
   fChain->SetBranchAddress("LepGood1_mass", &LepGood1_mass, &b_LepGood1_mass);
   fChain->SetBranchAddress("LepGood1_energy", &LepGood1_energy, &b_LepGood1_energy);
   fChain->SetBranchAddress("LepGood1_pdgId", &LepGood1_pdgId, &b_LepGood1_pdgId);
   fChain->SetBranchAddress("LepGood1_charge", &LepGood1_charge, &b_LepGood1_charge);
   fChain->SetBranchAddress("LepGood1_sip3d", &LepGood1_sip3d, &b_LepGood1_sip3d);
   fChain->SetBranchAddress("LepGood1_sip3ds", &LepGood1_sip3ds, &b_LepGood1_sip3ds);
   fChain->SetBranchAddress("LepGood1_ip3ds", &LepGood1_ip3ds, &b_LepGood1_ip3ds);
   fChain->SetBranchAddress("LepGood1_dxy", &LepGood1_dxy, &b_LepGood1_dxy);
   fChain->SetBranchAddress("LepGood1_dz", &LepGood1_dz, &b_LepGood1_dz);
   fChain->SetBranchAddress("LepGood1_relIso", &LepGood1_relIso, &b_LepGood1_relIso);
   fChain->SetBranchAddress("LepGood1_chargedIso", &LepGood1_chargedIso, &b_LepGood1_chargedIso);
   fChain->SetBranchAddress("LepGood1_drBJet", &LepGood1_drBJet, &b_LepGood1_drBJet);
   fChain->SetBranchAddress("LepGood1_jetPtRatio", &LepGood1_jetPtRatio, &b_LepGood1_jetPtRatio);
   fChain->SetBranchAddress("LepGood1_jetBTagCSV", &LepGood1_jetBTagCSV, &b_LepGood1_jetBTagCSV);
   fChain->SetBranchAddress("LepGood1_jetBTagTCHE", &LepGood1_jetBTagTCHE, &b_LepGood1_jetBTagTCHE);
   fChain->SetBranchAddress("LepGood1_ptRelJet", &LepGood1_ptRelJet, &b_LepGood1_ptRelJet);
   fChain->SetBranchAddress("LepGood1_jetDR", &LepGood1_jetDR, &b_LepGood1_jetDR);
   fChain->SetBranchAddress("LepGood1_jetSelf", &LepGood1_jetSelf, &b_LepGood1_jetSelf);
   fChain->SetBranchAddress("LepGood1_innerHits", &LepGood1_innerHits, &b_LepGood1_innerHits);
   fChain->SetBranchAddress("LepGood1_mvaId", &LepGood1_mvaId, &b_LepGood1_mvaId);
   fChain->SetBranchAddress("LepGood1_tightId", &LepGood1_tightId, &b_LepGood1_tightId);
   fChain->SetBranchAddress("LepGood1_mva", &LepGood1_mva, &b_LepGood1_mva);
   fChain->SetBranchAddress("LepGood1_tightCharge", &LepGood1_tightCharge, &b_LepGood1_tightCharge);
   fChain->SetBranchAddress("LepGood1_mcMatchId", &LepGood1_mcMatchId, &b_LepGood1_mcMatchId);
   fChain->SetBranchAddress("LepGood1_mcMatchAny", &LepGood1_mcMatchAny, &b_LepGood1_mcMatchAny);
   fChain->SetBranchAddress("LepGood1_mcDeltaRB", &LepGood1_mcDeltaRB, &b_LepGood1_mcDeltaRB);
   fChain->SetBranchAddress("LepLoose2_pt", &LepLoose2_pt, &b_LepLoose2_pt);
   fChain->SetBranchAddress("LepLoose2_eta", &LepLoose2_eta, &b_LepLoose2_eta);
   fChain->SetBranchAddress("LepLoose2_phi", &LepLoose2_phi, &b_LepLoose2_phi);
   fChain->SetBranchAddress("LepLoose2_mass", &LepLoose2_mass, &b_LepLoose2_mass);
   fChain->SetBranchAddress("LepLoose2_energy", &LepLoose2_energy, &b_LepLoose2_energy);
   fChain->SetBranchAddress("LepLoose2_pdgId", &LepLoose2_pdgId, &b_LepLoose2_pdgId);
   fChain->SetBranchAddress("LepLoose2_charge", &LepLoose2_charge, &b_LepLoose2_charge);
   fChain->SetBranchAddress("LepLoose2_sip3d", &LepLoose2_sip3d, &b_LepLoose2_sip3d);
   fChain->SetBranchAddress("LepLoose2_sip3ds", &LepLoose2_sip3ds, &b_LepLoose2_sip3ds);
   fChain->SetBranchAddress("LepLoose2_ip3ds", &LepLoose2_ip3ds, &b_LepLoose2_ip3ds);
   fChain->SetBranchAddress("LepLoose2_dxy", &LepLoose2_dxy, &b_LepLoose2_dxy);
   fChain->SetBranchAddress("LepLoose2_dz", &LepLoose2_dz, &b_LepLoose2_dz);
   fChain->SetBranchAddress("LepLoose2_relIso", &LepLoose2_relIso, &b_LepLoose2_relIso);
   fChain->SetBranchAddress("LepLoose2_chargedIso", &LepLoose2_chargedIso, &b_LepLoose2_chargedIso);
   fChain->SetBranchAddress("LepLoose2_drBJet", &LepLoose2_drBJet, &b_LepLoose2_drBJet);
   fChain->SetBranchAddress("LepLoose2_jetPtRatio", &LepLoose2_jetPtRatio, &b_LepLoose2_jetPtRatio);
   fChain->SetBranchAddress("LepLoose2_jetBTagCSV", &LepLoose2_jetBTagCSV, &b_LepLoose2_jetBTagCSV);
   fChain->SetBranchAddress("LepLoose2_jetBTagTCHE", &LepLoose2_jetBTagTCHE, &b_LepLoose2_jetBTagTCHE);
   fChain->SetBranchAddress("LepLoose2_ptRelJet", &LepLoose2_ptRelJet, &b_LepLoose2_ptRelJet);
   fChain->SetBranchAddress("LepLoose2_jetDR", &LepLoose2_jetDR, &b_LepLoose2_jetDR);
   fChain->SetBranchAddress("LepLoose2_jetSelf", &LepLoose2_jetSelf, &b_LepLoose2_jetSelf);
   fChain->SetBranchAddress("LepLoose2_innerHits", &LepLoose2_innerHits, &b_LepLoose2_innerHits);
   fChain->SetBranchAddress("LepLoose2_mvaId", &LepLoose2_mvaId, &b_LepLoose2_mvaId);
   fChain->SetBranchAddress("LepLoose2_tightId", &LepLoose2_tightId, &b_LepLoose2_tightId);
   fChain->SetBranchAddress("LepLoose2_mva", &LepLoose2_mva, &b_LepLoose2_mva);
   fChain->SetBranchAddress("LepLoose2_tightCharge", &LepLoose2_tightCharge, &b_LepLoose2_tightCharge);
   fChain->SetBranchAddress("LepGood2_pt", &LepGood2_pt, &b_LepGood2_pt);
   fChain->SetBranchAddress("LepGood2_eta", &LepGood2_eta, &b_LepGood2_eta);
   fChain->SetBranchAddress("LepGood2_phi", &LepGood2_phi, &b_LepGood2_phi);
   fChain->SetBranchAddress("LepGood2_mass", &LepGood2_mass, &b_LepGood2_mass);
   fChain->SetBranchAddress("LepGood2_energy", &LepGood2_energy, &b_LepGood2_energy);
   fChain->SetBranchAddress("LepGood2_pdgId", &LepGood2_pdgId, &b_LepGood2_pdgId);
   fChain->SetBranchAddress("LepGood2_charge", &LepGood2_charge, &b_LepGood2_charge);
   fChain->SetBranchAddress("LepGood2_sip3d", &LepGood2_sip3d, &b_LepGood2_sip3d);
   fChain->SetBranchAddress("LepGood2_sip3ds", &LepGood2_sip3ds, &b_LepGood2_sip3ds);
   fChain->SetBranchAddress("LepGood2_ip3ds", &LepGood2_ip3ds, &b_LepGood2_ip3ds);
   fChain->SetBranchAddress("LepGood2_dxy", &LepGood2_dxy, &b_LepGood2_dxy);
   fChain->SetBranchAddress("LepGood2_dz", &LepGood2_dz, &b_LepGood2_dz);
   fChain->SetBranchAddress("LepGood2_relIso", &LepGood2_relIso, &b_LepGood2_relIso);
   fChain->SetBranchAddress("LepGood2_chargedIso", &LepGood2_chargedIso, &b_LepGood2_chargedIso);
   fChain->SetBranchAddress("LepGood2_drBJet", &LepGood2_drBJet, &b_LepGood2_drBJet);
   fChain->SetBranchAddress("LepGood2_jetPtRatio", &LepGood2_jetPtRatio, &b_LepGood2_jetPtRatio);
   fChain->SetBranchAddress("LepGood2_jetBTagCSV", &LepGood2_jetBTagCSV, &b_LepGood2_jetBTagCSV);
   fChain->SetBranchAddress("LepGood2_jetBTagTCHE", &LepGood2_jetBTagTCHE, &b_LepGood2_jetBTagTCHE);
   fChain->SetBranchAddress("LepGood2_ptRelJet", &LepGood2_ptRelJet, &b_LepGood2_ptRelJet);
   fChain->SetBranchAddress("LepGood2_jetDR", &LepGood2_jetDR, &b_LepGood2_jetDR);
   fChain->SetBranchAddress("LepGood2_jetSelf", &LepGood2_jetSelf, &b_LepGood2_jetSelf);
   fChain->SetBranchAddress("LepGood2_innerHits", &LepGood2_innerHits, &b_LepGood2_innerHits);
   fChain->SetBranchAddress("LepGood2_mvaId", &LepGood2_mvaId, &b_LepGood2_mvaId);
   fChain->SetBranchAddress("LepGood2_tightId", &LepGood2_tightId, &b_LepGood2_tightId);
   fChain->SetBranchAddress("LepGood2_mva", &LepGood2_mva, &b_LepGood2_mva);
   fChain->SetBranchAddress("LepGood2_tightCharge", &LepGood2_tightCharge, &b_LepGood2_tightCharge);
   fChain->SetBranchAddress("LepGood2_mcMatchId", &LepGood2_mcMatchId, &b_LepGood2_mcMatchId);
   fChain->SetBranchAddress("LepGood2_mcMatchAny", &LepGood2_mcMatchAny, &b_LepGood2_mcMatchAny);
   fChain->SetBranchAddress("LepGood2_mcDeltaRB", &LepGood2_mcDeltaRB, &b_LepGood2_mcDeltaRB);
   fChain->SetBranchAddress("LepLoose3_pt", &LepLoose3_pt, &b_LepLoose3_pt);
   fChain->SetBranchAddress("LepLoose3_eta", &LepLoose3_eta, &b_LepLoose3_eta);
   fChain->SetBranchAddress("LepLoose3_phi", &LepLoose3_phi, &b_LepLoose3_phi);
   fChain->SetBranchAddress("LepLoose3_mass", &LepLoose3_mass, &b_LepLoose3_mass);
   fChain->SetBranchAddress("LepLoose3_energy", &LepLoose3_energy, &b_LepLoose3_energy);
   fChain->SetBranchAddress("LepLoose3_pdgId", &LepLoose3_pdgId, &b_LepLoose3_pdgId);
   fChain->SetBranchAddress("LepLoose3_charge", &LepLoose3_charge, &b_LepLoose3_charge);
   fChain->SetBranchAddress("LepLoose3_sip3d", &LepLoose3_sip3d, &b_LepLoose3_sip3d);
   fChain->SetBranchAddress("LepLoose3_sip3ds", &LepLoose3_sip3ds, &b_LepLoose3_sip3ds);
   fChain->SetBranchAddress("LepLoose3_ip3ds", &LepLoose3_ip3ds, &b_LepLoose3_ip3ds);
   fChain->SetBranchAddress("LepLoose3_dxy", &LepLoose3_dxy, &b_LepLoose3_dxy);
   fChain->SetBranchAddress("LepLoose3_dz", &LepLoose3_dz, &b_LepLoose3_dz);
   fChain->SetBranchAddress("LepLoose3_relIso", &LepLoose3_relIso, &b_LepLoose3_relIso);
   fChain->SetBranchAddress("LepLoose3_chargedIso", &LepLoose3_chargedIso, &b_LepLoose3_chargedIso);
   fChain->SetBranchAddress("LepLoose3_drBJet", &LepLoose3_drBJet, &b_LepLoose3_drBJet);
   fChain->SetBranchAddress("LepLoose3_jetPtRatio", &LepLoose3_jetPtRatio, &b_LepLoose3_jetPtRatio);
   fChain->SetBranchAddress("LepLoose3_jetBTagCSV", &LepLoose3_jetBTagCSV, &b_LepLoose3_jetBTagCSV);
   fChain->SetBranchAddress("LepLoose3_jetBTagTCHE", &LepLoose3_jetBTagTCHE, &b_LepLoose3_jetBTagTCHE);
   fChain->SetBranchAddress("LepLoose3_ptRelJet", &LepLoose3_ptRelJet, &b_LepLoose3_ptRelJet);
   fChain->SetBranchAddress("LepLoose3_jetDR", &LepLoose3_jetDR, &b_LepLoose3_jetDR);
   fChain->SetBranchAddress("LepLoose3_jetSelf", &LepLoose3_jetSelf, &b_LepLoose3_jetSelf);
   fChain->SetBranchAddress("LepLoose3_innerHits", &LepLoose3_innerHits, &b_LepLoose3_innerHits);
   fChain->SetBranchAddress("LepLoose3_mvaId", &LepLoose3_mvaId, &b_LepLoose3_mvaId);
   fChain->SetBranchAddress("LepLoose3_tightId", &LepLoose3_tightId, &b_LepLoose3_tightId);
   fChain->SetBranchAddress("LepLoose3_mva", &LepLoose3_mva, &b_LepLoose3_mva);
   fChain->SetBranchAddress("LepLoose3_tightCharge", &LepLoose3_tightCharge, &b_LepLoose3_tightCharge);
   fChain->SetBranchAddress("LepGood3_pt", &LepGood3_pt, &b_LepGood3_pt);
   fChain->SetBranchAddress("LepGood3_eta", &LepGood3_eta, &b_LepGood3_eta);
   fChain->SetBranchAddress("LepGood3_phi", &LepGood3_phi, &b_LepGood3_phi);
   fChain->SetBranchAddress("LepGood3_mass", &LepGood3_mass, &b_LepGood3_mass);
   fChain->SetBranchAddress("LepGood3_energy", &LepGood3_energy, &b_LepGood3_energy);
   fChain->SetBranchAddress("LepGood3_pdgId", &LepGood3_pdgId, &b_LepGood3_pdgId);
   fChain->SetBranchAddress("LepGood3_charge", &LepGood3_charge, &b_LepGood3_charge);
   fChain->SetBranchAddress("LepGood3_sip3d", &LepGood3_sip3d, &b_LepGood3_sip3d);
   fChain->SetBranchAddress("LepGood3_sip3ds", &LepGood3_sip3ds, &b_LepGood3_sip3ds);
   fChain->SetBranchAddress("LepGood3_ip3ds", &LepGood3_ip3ds, &b_LepGood3_ip3ds);
   fChain->SetBranchAddress("LepGood3_dxy", &LepGood3_dxy, &b_LepGood3_dxy);
   fChain->SetBranchAddress("LepGood3_dz", &LepGood3_dz, &b_LepGood3_dz);
   fChain->SetBranchAddress("LepGood3_relIso", &LepGood3_relIso, &b_LepGood3_relIso);
   fChain->SetBranchAddress("LepGood3_chargedIso", &LepGood3_chargedIso, &b_LepGood3_chargedIso);
   fChain->SetBranchAddress("LepGood3_drBJet", &LepGood3_drBJet, &b_LepGood3_drBJet);
   fChain->SetBranchAddress("LepGood3_jetPtRatio", &LepGood3_jetPtRatio, &b_LepGood3_jetPtRatio);
   fChain->SetBranchAddress("LepGood3_jetBTagCSV", &LepGood3_jetBTagCSV, &b_LepGood3_jetBTagCSV);
   fChain->SetBranchAddress("LepGood3_jetBTagTCHE", &LepGood3_jetBTagTCHE, &b_LepGood3_jetBTagTCHE);
   fChain->SetBranchAddress("LepGood3_ptRelJet", &LepGood3_ptRelJet, &b_LepGood3_ptRelJet);
   fChain->SetBranchAddress("LepGood3_jetDR", &LepGood3_jetDR, &b_LepGood3_jetDR);
   fChain->SetBranchAddress("LepGood3_jetSelf", &LepGood3_jetSelf, &b_LepGood3_jetSelf);
   fChain->SetBranchAddress("LepGood3_innerHits", &LepGood3_innerHits, &b_LepGood3_innerHits);
   fChain->SetBranchAddress("LepGood3_mvaId", &LepGood3_mvaId, &b_LepGood3_mvaId);
   fChain->SetBranchAddress("LepGood3_tightId", &LepGood3_tightId, &b_LepGood3_tightId);
   fChain->SetBranchAddress("LepGood3_mva", &LepGood3_mva, &b_LepGood3_mva);
   fChain->SetBranchAddress("LepGood3_tightCharge", &LepGood3_tightCharge, &b_LepGood3_tightCharge);
   fChain->SetBranchAddress("LepGood3_mcMatchId", &LepGood3_mcMatchId, &b_LepGood3_mcMatchId);
   fChain->SetBranchAddress("LepGood3_mcMatchAny", &LepGood3_mcMatchAny, &b_LepGood3_mcMatchAny);
   fChain->SetBranchAddress("LepGood3_mcDeltaRB", &LepGood3_mcDeltaRB, &b_LepGood3_mcDeltaRB);
   fChain->SetBranchAddress("LepLoose4_pt", &LepLoose4_pt, &b_LepLoose4_pt);
   fChain->SetBranchAddress("LepLoose4_eta", &LepLoose4_eta, &b_LepLoose4_eta);
   fChain->SetBranchAddress("LepLoose4_phi", &LepLoose4_phi, &b_LepLoose4_phi);
   fChain->SetBranchAddress("LepLoose4_mass", &LepLoose4_mass, &b_LepLoose4_mass);
   fChain->SetBranchAddress("LepLoose4_energy", &LepLoose4_energy, &b_LepLoose4_energy);
   fChain->SetBranchAddress("LepLoose4_pdgId", &LepLoose4_pdgId, &b_LepLoose4_pdgId);
   fChain->SetBranchAddress("LepLoose4_charge", &LepLoose4_charge, &b_LepLoose4_charge);
   fChain->SetBranchAddress("LepLoose4_sip3d", &LepLoose4_sip3d, &b_LepLoose4_sip3d);
   fChain->SetBranchAddress("LepLoose4_sip3ds", &LepLoose4_sip3ds, &b_LepLoose4_sip3ds);
   fChain->SetBranchAddress("LepLoose4_ip3ds", &LepLoose4_ip3ds, &b_LepLoose4_ip3ds);
   fChain->SetBranchAddress("LepLoose4_dxy", &LepLoose4_dxy, &b_LepLoose4_dxy);
   fChain->SetBranchAddress("LepLoose4_dz", &LepLoose4_dz, &b_LepLoose4_dz);
   fChain->SetBranchAddress("LepLoose4_relIso", &LepLoose4_relIso, &b_LepLoose4_relIso);
   fChain->SetBranchAddress("LepLoose4_chargedIso", &LepLoose4_chargedIso, &b_LepLoose4_chargedIso);
   fChain->SetBranchAddress("LepLoose4_drBJet", &LepLoose4_drBJet, &b_LepLoose4_drBJet);
   fChain->SetBranchAddress("LepLoose4_jetPtRatio", &LepLoose4_jetPtRatio, &b_LepLoose4_jetPtRatio);
   fChain->SetBranchAddress("LepLoose4_jetBTagCSV", &LepLoose4_jetBTagCSV, &b_LepLoose4_jetBTagCSV);
   fChain->SetBranchAddress("LepLoose4_jetBTagTCHE", &LepLoose4_jetBTagTCHE, &b_LepLoose4_jetBTagTCHE);
   fChain->SetBranchAddress("LepLoose4_ptRelJet", &LepLoose4_ptRelJet, &b_LepLoose4_ptRelJet);
   fChain->SetBranchAddress("LepLoose4_jetDR", &LepLoose4_jetDR, &b_LepLoose4_jetDR);
   fChain->SetBranchAddress("LepLoose4_jetSelf", &LepLoose4_jetSelf, &b_LepLoose4_jetSelf);
   fChain->SetBranchAddress("LepLoose4_innerHits", &LepLoose4_innerHits, &b_LepLoose4_innerHits);
   fChain->SetBranchAddress("LepLoose4_mvaId", &LepLoose4_mvaId, &b_LepLoose4_mvaId);
   fChain->SetBranchAddress("LepLoose4_tightId", &LepLoose4_tightId, &b_LepLoose4_tightId);
   fChain->SetBranchAddress("LepLoose4_mva", &LepLoose4_mva, &b_LepLoose4_mva);
   fChain->SetBranchAddress("LepLoose4_tightCharge", &LepLoose4_tightCharge, &b_LepLoose4_tightCharge);
   fChain->SetBranchAddress("LepGood4_pt", &LepGood4_pt, &b_LepGood4_pt);
   fChain->SetBranchAddress("LepGood4_eta", &LepGood4_eta, &b_LepGood4_eta);
   fChain->SetBranchAddress("LepGood4_phi", &LepGood4_phi, &b_LepGood4_phi);
   fChain->SetBranchAddress("LepGood4_mass", &LepGood4_mass, &b_LepGood4_mass);
   fChain->SetBranchAddress("LepGood4_energy", &LepGood4_energy, &b_LepGood4_energy);
   fChain->SetBranchAddress("LepGood4_pdgId", &LepGood4_pdgId, &b_LepGood4_pdgId);
   fChain->SetBranchAddress("LepGood4_charge", &LepGood4_charge, &b_LepGood4_charge);
   fChain->SetBranchAddress("LepGood4_sip3d", &LepGood4_sip3d, &b_LepGood4_sip3d);
   fChain->SetBranchAddress("LepGood4_sip3ds", &LepGood4_sip3ds, &b_LepGood4_sip3ds);
   fChain->SetBranchAddress("LepGood4_ip3ds", &LepGood4_ip3ds, &b_LepGood4_ip3ds);
   fChain->SetBranchAddress("LepGood4_dxy", &LepGood4_dxy, &b_LepGood4_dxy);
   fChain->SetBranchAddress("LepGood4_dz", &LepGood4_dz, &b_LepGood4_dz);
   fChain->SetBranchAddress("LepGood4_relIso", &LepGood4_relIso, &b_LepGood4_relIso);
   fChain->SetBranchAddress("LepGood4_chargedIso", &LepGood4_chargedIso, &b_LepGood4_chargedIso);
   fChain->SetBranchAddress("LepGood4_drBJet", &LepGood4_drBJet, &b_LepGood4_drBJet);
   fChain->SetBranchAddress("LepGood4_jetPtRatio", &LepGood4_jetPtRatio, &b_LepGood4_jetPtRatio);
   fChain->SetBranchAddress("LepGood4_jetBTagCSV", &LepGood4_jetBTagCSV, &b_LepGood4_jetBTagCSV);
   fChain->SetBranchAddress("LepGood4_jetBTagTCHE", &LepGood4_jetBTagTCHE, &b_LepGood4_jetBTagTCHE);
   fChain->SetBranchAddress("LepGood4_ptRelJet", &LepGood4_ptRelJet, &b_LepGood4_ptRelJet);
   fChain->SetBranchAddress("LepGood4_jetDR", &LepGood4_jetDR, &b_LepGood4_jetDR);
   fChain->SetBranchAddress("LepGood4_jetSelf", &LepGood4_jetSelf, &b_LepGood4_jetSelf);
   fChain->SetBranchAddress("LepGood4_innerHits", &LepGood4_innerHits, &b_LepGood4_innerHits);
   fChain->SetBranchAddress("LepGood4_mvaId", &LepGood4_mvaId, &b_LepGood4_mvaId);
   fChain->SetBranchAddress("LepGood4_tightId", &LepGood4_tightId, &b_LepGood4_tightId);
   fChain->SetBranchAddress("LepGood4_mva", &LepGood4_mva, &b_LepGood4_mva);
   fChain->SetBranchAddress("LepGood4_tightCharge", &LepGood4_tightCharge, &b_LepGood4_tightCharge);
   fChain->SetBranchAddress("LepGood4_mcMatchId", &LepGood4_mcMatchId, &b_LepGood4_mcMatchId);
   fChain->SetBranchAddress("LepGood4_mcMatchAny", &LepGood4_mcMatchAny, &b_LepGood4_mcMatchAny);
   fChain->SetBranchAddress("LepGood4_mcDeltaRB", &LepGood4_mcDeltaRB, &b_LepGood4_mcDeltaRB);
   fChain->SetBranchAddress("LepLoose5_pt", &LepLoose5_pt, &b_LepLoose5_pt);
   fChain->SetBranchAddress("LepLoose5_eta", &LepLoose5_eta, &b_LepLoose5_eta);
   fChain->SetBranchAddress("LepLoose5_phi", &LepLoose5_phi, &b_LepLoose5_phi);
   fChain->SetBranchAddress("LepLoose5_mass", &LepLoose5_mass, &b_LepLoose5_mass);
   fChain->SetBranchAddress("LepLoose5_energy", &LepLoose5_energy, &b_LepLoose5_energy);
   fChain->SetBranchAddress("LepLoose5_pdgId", &LepLoose5_pdgId, &b_LepLoose5_pdgId);
   fChain->SetBranchAddress("LepLoose5_charge", &LepLoose5_charge, &b_LepLoose5_charge);
   fChain->SetBranchAddress("LepLoose5_sip3d", &LepLoose5_sip3d, &b_LepLoose5_sip3d);
   fChain->SetBranchAddress("LepLoose5_sip3ds", &LepLoose5_sip3ds, &b_LepLoose5_sip3ds);
   fChain->SetBranchAddress("LepLoose5_ip3ds", &LepLoose5_ip3ds, &b_LepLoose5_ip3ds);
   fChain->SetBranchAddress("LepLoose5_dxy", &LepLoose5_dxy, &b_LepLoose5_dxy);
   fChain->SetBranchAddress("LepLoose5_dz", &LepLoose5_dz, &b_LepLoose5_dz);
   fChain->SetBranchAddress("LepLoose5_relIso", &LepLoose5_relIso, &b_LepLoose5_relIso);
   fChain->SetBranchAddress("LepLoose5_chargedIso", &LepLoose5_chargedIso, &b_LepLoose5_chargedIso);
   fChain->SetBranchAddress("LepLoose5_drBJet", &LepLoose5_drBJet, &b_LepLoose5_drBJet);
   fChain->SetBranchAddress("LepLoose5_jetPtRatio", &LepLoose5_jetPtRatio, &b_LepLoose5_jetPtRatio);
   fChain->SetBranchAddress("LepLoose5_jetBTagCSV", &LepLoose5_jetBTagCSV, &b_LepLoose5_jetBTagCSV);
   fChain->SetBranchAddress("LepLoose5_jetBTagTCHE", &LepLoose5_jetBTagTCHE, &b_LepLoose5_jetBTagTCHE);
   fChain->SetBranchAddress("LepLoose5_ptRelJet", &LepLoose5_ptRelJet, &b_LepLoose5_ptRelJet);
   fChain->SetBranchAddress("LepLoose5_jetDR", &LepLoose5_jetDR, &b_LepLoose5_jetDR);
   fChain->SetBranchAddress("LepLoose5_jetSelf", &LepLoose5_jetSelf, &b_LepLoose5_jetSelf);
   fChain->SetBranchAddress("LepLoose5_innerHits", &LepLoose5_innerHits, &b_LepLoose5_innerHits);
   fChain->SetBranchAddress("LepLoose5_mvaId", &LepLoose5_mvaId, &b_LepLoose5_mvaId);
   fChain->SetBranchAddress("LepLoose5_tightId", &LepLoose5_tightId, &b_LepLoose5_tightId);
   fChain->SetBranchAddress("LepLoose5_mva", &LepLoose5_mva, &b_LepLoose5_mva);
   fChain->SetBranchAddress("LepLoose5_tightCharge", &LepLoose5_tightCharge, &b_LepLoose5_tightCharge);
   fChain->SetBranchAddress("LepGood5_pt", &LepGood5_pt, &b_LepGood5_pt);
   fChain->SetBranchAddress("LepGood5_eta", &LepGood5_eta, &b_LepGood5_eta);
   fChain->SetBranchAddress("LepGood5_phi", &LepGood5_phi, &b_LepGood5_phi);
   fChain->SetBranchAddress("LepGood5_mass", &LepGood5_mass, &b_LepGood5_mass);
   fChain->SetBranchAddress("LepGood5_energy", &LepGood5_energy, &b_LepGood5_energy);
   fChain->SetBranchAddress("LepGood5_pdgId", &LepGood5_pdgId, &b_LepGood5_pdgId);
   fChain->SetBranchAddress("LepGood5_charge", &LepGood5_charge, &b_LepGood5_charge);
   fChain->SetBranchAddress("LepGood5_sip3d", &LepGood5_sip3d, &b_LepGood5_sip3d);
   fChain->SetBranchAddress("LepGood5_sip3ds", &LepGood5_sip3ds, &b_LepGood5_sip3ds);
   fChain->SetBranchAddress("LepGood5_ip3ds", &LepGood5_ip3ds, &b_LepGood5_ip3ds);
   fChain->SetBranchAddress("LepGood5_dxy", &LepGood5_dxy, &b_LepGood5_dxy);
   fChain->SetBranchAddress("LepGood5_dz", &LepGood5_dz, &b_LepGood5_dz);
   fChain->SetBranchAddress("LepGood5_relIso", &LepGood5_relIso, &b_LepGood5_relIso);
   fChain->SetBranchAddress("LepGood5_chargedIso", &LepGood5_chargedIso, &b_LepGood5_chargedIso);
   fChain->SetBranchAddress("LepGood5_drBJet", &LepGood5_drBJet, &b_LepGood5_drBJet);
   fChain->SetBranchAddress("LepGood5_jetPtRatio", &LepGood5_jetPtRatio, &b_LepGood5_jetPtRatio);
   fChain->SetBranchAddress("LepGood5_jetBTagCSV", &LepGood5_jetBTagCSV, &b_LepGood5_jetBTagCSV);
   fChain->SetBranchAddress("LepGood5_jetBTagTCHE", &LepGood5_jetBTagTCHE, &b_LepGood5_jetBTagTCHE);
   fChain->SetBranchAddress("LepGood5_ptRelJet", &LepGood5_ptRelJet, &b_LepGood5_ptRelJet);
   fChain->SetBranchAddress("LepGood5_jetDR", &LepGood5_jetDR, &b_LepGood5_jetDR);
   fChain->SetBranchAddress("LepGood5_jetSelf", &LepGood5_jetSelf, &b_LepGood5_jetSelf);
   fChain->SetBranchAddress("LepGood5_innerHits", &LepGood5_innerHits, &b_LepGood5_innerHits);
   fChain->SetBranchAddress("LepGood5_mvaId", &LepGood5_mvaId, &b_LepGood5_mvaId);
   fChain->SetBranchAddress("LepGood5_tightId", &LepGood5_tightId, &b_LepGood5_tightId);
   fChain->SetBranchAddress("LepGood5_mva", &LepGood5_mva, &b_LepGood5_mva);
   fChain->SetBranchAddress("LepGood5_tightCharge", &LepGood5_tightCharge, &b_LepGood5_tightCharge);
   fChain->SetBranchAddress("LepGood5_mcMatchId", &LepGood5_mcMatchId, &b_LepGood5_mcMatchId);
   fChain->SetBranchAddress("LepGood5_mcMatchAny", &LepGood5_mcMatchAny, &b_LepGood5_mcMatchAny);
   fChain->SetBranchAddress("LepGood5_mcDeltaRB", &LepGood5_mcDeltaRB, &b_LepGood5_mcDeltaRB);
   fChain->SetBranchAddress("LepLoose6_pt", &LepLoose6_pt, &b_LepLoose6_pt);
   fChain->SetBranchAddress("LepLoose6_eta", &LepLoose6_eta, &b_LepLoose6_eta);
   fChain->SetBranchAddress("LepLoose6_phi", &LepLoose6_phi, &b_LepLoose6_phi);
   fChain->SetBranchAddress("LepLoose6_mass", &LepLoose6_mass, &b_LepLoose6_mass);
   fChain->SetBranchAddress("LepLoose6_energy", &LepLoose6_energy, &b_LepLoose6_energy);
   fChain->SetBranchAddress("LepLoose6_pdgId", &LepLoose6_pdgId, &b_LepLoose6_pdgId);
   fChain->SetBranchAddress("LepLoose6_charge", &LepLoose6_charge, &b_LepLoose6_charge);
   fChain->SetBranchAddress("LepLoose6_sip3d", &LepLoose6_sip3d, &b_LepLoose6_sip3d);
   fChain->SetBranchAddress("LepLoose6_sip3ds", &LepLoose6_sip3ds, &b_LepLoose6_sip3ds);
   fChain->SetBranchAddress("LepLoose6_ip3ds", &LepLoose6_ip3ds, &b_LepLoose6_ip3ds);
   fChain->SetBranchAddress("LepLoose6_dxy", &LepLoose6_dxy, &b_LepLoose6_dxy);
   fChain->SetBranchAddress("LepLoose6_dz", &LepLoose6_dz, &b_LepLoose6_dz);
   fChain->SetBranchAddress("LepLoose6_relIso", &LepLoose6_relIso, &b_LepLoose6_relIso);
   fChain->SetBranchAddress("LepLoose6_chargedIso", &LepLoose6_chargedIso, &b_LepLoose6_chargedIso);
   fChain->SetBranchAddress("LepLoose6_drBJet", &LepLoose6_drBJet, &b_LepLoose6_drBJet);
   fChain->SetBranchAddress("LepLoose6_jetPtRatio", &LepLoose6_jetPtRatio, &b_LepLoose6_jetPtRatio);
   fChain->SetBranchAddress("LepLoose6_jetBTagCSV", &LepLoose6_jetBTagCSV, &b_LepLoose6_jetBTagCSV);
   fChain->SetBranchAddress("LepLoose6_jetBTagTCHE", &LepLoose6_jetBTagTCHE, &b_LepLoose6_jetBTagTCHE);
   fChain->SetBranchAddress("LepLoose6_ptRelJet", &LepLoose6_ptRelJet, &b_LepLoose6_ptRelJet);
   fChain->SetBranchAddress("LepLoose6_jetDR", &LepLoose6_jetDR, &b_LepLoose6_jetDR);
   fChain->SetBranchAddress("LepLoose6_jetSelf", &LepLoose6_jetSelf, &b_LepLoose6_jetSelf);
   fChain->SetBranchAddress("LepLoose6_innerHits", &LepLoose6_innerHits, &b_LepLoose6_innerHits);
   fChain->SetBranchAddress("LepLoose6_mvaId", &LepLoose6_mvaId, &b_LepLoose6_mvaId);
   fChain->SetBranchAddress("LepLoose6_tightId", &LepLoose6_tightId, &b_LepLoose6_tightId);
   fChain->SetBranchAddress("LepLoose6_mva", &LepLoose6_mva, &b_LepLoose6_mva);
   fChain->SetBranchAddress("LepLoose6_tightCharge", &LepLoose6_tightCharge, &b_LepLoose6_tightCharge);
   fChain->SetBranchAddress("LepGood6_pt", &LepGood6_pt, &b_LepGood6_pt);
   fChain->SetBranchAddress("LepGood6_eta", &LepGood6_eta, &b_LepGood6_eta);
   fChain->SetBranchAddress("LepGood6_phi", &LepGood6_phi, &b_LepGood6_phi);
   fChain->SetBranchAddress("LepGood6_mass", &LepGood6_mass, &b_LepGood6_mass);
   fChain->SetBranchAddress("LepGood6_energy", &LepGood6_energy, &b_LepGood6_energy);
   fChain->SetBranchAddress("LepGood6_pdgId", &LepGood6_pdgId, &b_LepGood6_pdgId);
   fChain->SetBranchAddress("LepGood6_charge", &LepGood6_charge, &b_LepGood6_charge);
   fChain->SetBranchAddress("LepGood6_sip3d", &LepGood6_sip3d, &b_LepGood6_sip3d);
   fChain->SetBranchAddress("LepGood6_sip3ds", &LepGood6_sip3ds, &b_LepGood6_sip3ds);
   fChain->SetBranchAddress("LepGood6_ip3ds", &LepGood6_ip3ds, &b_LepGood6_ip3ds);
   fChain->SetBranchAddress("LepGood6_dxy", &LepGood6_dxy, &b_LepGood6_dxy);
   fChain->SetBranchAddress("LepGood6_dz", &LepGood6_dz, &b_LepGood6_dz);
   fChain->SetBranchAddress("LepGood6_relIso", &LepGood6_relIso, &b_LepGood6_relIso);
   fChain->SetBranchAddress("LepGood6_chargedIso", &LepGood6_chargedIso, &b_LepGood6_chargedIso);
   fChain->SetBranchAddress("LepGood6_drBJet", &LepGood6_drBJet, &b_LepGood6_drBJet);
   fChain->SetBranchAddress("LepGood6_jetPtRatio", &LepGood6_jetPtRatio, &b_LepGood6_jetPtRatio);
   fChain->SetBranchAddress("LepGood6_jetBTagCSV", &LepGood6_jetBTagCSV, &b_LepGood6_jetBTagCSV);
   fChain->SetBranchAddress("LepGood6_jetBTagTCHE", &LepGood6_jetBTagTCHE, &b_LepGood6_jetBTagTCHE);
   fChain->SetBranchAddress("LepGood6_ptRelJet", &LepGood6_ptRelJet, &b_LepGood6_ptRelJet);
   fChain->SetBranchAddress("LepGood6_jetDR", &LepGood6_jetDR, &b_LepGood6_jetDR);
   fChain->SetBranchAddress("LepGood6_jetSelf", &LepGood6_jetSelf, &b_LepGood6_jetSelf);
   fChain->SetBranchAddress("LepGood6_innerHits", &LepGood6_innerHits, &b_LepGood6_innerHits);
   fChain->SetBranchAddress("LepGood6_mvaId", &LepGood6_mvaId, &b_LepGood6_mvaId);
   fChain->SetBranchAddress("LepGood6_tightId", &LepGood6_tightId, &b_LepGood6_tightId);
   fChain->SetBranchAddress("LepGood6_mva", &LepGood6_mva, &b_LepGood6_mva);
   fChain->SetBranchAddress("LepGood6_tightCharge", &LepGood6_tightCharge, &b_LepGood6_tightCharge);
   fChain->SetBranchAddress("LepGood6_mcMatchId", &LepGood6_mcMatchId, &b_LepGood6_mcMatchId);
   fChain->SetBranchAddress("LepGood6_mcMatchAny", &LepGood6_mcMatchAny, &b_LepGood6_mcMatchAny);
   fChain->SetBranchAddress("LepGood6_mcDeltaRB", &LepGood6_mcDeltaRB, &b_LepGood6_mcDeltaRB);
   fChain->SetBranchAddress("LepLoose7_pt", &LepLoose7_pt, &b_LepLoose7_pt);
   fChain->SetBranchAddress("LepLoose7_eta", &LepLoose7_eta, &b_LepLoose7_eta);
   fChain->SetBranchAddress("LepLoose7_phi", &LepLoose7_phi, &b_LepLoose7_phi);
   fChain->SetBranchAddress("LepLoose7_mass", &LepLoose7_mass, &b_LepLoose7_mass);
   fChain->SetBranchAddress("LepLoose7_energy", &LepLoose7_energy, &b_LepLoose7_energy);
   fChain->SetBranchAddress("LepLoose7_pdgId", &LepLoose7_pdgId, &b_LepLoose7_pdgId);
   fChain->SetBranchAddress("LepLoose7_charge", &LepLoose7_charge, &b_LepLoose7_charge);
   fChain->SetBranchAddress("LepLoose7_sip3d", &LepLoose7_sip3d, &b_LepLoose7_sip3d);
   fChain->SetBranchAddress("LepLoose7_sip3ds", &LepLoose7_sip3ds, &b_LepLoose7_sip3ds);
   fChain->SetBranchAddress("LepLoose7_ip3ds", &LepLoose7_ip3ds, &b_LepLoose7_ip3ds);
   fChain->SetBranchAddress("LepLoose7_dxy", &LepLoose7_dxy, &b_LepLoose7_dxy);
   fChain->SetBranchAddress("LepLoose7_dz", &LepLoose7_dz, &b_LepLoose7_dz);
   fChain->SetBranchAddress("LepLoose7_relIso", &LepLoose7_relIso, &b_LepLoose7_relIso);
   fChain->SetBranchAddress("LepLoose7_chargedIso", &LepLoose7_chargedIso, &b_LepLoose7_chargedIso);
   fChain->SetBranchAddress("LepLoose7_drBJet", &LepLoose7_drBJet, &b_LepLoose7_drBJet);
   fChain->SetBranchAddress("LepLoose7_jetPtRatio", &LepLoose7_jetPtRatio, &b_LepLoose7_jetPtRatio);
   fChain->SetBranchAddress("LepLoose7_jetBTagCSV", &LepLoose7_jetBTagCSV, &b_LepLoose7_jetBTagCSV);
   fChain->SetBranchAddress("LepLoose7_jetBTagTCHE", &LepLoose7_jetBTagTCHE, &b_LepLoose7_jetBTagTCHE);
   fChain->SetBranchAddress("LepLoose7_ptRelJet", &LepLoose7_ptRelJet, &b_LepLoose7_ptRelJet);
   fChain->SetBranchAddress("LepLoose7_jetDR", &LepLoose7_jetDR, &b_LepLoose7_jetDR);
   fChain->SetBranchAddress("LepLoose7_jetSelf", &LepLoose7_jetSelf, &b_LepLoose7_jetSelf);
   fChain->SetBranchAddress("LepLoose7_innerHits", &LepLoose7_innerHits, &b_LepLoose7_innerHits);
   fChain->SetBranchAddress("LepLoose7_mvaId", &LepLoose7_mvaId, &b_LepLoose7_mvaId);
   fChain->SetBranchAddress("LepLoose7_tightId", &LepLoose7_tightId, &b_LepLoose7_tightId);
   fChain->SetBranchAddress("LepLoose7_mva", &LepLoose7_mva, &b_LepLoose7_mva);
   fChain->SetBranchAddress("LepLoose7_tightCharge", &LepLoose7_tightCharge, &b_LepLoose7_tightCharge);
   fChain->SetBranchAddress("LepGood7_pt", &LepGood7_pt, &b_LepGood7_pt);
   fChain->SetBranchAddress("LepGood7_eta", &LepGood7_eta, &b_LepGood7_eta);
   fChain->SetBranchAddress("LepGood7_phi", &LepGood7_phi, &b_LepGood7_phi);
   fChain->SetBranchAddress("LepGood7_mass", &LepGood7_mass, &b_LepGood7_mass);
   fChain->SetBranchAddress("LepGood7_energy", &LepGood7_energy, &b_LepGood7_energy);
   fChain->SetBranchAddress("LepGood7_pdgId", &LepGood7_pdgId, &b_LepGood7_pdgId);
   fChain->SetBranchAddress("LepGood7_charge", &LepGood7_charge, &b_LepGood7_charge);
   fChain->SetBranchAddress("LepGood7_sip3d", &LepGood7_sip3d, &b_LepGood7_sip3d);
   fChain->SetBranchAddress("LepGood7_sip3ds", &LepGood7_sip3ds, &b_LepGood7_sip3ds);
   fChain->SetBranchAddress("LepGood7_ip3ds", &LepGood7_ip3ds, &b_LepGood7_ip3ds);
   fChain->SetBranchAddress("LepGood7_dxy", &LepGood7_dxy, &b_LepGood7_dxy);
   fChain->SetBranchAddress("LepGood7_dz", &LepGood7_dz, &b_LepGood7_dz);
   fChain->SetBranchAddress("LepGood7_relIso", &LepGood7_relIso, &b_LepGood7_relIso);
   fChain->SetBranchAddress("LepGood7_chargedIso", &LepGood7_chargedIso, &b_LepGood7_chargedIso);
   fChain->SetBranchAddress("LepGood7_drBJet", &LepGood7_drBJet, &b_LepGood7_drBJet);
   fChain->SetBranchAddress("LepGood7_jetPtRatio", &LepGood7_jetPtRatio, &b_LepGood7_jetPtRatio);
   fChain->SetBranchAddress("LepGood7_jetBTagCSV", &LepGood7_jetBTagCSV, &b_LepGood7_jetBTagCSV);
   fChain->SetBranchAddress("LepGood7_jetBTagTCHE", &LepGood7_jetBTagTCHE, &b_LepGood7_jetBTagTCHE);
   fChain->SetBranchAddress("LepGood7_ptRelJet", &LepGood7_ptRelJet, &b_LepGood7_ptRelJet);
   fChain->SetBranchAddress("LepGood7_jetDR", &LepGood7_jetDR, &b_LepGood7_jetDR);
   fChain->SetBranchAddress("LepGood7_jetSelf", &LepGood7_jetSelf, &b_LepGood7_jetSelf);
   fChain->SetBranchAddress("LepGood7_innerHits", &LepGood7_innerHits, &b_LepGood7_innerHits);
   fChain->SetBranchAddress("LepGood7_mvaId", &LepGood7_mvaId, &b_LepGood7_mvaId);
   fChain->SetBranchAddress("LepGood7_tightId", &LepGood7_tightId, &b_LepGood7_tightId);
   fChain->SetBranchAddress("LepGood7_mva", &LepGood7_mva, &b_LepGood7_mva);
   fChain->SetBranchAddress("LepGood7_tightCharge", &LepGood7_tightCharge, &b_LepGood7_tightCharge);
   fChain->SetBranchAddress("LepGood7_mcMatchId", &LepGood7_mcMatchId, &b_LepGood7_mcMatchId);
   fChain->SetBranchAddress("LepGood7_mcMatchAny", &LepGood7_mcMatchAny, &b_LepGood7_mcMatchAny);
   fChain->SetBranchAddress("LepGood7_mcDeltaRB", &LepGood7_mcDeltaRB, &b_LepGood7_mcDeltaRB);
   fChain->SetBranchAddress("LepLoose8_pt", &LepLoose8_pt, &b_LepLoose8_pt);
   fChain->SetBranchAddress("LepLoose8_eta", &LepLoose8_eta, &b_LepLoose8_eta);
   fChain->SetBranchAddress("LepLoose8_phi", &LepLoose8_phi, &b_LepLoose8_phi);
   fChain->SetBranchAddress("LepLoose8_mass", &LepLoose8_mass, &b_LepLoose8_mass);
   fChain->SetBranchAddress("LepLoose8_energy", &LepLoose8_energy, &b_LepLoose8_energy);
   fChain->SetBranchAddress("LepLoose8_pdgId", &LepLoose8_pdgId, &b_LepLoose8_pdgId);
   fChain->SetBranchAddress("LepLoose8_charge", &LepLoose8_charge, &b_LepLoose8_charge);
   fChain->SetBranchAddress("LepLoose8_sip3d", &LepLoose8_sip3d, &b_LepLoose8_sip3d);
   fChain->SetBranchAddress("LepLoose8_sip3ds", &LepLoose8_sip3ds, &b_LepLoose8_sip3ds);
   fChain->SetBranchAddress("LepLoose8_ip3ds", &LepLoose8_ip3ds, &b_LepLoose8_ip3ds);
   fChain->SetBranchAddress("LepLoose8_dxy", &LepLoose8_dxy, &b_LepLoose8_dxy);
   fChain->SetBranchAddress("LepLoose8_dz", &LepLoose8_dz, &b_LepLoose8_dz);
   fChain->SetBranchAddress("LepLoose8_relIso", &LepLoose8_relIso, &b_LepLoose8_relIso);
   fChain->SetBranchAddress("LepLoose8_chargedIso", &LepLoose8_chargedIso, &b_LepLoose8_chargedIso);
   fChain->SetBranchAddress("LepLoose8_drBJet", &LepLoose8_drBJet, &b_LepLoose8_drBJet);
   fChain->SetBranchAddress("LepLoose8_jetPtRatio", &LepLoose8_jetPtRatio, &b_LepLoose8_jetPtRatio);
   fChain->SetBranchAddress("LepLoose8_jetBTagCSV", &LepLoose8_jetBTagCSV, &b_LepLoose8_jetBTagCSV);
   fChain->SetBranchAddress("LepLoose8_jetBTagTCHE", &LepLoose8_jetBTagTCHE, &b_LepLoose8_jetBTagTCHE);
   fChain->SetBranchAddress("LepLoose8_ptRelJet", &LepLoose8_ptRelJet, &b_LepLoose8_ptRelJet);
   fChain->SetBranchAddress("LepLoose8_jetDR", &LepLoose8_jetDR, &b_LepLoose8_jetDR);
   fChain->SetBranchAddress("LepLoose8_jetSelf", &LepLoose8_jetSelf, &b_LepLoose8_jetSelf);
   fChain->SetBranchAddress("LepLoose8_innerHits", &LepLoose8_innerHits, &b_LepLoose8_innerHits);
   fChain->SetBranchAddress("LepLoose8_mvaId", &LepLoose8_mvaId, &b_LepLoose8_mvaId);
   fChain->SetBranchAddress("LepLoose8_tightId", &LepLoose8_tightId, &b_LepLoose8_tightId);
   fChain->SetBranchAddress("LepLoose8_mva", &LepLoose8_mva, &b_LepLoose8_mva);
   fChain->SetBranchAddress("LepLoose8_tightCharge", &LepLoose8_tightCharge, &b_LepLoose8_tightCharge);
   fChain->SetBranchAddress("LepGood8_pt", &LepGood8_pt, &b_LepGood8_pt);
   fChain->SetBranchAddress("LepGood8_eta", &LepGood8_eta, &b_LepGood8_eta);
   fChain->SetBranchAddress("LepGood8_phi", &LepGood8_phi, &b_LepGood8_phi);
   fChain->SetBranchAddress("LepGood8_mass", &LepGood8_mass, &b_LepGood8_mass);
   fChain->SetBranchAddress("LepGood8_energy", &LepGood8_energy, &b_LepGood8_energy);
   fChain->SetBranchAddress("LepGood8_pdgId", &LepGood8_pdgId, &b_LepGood8_pdgId);
   fChain->SetBranchAddress("LepGood8_charge", &LepGood8_charge, &b_LepGood8_charge);
   fChain->SetBranchAddress("LepGood8_sip3d", &LepGood8_sip3d, &b_LepGood8_sip3d);
   fChain->SetBranchAddress("LepGood8_sip3ds", &LepGood8_sip3ds, &b_LepGood8_sip3ds);
   fChain->SetBranchAddress("LepGood8_ip3ds", &LepGood8_ip3ds, &b_LepGood8_ip3ds);
   fChain->SetBranchAddress("LepGood8_dxy", &LepGood8_dxy, &b_LepGood8_dxy);
   fChain->SetBranchAddress("LepGood8_dz", &LepGood8_dz, &b_LepGood8_dz);
   fChain->SetBranchAddress("LepGood8_relIso", &LepGood8_relIso, &b_LepGood8_relIso);
   fChain->SetBranchAddress("LepGood8_chargedIso", &LepGood8_chargedIso, &b_LepGood8_chargedIso);
   fChain->SetBranchAddress("LepGood8_drBJet", &LepGood8_drBJet, &b_LepGood8_drBJet);
   fChain->SetBranchAddress("LepGood8_jetPtRatio", &LepGood8_jetPtRatio, &b_LepGood8_jetPtRatio);
   fChain->SetBranchAddress("LepGood8_jetBTagCSV", &LepGood8_jetBTagCSV, &b_LepGood8_jetBTagCSV);
   fChain->SetBranchAddress("LepGood8_jetBTagTCHE", &LepGood8_jetBTagTCHE, &b_LepGood8_jetBTagTCHE);
   fChain->SetBranchAddress("LepGood8_ptRelJet", &LepGood8_ptRelJet, &b_LepGood8_ptRelJet);
   fChain->SetBranchAddress("LepGood8_jetDR", &LepGood8_jetDR, &b_LepGood8_jetDR);
   fChain->SetBranchAddress("LepGood8_jetSelf", &LepGood8_jetSelf, &b_LepGood8_jetSelf);
   fChain->SetBranchAddress("LepGood8_innerHits", &LepGood8_innerHits, &b_LepGood8_innerHits);
   fChain->SetBranchAddress("LepGood8_mvaId", &LepGood8_mvaId, &b_LepGood8_mvaId);
   fChain->SetBranchAddress("LepGood8_tightId", &LepGood8_tightId, &b_LepGood8_tightId);
   fChain->SetBranchAddress("LepGood8_mva", &LepGood8_mva, &b_LepGood8_mva);
   fChain->SetBranchAddress("LepGood8_tightCharge", &LepGood8_tightCharge, &b_LepGood8_tightCharge);
   fChain->SetBranchAddress("LepGood8_mcMatchId", &LepGood8_mcMatchId, &b_LepGood8_mcMatchId);
   fChain->SetBranchAddress("LepGood8_mcMatchAny", &LepGood8_mcMatchAny, &b_LepGood8_mcMatchAny);
   fChain->SetBranchAddress("LepGood8_mcDeltaRB", &LepGood8_mcDeltaRB, &b_LepGood8_mcDeltaRB);
   fChain->SetBranchAddress("Jet1_pt", &Jet1_pt, &b_Jet1_pt);
   fChain->SetBranchAddress("Jet1_eta", &Jet1_eta, &b_Jet1_eta);
   fChain->SetBranchAddress("Jet1_phi", &Jet1_phi, &b_Jet1_phi);
   fChain->SetBranchAddress("Jet1_mass", &Jet1_mass, &b_Jet1_mass);
   fChain->SetBranchAddress("Jet1_energy", &Jet1_energy, &b_Jet1_energy);
   fChain->SetBranchAddress("Jet1_btagCSV", &Jet1_btagCSV, &b_Jet1_btagCSV);
   fChain->SetBranchAddress("Jet1_leg_pt", &Jet1_leg_pt, &b_Jet1_leg_pt);
   fChain->SetBranchAddress("Jet1_leg_eta", &Jet1_leg_eta, &b_Jet1_leg_eta);
   fChain->SetBranchAddress("Jet1_leg_phi", &Jet1_leg_phi, &b_Jet1_leg_phi);
   fChain->SetBranchAddress("Jet1_leg_mass", &Jet1_leg_mass, &b_Jet1_leg_mass);
   fChain->SetBranchAddress("Jet1_leg_energy", &Jet1_leg_energy, &b_Jet1_leg_energy);
   fChain->SetBranchAddress("Jet1_mcMatchId", &Jet1_mcMatchId, &b_Jet1_mcMatchId);
   fChain->SetBranchAddress("Jet1_mcMatchFlav", &Jet1_mcMatchFlav, &b_Jet1_mcMatchFlav);
   fChain->SetBranchAddress("Jet2_pt", &Jet2_pt, &b_Jet2_pt);
   fChain->SetBranchAddress("Jet2_eta", &Jet2_eta, &b_Jet2_eta);
   fChain->SetBranchAddress("Jet2_phi", &Jet2_phi, &b_Jet2_phi);
   fChain->SetBranchAddress("Jet2_mass", &Jet2_mass, &b_Jet2_mass);
   fChain->SetBranchAddress("Jet2_energy", &Jet2_energy, &b_Jet2_energy);
   fChain->SetBranchAddress("Jet2_btagCSV", &Jet2_btagCSV, &b_Jet2_btagCSV);
   fChain->SetBranchAddress("Jet2_leg_pt", &Jet2_leg_pt, &b_Jet2_leg_pt);
   fChain->SetBranchAddress("Jet2_leg_eta", &Jet2_leg_eta, &b_Jet2_leg_eta);
   fChain->SetBranchAddress("Jet2_leg_phi", &Jet2_leg_phi, &b_Jet2_leg_phi);
   fChain->SetBranchAddress("Jet2_leg_mass", &Jet2_leg_mass, &b_Jet2_leg_mass);
   fChain->SetBranchAddress("Jet2_leg_energy", &Jet2_leg_energy, &b_Jet2_leg_energy);
   fChain->SetBranchAddress("Jet2_mcMatchId", &Jet2_mcMatchId, &b_Jet2_mcMatchId);
   fChain->SetBranchAddress("Jet2_mcMatchFlav", &Jet2_mcMatchFlav, &b_Jet2_mcMatchFlav);
   fChain->SetBranchAddress("Jet3_pt", &Jet3_pt, &b_Jet3_pt);
   fChain->SetBranchAddress("Jet3_eta", &Jet3_eta, &b_Jet3_eta);
   fChain->SetBranchAddress("Jet3_phi", &Jet3_phi, &b_Jet3_phi);
   fChain->SetBranchAddress("Jet3_mass", &Jet3_mass, &b_Jet3_mass);
   fChain->SetBranchAddress("Jet3_energy", &Jet3_energy, &b_Jet3_energy);
   fChain->SetBranchAddress("Jet3_btagCSV", &Jet3_btagCSV, &b_Jet3_btagCSV);
   fChain->SetBranchAddress("Jet3_leg_pt", &Jet3_leg_pt, &b_Jet3_leg_pt);
   fChain->SetBranchAddress("Jet3_leg_eta", &Jet3_leg_eta, &b_Jet3_leg_eta);
   fChain->SetBranchAddress("Jet3_leg_phi", &Jet3_leg_phi, &b_Jet3_leg_phi);
   fChain->SetBranchAddress("Jet3_leg_mass", &Jet3_leg_mass, &b_Jet3_leg_mass);
   fChain->SetBranchAddress("Jet3_leg_energy", &Jet3_leg_energy, &b_Jet3_leg_energy);
   fChain->SetBranchAddress("Jet3_mcMatchId", &Jet3_mcMatchId, &b_Jet3_mcMatchId);
   fChain->SetBranchAddress("Jet3_mcMatchFlav", &Jet3_mcMatchFlav, &b_Jet3_mcMatchFlav);
   fChain->SetBranchAddress("Jet4_pt", &Jet4_pt, &b_Jet4_pt);
   fChain->SetBranchAddress("Jet4_eta", &Jet4_eta, &b_Jet4_eta);
   fChain->SetBranchAddress("Jet4_phi", &Jet4_phi, &b_Jet4_phi);
   fChain->SetBranchAddress("Jet4_mass", &Jet4_mass, &b_Jet4_mass);
   fChain->SetBranchAddress("Jet4_energy", &Jet4_energy, &b_Jet4_energy);
   fChain->SetBranchAddress("Jet4_btagCSV", &Jet4_btagCSV, &b_Jet4_btagCSV);
   fChain->SetBranchAddress("Jet4_leg_pt", &Jet4_leg_pt, &b_Jet4_leg_pt);
   fChain->SetBranchAddress("Jet4_leg_eta", &Jet4_leg_eta, &b_Jet4_leg_eta);
   fChain->SetBranchAddress("Jet4_leg_phi", &Jet4_leg_phi, &b_Jet4_leg_phi);
   fChain->SetBranchAddress("Jet4_leg_mass", &Jet4_leg_mass, &b_Jet4_leg_mass);
   fChain->SetBranchAddress("Jet4_leg_energy", &Jet4_leg_energy, &b_Jet4_leg_energy);
   fChain->SetBranchAddress("Jet4_mcMatchId", &Jet4_mcMatchId, &b_Jet4_mcMatchId);
   fChain->SetBranchAddress("Jet4_mcMatchFlav", &Jet4_mcMatchFlav, &b_Jet4_mcMatchFlav);
   fChain->SetBranchAddress("Jet5_pt", &Jet5_pt, &b_Jet5_pt);
   fChain->SetBranchAddress("Jet5_eta", &Jet5_eta, &b_Jet5_eta);
   fChain->SetBranchAddress("Jet5_phi", &Jet5_phi, &b_Jet5_phi);
   fChain->SetBranchAddress("Jet5_mass", &Jet5_mass, &b_Jet5_mass);
   fChain->SetBranchAddress("Jet5_energy", &Jet5_energy, &b_Jet5_energy);
   fChain->SetBranchAddress("Jet5_btagCSV", &Jet5_btagCSV, &b_Jet5_btagCSV);
   fChain->SetBranchAddress("Jet5_leg_pt", &Jet5_leg_pt, &b_Jet5_leg_pt);
   fChain->SetBranchAddress("Jet5_leg_eta", &Jet5_leg_eta, &b_Jet5_leg_eta);
   fChain->SetBranchAddress("Jet5_leg_phi", &Jet5_leg_phi, &b_Jet5_leg_phi);
   fChain->SetBranchAddress("Jet5_leg_mass", &Jet5_leg_mass, &b_Jet5_leg_mass);
   fChain->SetBranchAddress("Jet5_leg_energy", &Jet5_leg_energy, &b_Jet5_leg_energy);
   fChain->SetBranchAddress("Jet5_mcMatchId", &Jet5_mcMatchId, &b_Jet5_mcMatchId);
   fChain->SetBranchAddress("Jet5_mcMatchFlav", &Jet5_mcMatchFlav, &b_Jet5_mcMatchFlav);
   fChain->SetBranchAddress("Jet6_pt", &Jet6_pt, &b_Jet6_pt);
   fChain->SetBranchAddress("Jet6_eta", &Jet6_eta, &b_Jet6_eta);
   fChain->SetBranchAddress("Jet6_phi", &Jet6_phi, &b_Jet6_phi);
   fChain->SetBranchAddress("Jet6_mass", &Jet6_mass, &b_Jet6_mass);
   fChain->SetBranchAddress("Jet6_energy", &Jet6_energy, &b_Jet6_energy);
   fChain->SetBranchAddress("Jet6_btagCSV", &Jet6_btagCSV, &b_Jet6_btagCSV);
   fChain->SetBranchAddress("Jet6_leg_pt", &Jet6_leg_pt, &b_Jet6_leg_pt);
   fChain->SetBranchAddress("Jet6_leg_eta", &Jet6_leg_eta, &b_Jet6_leg_eta);
   fChain->SetBranchAddress("Jet6_leg_phi", &Jet6_leg_phi, &b_Jet6_leg_phi);
   fChain->SetBranchAddress("Jet6_leg_mass", &Jet6_leg_mass, &b_Jet6_leg_mass);
   fChain->SetBranchAddress("Jet6_leg_energy", &Jet6_leg_energy, &b_Jet6_leg_energy);
   fChain->SetBranchAddress("Jet6_mcMatchId", &Jet6_mcMatchId, &b_Jet6_mcMatchId);
   fChain->SetBranchAddress("Jet6_mcMatchFlav", &Jet6_mcMatchFlav, &b_Jet6_mcMatchFlav);
   fChain->SetBranchAddress("Jet7_pt", &Jet7_pt, &b_Jet7_pt);
   fChain->SetBranchAddress("Jet7_eta", &Jet7_eta, &b_Jet7_eta);
   fChain->SetBranchAddress("Jet7_phi", &Jet7_phi, &b_Jet7_phi);
   fChain->SetBranchAddress("Jet7_mass", &Jet7_mass, &b_Jet7_mass);
   fChain->SetBranchAddress("Jet7_energy", &Jet7_energy, &b_Jet7_energy);
   fChain->SetBranchAddress("Jet7_btagCSV", &Jet7_btagCSV, &b_Jet7_btagCSV);
   fChain->SetBranchAddress("Jet7_leg_pt", &Jet7_leg_pt, &b_Jet7_leg_pt);
   fChain->SetBranchAddress("Jet7_leg_eta", &Jet7_leg_eta, &b_Jet7_leg_eta);
   fChain->SetBranchAddress("Jet7_leg_phi", &Jet7_leg_phi, &b_Jet7_leg_phi);
   fChain->SetBranchAddress("Jet7_leg_mass", &Jet7_leg_mass, &b_Jet7_leg_mass);
   fChain->SetBranchAddress("Jet7_leg_energy", &Jet7_leg_energy, &b_Jet7_leg_energy);
   fChain->SetBranchAddress("Jet7_mcMatchId", &Jet7_mcMatchId, &b_Jet7_mcMatchId);
   fChain->SetBranchAddress("Jet7_mcMatchFlav", &Jet7_mcMatchFlav, &b_Jet7_mcMatchFlav);
   fChain->SetBranchAddress("Jet8_pt", &Jet8_pt, &b_Jet8_pt);
   fChain->SetBranchAddress("Jet8_eta", &Jet8_eta, &b_Jet8_eta);
   fChain->SetBranchAddress("Jet8_phi", &Jet8_phi, &b_Jet8_phi);
   fChain->SetBranchAddress("Jet8_mass", &Jet8_mass, &b_Jet8_mass);
   fChain->SetBranchAddress("Jet8_energy", &Jet8_energy, &b_Jet8_energy);
   fChain->SetBranchAddress("Jet8_btagCSV", &Jet8_btagCSV, &b_Jet8_btagCSV);
   fChain->SetBranchAddress("Jet8_leg_pt", &Jet8_leg_pt, &b_Jet8_leg_pt);
   fChain->SetBranchAddress("Jet8_leg_eta", &Jet8_leg_eta, &b_Jet8_leg_eta);
   fChain->SetBranchAddress("Jet8_leg_phi", &Jet8_leg_phi, &b_Jet8_leg_phi);
   fChain->SetBranchAddress("Jet8_leg_mass", &Jet8_leg_mass, &b_Jet8_leg_mass);
   fChain->SetBranchAddress("Jet8_leg_energy", &Jet8_leg_energy, &b_Jet8_leg_energy);
   fChain->SetBranchAddress("Jet8_mcMatchId", &Jet8_mcMatchId, &b_Jet8_mcMatchId);
   fChain->SetBranchAddress("Jet8_mcMatchFlav", &Jet8_mcMatchFlav, &b_Jet8_mcMatchFlav);
   fChain->SetBranchAddress("nBJetLoose25", &nBJetLoose25, &b_nBJetLoose25);
   fChain->SetBranchAddress("nBJetMedium25", &nBJetMedium25, &b_nBJetMedium25);
   fChain->SetBranchAddress("nBJetLoose30", &nBJetLoose30, &b_nBJetLoose30);
   fChain->SetBranchAddress("nBJetMedium30", &nBJetMedium30, &b_nBJetMedium30);
   fChain->SetBranchAddress("met", &met, &b_met);
   fChain->SetBranchAddress("metSignificance", &metSignificance, &b_metSignificance);
   fChain->SetBranchAddress("projMetAll1S", &projMetAll1S, &b_projMetAll1S);
   fChain->SetBranchAddress("projMetAll2S", &projMetAll2S, &b_projMetAll2S);
   fChain->SetBranchAddress("projMetJet1S", &projMetJet1S, &b_projMetJet1S);
   fChain->SetBranchAddress("projMetJet2S", &projMetJet2S, &b_projMetJet2S);
   fChain->SetBranchAddress("htJet30", &htJet30, &b_htJet30);
   fChain->SetBranchAddress("htJet25", &htJet25, &b_htJet25);
   fChain->SetBranchAddress("mhtJet30", &mhtJet30, &b_mhtJet30);
   fChain->SetBranchAddress("mhtJet25", &mhtJet25, &b_mhtJet25);
   fChain->SetBranchAddress("mZ1", &mZ1, &b_mZ1);
   fChain->SetBranchAddress("mZ2", &mZ2, &b_mZ2);
   fChain->SetBranchAddress("minMllSFOS", &minMllSFOS, &b_minMllSFOS);
   fChain->SetBranchAddress("minMllAFOS", &minMllAFOS, &b_minMllAFOS);
   fChain->SetBranchAddress("minMllAFAS", &minMllAFAS, &b_minMllAFAS);
   fChain->SetBranchAddress("GenHiggsDecayMode", &GenHiggsDecayMode, &b_GenHiggsDecayMode);
   fChain->SetBranchAddress("GenHiggs_pt", &GenHiggs_pt, &b_GenHiggs_pt);
   fChain->SetBranchAddress("GenHiggs_eta", &GenHiggs_eta, &b_GenHiggs_eta);
   fChain->SetBranchAddress("GenHiggs_phi", &GenHiggs_phi, &b_GenHiggs_phi);
   fChain->SetBranchAddress("GenHiggs_mass", &GenHiggs_mass, &b_GenHiggs_mass);
   fChain->SetBranchAddress("GenHiggs_energy", &GenHiggs_energy, &b_GenHiggs_energy);
   fChain->SetBranchAddress("GenHiggs_pdgId", &GenHiggs_pdgId, &b_GenHiggs_pdgId);
   fChain->SetBranchAddress("GenHiggs_charge", &GenHiggs_charge, &b_GenHiggs_charge);
   fChain->SetBranchAddress("GenTop1_pt", &GenTop1_pt, &b_GenTop1_pt);
   fChain->SetBranchAddress("GenTop1_eta", &GenTop1_eta, &b_GenTop1_eta);
   fChain->SetBranchAddress("GenTop1_phi", &GenTop1_phi, &b_GenTop1_phi);
   fChain->SetBranchAddress("GenTop1_mass", &GenTop1_mass, &b_GenTop1_mass);
   fChain->SetBranchAddress("GenTop1_energy", &GenTop1_energy, &b_GenTop1_energy);
   fChain->SetBranchAddress("GenTop1_pdgId", &GenTop1_pdgId, &b_GenTop1_pdgId);
   fChain->SetBranchAddress("GenTop1_charge", &GenTop1_charge, &b_GenTop1_charge);
   fChain->SetBranchAddress("GenTop2_pt", &GenTop2_pt, &b_GenTop2_pt);
   fChain->SetBranchAddress("GenTop2_eta", &GenTop2_eta, &b_GenTop2_eta);
   fChain->SetBranchAddress("GenTop2_phi", &GenTop2_phi, &b_GenTop2_phi);
   fChain->SetBranchAddress("GenTop2_mass", &GenTop2_mass, &b_GenTop2_mass);
   fChain->SetBranchAddress("GenTop2_energy", &GenTop2_energy, &b_GenTop2_energy);
   fChain->SetBranchAddress("GenTop2_pdgId", &GenTop2_pdgId, &b_GenTop2_pdgId);
   fChain->SetBranchAddress("GenTop2_charge", &GenTop2_charge, &b_GenTop2_charge);
   fChain->SetBranchAddress("nGenLeps", &nGenLeps, &b_nGenLeps);
   fChain->SetBranchAddress("nGenLepsFromTau", &nGenLepsFromTau, &b_nGenLepsFromTau);
   fChain->SetBranchAddress("nGenQuarks", &nGenQuarks, &b_nGenQuarks);
   fChain->SetBranchAddress("nGenBQuarks", &nGenBQuarks, &b_nGenBQuarks);
   fChain->SetBranchAddress("GenLep1_pt", &GenLep1_pt, &b_GenLep1_pt);
   fChain->SetBranchAddress("GenLep1_eta", &GenLep1_eta, &b_GenLep1_eta);
   fChain->SetBranchAddress("GenLep1_phi", &GenLep1_phi, &b_GenLep1_phi);
   fChain->SetBranchAddress("GenLep1_mass", &GenLep1_mass, &b_GenLep1_mass);
   fChain->SetBranchAddress("GenLep1_energy", &GenLep1_energy, &b_GenLep1_energy);
   fChain->SetBranchAddress("GenLep1_pdgId", &GenLep1_pdgId, &b_GenLep1_pdgId);
   fChain->SetBranchAddress("GenLep1_charge", &GenLep1_charge, &b_GenLep1_charge);
   fChain->SetBranchAddress("GenLep1_sourceId", &GenLep1_sourceId, &b_GenLep1_sourceId);
   fChain->SetBranchAddress("GenLepFromTau1_pt", &GenLepFromTau1_pt, &b_GenLepFromTau1_pt);
   fChain->SetBranchAddress("GenLepFromTau1_eta", &GenLepFromTau1_eta, &b_GenLepFromTau1_eta);
   fChain->SetBranchAddress("GenLepFromTau1_phi", &GenLepFromTau1_phi, &b_GenLepFromTau1_phi);
   fChain->SetBranchAddress("GenLepFromTau1_mass", &GenLepFromTau1_mass, &b_GenLepFromTau1_mass);
   fChain->SetBranchAddress("GenLepFromTau1_energy", &GenLepFromTau1_energy, &b_GenLepFromTau1_energy);
   fChain->SetBranchAddress("GenLepFromTau1_pdgId", &GenLepFromTau1_pdgId, &b_GenLepFromTau1_pdgId);
   fChain->SetBranchAddress("GenLepFromTau1_charge", &GenLepFromTau1_charge, &b_GenLepFromTau1_charge);
   fChain->SetBranchAddress("GenLepFromTau1_sourceId", &GenLepFromTau1_sourceId, &b_GenLepFromTau1_sourceId);
   fChain->SetBranchAddress("GenQuark1_pt", &GenQuark1_pt, &b_GenQuark1_pt);
   fChain->SetBranchAddress("GenQuark1_eta", &GenQuark1_eta, &b_GenQuark1_eta);
   fChain->SetBranchAddress("GenQuark1_phi", &GenQuark1_phi, &b_GenQuark1_phi);
   fChain->SetBranchAddress("GenQuark1_mass", &GenQuark1_mass, &b_GenQuark1_mass);
   fChain->SetBranchAddress("GenQuark1_energy", &GenQuark1_energy, &b_GenQuark1_energy);
   fChain->SetBranchAddress("GenQuark1_pdgId", &GenQuark1_pdgId, &b_GenQuark1_pdgId);
   fChain->SetBranchAddress("GenQuark1_charge", &GenQuark1_charge, &b_GenQuark1_charge);
   fChain->SetBranchAddress("GenQuark1_sourceId", &GenQuark1_sourceId, &b_GenQuark1_sourceId);
   fChain->SetBranchAddress("GenLep2_pt", &GenLep2_pt, &b_GenLep2_pt);
   fChain->SetBranchAddress("GenLep2_eta", &GenLep2_eta, &b_GenLep2_eta);
   fChain->SetBranchAddress("GenLep2_phi", &GenLep2_phi, &b_GenLep2_phi);
   fChain->SetBranchAddress("GenLep2_mass", &GenLep2_mass, &b_GenLep2_mass);
   fChain->SetBranchAddress("GenLep2_energy", &GenLep2_energy, &b_GenLep2_energy);
   fChain->SetBranchAddress("GenLep2_pdgId", &GenLep2_pdgId, &b_GenLep2_pdgId);
   fChain->SetBranchAddress("GenLep2_charge", &GenLep2_charge, &b_GenLep2_charge);
   fChain->SetBranchAddress("GenLep2_sourceId", &GenLep2_sourceId, &b_GenLep2_sourceId);
   fChain->SetBranchAddress("GenLepFromTau2_pt", &GenLepFromTau2_pt, &b_GenLepFromTau2_pt);
   fChain->SetBranchAddress("GenLepFromTau2_eta", &GenLepFromTau2_eta, &b_GenLepFromTau2_eta);
   fChain->SetBranchAddress("GenLepFromTau2_phi", &GenLepFromTau2_phi, &b_GenLepFromTau2_phi);
   fChain->SetBranchAddress("GenLepFromTau2_mass", &GenLepFromTau2_mass, &b_GenLepFromTau2_mass);
   fChain->SetBranchAddress("GenLepFromTau2_energy", &GenLepFromTau2_energy, &b_GenLepFromTau2_energy);
   fChain->SetBranchAddress("GenLepFromTau2_pdgId", &GenLepFromTau2_pdgId, &b_GenLepFromTau2_pdgId);
   fChain->SetBranchAddress("GenLepFromTau2_charge", &GenLepFromTau2_charge, &b_GenLepFromTau2_charge);
   fChain->SetBranchAddress("GenLepFromTau2_sourceId", &GenLepFromTau2_sourceId, &b_GenLepFromTau2_sourceId);
   fChain->SetBranchAddress("GenQuark2_pt", &GenQuark2_pt, &b_GenQuark2_pt);
   fChain->SetBranchAddress("GenQuark2_eta", &GenQuark2_eta, &b_GenQuark2_eta);
   fChain->SetBranchAddress("GenQuark2_phi", &GenQuark2_phi, &b_GenQuark2_phi);
   fChain->SetBranchAddress("GenQuark2_mass", &GenQuark2_mass, &b_GenQuark2_mass);
   fChain->SetBranchAddress("GenQuark2_energy", &GenQuark2_energy, &b_GenQuark2_energy);
   fChain->SetBranchAddress("GenQuark2_pdgId", &GenQuark2_pdgId, &b_GenQuark2_pdgId);
   fChain->SetBranchAddress("GenQuark2_charge", &GenQuark2_charge, &b_GenQuark2_charge);
   fChain->SetBranchAddress("GenQuark2_sourceId", &GenQuark2_sourceId, &b_GenQuark2_sourceId);
   fChain->SetBranchAddress("GenLep3_pt", &GenLep3_pt, &b_GenLep3_pt);
   fChain->SetBranchAddress("GenLep3_eta", &GenLep3_eta, &b_GenLep3_eta);
   fChain->SetBranchAddress("GenLep3_phi", &GenLep3_phi, &b_GenLep3_phi);
   fChain->SetBranchAddress("GenLep3_mass", &GenLep3_mass, &b_GenLep3_mass);
   fChain->SetBranchAddress("GenLep3_energy", &GenLep3_energy, &b_GenLep3_energy);
   fChain->SetBranchAddress("GenLep3_pdgId", &GenLep3_pdgId, &b_GenLep3_pdgId);
   fChain->SetBranchAddress("GenLep3_charge", &GenLep3_charge, &b_GenLep3_charge);
   fChain->SetBranchAddress("GenLep3_sourceId", &GenLep3_sourceId, &b_GenLep3_sourceId);
   fChain->SetBranchAddress("GenLepFromTau3_pt", &GenLepFromTau3_pt, &b_GenLepFromTau3_pt);
   fChain->SetBranchAddress("GenLepFromTau3_eta", &GenLepFromTau3_eta, &b_GenLepFromTau3_eta);
   fChain->SetBranchAddress("GenLepFromTau3_phi", &GenLepFromTau3_phi, &b_GenLepFromTau3_phi);
   fChain->SetBranchAddress("GenLepFromTau3_mass", &GenLepFromTau3_mass, &b_GenLepFromTau3_mass);
   fChain->SetBranchAddress("GenLepFromTau3_energy", &GenLepFromTau3_energy, &b_GenLepFromTau3_energy);
   fChain->SetBranchAddress("GenLepFromTau3_pdgId", &GenLepFromTau3_pdgId, &b_GenLepFromTau3_pdgId);
   fChain->SetBranchAddress("GenLepFromTau3_charge", &GenLepFromTau3_charge, &b_GenLepFromTau3_charge);
   fChain->SetBranchAddress("GenLepFromTau3_sourceId", &GenLepFromTau3_sourceId, &b_GenLepFromTau3_sourceId);
   fChain->SetBranchAddress("GenQuark3_pt", &GenQuark3_pt, &b_GenQuark3_pt);
   fChain->SetBranchAddress("GenQuark3_eta", &GenQuark3_eta, &b_GenQuark3_eta);
   fChain->SetBranchAddress("GenQuark3_phi", &GenQuark3_phi, &b_GenQuark3_phi);
   fChain->SetBranchAddress("GenQuark3_mass", &GenQuark3_mass, &b_GenQuark3_mass);
   fChain->SetBranchAddress("GenQuark3_energy", &GenQuark3_energy, &b_GenQuark3_energy);
   fChain->SetBranchAddress("GenQuark3_pdgId", &GenQuark3_pdgId, &b_GenQuark3_pdgId);
   fChain->SetBranchAddress("GenQuark3_charge", &GenQuark3_charge, &b_GenQuark3_charge);
   fChain->SetBranchAddress("GenQuark3_sourceId", &GenQuark3_sourceId, &b_GenQuark3_sourceId);
   fChain->SetBranchAddress("GenLep4_pt", &GenLep4_pt, &b_GenLep4_pt);
   fChain->SetBranchAddress("GenLep4_eta", &GenLep4_eta, &b_GenLep4_eta);
   fChain->SetBranchAddress("GenLep4_phi", &GenLep4_phi, &b_GenLep4_phi);
   fChain->SetBranchAddress("GenLep4_mass", &GenLep4_mass, &b_GenLep4_mass);
   fChain->SetBranchAddress("GenLep4_energy", &GenLep4_energy, &b_GenLep4_energy);
   fChain->SetBranchAddress("GenLep4_pdgId", &GenLep4_pdgId, &b_GenLep4_pdgId);
   fChain->SetBranchAddress("GenLep4_charge", &GenLep4_charge, &b_GenLep4_charge);
   fChain->SetBranchAddress("GenLep4_sourceId", &GenLep4_sourceId, &b_GenLep4_sourceId);
   fChain->SetBranchAddress("GenLepFromTau4_pt", &GenLepFromTau4_pt, &b_GenLepFromTau4_pt);
   fChain->SetBranchAddress("GenLepFromTau4_eta", &GenLepFromTau4_eta, &b_GenLepFromTau4_eta);
   fChain->SetBranchAddress("GenLepFromTau4_phi", &GenLepFromTau4_phi, &b_GenLepFromTau4_phi);
   fChain->SetBranchAddress("GenLepFromTau4_mass", &GenLepFromTau4_mass, &b_GenLepFromTau4_mass);
   fChain->SetBranchAddress("GenLepFromTau4_energy", &GenLepFromTau4_energy, &b_GenLepFromTau4_energy);
   fChain->SetBranchAddress("GenLepFromTau4_pdgId", &GenLepFromTau4_pdgId, &b_GenLepFromTau4_pdgId);
   fChain->SetBranchAddress("GenLepFromTau4_charge", &GenLepFromTau4_charge, &b_GenLepFromTau4_charge);
   fChain->SetBranchAddress("GenLepFromTau4_sourceId", &GenLepFromTau4_sourceId, &b_GenLepFromTau4_sourceId);
   fChain->SetBranchAddress("GenQuark4_pt", &GenQuark4_pt, &b_GenQuark4_pt);
   fChain->SetBranchAddress("GenQuark4_eta", &GenQuark4_eta, &b_GenQuark4_eta);
   fChain->SetBranchAddress("GenQuark4_phi", &GenQuark4_phi, &b_GenQuark4_phi);
   fChain->SetBranchAddress("GenQuark4_mass", &GenQuark4_mass, &b_GenQuark4_mass);
   fChain->SetBranchAddress("GenQuark4_energy", &GenQuark4_energy, &b_GenQuark4_energy);
   fChain->SetBranchAddress("GenQuark4_pdgId", &GenQuark4_pdgId, &b_GenQuark4_pdgId);
   fChain->SetBranchAddress("GenQuark4_charge", &GenQuark4_charge, &b_GenQuark4_charge);
   fChain->SetBranchAddress("GenQuark4_sourceId", &GenQuark4_sourceId, &b_GenQuark4_sourceId);
   fChain->SetBranchAddress("GenLep5_pt", &GenLep5_pt, &b_GenLep5_pt);
   fChain->SetBranchAddress("GenLep5_eta", &GenLep5_eta, &b_GenLep5_eta);
   fChain->SetBranchAddress("GenLep5_phi", &GenLep5_phi, &b_GenLep5_phi);
   fChain->SetBranchAddress("GenLep5_mass", &GenLep5_mass, &b_GenLep5_mass);
   fChain->SetBranchAddress("GenLep5_energy", &GenLep5_energy, &b_GenLep5_energy);
   fChain->SetBranchAddress("GenLep5_pdgId", &GenLep5_pdgId, &b_GenLep5_pdgId);
   fChain->SetBranchAddress("GenLep5_charge", &GenLep5_charge, &b_GenLep5_charge);
   fChain->SetBranchAddress("GenLep5_sourceId", &GenLep5_sourceId, &b_GenLep5_sourceId);
   fChain->SetBranchAddress("GenLepFromTau5_pt", &GenLepFromTau5_pt, &b_GenLepFromTau5_pt);
   fChain->SetBranchAddress("GenLepFromTau5_eta", &GenLepFromTau5_eta, &b_GenLepFromTau5_eta);
   fChain->SetBranchAddress("GenLepFromTau5_phi", &GenLepFromTau5_phi, &b_GenLepFromTau5_phi);
   fChain->SetBranchAddress("GenLepFromTau5_mass", &GenLepFromTau5_mass, &b_GenLepFromTau5_mass);
   fChain->SetBranchAddress("GenLepFromTau5_energy", &GenLepFromTau5_energy, &b_GenLepFromTau5_energy);
   fChain->SetBranchAddress("GenLepFromTau5_pdgId", &GenLepFromTau5_pdgId, &b_GenLepFromTau5_pdgId);
   fChain->SetBranchAddress("GenLepFromTau5_charge", &GenLepFromTau5_charge, &b_GenLepFromTau5_charge);
   fChain->SetBranchAddress("GenLepFromTau5_sourceId", &GenLepFromTau5_sourceId, &b_GenLepFromTau5_sourceId);
   fChain->SetBranchAddress("GenQuark5_pt", &GenQuark5_pt, &b_GenQuark5_pt);
   fChain->SetBranchAddress("GenQuark5_eta", &GenQuark5_eta, &b_GenQuark5_eta);
   fChain->SetBranchAddress("GenQuark5_phi", &GenQuark5_phi, &b_GenQuark5_phi);
   fChain->SetBranchAddress("GenQuark5_mass", &GenQuark5_mass, &b_GenQuark5_mass);
   fChain->SetBranchAddress("GenQuark5_energy", &GenQuark5_energy, &b_GenQuark5_energy);
   fChain->SetBranchAddress("GenQuark5_pdgId", &GenQuark5_pdgId, &b_GenQuark5_pdgId);
   fChain->SetBranchAddress("GenQuark5_charge", &GenQuark5_charge, &b_GenQuark5_charge);
   fChain->SetBranchAddress("GenQuark5_sourceId", &GenQuark5_sourceId, &b_GenQuark5_sourceId);
   fChain->SetBranchAddress("GenLep6_pt", &GenLep6_pt, &b_GenLep6_pt);
   fChain->SetBranchAddress("GenLep6_eta", &GenLep6_eta, &b_GenLep6_eta);
   fChain->SetBranchAddress("GenLep6_phi", &GenLep6_phi, &b_GenLep6_phi);
   fChain->SetBranchAddress("GenLep6_mass", &GenLep6_mass, &b_GenLep6_mass);
   fChain->SetBranchAddress("GenLep6_energy", &GenLep6_energy, &b_GenLep6_energy);
   fChain->SetBranchAddress("GenLep6_pdgId", &GenLep6_pdgId, &b_GenLep6_pdgId);
   fChain->SetBranchAddress("GenLep6_charge", &GenLep6_charge, &b_GenLep6_charge);
   fChain->SetBranchAddress("GenLep6_sourceId", &GenLep6_sourceId, &b_GenLep6_sourceId);
   fChain->SetBranchAddress("GenLepFromTau6_pt", &GenLepFromTau6_pt, &b_GenLepFromTau6_pt);
   fChain->SetBranchAddress("GenLepFromTau6_eta", &GenLepFromTau6_eta, &b_GenLepFromTau6_eta);
   fChain->SetBranchAddress("GenLepFromTau6_phi", &GenLepFromTau6_phi, &b_GenLepFromTau6_phi);
   fChain->SetBranchAddress("GenLepFromTau6_mass", &GenLepFromTau6_mass, &b_GenLepFromTau6_mass);
   fChain->SetBranchAddress("GenLepFromTau6_energy", &GenLepFromTau6_energy, &b_GenLepFromTau6_energy);
   fChain->SetBranchAddress("GenLepFromTau6_pdgId", &GenLepFromTau6_pdgId, &b_GenLepFromTau6_pdgId);
   fChain->SetBranchAddress("GenLepFromTau6_charge", &GenLepFromTau6_charge, &b_GenLepFromTau6_charge);
   fChain->SetBranchAddress("GenLepFromTau6_sourceId", &GenLepFromTau6_sourceId, &b_GenLepFromTau6_sourceId);
   fChain->SetBranchAddress("GenQuark6_pt", &GenQuark6_pt, &b_GenQuark6_pt);
   fChain->SetBranchAddress("GenQuark6_eta", &GenQuark6_eta, &b_GenQuark6_eta);
   fChain->SetBranchAddress("GenQuark6_phi", &GenQuark6_phi, &b_GenQuark6_phi);
   fChain->SetBranchAddress("GenQuark6_mass", &GenQuark6_mass, &b_GenQuark6_mass);
   fChain->SetBranchAddress("GenQuark6_energy", &GenQuark6_energy, &b_GenQuark6_energy);
   fChain->SetBranchAddress("GenQuark6_pdgId", &GenQuark6_pdgId, &b_GenQuark6_pdgId);
   fChain->SetBranchAddress("GenQuark6_charge", &GenQuark6_charge, &b_GenQuark6_charge);
   fChain->SetBranchAddress("GenQuark6_sourceId", &GenQuark6_sourceId, &b_GenQuark6_sourceId);
   fChain->SetBranchAddress("GenBQuark1_pt", &GenBQuark1_pt, &b_GenBQuark1_pt);
   fChain->SetBranchAddress("GenBQuark1_eta", &GenBQuark1_eta, &b_GenBQuark1_eta);
   fChain->SetBranchAddress("GenBQuark1_phi", &GenBQuark1_phi, &b_GenBQuark1_phi);
   fChain->SetBranchAddress("GenBQuark1_mass", &GenBQuark1_mass, &b_GenBQuark1_mass);
   fChain->SetBranchAddress("GenBQuark1_energy", &GenBQuark1_energy, &b_GenBQuark1_energy);
   fChain->SetBranchAddress("GenBQuark1_pdgId", &GenBQuark1_pdgId, &b_GenBQuark1_pdgId);
   fChain->SetBranchAddress("GenBQuark1_charge", &GenBQuark1_charge, &b_GenBQuark1_charge);
   fChain->SetBranchAddress("GenBQuark2_pt", &GenBQuark2_pt, &b_GenBQuark2_pt);
   fChain->SetBranchAddress("GenBQuark2_eta", &GenBQuark2_eta, &b_GenBQuark2_eta);
   fChain->SetBranchAddress("GenBQuark2_phi", &GenBQuark2_phi, &b_GenBQuark2_phi);
   fChain->SetBranchAddress("GenBQuark2_mass", &GenBQuark2_mass, &b_GenBQuark2_mass);
   fChain->SetBranchAddress("GenBQuark2_energy", &GenBQuark2_energy, &b_GenBQuark2_energy);
   fChain->SetBranchAddress("GenBQuark2_pdgId", &GenBQuark2_pdgId, &b_GenBQuark2_pdgId);
   fChain->SetBranchAddress("GenBQuark2_charge", &GenBQuark2_charge, &b_GenBQuark2_charge);
   fChain->SetBranchAddress("nGoodLepsMatchId", &nGoodLepsMatchId, &b_nGoodLepsMatchId);
   fChain->SetBranchAddress("nGoodLepsMatchAny", &nGoodLepsMatchAny, &b_nGoodLepsMatchAny);
   Notify();
}

Bool_t TTHTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void TTHTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t TTHTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef TTHTree_cxx
