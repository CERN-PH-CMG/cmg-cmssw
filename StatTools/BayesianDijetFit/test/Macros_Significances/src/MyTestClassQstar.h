//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Jun  5 15:32:37 2012 by ROOT version 5.32/01
// from TTree tree/
// found on file: QstarToJJ_M-1200_TuneD6T_7TeV_pythia6__Fall11-PU_S6_START42_V14B-v3__GEN-RAW_1_1_bbb.root
//////////////////////////////////////////////////////////

#ifndef MyTestClass_h
#define MyTestClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <string>
#include <vector>
#include <vector>
#include <vector>
#include <vector>

// Fixed size dimensions of array or collections stored in the TTree if any.

class MyTestClass {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   string          *HLTKey;
   vector<string>  *HLTTriggerNames;
   Bool_t          isData;
   vector<bool>    *HLTPFJetPassLooseID;
   vector<bool>    *HLTPFJetPassTightID;
   vector<bool>    *HLTPixelVertexIsValid;
   vector<bool>    *HLTTriggerDecisions;
   Double_t        PtHat;
   Double_t        Weight;
   Double_t        time;
   vector<double>  *PDFWeights;
   vector<double>  *GenParticleEnergy;
   vector<double>  *GenParticleEta;
   vector<double>  *GenParticleP;
   vector<double>  *GenParticlePhi;
   vector<double>  *GenParticlePt;
   vector<double>  *GenParticlePx;
   vector<double>  *GenParticlePy;
   vector<double>  *GenParticlePz;
   vector<double>  *GenParticleVX;
   vector<double>  *GenParticleVY;
   vector<double>  *GenParticleVZ;
   vector<double>  *HLTCaloJetCorrEmEnergyInEB;
   vector<double>  *HLTCaloJetCorrEmEnergyInEE;
   vector<double>  *HLTCaloJetCorrEmEnergyInHF;
   vector<double>  *HLTCaloJetCorrEnergy;
   vector<double>  *HLTCaloJetCorrEnergyFractionEm;
   vector<double>  *HLTCaloJetCorrEnergyFractionHadronic;
   vector<double>  *HLTCaloJetCorrEta;
   vector<double>  *HLTCaloJetCorrHadEnergyInHB;
   vector<double>  *HLTCaloJetCorrHadEnergyInHE;
   vector<double>  *HLTCaloJetCorrHadEnergyInHF;
   vector<double>  *HLTCaloJetCorrHadEnergyInHO;
   vector<double>  *HLTCaloJetCorrMaxEInEmTowers;
   vector<double>  *HLTCaloJetCorrMaxEInHadTowers;
   vector<double>  *HLTCaloJetCorrPhi;
   vector<double>  *HLTCaloJetCorrPt;
   vector<double>  *HLTCaloJetCorrTowersArea;
   vector<double>  *HLTCaloJetRawEmEnergyInEB;
   vector<double>  *HLTCaloJetRawEmEnergyInEE;
   vector<double>  *HLTCaloJetRawEmEnergyInHF;
   vector<double>  *HLTCaloJetRawEnergy;
   vector<double>  *HLTCaloJetRawEnergyFractionEm;
   vector<double>  *HLTCaloJetRawEnergyFractionHadronic;
   vector<double>  *HLTCaloJetRawEta;
   vector<double>  *HLTCaloJetRawHadEnergyInHB;
   vector<double>  *HLTCaloJetRawHadEnergyInHE;
   vector<double>  *HLTCaloJetRawHadEnergyInHF;
   vector<double>  *HLTCaloJetRawHadEnergyInHO;
   vector<double>  *HLTCaloJetRawMaxEInEmTowers;
   vector<double>  *HLTCaloJetRawMaxEInHadTowers;
   vector<double>  *HLTCaloJetRawPhi;
   vector<double>  *HLTCaloJetRawPt;
   vector<double>  *HLTCaloJetRawTowersArea;
   vector<double>  *HLTPFJetChargedEmEnergyFraction;
   vector<double>  *HLTPFJetChargedHadronEnergyFraction;
   vector<double>  *HLTPFJetChargedMuEnergyFraction;
   vector<double>  *HLTPFJetElectronEnergyFraction;
   vector<double>  *HLTPFJetEnergy;
   vector<double>  *HLTPFJetEta;
   vector<double>  *HLTPFJetHFEMEnergyFraction;
   vector<double>  *HLTPFJetHFHadronEnergyFraction;
   vector<double>  *HLTPFJetMuonEnergyFraction;
   vector<double>  *HLTPFJetNeutralEmEnergyFraction;
   vector<double>  *HLTPFJetNeutralHadronEnergyFraction;
   vector<double>  *HLTPFJetPhi;
   vector<double>  *HLTPFJetPhotonEnergyFraction;
   vector<double>  *HLTPFJetPt;
   vector<double>  *HLTPixelVertexChi2;
   vector<double>  *HLTPixelVertexNdof;
   vector<double>  *HLTPixelVertexNormChi2;
   vector<double>  *HLTPixelVertexXCoord;
   vector<double>  *HLTPixelVertexXError;
   vector<double>  *HLTPixelVertexYCoord;
   vector<double>  *HLTPixelVertexYError;
   vector<double>  *HLTPixelVertexZCoord;
   vector<double>  *HLTPixelVertexZError;
   vector<int>     *PileUpInteractions;
   vector<int>     *PileUpOriginBX;
   vector<int>     *GenParticleMotherIndex;
   vector<int>     *GenParticleNumDaught;
   vector<int>     *GenParticlePdgId;
   vector<int>     *GenParticleStatus;
   vector<int>     *HLTPFJetChargedHadronMultiplicity;
   vector<int>     *HLTPFJetChargedMultiplicity;
   vector<int>     *HLTPFJetElectronMultiplicity;
   vector<int>     *HLTPFJetHFEMMultiplicity;
   vector<int>     *HLTPFJetHFHadronMultiplicity;
   vector<int>     *HLTPFJetMuonMultiplicity;
   vector<int>     *HLTPFJetNConstituents;
   vector<int>     *HLTPFJetNeutralHadronMultiplicity;
   vector<int>     *HLTPFJetNeutralMultiplicity;
   vector<int>     *HLTPFJetPhotonMultiplicity;
   UInt_t          ProcessID;
   UInt_t          bunch;
   UInt_t          event;
   UInt_t          ls;
   UInt_t          orbit;
   UInt_t          run;

   // List of branches
   TBranch        *b_HLTKey;   //!
   TBranch        *b_HLTTriggerNames;   //!
   TBranch        *b_isData;   //!
   TBranch        *b_HLTPFJetPassLooseID;   //!
   TBranch        *b_HLTPFJetPassTightID;   //!
   TBranch        *b_HLTPixelVertexIsValid;   //!
   TBranch        *b_HLTTriggerDecisions;   //!
   TBranch        *b_PtHat;   //!
   TBranch        *b_Weight;   //!
   TBranch        *b_time;   //!
   TBranch        *b_PDFWeights;   //!
   TBranch        *b_GenParticleEnergy;   //!
   TBranch        *b_GenParticleEta;   //!
   TBranch        *b_GenParticleP;   //!
   TBranch        *b_GenParticlePhi;   //!
   TBranch        *b_GenParticlePt;   //!
   TBranch        *b_GenParticlePx;   //!
   TBranch        *b_GenParticlePy;   //!
   TBranch        *b_GenParticlePz;   //!
   TBranch        *b_GenParticleVX;   //!
   TBranch        *b_GenParticleVY;   //!
   TBranch        *b_GenParticleVZ;   //!
   TBranch        *b_HLTCaloJetCorrEmEnergyInEB;   //!
   TBranch        *b_HLTCaloJetCorrEmEnergyInEE;   //!
   TBranch        *b_HLTCaloJetCorrEmEnergyInHF;   //!
   TBranch        *b_HLTCaloJetCorrEnergy;   //!
   TBranch        *b_HLTCaloJetCorrEnergyFractionEm;   //!
   TBranch        *b_HLTCaloJetCorrEnergyFractionHadronic;   //!
   TBranch        *b_HLTCaloJetCorrEta;   //!
   TBranch        *b_HLTCaloJetCorrHadEnergyInHB;   //!
   TBranch        *b_HLTCaloJetCorrHadEnergyInHE;   //!
   TBranch        *b_HLTCaloJetCorrHadEnergyInHF;   //!
   TBranch        *b_HLTCaloJetCorrHadEnergyInHO;   //!
   TBranch        *b_HLTCaloJetCorrMaxEInEmTowers;   //!
   TBranch        *b_HLTCaloJetCorrMaxEInHadTowers;   //!
   TBranch        *b_HLTCaloJetCorrPhi;   //!
   TBranch        *b_HLTCaloJetCorrPt;   //!
   TBranch        *b_HLTCaloJetCorrTowersArea;   //!
   TBranch        *b_HLTCaloJetRawEmEnergyInEB;   //!
   TBranch        *b_HLTCaloJetRawEmEnergyInEE;   //!
   TBranch        *b_HLTCaloJetRawEmEnergyInHF;   //!
   TBranch        *b_HLTCaloJetRawEnergy;   //!
   TBranch        *b_HLTCaloJetRawEnergyFractionEm;   //!
   TBranch        *b_HLTCaloJetRawEnergyFractionHadronic;   //!
   TBranch        *b_HLTCaloJetRawEta;   //!
   TBranch        *b_HLTCaloJetRawHadEnergyInHB;   //!
   TBranch        *b_HLTCaloJetRawHadEnergyInHE;   //!
   TBranch        *b_HLTCaloJetRawHadEnergyInHF;   //!
   TBranch        *b_HLTCaloJetRawHadEnergyInHO;   //!
   TBranch        *b_HLTCaloJetRawMaxEInEmTowers;   //!
   TBranch        *b_HLTCaloJetRawMaxEInHadTowers;   //!
   TBranch        *b_HLTCaloJetRawPhi;   //!
   TBranch        *b_HLTCaloJetRawPt;   //!
   TBranch        *b_HLTCaloJetRawTowersArea;   //!
   TBranch        *b_HLTPFJetChargedEmEnergyFraction;   //!
   TBranch        *b_HLTPFJetChargedHadronEnergyFraction;   //!
   TBranch        *b_HLTPFJetChargedMuEnergyFraction;   //!
   TBranch        *b_HLTPFJetElectronEnergyFraction;   //!
   TBranch        *b_HLTPFJetEnergy;   //!
   TBranch        *b_HLTPFJetEta;   //!
   TBranch        *b_HLTPFJetHFEMEnergyFraction;   //!
   TBranch        *b_HLTPFJetHFHadronEnergyFraction;   //!
   TBranch        *b_HLTPFJetMuonEnergyFraction;   //!
   TBranch        *b_HLTPFJetNeutralEmEnergyFraction;   //!
   TBranch        *b_HLTPFJetNeutralHadronEnergyFraction;   //!
   TBranch        *b_HLTPFJetPhi;   //!
   TBranch        *b_HLTPFJetPhotonEnergyFraction;   //!
   TBranch        *b_HLTPFJetPt;   //!
   TBranch        *b_HLTPixelVertexChi2;   //!
   TBranch        *b_HLTPixelVertexNdof;   //!
   TBranch        *b_HLTPixelVertexNormChi2;   //!
   TBranch        *b_HLTPixelVertexXCoord;   //!
   TBranch        *b_HLTPixelVertexXError;   //!
   TBranch        *b_HLTPixelVertexYCoord;   //!
   TBranch        *b_HLTPixelVertexYError;   //!
   TBranch        *b_HLTPixelVertexZCoord;   //!
   TBranch        *b_HLTPixelVertexZError;   //!
   TBranch        *b_PileUpInteractions;   //!
   TBranch        *b_PileUpOriginBX;   //!
   TBranch        *b_GenParticleMotherIndex;   //!
   TBranch        *b_GenParticleNumDaught;   //!
   TBranch        *b_GenParticlePdgId;   //!
   TBranch        *b_GenParticleStatus;   //!
   TBranch        *b_HLTPFJetChargedHadronMultiplicity;   //!
   TBranch        *b_HLTPFJetChargedMultiplicity;   //!
   TBranch        *b_HLTPFJetElectronMultiplicity;   //!
   TBranch        *b_HLTPFJetHFEMMultiplicity;   //!
   TBranch        *b_HLTPFJetHFHadronMultiplicity;   //!
   TBranch        *b_HLTPFJetMuonMultiplicity;   //!
   TBranch        *b_HLTPFJetNConstituents;   //!
   TBranch        *b_HLTPFJetNeutralHadronMultiplicity;   //!
   TBranch        *b_HLTPFJetNeutralMultiplicity;   //!
   TBranch        *b_HLTPFJetPhotonMultiplicity;   //!
   TBranch        *b_ProcessID;   //!
   TBranch        *b_bunch;   //!
   TBranch        *b_event;   //!
   TBranch        *b_ls;   //!
   TBranch        *b_orbit;   //!
   TBranch        *b_run;   //!

   MyTestClass(TTree *tree=0);
   virtual ~MyTestClass();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef MyTestClass_cxx
MyTestClass::MyTestClass(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("QstarToJJ_M-1200_TuneD6T_7TeV_pythia6__Fall11-PU_S6_START42_V14B-v3__GEN-RAW_1_1_bbb.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("QstarToJJ_M-1200_TuneD6T_7TeV_pythia6__Fall11-PU_S6_START42_V14B-v3__GEN-RAW_1_1_bbb.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("QstarToJJ_M-1200_TuneD6T_7TeV_pythia6__Fall11-PU_S6_START42_V14B-v3__GEN-RAW_1_1_bbb.root:/rootTupleTree");
      dir->GetObject("tree",tree);

   }
   Init(tree);
}

MyTestClass::~MyTestClass()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t MyTestClass::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t MyTestClass::LoadTree(Long64_t entry)
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

void MyTestClass::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   HLTKey = 0;
   HLTTriggerNames = 0;
   HLTPFJetPassLooseID = 0;
   HLTPFJetPassTightID = 0;
   HLTPixelVertexIsValid = 0;
   HLTTriggerDecisions = 0;
   PDFWeights = 0;
   GenParticleEnergy = 0;
   GenParticleEta = 0;
   GenParticleP = 0;
   GenParticlePhi = 0;
   GenParticlePt = 0;
   GenParticlePx = 0;
   GenParticlePy = 0;
   GenParticlePz = 0;
   GenParticleVX = 0;
   GenParticleVY = 0;
   GenParticleVZ = 0;
   HLTCaloJetCorrEmEnergyInEB = 0;
   HLTCaloJetCorrEmEnergyInEE = 0;
   HLTCaloJetCorrEmEnergyInHF = 0;
   HLTCaloJetCorrEnergy = 0;
   HLTCaloJetCorrEnergyFractionEm = 0;
   HLTCaloJetCorrEnergyFractionHadronic = 0;
   HLTCaloJetCorrEta = 0;
   HLTCaloJetCorrHadEnergyInHB = 0;
   HLTCaloJetCorrHadEnergyInHE = 0;
   HLTCaloJetCorrHadEnergyInHF = 0;
   HLTCaloJetCorrHadEnergyInHO = 0;
   HLTCaloJetCorrMaxEInEmTowers = 0;
   HLTCaloJetCorrMaxEInHadTowers = 0;
   HLTCaloJetCorrPhi = 0;
   HLTCaloJetCorrPt = 0;
   HLTCaloJetCorrTowersArea = 0;
   HLTCaloJetRawEmEnergyInEB = 0;
   HLTCaloJetRawEmEnergyInEE = 0;
   HLTCaloJetRawEmEnergyInHF = 0;
   HLTCaloJetRawEnergy = 0;
   HLTCaloJetRawEnergyFractionEm = 0;
   HLTCaloJetRawEnergyFractionHadronic = 0;
   HLTCaloJetRawEta = 0;
   HLTCaloJetRawHadEnergyInHB = 0;
   HLTCaloJetRawHadEnergyInHE = 0;
   HLTCaloJetRawHadEnergyInHF = 0;
   HLTCaloJetRawHadEnergyInHO = 0;
   HLTCaloJetRawMaxEInEmTowers = 0;
   HLTCaloJetRawMaxEInHadTowers = 0;
   HLTCaloJetRawPhi = 0;
   HLTCaloJetRawPt = 0;
   HLTCaloJetRawTowersArea = 0;
   HLTPFJetChargedEmEnergyFraction = 0;
   HLTPFJetChargedHadronEnergyFraction = 0;
   HLTPFJetChargedMuEnergyFraction = 0;
   HLTPFJetElectronEnergyFraction = 0;
   HLTPFJetEnergy = 0;
   HLTPFJetEta = 0;
   HLTPFJetHFEMEnergyFraction = 0;
   HLTPFJetHFHadronEnergyFraction = 0;
   HLTPFJetMuonEnergyFraction = 0;
   HLTPFJetNeutralEmEnergyFraction = 0;
   HLTPFJetNeutralHadronEnergyFraction = 0;
   HLTPFJetPhi = 0;
   HLTPFJetPhotonEnergyFraction = 0;
   HLTPFJetPt = 0;
   HLTPixelVertexChi2 = 0;
   HLTPixelVertexNdof = 0;
   HLTPixelVertexNormChi2 = 0;
   HLTPixelVertexXCoord = 0;
   HLTPixelVertexXError = 0;
   HLTPixelVertexYCoord = 0;
   HLTPixelVertexYError = 0;
   HLTPixelVertexZCoord = 0;
   HLTPixelVertexZError = 0;
   PileUpInteractions = 0;
   PileUpOriginBX = 0;
   GenParticleMotherIndex = 0;
   GenParticleNumDaught = 0;
   GenParticlePdgId = 0;
   GenParticleStatus = 0;
   HLTPFJetChargedHadronMultiplicity = 0;
   HLTPFJetChargedMultiplicity = 0;
   HLTPFJetElectronMultiplicity = 0;
   HLTPFJetHFEMMultiplicity = 0;
   HLTPFJetHFHadronMultiplicity = 0;
   HLTPFJetMuonMultiplicity = 0;
   HLTPFJetNConstituents = 0;
   HLTPFJetNeutralHadronMultiplicity = 0;
   HLTPFJetNeutralMultiplicity = 0;
   HLTPFJetPhotonMultiplicity = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("HLTKey", &HLTKey, &b_HLTKey);
   fChain->SetBranchAddress("HLTTriggerNames", &HLTTriggerNames, &b_HLTTriggerNames);
   fChain->SetBranchAddress("isData", &isData, &b_isData);
   fChain->SetBranchAddress("HLTPFJetPassLooseID", &HLTPFJetPassLooseID, &b_HLTPFJetPassLooseID);
   fChain->SetBranchAddress("HLTPFJetPassTightID", &HLTPFJetPassTightID, &b_HLTPFJetPassTightID);
   fChain->SetBranchAddress("HLTPixelVertexIsValid", &HLTPixelVertexIsValid, &b_HLTPixelVertexIsValid);
   fChain->SetBranchAddress("HLTTriggerDecisions", &HLTTriggerDecisions, &b_HLTTriggerDecisions);
   fChain->SetBranchAddress("PtHat", &PtHat, &b_PtHat);
   fChain->SetBranchAddress("Weight", &Weight, &b_Weight);
   fChain->SetBranchAddress("time", &time, &b_time);
   fChain->SetBranchAddress("PDFWeights", &PDFWeights, &b_PDFWeights);
   fChain->SetBranchAddress("GenParticleEnergy", &GenParticleEnergy, &b_GenParticleEnergy);
   fChain->SetBranchAddress("GenParticleEta", &GenParticleEta, &b_GenParticleEta);
   fChain->SetBranchAddress("GenParticleP", &GenParticleP, &b_GenParticleP);
   fChain->SetBranchAddress("GenParticlePhi", &GenParticlePhi, &b_GenParticlePhi);
   fChain->SetBranchAddress("GenParticlePt", &GenParticlePt, &b_GenParticlePt);
   fChain->SetBranchAddress("GenParticlePx", &GenParticlePx, &b_GenParticlePx);
   fChain->SetBranchAddress("GenParticlePy", &GenParticlePy, &b_GenParticlePy);
   fChain->SetBranchAddress("GenParticlePz", &GenParticlePz, &b_GenParticlePz);
   fChain->SetBranchAddress("GenParticleVX", &GenParticleVX, &b_GenParticleVX);
   fChain->SetBranchAddress("GenParticleVY", &GenParticleVY, &b_GenParticleVY);
   fChain->SetBranchAddress("GenParticleVZ", &GenParticleVZ, &b_GenParticleVZ);
   fChain->SetBranchAddress("HLTCaloJetCorrEmEnergyInEB", &HLTCaloJetCorrEmEnergyInEB, &b_HLTCaloJetCorrEmEnergyInEB);
   fChain->SetBranchAddress("HLTCaloJetCorrEmEnergyInEE", &HLTCaloJetCorrEmEnergyInEE, &b_HLTCaloJetCorrEmEnergyInEE);
   fChain->SetBranchAddress("HLTCaloJetCorrEmEnergyInHF", &HLTCaloJetCorrEmEnergyInHF, &b_HLTCaloJetCorrEmEnergyInHF);
   fChain->SetBranchAddress("HLTCaloJetCorrEnergy", &HLTCaloJetCorrEnergy, &b_HLTCaloJetCorrEnergy);
   fChain->SetBranchAddress("HLTCaloJetCorrEnergyFractionEm", &HLTCaloJetCorrEnergyFractionEm, &b_HLTCaloJetCorrEnergyFractionEm);
   fChain->SetBranchAddress("HLTCaloJetCorrEnergyFractionHadronic", &HLTCaloJetCorrEnergyFractionHadronic, &b_HLTCaloJetCorrEnergyFractionHadronic);
   fChain->SetBranchAddress("HLTCaloJetCorrEta", &HLTCaloJetCorrEta, &b_HLTCaloJetCorrEta);
   fChain->SetBranchAddress("HLTCaloJetCorrHadEnergyInHB", &HLTCaloJetCorrHadEnergyInHB, &b_HLTCaloJetCorrHadEnergyInHB);
   fChain->SetBranchAddress("HLTCaloJetCorrHadEnergyInHE", &HLTCaloJetCorrHadEnergyInHE, &b_HLTCaloJetCorrHadEnergyInHE);
   fChain->SetBranchAddress("HLTCaloJetCorrHadEnergyInHF", &HLTCaloJetCorrHadEnergyInHF, &b_HLTCaloJetCorrHadEnergyInHF);
   fChain->SetBranchAddress("HLTCaloJetCorrHadEnergyInHO", &HLTCaloJetCorrHadEnergyInHO, &b_HLTCaloJetCorrHadEnergyInHO);
   fChain->SetBranchAddress("HLTCaloJetCorrMaxEInEmTowers", &HLTCaloJetCorrMaxEInEmTowers, &b_HLTCaloJetCorrMaxEInEmTowers);
   fChain->SetBranchAddress("HLTCaloJetCorrMaxEInHadTowers", &HLTCaloJetCorrMaxEInHadTowers, &b_HLTCaloJetCorrMaxEInHadTowers);
   fChain->SetBranchAddress("HLTCaloJetCorrPhi", &HLTCaloJetCorrPhi, &b_HLTCaloJetCorrPhi);
   fChain->SetBranchAddress("HLTCaloJetCorrPt", &HLTCaloJetCorrPt, &b_HLTCaloJetCorrPt);
   fChain->SetBranchAddress("HLTCaloJetCorrTowersArea", &HLTCaloJetCorrTowersArea, &b_HLTCaloJetCorrTowersArea);
   fChain->SetBranchAddress("HLTCaloJetRawEmEnergyInEB", &HLTCaloJetRawEmEnergyInEB, &b_HLTCaloJetRawEmEnergyInEB);
   fChain->SetBranchAddress("HLTCaloJetRawEmEnergyInEE", &HLTCaloJetRawEmEnergyInEE, &b_HLTCaloJetRawEmEnergyInEE);
   fChain->SetBranchAddress("HLTCaloJetRawEmEnergyInHF", &HLTCaloJetRawEmEnergyInHF, &b_HLTCaloJetRawEmEnergyInHF);
   fChain->SetBranchAddress("HLTCaloJetRawEnergy", &HLTCaloJetRawEnergy, &b_HLTCaloJetRawEnergy);
   fChain->SetBranchAddress("HLTCaloJetRawEnergyFractionEm", &HLTCaloJetRawEnergyFractionEm, &b_HLTCaloJetRawEnergyFractionEm);
   fChain->SetBranchAddress("HLTCaloJetRawEnergyFractionHadronic", &HLTCaloJetRawEnergyFractionHadronic, &b_HLTCaloJetRawEnergyFractionHadronic);
   fChain->SetBranchAddress("HLTCaloJetRawEta", &HLTCaloJetRawEta, &b_HLTCaloJetRawEta);
   fChain->SetBranchAddress("HLTCaloJetRawHadEnergyInHB", &HLTCaloJetRawHadEnergyInHB, &b_HLTCaloJetRawHadEnergyInHB);
   fChain->SetBranchAddress("HLTCaloJetRawHadEnergyInHE", &HLTCaloJetRawHadEnergyInHE, &b_HLTCaloJetRawHadEnergyInHE);
   fChain->SetBranchAddress("HLTCaloJetRawHadEnergyInHF", &HLTCaloJetRawHadEnergyInHF, &b_HLTCaloJetRawHadEnergyInHF);
   fChain->SetBranchAddress("HLTCaloJetRawHadEnergyInHO", &HLTCaloJetRawHadEnergyInHO, &b_HLTCaloJetRawHadEnergyInHO);
   fChain->SetBranchAddress("HLTCaloJetRawMaxEInEmTowers", &HLTCaloJetRawMaxEInEmTowers, &b_HLTCaloJetRawMaxEInEmTowers);
   fChain->SetBranchAddress("HLTCaloJetRawMaxEInHadTowers", &HLTCaloJetRawMaxEInHadTowers, &b_HLTCaloJetRawMaxEInHadTowers);
   fChain->SetBranchAddress("HLTCaloJetRawPhi", &HLTCaloJetRawPhi, &b_HLTCaloJetRawPhi);
   fChain->SetBranchAddress("HLTCaloJetRawPt", &HLTCaloJetRawPt, &b_HLTCaloJetRawPt);
   fChain->SetBranchAddress("HLTCaloJetRawTowersArea", &HLTCaloJetRawTowersArea, &b_HLTCaloJetRawTowersArea);
   fChain->SetBranchAddress("HLTPFJetChargedEmEnergyFraction", &HLTPFJetChargedEmEnergyFraction, &b_HLTPFJetChargedEmEnergyFraction);
   fChain->SetBranchAddress("HLTPFJetChargedHadronEnergyFraction", &HLTPFJetChargedHadronEnergyFraction, &b_HLTPFJetChargedHadronEnergyFraction);
   fChain->SetBranchAddress("HLTPFJetChargedMuEnergyFraction", &HLTPFJetChargedMuEnergyFraction, &b_HLTPFJetChargedMuEnergyFraction);
   fChain->SetBranchAddress("HLTPFJetElectronEnergyFraction", &HLTPFJetElectronEnergyFraction, &b_HLTPFJetElectronEnergyFraction);
   fChain->SetBranchAddress("HLTPFJetEnergy", &HLTPFJetEnergy, &b_HLTPFJetEnergy);
   fChain->SetBranchAddress("HLTPFJetEta", &HLTPFJetEta, &b_HLTPFJetEta);
   fChain->SetBranchAddress("HLTPFJetHFEMEnergyFraction", &HLTPFJetHFEMEnergyFraction, &b_HLTPFJetHFEMEnergyFraction);
   fChain->SetBranchAddress("HLTPFJetHFHadronEnergyFraction", &HLTPFJetHFHadronEnergyFraction, &b_HLTPFJetHFHadronEnergyFraction);
   fChain->SetBranchAddress("HLTPFJetMuonEnergyFraction", &HLTPFJetMuonEnergyFraction, &b_HLTPFJetMuonEnergyFraction);
   fChain->SetBranchAddress("HLTPFJetNeutralEmEnergyFraction", &HLTPFJetNeutralEmEnergyFraction, &b_HLTPFJetNeutralEmEnergyFraction);
   fChain->SetBranchAddress("HLTPFJetNeutralHadronEnergyFraction", &HLTPFJetNeutralHadronEnergyFraction, &b_HLTPFJetNeutralHadronEnergyFraction);
   fChain->SetBranchAddress("HLTPFJetPhi", &HLTPFJetPhi, &b_HLTPFJetPhi);
   fChain->SetBranchAddress("HLTPFJetPhotonEnergyFraction", &HLTPFJetPhotonEnergyFraction, &b_HLTPFJetPhotonEnergyFraction);
   fChain->SetBranchAddress("HLTPFJetPt", &HLTPFJetPt, &b_HLTPFJetPt);
   fChain->SetBranchAddress("HLTPixelVertexChi2", &HLTPixelVertexChi2, &b_HLTPixelVertexChi2);
   fChain->SetBranchAddress("HLTPixelVertexNdof", &HLTPixelVertexNdof, &b_HLTPixelVertexNdof);
   fChain->SetBranchAddress("HLTPixelVertexNormChi2", &HLTPixelVertexNormChi2, &b_HLTPixelVertexNormChi2);
   fChain->SetBranchAddress("HLTPixelVertexXCoord", &HLTPixelVertexXCoord, &b_HLTPixelVertexXCoord);
   fChain->SetBranchAddress("HLTPixelVertexXError", &HLTPixelVertexXError, &b_HLTPixelVertexXError);
   fChain->SetBranchAddress("HLTPixelVertexYCoord", &HLTPixelVertexYCoord, &b_HLTPixelVertexYCoord);
   fChain->SetBranchAddress("HLTPixelVertexYError", &HLTPixelVertexYError, &b_HLTPixelVertexYError);
   fChain->SetBranchAddress("HLTPixelVertexZCoord", &HLTPixelVertexZCoord, &b_HLTPixelVertexZCoord);
   fChain->SetBranchAddress("HLTPixelVertexZError", &HLTPixelVertexZError, &b_HLTPixelVertexZError);
   fChain->SetBranchAddress("PileUpInteractions", &PileUpInteractions, &b_PileUpInteractions);
   fChain->SetBranchAddress("PileUpOriginBX", &PileUpOriginBX, &b_PileUpOriginBX);
   fChain->SetBranchAddress("GenParticleMotherIndex", &GenParticleMotherIndex, &b_GenParticleMotherIndex);
   fChain->SetBranchAddress("GenParticleNumDaught", &GenParticleNumDaught, &b_GenParticleNumDaught);
   fChain->SetBranchAddress("GenParticlePdgId", &GenParticlePdgId, &b_GenParticlePdgId);
   fChain->SetBranchAddress("GenParticleStatus", &GenParticleStatus, &b_GenParticleStatus);
   fChain->SetBranchAddress("HLTPFJetChargedHadronMultiplicity", &HLTPFJetChargedHadronMultiplicity, &b_HLTPFJetChargedHadronMultiplicity);
   fChain->SetBranchAddress("HLTPFJetChargedMultiplicity", &HLTPFJetChargedMultiplicity, &b_HLTPFJetChargedMultiplicity);
   fChain->SetBranchAddress("HLTPFJetElectronMultiplicity", &HLTPFJetElectronMultiplicity, &b_HLTPFJetElectronMultiplicity);
   fChain->SetBranchAddress("HLTPFJetHFEMMultiplicity", &HLTPFJetHFEMMultiplicity, &b_HLTPFJetHFEMMultiplicity);
   fChain->SetBranchAddress("HLTPFJetHFHadronMultiplicity", &HLTPFJetHFHadronMultiplicity, &b_HLTPFJetHFHadronMultiplicity);
   fChain->SetBranchAddress("HLTPFJetMuonMultiplicity", &HLTPFJetMuonMultiplicity, &b_HLTPFJetMuonMultiplicity);
   fChain->SetBranchAddress("HLTPFJetNConstituents", &HLTPFJetNConstituents, &b_HLTPFJetNConstituents);
   fChain->SetBranchAddress("HLTPFJetNeutralHadronMultiplicity", &HLTPFJetNeutralHadronMultiplicity, &b_HLTPFJetNeutralHadronMultiplicity);
   fChain->SetBranchAddress("HLTPFJetNeutralMultiplicity", &HLTPFJetNeutralMultiplicity, &b_HLTPFJetNeutralMultiplicity);
   fChain->SetBranchAddress("HLTPFJetPhotonMultiplicity", &HLTPFJetPhotonMultiplicity, &b_HLTPFJetPhotonMultiplicity);
   fChain->SetBranchAddress("ProcessID", &ProcessID, &b_ProcessID);
   fChain->SetBranchAddress("bunch", &bunch, &b_bunch);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("ls", &ls, &b_ls);
   fChain->SetBranchAddress("orbit", &orbit, &b_orbit);
   fChain->SetBranchAddress("run", &run, &b_run);
   Notify();
}

Bool_t MyTestClass::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void MyTestClass::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t MyTestClass::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef MyTestClass_cxx
