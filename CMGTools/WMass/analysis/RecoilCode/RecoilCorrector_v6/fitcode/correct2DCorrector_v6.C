
#include <vector>
#include <sstream>
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TProfile.h"
#include "TF1.h"
#include "TMath.h"
#include "TRandom1.h"
#include "/home/pharris/Analysis/W/condor/run/CMSSW_3_8_4/src/MitWlnu/NYStyle/test/NYStyle.h"
#include "RecoilCorrector.hh"

//TFile *fZMCFile = new TFile("../Training/FullTrain/s12-zjets_53_data_mc.root");
//TFile *fZMCFile = new TFile("../../Htt/Data/s12-wjets_v3_select.root");
TFile *fZMCFile = new TFile("../../Htt/Data/s12-higgs_v5.root");
TTree *fZMCTree = (TTree*) fZMCFile->FindObjectAny("Flat");

void correct2DCorrector_v6(int iNJet=-1,
			   std::string iNameZDat = "recoilfits/recoilfit_datamm53X_20pv_njet.root",
			   std::string iNameZMC  = "recoilfits/recoilfit_zmm53X_20pv_njet.root",
			   std::string iNameWMC  = "recoilfits/recoilfit_higgs53X_20pv_njet.root") {
  Prep();
  //TFile *lWFile = new TFile("Weight.root");                 //NNLO Reweighting file
  //TH1F  *lWH    = (TH1F*) lWFile->FindObjectAny("Weight");
  RecoilCorrector corrector1(iNameWMC);        // Replace this with iWMC for W events
  corrector1.addMCFile      (iNameZMC);
  corrector1.addDataFile    (iNameZDat);
  
  int   lFNJet = -1; 
  float lFGenPt  = 0; 
  float lFGenPhi = 0;
  float lMet     = 0;
  float lFMet    = 0;
  float lFMetH   = 0;
  float lFMetL   = 0;
  float lMetPhi  = 0;
  float lFMetPhi  = 0;
  float lFMetPhiH = 0;
  float lFMetPhiL = 0;
  float lFU1     = 0;
  float lFU2     = 0;
  float lPt1     = 0;
  float lPhi1    = 0; 

  fZMCTree->SetBranchAddress("pt_vis"      ,&lPt1); 
  fZMCTree->SetBranchAddress("phi_vis"     ,&lPhi1); 
  fZMCTree->SetBranchAddress("pt_z"        ,&lFGenPt); 
  fZMCTree->SetBranchAddress("phi_z"       ,&lFGenPhi); 
  //fZMCTree->SetBranchAddress("metx_mva"    ,&lFMet);
  //fZMCTree->SetBranchAddress("metphi_mva"  ,&lFMetPhi);
  fZMCTree->SetBranchAddress("mvamet"      ,&lMet);
  fZMCTree->SetBranchAddress("mvametphi"   ,&lMetPhi);
  //fZMCTree->SetBranchAddress("u1x_mva"     ,&lFU1);
  //fZMCTree->SetBranchAddress("u2x_mva"     ,&lFU2);
  fZMCTree->SetBranchAddress("njet"        ,&lFNJet); 
  
  TFile *lOFile  = new TFile("Output.root","RECREATE");
  TTree *lTree   = fZMCTree->CloneTree(0);
  lTree->Branch("metx_mva"     ,&lFMet    ,"lFMetL/F");
  lTree->Branch("metx_mvah"    ,&lFMetH   ,"lFMetH/F");
  lTree->Branch("metx_mval"    ,&lFMetL   ,"lFMetL/F");
  lTree->Branch("metphi_mva"   ,&lFMetPhi ,"lFMetPhi/F");
  lTree->Branch("metphi_mvah"  ,&lFMetPhiH,"lFMetPhiH/F");
  lTree->Branch("metphi_mval"  ,&lFMetPhiL,"lFMetPhiL/F");
  lTree->Branch("u1x_mva"      ,&lFU1     ,"lFU1/F");
  lTree->Branch("u2x_mva"      ,&lFU2     ,"lFU2/F");

  for(int i0 = 0; i0 < fZMCTree->GetEntries(); i0++) {
    fZMCTree->GetEntry(i0);
    lFMetH          = lMet;
    lFMetL          = lMet;
    lFMetPhiH       = lMetPhi;
    lFMetPhiL       = lMetPhi;
    double pPFMet0  = lMet;
    double pPFMPhi0 = lMetPhi;
    double pPFMetH  = lFMetH;
    double pPFMPhiH = lFMetPhiH;
    double pPFMetL  = lFMetL;
    double pPFMPhiL = lFMetPhiL;
    double pU1 = 0; double pU2 = 0; 
    //lFNJet = -1;
    //corrector1.CorrectAll  (pPFMet0,pPFMPhi0,lFGenPt,lFGenPhi,lFGenPt,lFGenPhi,pU1,pU2,0,0,lFNJet);
    //corrector1.CorrectType1(pPFMet0,pPFMPhi0,lFGenPt,lFGenPhi,lFGenPt,lFGenPhi,pU1,pU2,0,0,lFNJet);              //Tau Group
    //corrector1.CorrectType1(pPFMetH,pPFMPhiH,lFGenPt,lFGenPhi,lFGenPt,lFGenPhi,pU1,pU2, 1,1, lFNJet);              //Tau Group
    //corrector1.CorrectType1(pPFMetL,pPFMPhiL,lFGenPt,lFGenPhi,lFGenPt,lFGenPhi,pU1,pU2,-1,-1,lFNJet);              //Tau Group
    corrector1.CorrectType2(pPFMet0,pPFMPhi0,lFGenPt,lFGenPhi,lPt1,lPhi1,pU1,pU2, 0, 0,lFNJet);
    corrector1.CorrectType2(pPFMetH,pPFMPhiH,lFGenPt,lFGenPhi,lPt1,lPhi1,pU1,pU2, 1, 1,lFNJet);
    corrector1.CorrectType2(pPFMetL,pPFMPhiL,lFGenPt,lFGenPhi,lPt1,lPhi1,pU1,pU2,-1,-1,lFNJet);
    lFMet     = pPFMet0;
    lFMetPhi  = pPFMPhi0;
    lFMetH     = pPFMetH;
    lFMetPhiH  = pPFMPhiH;
    lFMetL     = pPFMetL;
    lFMetPhiL  = pPFMPhiL;
    lFU1      = pU1;
    lFU2      = pU2;
    lTree->Fill();
  }
  lTree->Write();
}
