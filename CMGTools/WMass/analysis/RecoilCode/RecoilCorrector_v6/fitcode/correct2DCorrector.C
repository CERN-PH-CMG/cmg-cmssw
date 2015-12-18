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

TFile *fZDataFile = new TFile("ZSummary/ZMM_Inc.root");
TTree *fZDataTree = (TTree*) fZDataFile->FindObjectAny("Flat");

TFile *fZMCFile = new TFile("ZSummary/ZMMMC.root");//dy_select.root");
TTree *fZMCTree = (TTree*) fZMCFile->FindObjectAny("Flat");

float fWeight = 0; float fMet     = 0; float fMPhi    = 0; int   fNJet    = 0; 
int   fNPR2   = 0; int   fNGam2   = 0; int   fId1     = 0; int   fId2     = 0; 
 
int   fTauId  = 0; int   fQ1      = 0; int   fQ2      = 0; float fU1      = 0; float fU2 = 0;
float fMass   = 0; float fSVFMass = 0; float fPVis    = 0;  
                   float fLepEta  = 0; float fLepEta1 = 0; float fLepEta2 = 0;
float fGenPhi = 0; float fLepPhi  = 0; float fLepPhi1 = 0; float fLepPhi2 = 0; 
float fGenPt  = 0; float fLepPt   = 0; float fLepPt1  = 0; float fLepPt2  = 0; 
float fGPhi1  = 0; float fGPhi2   = 0; float fGEta1   = 0; float fGEta2   = 0; float fGPt1 = 0; float fGPt2 = 0; 

float fFGenPt = 0;   float fFGenPhi  = 0; float fFLepPt   = 0; float fFLepPhi  = 0; int   fFNJet    = 0; 
float fFWeight = 0;  float fFLepEta  = 0; float fFLepPhi1 = 0; float fFLepPhi2 = 0; float fFLepPt1  = 0; float fFLepPt2 = 0; 
float fFMass   = 0;  float fFSVFMass = 0; float fFPVis    = 0; float fFLepEta2 = 0; float fFLepEta1 = 0;
float fFPMet   = 0;  float fFPMet0  = 0;  float fFPMet1   = 0; float fFPMet2   = 0; float fFPMet3   = 0;
float fFMet    = 0;  float fFMet0   = 0;  float fFMet1    = 0; float fFMet2    = 0; float fFMet3    = 0;
float fFMt     = 0;  float fFMt0    = 0;  float fFMt1     = 0; float fFMt2     = 0; float fFMt3     = 0;
float fFMPhi   = 0;  float fFMPhi0  = 0;  float fFMPhi1   = 0; float fFMPhi2   = 0; float fFMPhi3   = 0; float fFU1    = 0; float fFU2 = 0;
int   fFTauId  = 0;  int   fFQ1     = 0;  int   fFQ2      = 0; float fFPZeta   = 0; int   fFNPR2    = 0; float fFNGam2 = 0;

double mT(double iPt1,double iPt2,double iPhi1,double iPhi2) {
  double lPx = iPt1*cos(iPhi1)+iPt2*cos(iPhi2);
  double lPy = iPt1*sin(iPhi1)+iPt2*sin(iPhi2);
  return sqrt((iPt1+iPt2)*(iPt1+iPt2)-lPx*lPx-lPy*lPy);
}

double pZeta(double iPhi1,double iPhi2,double iMet,double iMPhi) { 
  double lDPhi = fabs(iPhi1-iPhi2); if(lDPhi > 2*TMath::Pi()-lDPhi) lDPhi = 2*TMath::Pi()-lDPhi;
  double projPhi = 0;
    if(iPhi1>iPhi2)
      if(iPhi1-iPhi2 < TMath::Pi()) projPhi = iPhi1 - lDPhi/2;
      else                          projPhi = iPhi1 + lDPhi/2;
    else
      if(iPhi2-iPhi1 < TMath::Pi()) projPhi = iPhi2 - lDPhi/2;
      else        	            projPhi = iPhi2 + lDPhi/2;
    Double_t projX  =  cos(projPhi);
    Double_t projY  =  sin(projPhi);
    double pMProj  = iMet*cos(iMPhi)*projX + iMet*sin(iMPhi)*projY;
    return pMProj;
}
void setupMCTree(TTree *iTree) { 
  iTree->ResetBranchAddresses();
  iTree->SetBranchAddress("id1_l"       ,&fId1); 
  iTree->SetBranchAddress("id2_l"       ,&fId2); 
  iTree->SetBranchAddress("pt1_l"       ,&fGPt1); 
  iTree->SetBranchAddress("pt2_l"       ,&fGPt2); 
  iTree->SetBranchAddress("eta1_l"      ,&fGEta1); 
  iTree->SetBranchAddress("eta2_l"      ,&fGEta2); 
  iTree->SetBranchAddress("phi1_l"      ,&fGPhi1); 
  iTree->SetBranchAddress("phi2_l"      ,&fGPhi2); 
  iTree->SetBranchAddress("tauid"       ,&fTauId); 
  iTree->SetBranchAddress("q_1"         ,&fQ1); 
  iTree->SetBranchAddress("q_2"         ,&fQ2); 
  iTree->SetBranchAddress("pt_z"        ,&fGenPt); 
  iTree->SetBranchAddress("phi_z"       ,&fGenPhi);
  iTree->SetBranchAddress("rpt_z"       ,&fLepPt);
  iTree->SetBranchAddress("rphi_z"      ,&fLepPhi);
  iTree->SetBranchAddress("pt_1"        ,&fLepPt1);
  iTree->SetBranchAddress("pt_2"        ,&fLepPt2);
  iTree->SetBranchAddress("eta_1"       ,&fLepEta1);
  iTree->SetBranchAddress("eta_2"       ,&fLepEta2);
  iTree->SetBranchAddress("phi_1"       ,&fLepPhi1);
  iTree->SetBranchAddress("phi_2"       ,&fLepPhi2);
  iTree->SetBranchAddress("npr_2"       ,&fNPR2);
  iTree->SetBranchAddress("ngam_2"      ,&fNGam2);
  iTree->SetBranchAddress("metphi"      ,&fMPhi);
  iTree->SetBranchAddress("Weight"      ,&fWeight);
  iTree->SetBranchAddress("m_vis"       ,&fMass);
  iTree->SetBranchAddress("svfmass"     ,&fSVFMass);
  iTree->SetBranchAddress("njet"        ,&fNJet);
  iTree->SetBranchAddress("pvis"        ,&fPVis);
  iTree->SetBranchAddress("met"         ,&fMet);
  iTree->SetBranchAddress("metphi"      ,&fMPhi);
  iTree->SetBranchAddress("u1"          ,&fU1);
  iTree->SetBranchAddress("u2"          ,&fU2);
}
void createFillTree(TTree *iTree) { 
  iTree->Branch("tauid"       ,&fFTauId,  "fFTauId/I");
  iTree->Branch("pt_z"        ,&fFGenPt,  "fFGenPt/F");
  iTree->Branch("phi_z"       ,&fFGenPhi, "fFGenPhi/F");
  iTree->Branch("pt_1"        ,&fFLepPt1, "fFLepPt1/F");
  iTree->Branch("pt_2"        ,&fFLepPt2, "fFLepPt2/F");
  iTree->Branch("q_1"         ,&fFQ1,     "fFQ1/I");
  iTree->Branch("q_2"         ,&fFQ2,     "fFQ2/I");
  iTree->Branch("pt_vis"      ,&fFLepPt,  "fFLepPt/F");
  iTree->Branch("phi_vis"     ,&fFLepPhi, "fFLepPhi/F");
  iTree->Branch("phi_1"       ,&fFLepPhi1,"fFLepPhi1/F");
  iTree->Branch("phi_2"       ,&fFLepPhi2,"fFLepPhi2/F");
  iTree->Branch("eta_1"       ,&fFLepEta1,"fFLepEta1/F");
  iTree->Branch("eta_2"       ,&fFLepEta2,"fFLepEta22/F");
  iTree->Branch("met"         ,&fFMet0,   "fFMet/F");
  iTree->Branch("met0"        ,&fFMet1,   "fFMet0/F");
  iTree->Branch("met1"        ,&fFMet1,   "fFMet1/F");
  iTree->Branch("met2"        ,&fFMet2,   "fFMet2/F");
  iTree->Branch("metphi"      ,&fFMPhi,   "fFMPhi/F");
  iTree->Branch("Weight"      ,&fFWeight, "fFWeight/F");
  iTree->Branch("m_vis"       ,&fFMass,   "fFMass/F");
  iTree->Branch("svfmass"     ,&fFSVFMass,"fFSVFMass/F");
  iTree->Branch("njet"        ,&fFNJet,   "fFNJet/I");
  iTree->Branch("pvis"        ,&fFPVis,   "fFPVis/F");
  iTree->Branch("mt0"         ,&fFMt,     "fFMt/F");
  iTree->Branch("mt"          ,&fFMt0,    "fFMt0/F");
  iTree->Branch("mt1"         ,&fFMt1,    "fFMt1/F");
  iTree->Branch("mt2"         ,&fFMt2,    "fFMt2/F");
  iTree->Branch("mt_1"        ,&fFMt3,    "fFMt3/F");
  iTree->Branch("pmet"        ,&fFPMet,   "fFPMet/F");
  iTree->Branch("pmet0"       ,&fFPMet0,  "fFPMet0/F");
  iTree->Branch("pmet1"       ,&fFPMet1,  "fFPMet1/F");
  iTree->Branch("pmet2"       ,&fFPMet2,  "fFPMet2/F");
  iTree->Branch("u1"          ,&fFU1,     "fFU1/F");
  iTree->Branch("u2"          ,&fFU2,     "fFU2/F");
  iTree->Branch("pzeta"       ,&fFPZeta,  "fFPZeta/F");
  iTree->Branch("npr_2"       ,&fFNPR2,   "fFNPR2/I");
  iTree->Branch("ngam_2"      ,&fFNGam2,  "fFNGam2/I");
}
void fillRegulars() { 
    fFTauId   = fTauId;
    fFQ1      = fQ1;
    fFQ2      = fQ2;
    fFGenPt   = fGenPt;
    fFGenPhi  = fGenPhi;
    fFLepPt   = fLepPt;
    fFLepPhi  = fLepPhi;
    fFLepPt1  = fLepPt1;
    fFLepPt2  = fLepPt2;
    fFLepPhi1 = fLepPhi1;
    fFLepPhi2 = fLepPhi2;
    fFLepEta1 = fLepEta1;
    fFLepEta2 = fLepEta2;
    fFWeight  = fWeight;
    fFMass    = fMass;
    fFSVFMass = fSVFMass;
    fFNJet    = fNJet;
    fFPVis    = fPVis;
    fFNPR2    = fNPR2;
    fFNGam2   = fNGam2;
}
void correct2DCorrector(int iNJet=-1,std::string iNameZDat = "recoilfits/recoilfit_datamm_njet.root",
			             std::string iNameZMC  = "recoilfits/recoilfit_zmm42X_njet.root") {
  Prep();
  RecoilCorrector corrector1(iNameZMC);        // Replace this with iWMC for W events
  corrector1.addMCFile      (iNameZMC);
  corrector1.addDataFile    (iNameZDat);
  setupMCTree(fZMCTree);
  TFile *lOFile = new TFile("zmc_corr.root","RECREATE");
  TTree *lTree = new TTree("Flat","Flat");
  createFillTree(lTree);
  TH1F* lDMet = new TH1F("dMET","dMET"   ,75,0,100); lDMet->Sumw2();  lDMet->SetMarkerStyle(kFullCircle); lDMet->SetLineWidth(1);
  TH1F* lMMet0 = new TH1F("mMET0","mMET0",75,0,100); lMMet0->Sumw2(); lMMet0->SetLineColor(kRed); lMMet0->SetLineWidth(1);
  TH1F* lMMet1 = new TH1F("mMET1","mMET1",75,0,100); lMMet1->Sumw2(); lMMet1->SetLineColor(kBlue); lMMet1->SetLineWidth(1);
  TH1F* lMMet2 = new TH1F("mMET2","mMET2",75,0,100); lMMet2->Sumw2(); lMMet2->SetLineColor(kOrange); lMMet2->SetLineWidth(1);
  TH1F* lMMet3 = new TH1F("mMET3","mMET3",75,0,100); lMMet3->Sumw2(); lMMet3->SetLineColor(kViolet); lMMet3->SetLineWidth(1);
  
  //TH1F* lDMet = new TH1F("dMET","dMET"   ,75,-150,100); lDMet->Sumw2();  lDMet->SetMarkerStyle(kFullCircle); lDMet->SetLineWidth(1);
  //TH1F* lMMet0 = new TH1F("mMET0","mMET0",75,-150,100); lMMet0->Sumw2(); lMMet0->SetLineColor(kRed); lMMet0->SetLineWidth(1);
  //TH1F* lMMet1 = new TH1F("mMET1","mMET1",75,-150,100); lMMet1->Sumw2(); lMMet1->SetLineColor(kBlue); lMMet1->SetLineWidth(1);
  //TH1F* lMMet2 = new TH1F("mMET2","mMET2",75,-150,100); lMMet2->Sumw2(); lMMet2->SetLineColor(kOrange); lMMet2->SetLineWidth(1);
  //TH1F* lMMet3 = new TH1F("mMET3","mMET3",75,-150,100); lMMet3->Sumw2(); lMMet3->SetLineColor(kViolet); lMMet3->SetLineWidth(1);
  int lN = 1;//Number of iterations ===> To increase stats (be careful with this)
  int lNEntries = fZMCTree->GetEntries(); 
  for(int i0 = 0; i0 < lNEntries; i0++) {
    fZMCTree->GetEntry(i0);
    if(i0 > 1000000) break;
    if(iNJet != fNJet && iNJet != -1) continue;
    //fFWeight = 1;
    fillRegulars();
    //double pPFMPhi  = fMPhi; 
    double pPFMPhi0 = fMPhi; 
    double pPFMPhi1 = fMPhi; 
    double pPFMPhi2 = fMPhi; 
    double pMet     = fMet;
    double pPFMet0  = fMet;   
    double pPFMet1  = fMet;  
    double pPFMet2  = fMet;  
    for(int i1 = 0; i1 < lN; i1++) { 
      double pU10 = 0; double pU20 = 0; double pU21 = 0; double pU22 = 0; double pU11 = 0; double pU12 = 0;
      corrector1.CorrectAll  (pPFMet0,pPFMPhi0,fFLepPt,fFLepPhi,fFLepPt,fFLepPhi, pU10, pU20,0,0,fFNJet);
      corrector1.CorrectType1(pPFMet1,pPFMPhi1,fFLepPt,fFLepPhi,fFLepPt,fFLepPhi, pU11, pU21,0,0,fFNJet);              //Tau Group
      corrector1.CorrectType2(pPFMet2,pPFMPhi2,fFLepPt,fFLepPhi,fFLepPt,fFLepPhi, pU12, pU22,0,0,fFNJet);              //Tau Group
      fFMet     = fMet;
      fFMPhi    = fMPhi;
      fFMet0    = pPFMet0;//(fabs(lId1) != 19  &&  fabs(lId2) != 19) ? lMet :   pPFMet0;
      fFU1      = fU1;
      fFU2      = fU2;
      fFMPhi0   = pPFMPhi0;
      fFMet1    = pPFMet1;
      fFMPhi1   = pPFMPhi1;
      fFMet2    = pPFMet2;
      fFMPhi2   = pPFMPhi2;
      //fFMet3    = lMet;
      //fFMPhi3   = lMPhi;
      fFMt      = mT(fFLepPt1,fFMet ,fFLepPhi1,fFMPhi);
      fFMt0     = mT(fFLepPt1,fFMet0,fFLepPhi1,fFMPhi0);
      fFMt1     = mT(fFLepPt1,fFMet1,fFLepPhi1,fFMPhi1);
      fFMt2     = mT(fFLepPt1,fFMet2,fFLepPhi1,fFMPhi2);
      //fFMt3     = mT(lFLepPt1,lFMet3,fFLepPhi1,lFMPhi3);
      fFPMet    = pZeta(fLepPhi1,fLepPhi2,fFMet, fFMPhi);
      fFPMet0   = pZeta(fLepPhi1,fLepPhi2,fFMet0,fFMPhi0);
      fFPMet1   = pZeta(fLepPhi1,fLepPhi2,fFMet1,fFMPhi1);
      fFPMet2   = pZeta(fLepPhi1,fLepPhi2,fFMet2,fFMPhi2);
      fFPMet3   = pZeta(fLepPhi1,fLepPhi2,fFMet3,fFMPhi3);
      fFPZeta   = 0.85*fFPVis - fFPMet3;
      //fFWeight = 1;
      lMMet0->Fill(fFMet ,fFWeight);//lFWeight);
      lMMet1->Fill(fFMet0,fFWeight);
      lMMet2->Fill(fFMet1,fFWeight);
      lMMet3->Fill(fFMet2,fFWeight);
      //fFWeight = 1;
      //lMMet0->Fill(fU1 ,fFWeight);//lFWeight);
      //lMMet1->Fill(pU10,fFWeight);
      //lMMet2->Fill(pU11,fFWeight);
      //lMMet3->Fill(pU12,fFWeight);
      lTree ->Fill();
    }
  }
  lTree->Write();
  std::stringstream lSNJet; 
  if(iNJet == -1) lSNJet << "njet > -1";
  if(iNJet != -1) lSNJet << " njet == " << iNJet;
  fZDataTree->Draw("met>>dMET",lSNJet.str().c_str());//&& nbtag == 0");

  TCanvas *lC0 = new TCanvas("c0","c0",800,600); lC0->cd(); lC0->SetLogy();
  TLegend *lL = new TLegend(0.6,0.6,0.9,0.9); lL->SetFillColor(0); lL->SetBorderSize(0);
  lL->AddEntry(lDMet,"data","lp"); lMMet0->SetMarkerStyle(kFullCircle); lMMet0->SetMarkerColor(kRed);
  lL->AddEntry(lMMet0,"MC","l");       lMMet0->SetMarkerStyle(kFullCircle); lMMet0->SetMarkerColor(kRed);
  lL->AddEntry(lMMet1,"MC corrected","lp"); 
  lL->AddEntry(lMMet2,"Correction Type1","lp"); 
  lL->AddEntry(lMMet3,"Correction Type2","lp"); 
  std::cout << "=====Int0 ===> " << lDMet ->Integral() << std::endl;
  std::cout << "=====Int0 ===> " << lMMet0->Integral() << std::endl;
  std::cout << "=====Int1 ===> " << lMMet1->Integral() << std::endl;
  std::cout << "=====Int2 ===> " << lMMet2->Integral() << std::endl;
  std::cout << "=====Int3 ===> " << lMMet3->Integral() << std::endl;

  lDMet ->Scale(1./lDMet ->Integral());
  lMMet0->Scale(1./lMMet0->Integral());
  lMMet1->Scale(1./lMMet1->Integral());
  lMMet2->Scale(1./lMMet2->Integral());
  lMMet3->Scale(1./lMMet3->Integral());

  lDMet->GetYaxis()->SetRangeUser(0.000001,0.15);
  lDMet->GetXaxis()->SetTitle("#slash{E_{T}} (GeV)");
  lDMet->Draw("EP");
  lMMet0->Draw(" hist sames");
  lMMet1->Draw(" hist sames");
  lMMet2->Draw(" hist sames");
  lMMet3->Draw(" hist sames");
  lL->Draw();
}
