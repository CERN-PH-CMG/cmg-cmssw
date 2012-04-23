//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Mar 23 12:04:21 2012 by ROOT version 5.27/06b
// from TTree tree/tree
// found on file: /tmp/malberti/JetAnalysisTree_merged.root
//////////////////////////////////////////////////////////

#ifndef JetTree_h
#define JetTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class JetTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Float_t         beta;
   Float_t         betaStar; 
   Float_t         chFrac01;
   Float_t         chFrac02;
   Float_t         chFrac03;
   Float_t         chFrac04;
   Float_t         chFrac05;
   Float_t         chgEMfrac;
   Float_t         chgHadrfrac;
   Float_t         d0;
   Float_t         dRLead2nd;
   Float_t         dRLeadCent;
   Float_t         dR2Mean;
   Float_t         dRMean;
   Float_t         dRMeanCh;
   Float_t         dRMeanEm;
   Float_t         dRMeanNeut;
   Float_t         dZ;
   Float_t         emFrac01;
   Float_t         emFrac02;
   Float_t         emFrac03;
   Float_t         emFrac04;
   Float_t         emFrac05;
   Float_t         etaW;
   Float_t         fourthChFrac;
   Float_t         fourthEmFrac;
   Float_t         fourthFrac;
   Float_t         fourthNeutFrac;
   Float_t         frac01;
   Float_t         frac02;
   Float_t         frac03;
   Float_t         frac04;
   Float_t         frac05;
   Float_t         jetEta;
   Float_t         jetM;
   Float_t         jetPhi;
   Float_t         jetPt;
   Float_t         jetW;
   Float_t         leadChEta;
   Float_t         leadChFrac;
   Float_t         leadChPhi;
   Float_t         leadChPt;
   Float_t         leadEmEta;
   Float_t         leadEmFrac;
   Float_t         leadEmPhi;
   Float_t         leadEmPt;
   Float_t         leadEta;
   Float_t         leadFrac;
   Float_t         leadNeutEta;
   Float_t         leadNeutFrac;
   Float_t         leadNeutPhi;
   Float_t         leadNeutPt;
   Float_t         leadPhi;
   Float_t         leadPt;
   Float_t         majW;
   Float_t         minW;
   Float_t         mva;
   Float_t         nCharged;
   Float_t         nNeutrals;
   Float_t         nParticles;
   Float_t         neuEMfrac;
   Float_t         neuHadrfrac;
   Float_t         neutFrac01;
   Float_t         neutFrac02;
   Float_t         neutFrac03;
   Float_t         neutFrac04;
   Float_t         neutFrac05;
   Float_t         phiW;
   Float_t         ptD;
   Float_t         secondChFrac;
   Float_t         secondEmFrac;
   Float_t         secondEta;
   Float_t         secondFrac;
   Float_t         secondNeutFrac;
   Float_t         secondPhi;
   Float_t         secondPt;
   Float_t         thirdChFrac;
   Float_t         thirdEmFrac;
   Float_t         thirdFrac;
   Float_t         thirdNeutFrac;
   Int_t           ijet;
   Int_t           ievent;
   Int_t           PUit_n;
   Float_t         PUit_nTrue;
   Int_t           PUoot_early_n;
   Float_t         PUoot_early_nTrue;
   Int_t           PUoot_late_n;
   Float_t         PUoot_late_nTrue;
   Float_t         nvtx;
   Bool_t          jetLooseID;
   Bool_t          isMatched;
   Int_t           jetFlavour;
   Float_t         jetGenPt;
   Float_t         jetGenDr;
   Int_t           njets;
   Float_t         dimuonPt;
   Float_t         simpleDiscriminant;
   Float_t         fullDiscriminant;
   Float_t         philv1Discriminant;
   Int_t           simpleId;
   Int_t           fullId;
   Int_t           philv1Id;


   // List of branches
   TBranch        *b_beta;   //!   
   TBranch        *b_betaStar;   //!
   TBranch        *b_chFrac01;   //!
   TBranch        *b_chFrac02;   //!
   TBranch        *b_chFrac03;   //!
   TBranch        *b_chFrac04;   //!
   TBranch        *b_chFrac05;   //!
   TBranch        *b_chgEMfrac;   //!
   TBranch        *b_chgHadrfrac;   //!
   TBranch        *b_d0;   //!
   TBranch        *b_dRLead2nd;   //!
   TBranch        *b_dRLeadCent;   //!
   TBranch        *b_dRMean;   //!
   TBranch        *b_dR2Mean;   //!
   TBranch        *b_dRMeanCh;   //!
   TBranch        *b_dRMeanEm;   //!
   TBranch        *b_dRMeanNeut;   //!
   TBranch        *b_dZ;   //!
   TBranch        *b_emFrac01;   //!
   TBranch        *b_emFrac02;   //!
   TBranch        *b_emFrac03;   //!
   TBranch        *b_emFrac04;   //!
   TBranch        *b_emFrac05;   //!
   TBranch        *b_etaW;   //!
   TBranch        *b_fourthChFrac;   //!
   TBranch        *b_fourthEmFrac;   //!
   TBranch        *b_fourthFrac;   //!
   TBranch        *b_fourthNeutFrac;   //!
   TBranch        *b_frac01;   //!
   TBranch        *b_frac02;   //!
   TBranch        *b_frac03;   //!
   TBranch        *b_frac04;   //!
   TBranch        *b_frac05;   //!
   TBranch        *b_jetEta;   //!
   TBranch        *b_jetM;   //!
   TBranch        *b_jetPhi;   //!
   TBranch        *b_jetPt;   //!
   TBranch        *b_jetW;   //!
   TBranch        *b_leadChEta;   //!
   TBranch        *b_leadChFrac;   //!
   TBranch        *b_leadChPhi;   //!
   TBranch        *b_leadChPt;   //!
   TBranch        *b_leadEmEta;   //!
   TBranch        *b_leadEmFrac;   //!
   TBranch        *b_leadEmPhi;   //!
   TBranch        *b_leadEmPt;   //!
   TBranch        *b_leadEta;   //!
   TBranch        *b_leadFrac;   //!
   TBranch        *b_leadNeutEta;   //!
   TBranch        *b_leadNeutFrac;   //!
   TBranch        *b_leadNeutPhi;   //!
   TBranch        *b_leadNeutPt;   //!
   TBranch        *b_leadPhi;   //!
   TBranch        *b_leadPt;   //!
   TBranch        *b_majW;   //!
   TBranch        *b_minW;   //!
   TBranch        *b_mva;   //!
   TBranch        *b_nCharged;   //!
   TBranch        *b_nNeutrals;   //!
   TBranch        *b_nParticles;   //!
   TBranch        *b_neuEMfrac;   //!
   TBranch        *b_neuHadrfrac;   //!
   TBranch        *b_neutFrac01;   //!
   TBranch        *b_neutFrac02;   //!
   TBranch        *b_neutFrac03;   //!
   TBranch        *b_neutFrac04;   //!
   TBranch        *b_neutFrac05;   //!
   TBranch        *b_phiW;   //!
   TBranch        *b_ptD;   //!
   TBranch        *b_secondChFrac;   //!
   TBranch        *b_secondEmFrac;   //!
   TBranch        *b_secondEta;   //!
   TBranch        *b_secondFrac;   //!
   TBranch        *b_secondNeutFrac;   //!
   TBranch        *b_secondPhi;   //!
   TBranch        *b_secondPt;   //!
   TBranch        *b_thirdChFrac;   //!
   TBranch        *b_thirdEmFrac;   //!
   TBranch        *b_thirdFrac;   //!
   TBranch        *b_thirdNeutFrac;   //!
   TBranch        *b_ijet;   //!
   TBranch        *b_ievent;   //!
   TBranch        *b_PUit_n;   //!
   TBranch        *b_PUit_nTrue;   //!
   TBranch        *b_PUoot_early_n;   //!
   TBranch        *b_PUoot_early_nTrue;   //!
   TBranch        *b_PUoot_late_n;   //!
   TBranch        *b_PUoot_late_nTrue;   //!
   TBranch        *b_nvtx;   //!
   TBranch        *b_jetLooseID;   //!
   TBranch        *b_isMatched;   //!
   TBranch        *b_jetFlavour;   //!
   TBranch        *b_jetGenPt;   //!
   TBranch        *b_jetGenDr;   //!
   TBranch        *b_njets;   //!
   TBranch        *b_dimuonPt;   //!
   TBranch        *b_simpleDiscriminant;   //!
   TBranch        *b_fullDiscriminant;   //!
   TBranch        *b_philv1Discriminant;   //!
   TBranch        *b_simpleId;   //!
   TBranch        *b_fullId;   //!
   TBranch        *b_philv1Id;   //!


   JetTree(TTree *tree=0);
   virtual ~JetTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

JetTree::JetTree(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/tmp/malberti/JetAnalysisTree_merged.root");
      if (!f) {
         f = new TFile("/tmp/malberti/JetAnalysisTree_merged.root");
         f->cd("/tmp/malberti/JetAnalysisTree_merged.root:/pfjetanalyzer");
      }
      tree = (TTree*)gDirectory->Get("tree");

   }
   Init(tree);
}

JetTree::~JetTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t JetTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t JetTree::LoadTree(Long64_t entry)
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

void JetTree::Init(TTree *tree)
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

   fChain->SetBranchAddress("beta", &beta, &b_beta);
   fChain->SetBranchAddress("betaStar", &betaStar, &b_betaStar);
   fChain->SetBranchAddress("chFrac01", &chFrac01, &b_chFrac01);
   fChain->SetBranchAddress("chFrac02", &chFrac02, &b_chFrac02);
   fChain->SetBranchAddress("chFrac03", &chFrac03, &b_chFrac03);
   fChain->SetBranchAddress("chFrac04", &chFrac04, &b_chFrac04);
   fChain->SetBranchAddress("chFrac05", &chFrac05, &b_chFrac05);
   fChain->SetBranchAddress("chgEMfrac", &chgEMfrac, &b_chgEMfrac);
   fChain->SetBranchAddress("chgHadrfrac", &chgHadrfrac, &b_chgHadrfrac);
   fChain->SetBranchAddress("d0", &d0, &b_d0);
   fChain->SetBranchAddress("dRLead2nd", &dRLead2nd, &b_dRLead2nd);
   fChain->SetBranchAddress("dRLeadCent", &dRLeadCent, &b_dRLeadCent);
   fChain->SetBranchAddress("dRMean", &dRMean, &b_dRMean);
   fChain->SetBranchAddress("dR2Mean", &dR2Mean, &b_dR2Mean);
   fChain->SetBranchAddress("dRMeanCh", &dRMeanCh, &b_dRMeanCh);
   fChain->SetBranchAddress("dRMeanEm", &dRMeanEm, &b_dRMeanEm);
   fChain->SetBranchAddress("dRMeanNeut", &dRMeanNeut, &b_dRMeanNeut);
   fChain->SetBranchAddress("dZ", &dZ, &b_dZ);
   fChain->SetBranchAddress("emFrac01", &emFrac01, &b_emFrac01);
   fChain->SetBranchAddress("emFrac02", &emFrac02, &b_emFrac02);
   fChain->SetBranchAddress("emFrac03", &emFrac03, &b_emFrac03);
   fChain->SetBranchAddress("emFrac04", &emFrac04, &b_emFrac04);
   fChain->SetBranchAddress("emFrac05", &emFrac05, &b_emFrac05);
   fChain->SetBranchAddress("etaW", &etaW, &b_etaW);
   fChain->SetBranchAddress("fourthChFrac", &fourthChFrac, &b_fourthChFrac);
   fChain->SetBranchAddress("fourthEmFrac", &fourthEmFrac, &b_fourthEmFrac);
   fChain->SetBranchAddress("fourthFrac", &fourthFrac, &b_fourthFrac);
   fChain->SetBranchAddress("fourthNeutFrac", &fourthNeutFrac, &b_fourthNeutFrac);
   fChain->SetBranchAddress("frac01", &frac01, &b_frac01);
   fChain->SetBranchAddress("frac02", &frac02, &b_frac02);
   fChain->SetBranchAddress("frac03", &frac03, &b_frac03);
   fChain->SetBranchAddress("frac04", &frac04, &b_frac04);
   fChain->SetBranchAddress("frac05", &frac05, &b_frac05);
   fChain->SetBranchAddress("jetEta", &jetEta, &b_jetEta);
   fChain->SetBranchAddress("jetM", &jetM, &b_jetM);
   fChain->SetBranchAddress("jetPhi", &jetPhi, &b_jetPhi);
   fChain->SetBranchAddress("jetPt", &jetPt, &b_jetPt);
   fChain->SetBranchAddress("jetW", &jetW, &b_jetW);
   fChain->SetBranchAddress("leadChEta", &leadChEta, &b_leadChEta);
   fChain->SetBranchAddress("leadChFrac", &leadChFrac, &b_leadChFrac);
   fChain->SetBranchAddress("leadChPhi", &leadChPhi, &b_leadChPhi);
   fChain->SetBranchAddress("leadChPt", &leadChPt, &b_leadChPt);
   fChain->SetBranchAddress("leadEmEta", &leadEmEta, &b_leadEmEta);
   fChain->SetBranchAddress("leadEmFrac", &leadEmFrac, &b_leadEmFrac);
   fChain->SetBranchAddress("leadEmPhi", &leadEmPhi, &b_leadEmPhi);
   fChain->SetBranchAddress("leadEmPt", &leadEmPt, &b_leadEmPt);
   fChain->SetBranchAddress("leadEta", &leadEta, &b_leadEta);
   fChain->SetBranchAddress("leadFrac", &leadFrac, &b_leadFrac);
   fChain->SetBranchAddress("leadNeutEta", &leadNeutEta, &b_leadNeutEta);
   fChain->SetBranchAddress("leadNeutFrac", &leadNeutFrac, &b_leadNeutFrac);
   fChain->SetBranchAddress("leadNeutPhi", &leadNeutPhi, &b_leadNeutPhi);
   fChain->SetBranchAddress("leadNeutPt", &leadNeutPt, &b_leadNeutPt);
   fChain->SetBranchAddress("leadPhi", &leadPhi, &b_leadPhi);
   fChain->SetBranchAddress("leadPt", &leadPt, &b_leadPt);
   fChain->SetBranchAddress("majW", &majW, &b_majW);
   fChain->SetBranchAddress("minW", &minW, &b_minW);
   fChain->SetBranchAddress("mva", &mva, &b_mva);
   fChain->SetBranchAddress("nCharged", &nCharged, &b_nCharged);
   fChain->SetBranchAddress("nNeutrals", &nNeutrals, &b_nNeutrals);
   fChain->SetBranchAddress("nParticles", &nParticles, &b_nParticles);
   fChain->SetBranchAddress("neuEMfrac", &neuEMfrac, &b_neuEMfrac);
   fChain->SetBranchAddress("neuHadrfrac", &neuHadrfrac, &b_neuHadrfrac);
   fChain->SetBranchAddress("neutFrac01", &neutFrac01, &b_neutFrac01);
   fChain->SetBranchAddress("neutFrac02", &neutFrac02, &b_neutFrac02);
   fChain->SetBranchAddress("neutFrac03", &neutFrac03, &b_neutFrac03);
   fChain->SetBranchAddress("neutFrac04", &neutFrac04, &b_neutFrac04);
   fChain->SetBranchAddress("neutFrac05", &neutFrac05, &b_neutFrac05);
   fChain->SetBranchAddress("phiW", &phiW, &b_phiW);
   fChain->SetBranchAddress("ptD", &ptD, &b_ptD);
   fChain->SetBranchAddress("secondChFrac", &secondChFrac, &b_secondChFrac);
   fChain->SetBranchAddress("secondEmFrac", &secondEmFrac, &b_secondEmFrac);
   fChain->SetBranchAddress("secondEta", &secondEta, &b_secondEta);
   fChain->SetBranchAddress("secondFrac", &secondFrac, &b_secondFrac);
   fChain->SetBranchAddress("secondNeutFrac", &secondNeutFrac, &b_secondNeutFrac);
   fChain->SetBranchAddress("secondPhi", &secondPhi, &b_secondPhi);
   fChain->SetBranchAddress("secondPt", &secondPt, &b_secondPt);
   fChain->SetBranchAddress("thirdChFrac", &thirdChFrac, &b_thirdChFrac);
   fChain->SetBranchAddress("thirdEmFrac", &thirdEmFrac, &b_thirdEmFrac);
   fChain->SetBranchAddress("thirdFrac", &thirdFrac, &b_thirdFrac);
   fChain->SetBranchAddress("thirdNeutFrac", &thirdNeutFrac, &b_thirdNeutFrac);
   fChain->SetBranchAddress("ijet", &ijet, &b_ijet);
   fChain->SetBranchAddress("ievent", &ievent, &b_ievent);
   fChain->SetBranchAddress("PUit_n", &PUit_n, &b_PUit_n);
   fChain->SetBranchAddress("PUit_nTrue", &PUit_nTrue, &b_PUit_nTrue);
   fChain->SetBranchAddress("PUoot_early_n", &PUoot_early_n, &b_PUoot_early_n);
   fChain->SetBranchAddress("PUoot_early_nTrue", &PUoot_early_nTrue, &b_PUoot_early_nTrue);
   fChain->SetBranchAddress("PUoot_late_n", &PUoot_late_n, &b_PUoot_late_n);
   fChain->SetBranchAddress("PUoot_late_nTrue", &PUoot_late_nTrue, &b_PUoot_late_nTrue);
   fChain->SetBranchAddress("nvtx", &nvtx, &b_nvtx);
   fChain->SetBranchAddress("jetLooseID", &jetLooseID, &b_jetLooseID);
   fChain->SetBranchAddress("isMatched", &isMatched, &b_isMatched);
   fChain->SetBranchAddress("jetFlavour", &jetFlavour, &b_jetFlavour);
   fChain->SetBranchAddress("jetGenPt", &jetGenPt, &b_jetGenPt);
   fChain->SetBranchAddress("jetGenDr", &jetGenDr, &b_jetGenDr);
   fChain->SetBranchAddress("njets", &njets, &b_njets);
   fChain->SetBranchAddress("dimuonPt", &dimuonPt, &b_dimuonPt);
   fChain->SetBranchAddress("simpleDiscriminant", &simpleDiscriminant, &b_simpleDiscriminant);
   fChain->SetBranchAddress("fullDiscriminant", &fullDiscriminant, &b_fullDiscriminant);
   fChain->SetBranchAddress("philv1Discriminant", &philv1Discriminant, &b_philv1Discriminant);
   fChain->SetBranchAddress("simpleId", &simpleId, &b_simpleId);
   fChain->SetBranchAddress("fullId", &fullId, &b_fullId);
   fChain->SetBranchAddress("philv1Id", &philv1Id, &b_philv1Id);
   Notify();
}

Bool_t JetTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void JetTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t JetTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
