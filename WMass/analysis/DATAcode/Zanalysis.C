#define Zanalysis_cxx
#include "Zanalysis.h"
#include "../includes/common.h"
#include "rochcor_42X.h"
#include "rochcor_44X_v2.h"
#include "MuScleFitCorrector.h"
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <TGraphAsymmErrors.h>

void Zanalysis::Loop(int IS_MC_CLOSURE_TEST, int isMCorDATA, TString outputdir, int buildTemplates, int useMomentumCorr, int smearRochCorrByNsigma, int useEffSF, int useVtxSF, TString sampleName)
{
  //   In a ROOT session, you can do:
  //      Root > .L Zanalysis.C
  //      Root > Zanalysis t
  //      Root > t.GetEntry(12); // Fill t data members with entry number 12
  //      Root > t.Show();       // Show values of entry 12
  //      Root > t.Show(16);     // Read and show values of entry 16
  //      Root > t.Loop();       // Loop on all entries
  //

  //     This is the loop skeleton where:
  //    jentry is the global entry number in the chain
  //    ientry is the entry number in the current Tree
  //  Note that the argument to GetEntry must be:
  //    jentry for TChain::GetEntry
  //    ientry for TTree::GetEntry and TBranch::GetEntry
  //
  //       To read only selected branches, Insert statements like:
  // METHOD1:
  //    fChain->SetBranchStatus("*",0);  // disable all branches
  //    fChain->SetBranchStatus("branchname",1);  // activate branchname
  // METHOD2: replace line
  //    fChain->GetEntry(jentry);       //read all branches
  //by  b_branchname->GetEntry(ientry); //read only this branch

  TRandom3 *r = new TRandom3(0);

  cout << "output filename= " << Form("%s/Zanalysis.root",outputdir.Data()) << endl;
  TFile*fout = new TFile(Form("%s/Zanalysis.root",outputdir.Data()),"RECREATE");
  
  // TFile*feffSF = new TFile(Form("../Zanalysis.root",outputdir.Data()),"RECREATE");
  
  TH1D *hPileUp_Fall11=new TH1D("hPileUp_Fall11","hPileUp_Fall11",50,0,50);
  TH1D *hWlikePos_PtScaled_1_Gen[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWlikePos_PtScaled_2_ZGenMassCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWlikePos_PtScaled_3_Mu1GenCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWlikePos_PtScaled_4_Mu2GenCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWlikePos_PtScaled_5_RecoCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWlikePos_PtScaled_6_METCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWlikePos_PtScaled_7_RecoilCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWlikePos_PtScaled_8_JetCut[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWlikePos_PtScaled_QCD[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];

  TH1D *hWlikePos_PtScaled_RWeighted_Templates[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1]; // used only to build templates
  TH1D*hWlikePos_R_WdivZ[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1]; // used only to build templates
  TFile*finTemplates, *finEffSF, *finPileupSF;
  TGraphAsymmErrors*hEffSF_MuId_eta_2011[2],*hEffSF_Iso_eta_2011[2]/* ,*hEffSF_Iso_vtx_2011A,*hEffSF_Iso_vtx_2011B*/;
  TH1D*hPileupSF;
  
  if(buildTemplates){
    finTemplates = new TFile(Form("%s/R_WdivZ_OnMC.root",outputdir.Data())); // used only to build templates
    if(!finTemplates){
      cout << "file R_WdivZ_OnMC.root is missing, impossible to build templates" << endl;
      return;
    }
  }
  if(useEffSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0)){
    finEffSF = new TFile("../utils/MuonEfficiencies_SF_2011_42X_DataMC.root"); // used only to build templates
    hEffSF_MuId_eta_2011[0]=(TGraphAsymmErrors*)finEffSF->Get("SF_TIGHT_nL8_2011A_eta__pt>20");
    hEffSF_MuId_eta_2011[1]=(TGraphAsymmErrors*)finEffSF->Get("SF_TIGHT_nL8_2011B_eta__pt>20");
    hEffSF_Iso_eta_2011[0]=(TGraphAsymmErrors*)finEffSF->Get("combRelPFISO12_2011A_eta__pt>20");
    hEffSF_Iso_eta_2011[1]=(TGraphAsymmErrors*)finEffSF->Get("combRelPFISO12_2011B_eta__pt>20");
    // hEffSF_Iso_vtx_2011A=(TH1D*)finEffSF->Get("combRelPFISO12_2011A_vtx__pt>20_abseta<2.4");
    // hEffSF_Iso_vtx_2011B=(TH1D*)finEffSF->Get("combRelPFISO12_2011B_vtx__pt>20_abseta<2.4");
    if(!finEffSF){
      cout << "file MuonEfficiencies_SF_2011_42X_DataMC.root is missing, impossible to retrieve efficiency scale factors" << endl;
      return;
    }
  }
  if(useVtxSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0)){
    TString vtx_str = sampleName; vtx_str.ReplaceAll("Sig",""); vtx_str.ReplaceAll("Fake","");
    // finPileupSF = new TFile(Form("../utils/pileup_reweighting_%s.root",vtx_str.Data())); // used only to build templates
    finPileupSF = new TFile(Form("../utils/pileup/pileup_reweighting_Fall11.root")); // used only to build templates
    if(!finPileupSF){
      cout << "file " << Form("../utils/pileup/pileup_reweighting_Fall11.root") << " is missing, impossible to retrieve pileup reweighting factors" << endl;
      return;
    }else{
      hPileupSF=(TH1D*)finPileupSF->Get("hpileup_reweighting_Fall11");
    }
  }
  // return;
  
  TH1D *hnvtx[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  TH1D *Zmass[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  TH1D *Zmass_zoomed[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  TH2D *ZmassVsMuPosEta[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  
  TH2D *PosMuonPtCorrVsMuPosEta[WMass::nSigOrQCD][WMass::etaMuonNSteps], *PosMuonPtCorrVsMuPosPhi[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  TH2D *NegMuonPtCorrVsMuNegEta[WMass::nSigOrQCD][WMass::etaMuonNSteps], *NegMuonPtCorrVsMuNegPhi[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  
  TH1D *ZDeltaRMuPosJet[WMass::nSigOrQCD][WMass::etaMuonNSteps], *ZDeltaEtaMuPosJet[WMass::nSigOrQCD][WMass::etaMuonNSteps], *ZDeltaPhiMuPosJet[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  TH1D *ZDeltaRMuNegJet[WMass::nSigOrQCD][WMass::etaMuonNSteps], *ZDeltaEtaMuNegJet[WMass::nSigOrQCD][WMass::etaMuonNSteps], *ZDeltaPhiMuNegJet[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  TH1D *ZlepDeltaEta[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  TH2D *ZlepEta1VsEta2[WMass::nSigOrQCD][WMass::etaMuonNSteps], *ZlepDeltaEtaVsEta1[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  TH1D *ZlepDeltaPt[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  TH2D *ZlepPt1VsPt2[WMass::nSigOrQCD][WMass::etaMuonNSteps], *ZlepDeltaPtVsPt1[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  TH1D *ZlepDeltaPhi[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  TH2D *ZlepPhi1VsPhi2[WMass::nSigOrQCD][WMass::etaMuonNSteps], *ZlepDeltaPhiVsPhi1[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  TH1D *ZlepDeltaR[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  TH2D *ZlepDeltaPhiVsMass[WMass::nSigOrQCD][WMass::etaMuonNSteps], *ZlepDeltaEtaVsMass[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  TH2D *ZlepDeltaPtVsMass[WMass::nSigOrQCD][WMass::etaMuonNSteps], *ZlepDeltaRVsMass[WMass::nSigOrQCD][WMass::etaMuonNSteps];

  TH1D *hpfMET_Z[WMass::nSigOrQCD][WMass::etaMuonNSteps], *hpfMETphi_Z[WMass::nSigOrQCD][WMass::etaMuonNSteps], *hpfMET_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  TH1D *hpfMETphi_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps], *hZ_pt[WMass::nSigOrQCD][WMass::etaMuonNSteps], *hZ_phi[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  TH1D *hZ_mt[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  TH2D *hZ_ptVsmt[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  
  TH1D *hWlikePos_pt[WMass::nSigOrQCD][WMass::etaMuonNSteps], *hWlikePos_phi[WMass::nSigOrQCD][WMass::etaMuonNSteps], *hWlikePos_mt[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  TH1D *hMupt_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps], *hMueta_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps], *hMuphi_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps], *hMuiso_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  TH1D *hMuMETpt_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps], *hMuMETeta_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps], *hMuMETphi_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  TH1D *hMuMETiso_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps], *hJetpt_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps], *hJeteta_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  TH1D *hJetphi_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  TH1D *hu1_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps], *hu2_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  TH2D *hu1vsZpt_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps], *hu2vsZpt_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps];
  TH2D *hWlikePos_logiso_vs_logdxy[WMass::etaMuonNSteps];
  TH2D *hWlikePos_iso_vs_dxy[WMass::etaMuonNSteps];

  static const int nbins=75;
  float bins[nbins]={0.};
  float xmin=0.6,xmax=1.8, x=xmin;
  float binsize1=0.01,binsize2=0.04;
  float binsize=binsize1;
  for(int i=0;i<nbins;i++){
    bins[i]=x;
    if(x>1.2-binsize) binsize=binsize2;
    x+=binsize;
    // cout << bins[i] << endl;
  }
  bins[nbins]=xmax;
  // cout << xmax << endl;
  
  for(int i=0; i<WMass::etaMuonNSteps; i++){
    TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
    
    hWlikePos_logiso_vs_logdxy[i]=new TH2D(Form("hWlikePos_logiso_vs_logdxy_eta%s",eta_str.Data()),Form("hWlikePos_logiso_vs_logdxy_eta%s",eta_str.Data()),1000,-5,1,1000,-5,1);
    hWlikePos_iso_vs_dxy[i]=new TH2D(Form("hWlikePos_iso_vs_dxy_eta%s",eta_str.Data()),Form("hWlikePos_iso_vs_dxy_eta%s",eta_str.Data()),1000,-0.001,0.999,1000,0,1);
    
    for(int k=0; k<WMass::nSigOrQCD; k++){
      
      hnvtx[k][i]=new TH1D(Form("hnvtx_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hnvtx_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),50,0,50);
      Zmass[k][i]=new TH1D(Form("Zmass_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("Zmass_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),300,50,200);
      Zmass_zoomed[k][i]=new TH1D(Form("Zmass_zoomed_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("Zmass_zoomed_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),6000,60,120);
      ZmassVsMuPosEta[k][i]=new TH2D(Form("ZmassVsMuPosEta_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("ZmassVsMuPosEta_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),50,-2.5,2.5,200,90,92);
      
      PosMuonPtCorrVsMuPosEta[k][i]=new TH2D(Form("PosMuonPtCorrVsMuPosEta_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("PosMuonPtCorrVsMuPosEta_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),50,-2.5,2.5,300,60,120);
      PosMuonPtCorrVsMuPosPhi[k][i]=new TH2D(Form("PosMuonPtCorrVsMuPosPhi_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("PosMuonPtCorrVsMuPosPhi_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),50,-2*TMath::Pi(),2*TMath::Pi(),300,60,120);
      NegMuonPtCorrVsMuNegEta[k][i]=new TH2D(Form("NegMuonPtCorrVsMuNegEta_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("NegMuonPtCorrVsMuNegEta_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),50,-2.5,2.5,300,60,120);
      NegMuonPtCorrVsMuNegPhi[k][i]=new TH2D(Form("NegMuonPtCorrVsMuNegPhi_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("NegMuonPtCorrVsMuNegPhi_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),50,-2*TMath::Pi(),2*TMath::Pi(),300,60,120);
      
      ZDeltaRMuPosJet[k][i]=new TH1D(Form("ZDeltaRMuPosJet_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("ZDeltaRMuPosJet_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),1000,0,10);
      ZDeltaEtaMuPosJet[k][i]=new TH1D(Form("ZDeltaEtaMuPosJet_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("ZDeltaEtaMuPosJet_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),200,-5,5);
      ZDeltaPhiMuPosJet[k][i]=new TH1D(Form("ZDeltaPhiMuPosJet_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("ZDeltaPhiMuPosJet_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),200,-2*TMath::Pi(),2*TMath::Pi());
      ZDeltaRMuNegJet[k][i]=new TH1D(Form("ZDeltaRMuNegJet_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("ZDeltaRMuNegJet_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),1000,0,10);
      ZDeltaEtaMuNegJet[k][i]=new TH1D(Form("ZDeltaEtaMuNegJet_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("ZDeltaEtaMuNegJet_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),200,-5,5);
      ZDeltaPhiMuNegJet[k][i]=new TH1D(Form("ZDeltaPhiMuNegJet_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("ZDeltaPhiMuNegJet_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),200,-2*TMath::Pi(),2*TMath::Pi());
      ZlepDeltaEta[k][i]=new TH1D(Form("ZlepDeltaEta_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("ZlepDeltaEta_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),200,-5,5);
      ZlepEta1VsEta2[k][i]=new TH2D(Form("ZlepEta1VsEta2_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("ZlepEta1VsEta2_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),50,-2.5,2.5,50,-2.5,2.5);
      ZlepDeltaEtaVsEta1[k][i]=new TH2D(Form("ZlepDeltaEtaVsEta1_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("ZlepDeltaEtaVsEta1_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),50,-2.5,2.5,100,-5,5);
      ZlepDeltaPt[k][i]=new TH1D(Form("ZlepDeltaPt_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("ZlepDeltaPt_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),100,-50,50);
      ZlepPt1VsPt2[k][i]=new TH2D(Form("ZlepPt1VsPt2_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("ZlepPt1VsPt2_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),200,0,200,200,0,200);
      ZlepDeltaPtVsPt1[k][i]=new TH2D(Form("ZlepDeltaPtVsPt1_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("ZlepDeltaPtVsPt1_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),200,0,200,100,-50,50);
      ZlepDeltaPhi[k][i]=new TH1D(Form("ZlepDeltaPhi_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("ZlepDeltaPhi_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),200,-2*TMath::Pi(),2*TMath::Pi());
      ZlepPhi1VsPhi2[k][i]=new TH2D(Form("ZlepPhi1VsPhi2_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("ZlepPhi1VsPhi2_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),200,-2*TMath::Pi(),2*TMath::Pi(),200,-2*TMath::Pi(),2*TMath::Pi());
      ZlepDeltaPhiVsPhi1[k][i]=new TH2D(Form("ZlepDeltaPhiVsPhi1_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("ZlepDeltaPhiVsPhi1_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),200,-2*TMath::Pi(),2*TMath::Pi(),200,-2*TMath::Pi(),2*TMath::Pi());
      ZlepDeltaR[k][i]=new TH1D(Form("ZlepDeltaR_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("ZlepDeltaR_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),1000,0,10);
      ZlepDeltaPhiVsMass[k][i]=new TH2D(Form("ZlepDeltaPhiVsMass_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("ZlepDeltaPhiVsMass_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),200,0,200,200,-2*TMath::Pi(),2*TMath::Pi());
      ZlepDeltaEtaVsMass[k][i]=new TH2D(Form("ZlepDeltaEtaVsMass_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("ZlepDeltaEtaVsMass_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),200,0,200,50,-2.5,2.5);
      ZlepDeltaPtVsMass[k][i]=new TH2D(Form("ZlepDeltaPtVsMass_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("ZlepDeltaPtVsMass_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),200,0,200,100,-50,50);
      ZlepDeltaRVsMass[k][i]=new TH2D(Form("ZlepDeltaRVsMass_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("ZlepDeltaRVsMass_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),200,0,200,1000,0,10);

      hpfMET_Z[k][i]=new TH1D(Form("hpfMET_Z_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hpfMET_Z_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),100,0,200);
      hpfMETphi_Z[k][i]=new TH1D(Form("hpfMETphi_Z_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hpfMETphi_Z_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),100,-TMath::Pi(),TMath::Pi());
      hpfMET_WlikePos[k][i]=new TH1D(Form("hpfMET_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hpfMET_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),100,0,200);
      hpfMETphi_WlikePos[k][i]=new TH1D(Form("hpfMETphi_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hpfMETphi_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),100,-TMath::Pi(),TMath::Pi());
      hZ_pt[k][i]=new TH1D(Form("hZ_pt_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hZ_pt_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),100,0,25);
      hZ_phi[k][i]=new TH1D(Form("hZ_phi_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hZ_phi_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),100,-TMath::Pi(),TMath::Pi());
      hZ_mt[k][i]=new TH1D(Form("hZ_mt_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hZ_mt_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),100,0,200);
      hZ_ptVsmt[k][i]=new TH2D(Form("hZ_ptVsmt_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hZ_ptVsmt_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),150,50,200,100,0,25);
      
      hWlikePos_pt[k][i]=new TH1D(Form("hWlikePos_pt_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hWlikePos_pt_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),100,0,25);
      hWlikePos_phi[k][i]=new TH1D(Form("hWlikePos_phi_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hWlikePos_phi_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),100,-TMath::Pi(),TMath::Pi());
      hWlikePos_mt[k][i]=new TH1D(Form("hWlikePos_mt_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hWlikePos_mt_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),200,0,200);
      hMupt_WlikePos[k][i]=new TH1D(Form("hMupt_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hMupt_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),200,0,200);
      hMueta_WlikePos[k][i]=new TH1D(Form("hMueta_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hMueta_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),100,-2.5,2.5);
      hMuphi_WlikePos[k][i]=new TH1D(Form("hMuphi_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hMuphi_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),100,-TMath::Pi(),TMath::Pi());
      hMuiso_WlikePos[k][i]=new TH1D(Form("hMuiso_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hMuiso_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),1000,0,10);
      hMuMETpt_WlikePos[k][i]=new TH1D(Form("hMuMETpt_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hMuMETpt_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),100,0,200);
      hMuMETeta_WlikePos[k][i]=new TH1D(Form("hMuMETeta_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hMuMETeta_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),100,-2.5,2.5);
      hMuMETphi_WlikePos[k][i]=new TH1D(Form("hMuMETphi_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hMuMETphi_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),100,-TMath::Pi(),TMath::Pi());
      hMuMETiso_WlikePos[k][i]=new TH1D(Form("hMuMETiso_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hMuMETiso_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),100,0,10);
      hJetpt_WlikePos[k][i]=new TH1D(Form("hJetpt_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hJetpt_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),100,0,50);
      hJeteta_WlikePos[k][i]=new TH1D(Form("hJeteta_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hJeteta_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),100,-2.5,2.5);
      hJetphi_WlikePos[k][i]=new TH1D(Form("hJetphi_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hJetphi_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),100,-TMath::Pi(),TMath::Pi());
      hu1_WlikePos[k][i]=new TH1D(Form("hu1_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hu1_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),360,-30,150);
      hu2_WlikePos[k][i]=new TH1D(Form("hu2_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hu2_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),250,-50,50);
      hu1vsZpt_WlikePos[k][i]=new TH2D(Form("hu1vsZpt_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hu1vsZpt_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),100,0,25,360,-30,150);
      hu2vsZpt_WlikePos[k][i]=new TH2D(Form("hu2vsZpt_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),Form("hu2vsZpt_WlikePos_%s_eta%s",WMass::nSigOrQCD_str[k].Data(),eta_str.Data()),100,0,25,250,-50,50);
    }  
    
    for(int j=0; j<2*WMass::WMassNSteps+1; j++){
      int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV;
      hWlikePos_PtScaled_1_Gen[i][j]=new TH1D(Form("hWlikePos_PtScaled_1_Gen_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_1_Gen_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
      hWlikePos_PtScaled_1_Gen[i][j]->Sumw2();
      hWlikePos_PtScaled_2_ZGenMassCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_2_ZGenMassCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_2_ZGenMassCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
      hWlikePos_PtScaled_2_ZGenMassCut[i][j]->Sumw2();
      hWlikePos_PtScaled_3_Mu1GenCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_3_Mu1GenCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_3_Mu1GenCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
      hWlikePos_PtScaled_3_Mu1GenCut[i][j]->Sumw2();
      hWlikePos_PtScaled_4_Mu2GenCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_4_Mu2GenCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_4_Mu2GenCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
      hWlikePos_PtScaled_4_Mu2GenCut[i][j]->Sumw2();
      hWlikePos_PtScaled_5_RecoCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_5_RecoCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_5_RecoCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
      hWlikePos_PtScaled_5_RecoCut[i][j]->Sumw2();
      hWlikePos_PtScaled_6_METCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_6_METCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_6_METCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
      hWlikePos_PtScaled_6_METCut[i][j]->Sumw2();
      hWlikePos_PtScaled_7_RecoilCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_7_RecoilCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_7_RecoilCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
      hWlikePos_PtScaled_7_RecoilCut[i][j]->Sumw2();
      hWlikePos_PtScaled_8_JetCut[i][j]=new TH1D(Form("hWlikePos_PtScaled_8_JetCut_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_8_JetCut_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
      hWlikePos_PtScaled_8_JetCut[i][j]->Sumw2();
      hWlikePos_PtScaled_QCD[i][j]=new TH1D(Form("hWlikePos_PtScaled_QCD_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_QCD_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
      hWlikePos_PtScaled_QCD[i][j]->Sumw2();
      if(buildTemplates){
        hWlikePos_PtScaled_RWeighted_Templates[i][j]=new TH1D(Form("hWlikePos_PtScaled_RWeighted_Templates_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_PtScaled_RWeighted_Templates_eta%s_%d",eta_str.Data(),jWmass),nbins,bins);
        hWlikePos_PtScaled_RWeighted_Templates[i][j]->Sumw2();
        cout << Form("hR_WdivZ_WlikePos_8_JetCut_eta%s_%d",eta_str.Data(),jWmass) << endl;
        hWlikePos_R_WdivZ[i][j]=(TH1D*)finTemplates->Get(Form("hR_WdivZ_WlikePos_8_JetCut_eta%s_%d",eta_str.Data(),jWmass));
        hWlikePos_R_WdivZ[i][j]->Print();
      }
    }
  }


  if (fChain == 0) return;

  Long64_t first_entry = 0;
  Long64_t nentries = fChain->GetEntriesFast();
  if(IS_MC_CLOSURE_TEST==1 && isMCorDATA==1) first_entry=nentries/2; // in case of closure test, DATA runs from N/2 to N
  if(IS_MC_CLOSURE_TEST==1 && isMCorDATA==0) nentries=nentries/2; // in case of closure test, MC runs from 0 to N/2
  if(IS_MC_CLOSURE_TEST==1) lumi_scaling=lumi_scaling*2; // in case of closure test, scaling must be multiplied by 2
  
  //To get the central value of the momentum correction
  // rochcor42X *rmcor42X = new rochcor42X();  // make the pointer of rochcor class
  rochcor44X *rmcor44X = new rochcor44X();  // make the pointer of rochcor class
  TString MuscleCard = (IS_MC_CLOSURE_TEST || isMCorDATA==0) ? "MuScleFit_2011_MC_44X" : "MuScleFit_2011_DATA_44X";
  TString fitParametersFile = MuscleCard+".txt";
  MuScleFitCorrector *corrector;
  if(useMomentumCorr==2){
    cout << "using MuscleFit card " << fitParametersFile << endl;
    corrector = new MuScleFitCorrector(fitParametersFile);
  }

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=first_entry; jentry<nentries;jentry++) {
    // for (Long64_t jentry=0; jentry<5e5;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    if(jentry%250000==0) cout <<"Analyzed entry "<<jentry<<"/"<<nentries<<endl;

    if(run>175832) continue; // TO TEST ROCHESTER CORRECTIONS ONLY ON RUN2011A
    
    double evt_weight = lumi_scaling;
    if(useVtxSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && npu>0) evt_weight=lumi_scaling*hPileupSF->GetBinContent(hPileupSF->GetXaxis()->FindBin(npu));
    
    int runopt = r->Rndm()<0.457451 ? 0 : 1;
    double MuPos_tight_muon_SF = 1;
    double MuNeg_tight_muon_SF = 1;
    if(useEffSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0)){
      MuPos_tight_muon_SF = hEffSF_MuId_eta_2011[runopt]->Eval(MuPos_eta)*hEffSF_Iso_eta_2011[runopt]->Eval(MuPos_eta);
      MuNeg_tight_muon_SF = hEffSF_MuId_eta_2011[runopt]->Eval(MuNeg_eta)*hEffSF_Iso_eta_2011[runopt]->Eval(MuNeg_eta);
    }
    // cout << "MuPos_tight_muon_SF= " << MuPos_tight_muon_SF << endl;
    // if(!(IS_MC_CLOSURE_TEST || isMCorDATA==0) && run<175832) continue; // TEMPORARY TO TEST ROCHESTER CORRECTIONS
    
    if(IS_MC_CLOSURE_TEST || isMCorDATA==0) hPileUp_Fall11->Fill(npu);

    for(int i=0; i<WMass::etaMuonNSteps; i++){
      for(int j=0; j<2*WMass::WMassNSteps+1; j++){
        
        double iWmass = (WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV)/1e3;
        // SELECT EVENT WITH Z/W SCALED CUTS
        // ORIGINAL CUTS:
        // muon pt > 30,
        // pfmetcut > 25,
        // Boson pt (recoilcut) < 20,
        
        // gen stuff to be used only when required
        if(useGenVar){
          if(ZGen_mass>0){
            double MuPosGen_pt_jacobian = 2*MuPosGen_pt/WMass::ZMass;
            hWlikePos_PtScaled_1_Gen[i][j]->Fill(MuPosGen_pt_jacobian,evt_weight);
            
            if(ZGen_mass>50){
              hWlikePos_PtScaled_2_ZGenMassCut[i][j]->Fill(MuPosGen_pt_jacobian,evt_weight);
              
              if(TMath::Abs(MuPosGen_eta)<WMass::etaMaxMuons[i]){
                hWlikePos_PtScaled_3_Mu1GenCut[i][j]->Fill(MuPosGen_pt_jacobian,evt_weight);
                
                if(TMath::Abs(MuNegGen_eta)<2.4){
                  hWlikePos_PtScaled_4_Mu2GenCut[i][j]->Fill(MuPosGen_pt_jacobian,evt_weight);
                }
              }
            }
          }
        } // end gen stuff 
        
        if(!useGenVar || Z_mass>0){ // dummy thing to separate signal and background in DY+Jets (useless)
          if( evtHasGoodVtx && evtHasTrg){ // good reco event

            TLorentzVector muPosCorr,muNegCorr, muPosNoCorr,muNegNoCorr, Zcorr; //TLorentzVector of the reconstructed muon
            //Set TLorentzVector of mu, and mubar
            muPosCorr.SetPtEtaPhiM(MuPos_pt,MuPos_eta,MuPos_phi,MuPos_mass);
            muPosNoCorr = muPosCorr;
            muNegCorr.SetPtEtaPhiM(MuNeg_pt,MuNeg_eta,MuNeg_phi,MuNeg_mass);
            muNegNoCorr = muNegCorr;

            if(useMomentumCorr==1){ // use rochester corrections if required
              if(IS_MC_CLOSURE_TEST || isMCorDATA==0){
                // int runopt = r->Rndm()<0.457451 ? 0 : 1;
                // int runopt = 1;
                // rmcor42X->momcor_mc(muPosCorr, MuPos_charge, smearRochCorrByNsigma, runopt);
                // rmcor42X->momcor_mc(muNegCorr, MuNeg_charge, smearRochCorrByNsigma, runopt);
                rmcor44X->momcor_mc(muPosCorr, MuPos_charge, smearRochCorrByNsigma/* , runopt */);
                rmcor44X->momcor_mc(muNegCorr, MuNeg_charge, smearRochCorrByNsigma/* , runopt */);
                // musclefit_data
              }
              else{
                rmcor44X->momcor_data(muPosCorr, MuPos_charge, smearRochCorrByNsigma/* , run<175832 ? 0 : 1  */);
                rmcor44X->momcor_data(muNegCorr, MuNeg_charge, smearRochCorrByNsigma/* , run<175832 ? 0 : 1  */);
              }
            }else if(useMomentumCorr==2){ // use Momentum scale corrections if required
              corrector->applyPtCorrection(muNegCorr,MuNeg_charge);
              corrector->applyPtCorrection(muPosCorr,MuPos_charge);
            }
            
            Zcorr = muPosCorr + muNegCorr;

            double MuPos_pt_jacobian = 2*muPosCorr.Pt()/WMass::ZMass;
            int wmass1 = iWmass*1e3;
            
            // good pair within acceptance cuts for both muons
            if( /* Z_mass */Zcorr.M()>50
                && TMath::Abs(/* MuPos_eta */muPosCorr.Eta())<WMass::etaMaxMuons[i] && muPosCorr.Pt()>30*WMass::ZMass/iWmass && MuPosTrg
                && TMath::Abs(muNegCorr.Eta())<2.4 && muNegCorr.Pt()>10
                ){
              // full ID and tight requirements on the muon
              if(MuPosIsTightAndIso && MuPosRelIso<0.12 && MuPos_dxy<0.02){
                hWlikePos_PtScaled_5_RecoCut[i][j]->Fill(MuPos_pt_jacobian,evt_weight*MuPos_tight_muon_SF);

                if(pfmetWlikePos>25*WMass::ZMass/iWmass){
                  hWlikePos_PtScaled_6_METCut[i][j]->Fill(MuPos_pt_jacobian,evt_weight*MuPos_tight_muon_SF);
                  
                  if(WlikePos_pt<20*WMass::ZMass/iWmass){
                    hWlikePos_PtScaled_7_RecoilCut[i][j]->Fill(MuPos_pt_jacobian,evt_weight*MuPos_tight_muon_SF);
                    
                    if(true){ // for the momet remove jet pt cut
                    // if(Jet_leading_pt<30){
                      hWlikePos_PtScaled_8_JetCut[i][j]->Fill(MuPos_pt_jacobian,evt_weight*MuPos_tight_muon_SF);
                      
                      if(buildTemplates){
                        double weight=hWlikePos_R_WdivZ[i][j]->GetBinContent(hWlikePos_R_WdivZ[i][j]->GetXaxis()->FindBin(MuPos_pt_jacobian));
                        // cout << "etamax= " << WMass::etaMaxMuons[i] << " mass= " << iWmass << " MuPos_pt_jacobian= " << MuPos_pt_jacobian << " weight= " << weight << endl;
                        hWlikePos_PtScaled_RWeighted_Templates[i][j]->Fill(MuPos_pt_jacobian,weight*evt_weight*MuPos_tight_muon_SF);
                      }
                      
                      // control plots for different etas but only for central W mass
                      if(TMath::Abs(wmass1 - WMass::WMassCentral_MeV) > 1)  continue;
                      
                      // control distributions for central W mass
                      hnvtx[0][i]->Fill(nvtx,evt_weight);
                      Zmass[0][i]->Fill(Zcorr.M(),evt_weight*MuPos_tight_muon_SF);
                      Zmass_zoomed[0][i]->Fill(Zcorr.M(),evt_weight*MuPos_tight_muon_SF);
                      // ZmassVsMuPosEta[0][i]->Fill(MuPos_eta,Zcorr.M(),evt_weight*MuPos_tight_muon_SF);
                      ZmassVsMuPosEta[0][i]->Fill(muPosCorr.Eta(),Zcorr.M(),evt_weight*MuPos_tight_muon_SF);
                      
                      // PosMuonPtCorrVsMuPosEta[0][i]->Fill(MuPos_eta,Zcorr.M(),evt_weight*MuPos_tight_muon_SF);
                      
                      ZlepDeltaEta[0][i]->Fill(muPosCorr.Eta()-muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                      ZlepEta1VsEta2[0][i]->Fill(muPosCorr.Eta(),muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                      ZlepDeltaEtaVsEta1[0][i]->Fill(muPosCorr.Eta(),muPosCorr.Eta()-muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                      ZlepDeltaEtaVsMass[0][i]->Fill(Zcorr.M(),muPosCorr.Eta()-muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                      ZlepDeltaPt[0][i]->Fill(muPosCorr.Pt()-muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                      ZlepPt1VsPt2[0][i]->Fill(muPosCorr.Pt(),muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                      ZlepDeltaPtVsPt1[0][i]->Fill(muPosCorr.Pt(),muPosCorr.Pt()-muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                      ZlepDeltaPtVsMass[0][i]->Fill(Zcorr.M(),muPosCorr.Pt()-muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                      ZlepDeltaPhi[0][i]->Fill(muPosCorr.Phi()-muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                      ZlepPhi1VsPhi2[0][i]->Fill(muPosCorr.Phi(),muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                      ZlepDeltaPhiVsPhi1[0][i]->Fill(muPosCorr.Phi(),muPosCorr.Phi()-muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                      ZlepDeltaPhiVsMass[0][i]->Fill(Zcorr.M(),muPosCorr.Phi()-muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                      ZlepDeltaR[0][i]->Fill(TMath::Hypot(muPosCorr.Phi()-muNegCorr.Phi(),muPosCorr.Eta()-muNegCorr.Eta()),evt_weight*MuPos_tight_muon_SF);
                      ZlepDeltaRVsMass[0][i]->Fill(Zcorr.M(),TMath::Hypot(muNegCorr.Phi()-Jet_leading_phi,muNegCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF);
                      ZDeltaEtaMuPosJet[0][i]->Fill(muPosCorr.Eta()-Jet_leading_eta,evt_weight*MuPos_tight_muon_SF);
                      ZDeltaPhiMuPosJet[0][i]->Fill(muPosCorr.Phi()-Jet_leading_phi,evt_weight*MuPos_tight_muon_SF);
                      ZDeltaRMuPosJet[0][i]->Fill(TMath::Hypot(muPosCorr.Phi()-Jet_leading_phi,muPosCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF);
                      ZDeltaEtaMuNegJet[0][i]->Fill(muNegCorr.Eta()-Jet_leading_eta,evt_weight*MuPos_tight_muon_SF);
                      ZDeltaPhiMuNegJet[0][i]->Fill(muNegCorr.Phi()-Jet_leading_phi,evt_weight*MuPos_tight_muon_SF);
                      ZDeltaRMuNegJet[0][i]->Fill(TMath::Hypot(muNegCorr.Phi()-Jet_leading_phi,muNegCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF);
                      hpfMET_Z[0][i]->Fill(pfmet,evt_weight*MuPos_tight_muon_SF);
                      hpfMETphi_Z[0][i]->Fill(pfmet_phi,evt_weight*MuPos_tight_muon_SF);
                      hZ_pt[0][i]->Fill(Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                      hZ_phi[0][i]->Fill(Zcorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                      hZ_mt[0][i]->Fill(Zcorr.Mt(),evt_weight*MuPos_tight_muon_SF);
                      hZ_ptVsmt[0][i]->Fill(Zcorr.Mt(),Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                      hWlikePos_pt[0][i]->Fill(WlikePos_pt,evt_weight*MuPos_tight_muon_SF);
                      hWlikePos_phi[0][i]->Fill(WlikePos_phi,evt_weight*MuPos_tight_muon_SF);
                      hWlikePos_mt[0][i]->Fill(WlikePos_mt,evt_weight*MuPos_tight_muon_SF);
                      hpfMET_WlikePos[0][i]->Fill(pfmetWlikePos,evt_weight*MuPos_tight_muon_SF);
                      hpfMETphi_WlikePos[0][i]->Fill(pfmetWlikePos_phi,evt_weight*MuPos_tight_muon_SF);
                      hMupt_WlikePos[0][i]->Fill(muPosCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                      hMueta_WlikePos[0][i]->Fill(muPosCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                      hMuphi_WlikePos[0][i]->Fill(muPosCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                      hMuiso_WlikePos[0][i]->Fill(MuPosRelIso,evt_weight*MuPos_tight_muon_SF);
                      hMuMETpt_WlikePos[0][i]->Fill(muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                      hMuMETeta_WlikePos[0][i]->Fill(muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                      hMuMETphi_WlikePos[0][i]->Fill(muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                      hMuMETiso_WlikePos[0][i]->Fill(MuNegRelIso,evt_weight*MuPos_tight_muon_SF);
                      hJetpt_WlikePos[0][i]->Fill(Jet_leading_pt,evt_weight*MuPos_tight_muon_SF);
                      hJeteta_WlikePos[0][i]->Fill(Jet_leading_eta,evt_weight*MuPos_tight_muon_SF);
                      hJetphi_WlikePos[0][i]->Fill(Jet_leading_phi,evt_weight*MuPos_tight_muon_SF);
                      hu1_WlikePos[0][i]->Fill(u1,evt_weight*MuPos_tight_muon_SF);
                      hu2_WlikePos[0][i]->Fill(u2,evt_weight*MuPos_tight_muon_SF);
                      hu1vsZpt_WlikePos[0][i]->Fill(Zcorr.Pt(),u1,evt_weight*MuPos_tight_muon_SF);
                      hu2vsZpt_WlikePos[0][i]->Fill(Zcorr.Pt(),u2,evt_weight*MuPos_tight_muon_SF);

                    }
                  }
                }
              }else{
                
                // Iso and dxy for muons which fail either tight requirement, isolation or dxy cut
                if(pfmetWlikePos>25 && WlikePos_pt<20){
                  if( (TMath::Abs(wmass1 - WMass::WMassCentral_MeV) > 1)) continue;
                  hWlikePos_logiso_vs_logdxy[i]->Fill(TMath::Log10(MuPos_dxy),TMath::Log10(MuPosRelIso),evt_weight*MuPos_tight_muon_SF);
                  hWlikePos_iso_vs_dxy[i]->Fill(MuPos_dxy,MuPosRelIso,evt_weight*MuPos_tight_muon_SF);
                }

                // invert cuts to select QCD enriched region, no cuts on met or jet
                if(
                  MuPosRelIso>0.12 // single muon cuts (inverted iso (is <0.5 for signal) , no tight requirement)
                  && MuPos_dxy>0.02 // single muon cuts (MuPosIsTightAndIso contains dxy < 0.2 cut)
                  && WlikePos_pt<20*WMass::ZMass/iWmass 
                  ){

                  hWlikePos_PtScaled_QCD[i][j]->Fill(MuPos_pt_jacobian,evt_weight*MuPos_tight_muon_SF);
                  
                  if( (TMath::Abs(wmass1 - WMass::WMassCentral_MeV) > 1)
                      ) 
                  continue;

                  // QCD distributions for central W mass
                  hnvtx[1][i]->Fill(nvtx,evt_weight);
                  Zmass[1][i]->Fill(Zcorr.M(),evt_weight*MuPos_tight_muon_SF);
                  Zmass_zoomed[1][i]->Fill(Zcorr.M(),evt_weight*MuPos_tight_muon_SF);
                  ZmassVsMuPosEta[1][i]->Fill(muPosCorr.Eta(),Zcorr.M(),evt_weight*MuPos_tight_muon_SF);
                  ZlepDeltaEta[1][i]->Fill(muPosCorr.Eta()-muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                  ZlepEta1VsEta2[1][i]->Fill(muPosCorr.Eta(),muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                  ZlepDeltaEtaVsEta1[1][i]->Fill(muPosCorr.Eta(),muPosCorr.Eta()-muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                  ZlepDeltaEtaVsMass[1][i]->Fill(Zcorr.M(),muPosCorr.Eta()-muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                  ZlepDeltaPt[1][i]->Fill(muPosCorr.Pt()-muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                  ZlepPt1VsPt2[1][i]->Fill(muPosCorr.Pt(),muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                  ZlepDeltaPtVsPt1[1][i]->Fill(muPosCorr.Pt(),muPosCorr.Pt()-muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                  ZlepDeltaPtVsMass[1][i]->Fill(Zcorr.M(),muPosCorr.Pt()-muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                  ZlepDeltaPhi[1][i]->Fill(muPosCorr.Phi()-muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                  ZlepPhi1VsPhi2[1][i]->Fill(muPosCorr.Phi(),muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                  ZlepDeltaPhiVsPhi1[1][i]->Fill(muPosCorr.Phi(),muPosCorr.Phi()-muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                  ZlepDeltaPhiVsMass[1][i]->Fill(Zcorr.M(),muPosCorr.Phi()-muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                  ZlepDeltaR[1][i]->Fill(TMath::Hypot(muPosCorr.Phi()-muNegCorr.Phi(),muPosCorr.Eta()-muNegCorr.Eta()),evt_weight*MuPos_tight_muon_SF);
                  ZlepDeltaRVsMass[1][i]->Fill(Zcorr.M(),TMath::Hypot(muNegCorr.Phi()-Jet_leading_phi,muNegCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF);
                  ZDeltaEtaMuPosJet[1][i]->Fill(muPosCorr.Eta()-Jet_leading_eta,evt_weight*MuPos_tight_muon_SF);
                  ZDeltaPhiMuPosJet[1][i]->Fill(muPosCorr.Phi()-Jet_leading_phi,evt_weight*MuPos_tight_muon_SF);
                  ZDeltaRMuPosJet[1][i]->Fill(TMath::Hypot(muPosCorr.Phi()-Jet_leading_phi,muPosCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF);
                  ZDeltaEtaMuNegJet[1][i]->Fill(muNegCorr.Eta()-Jet_leading_eta,evt_weight*MuPos_tight_muon_SF);
                  ZDeltaPhiMuNegJet[1][i]->Fill(muNegCorr.Phi()-Jet_leading_phi,evt_weight*MuPos_tight_muon_SF);
                  ZDeltaRMuNegJet[1][i]->Fill(TMath::Hypot(muNegCorr.Phi()-Jet_leading_phi,muNegCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF);
                  hpfMET_Z[1][i]->Fill(pfmet,evt_weight*MuPos_tight_muon_SF);
                  hpfMETphi_Z[1][i]->Fill(pfmet_phi,evt_weight*MuPos_tight_muon_SF);
                  hZ_pt[1][i]->Fill(Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                  hZ_phi[1][i]->Fill(Zcorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                  hZ_mt[1][i]->Fill(Zcorr.Mt(),evt_weight*MuPos_tight_muon_SF);
                  hZ_ptVsmt[1][i]->Fill(Zcorr.Mt(),Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                  hWlikePos_pt[1][i]->Fill(WlikePos_pt,evt_weight*MuPos_tight_muon_SF);
                  hWlikePos_phi[1][i]->Fill(WlikePos_phi,evt_weight*MuPos_tight_muon_SF);
                  hWlikePos_mt[1][i]->Fill(WlikePos_mt,evt_weight*MuPos_tight_muon_SF);
                  hpfMET_WlikePos[1][i]->Fill(pfmetWlikePos,evt_weight*MuPos_tight_muon_SF);
                  hpfMETphi_WlikePos[1][i]->Fill(pfmetWlikePos_phi,evt_weight*MuPos_tight_muon_SF);
                  hMupt_WlikePos[1][i]->Fill(muPosCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                  hMueta_WlikePos[1][i]->Fill(muPosCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                  hMuphi_WlikePos[1][i]->Fill(muPosCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                  hMuiso_WlikePos[1][i]->Fill(MuPosRelIso,evt_weight*MuPos_tight_muon_SF);
                  hMuMETpt_WlikePos[1][i]->Fill(muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                  hMuMETeta_WlikePos[1][i]->Fill(muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                  hMuMETphi_WlikePos[1][i]->Fill(muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                  hMuMETiso_WlikePos[1][i]->Fill(MuNegRelIso,evt_weight*MuPos_tight_muon_SF);
                  hJetpt_WlikePos[1][i]->Fill(Jet_leading_pt,evt_weight*MuPos_tight_muon_SF);
                  hJeteta_WlikePos[1][i]->Fill(Jet_leading_eta,evt_weight*MuPos_tight_muon_SF);
                  hJetphi_WlikePos[1][i]->Fill(Jet_leading_phi,evt_weight*MuPos_tight_muon_SF);
                  hu1_WlikePos[1][i]->Fill(u1,evt_weight*MuPos_tight_muon_SF);
                  hu2_WlikePos[1][i]->Fill(u2,evt_weight*MuPos_tight_muon_SF);
                  hu1vsZpt_WlikePos[1][i]->Fill(Zcorr.Pt(),u1,evt_weight*MuPos_tight_muon_SF);
                  hu2vsZpt_WlikePos[1][i]->Fill(Zcorr.Pt(),u2,evt_weight*MuPos_tight_muon_SF);
                  
                } // end if for qcd enriched
              } // end else for muon cuts (sig or qcd enriched)
            } // end if for good pair within acceptance cuts for both muons
          } // end if for good reco event
        } // end if for dummy signal/fake separation
      } // end W mass loop
    } // end muon eta loop
        
  } // end event loop
  
  fout->cd();
  hPileUp_Fall11->Write();
  for(int i=0; i<WMass::etaMuonNSteps; i++){
    if(!buildTemplates){
      
      hWlikePos_logiso_vs_logdxy[i]->Write();
      hWlikePos_iso_vs_dxy[i]->Write();
      
      for(int k=0; k<WMass::nSigOrQCD; k++){
        hnvtx[k][i]->Write();
        Zmass[k][i]->Write();
        Zmass_zoomed[k][i]->Write();
        ZmassVsMuPosEta[k][i]->Write();
        ZlepDeltaEta[k][i]->Write();
        ZlepEta1VsEta2[k][i]->Write();
        ZlepDeltaEtaVsEta1[k][i]->Write();
        ZlepDeltaEtaVsMass[k][i]->Write();
        ZlepDeltaPt[k][i]->Write();
        ZlepPt1VsPt2[k][i]->Write();
        ZlepDeltaPtVsPt1[k][i]->Write();
        ZlepDeltaPtVsMass[k][i]->Write();
        ZlepDeltaPhi[k][i]->Write();
        ZlepPhi1VsPhi2[k][i]->Write();
        ZlepDeltaPhiVsPhi1[k][i]->Write();
        ZlepDeltaPhiVsMass[k][i]->Write();
        ZlepDeltaR[k][i]->Write();
        ZlepDeltaRVsMass[k][i]->Write();
        ZDeltaEtaMuPosJet[k][i]->Write();
        ZDeltaPhiMuPosJet[k][i]->Write();
        ZDeltaRMuPosJet[k][i]->Write();
        ZDeltaEtaMuNegJet[k][i]->Write();
        ZDeltaPhiMuNegJet[k][i]->Write();
        ZDeltaRMuNegJet[k][i]->Write();
        hpfMET_Z[k][i]->Write();
        hpfMETphi_Z[k][i]->Write();
        hpfMET_WlikePos[k][i]->Write();
        hpfMETphi_WlikePos[k][i]->Write();
        hZ_pt[k][i]->Write();
        hZ_phi[k][i]->Write();
        hZ_mt[k][i]->Write();
        hZ_ptVsmt[k][i]->Write();
        hWlikePos_pt[k][i]->Write();
        hWlikePos_phi[k][i]->Write();
        hWlikePos_mt[k][i]->Write();
        hMupt_WlikePos[k][i]->Write();
        hMueta_WlikePos[k][i]->Write();
        hMuphi_WlikePos[k][i]->Write();
        hMuiso_WlikePos[k][i]->Write();
        hMuMETpt_WlikePos[k][i]->Write();
        hMuMETeta_WlikePos[k][i]->Write();
        hMuMETphi_WlikePos[k][i]->Write();
        hMuMETiso_WlikePos[k][i]->Write();
        hJetpt_WlikePos[k][i]->Write();
        hJeteta_WlikePos[k][i]->Write();
        hJetphi_WlikePos[k][i]->Write();
        hu1_WlikePos[k][i]->Write();
        hu2_WlikePos[k][i]->Write();
        hu1vsZpt_WlikePos[k][i]->Write();
        hu2vsZpt_WlikePos[k][i]->Write();
      }
    }
    for(int j=0; j<2*WMass::WMassNSteps+1; j++){
      if(!buildTemplates){
        hWlikePos_PtScaled_1_Gen[i][j]->Write();
        hWlikePos_PtScaled_2_ZGenMassCut[i][j]->Write();
        hWlikePos_PtScaled_3_Mu1GenCut[i][j]->Write();
        hWlikePos_PtScaled_4_Mu2GenCut[i][j]->Write();
        hWlikePos_PtScaled_5_RecoCut[i][j]->Write();
        hWlikePos_PtScaled_6_METCut[i][j]->Write();
        hWlikePos_PtScaled_7_RecoilCut[i][j]->Write();
        hWlikePos_PtScaled_8_JetCut[i][j]->Write();
        hWlikePos_PtScaled_QCD[i][j]->Write();
      }else{
        hWlikePos_PtScaled_RWeighted_Templates[i][j]->Write();
      }
    }
  }
  
  fout->Write();
  fout->Close();

}
