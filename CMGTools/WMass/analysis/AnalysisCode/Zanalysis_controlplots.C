// UNCOMMENT TO USE PDF REWEIGHTING
//#define LHAPDF_ON

#ifdef LHAPDF_ON
  #include "LHAPDF/LHAPDF.h"
#endif 

#define Zanalysis_controlplots_cxx
#include "Zanalysis_controlplots.h"

#include "../includes/common.h"
// #include "rochcor_42X.h"
#include "rochcor_44X_v3.h"
#include "MuScleFitCorrector.h"
#include "RecoilCorrector.cc"
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

void Zanalysis_controlplots::Loop(int IS_MC_CLOSURE_TEST, int isMCorDATA, TString outputdir, int buildTemplates, int useMomentumCorr, int smearRochCorrByNsigma, int useEffSF, int usePtSF, int useVtxSF, int controlplots, TString sampleName, int generated_PDF_set, int generated_PDF_member, int contains_PDF_reweight)
{

  std::map<std::string, TH1F*> h_1d;
  std::map<std::string, TH2F*> h_2d;

  if (fChain == 0) return;

  ofstream outTXTfile;

  TRandom3 *r = new TRandom3(0);

  if(!outputdir.Contains("../")) outputdir = "../"+outputdir;
  cout << "output filename= " << Form("%s/Zanalysis_controlplots.root",outputdir.Data()) << endl;
  outTXTfile.open(Form("%s/Zanalysis_controlplots_EVlog.log",outputdir.Data()));
  
  #ifdef LHAPDF_ON
    cout << "inizializing LHAPDF::initPDFSet(0)" << endl;
    // LHAPDF::initPDFSet();
    if(WMass::PDF_sets==11200)
      LHAPDF::initPDFSet(0,"CT10nnlo.LHgrid");
    else if(WMass::PDF_sets==232000)
      LHAPDF::initPDFSet(0,"NNPDF23_nnlo_as_0118.LHgrid");
    else if(WMass::PDF_sets==21200)
      LHAPDF::initPDFSet(0,"MSTW2008nnlo68cl.LHgrid");
    // else if(WMass::PDF_sets<0)
      // LHAPDF::initPDFSet(0,generated_PDF_set,generated_PDF_member);
    
    if(!sampleName.Contains("DATA")){
      cout << "inizializing LHAPDF::initPDFSet(1)" << endl;
      // LHAPDF::initPDFSet(1,"CT10nnlo.LHgrid");
      LHAPDF::initPDFSet(1,generated_PDF_set,generated_PDF_member); // CMSSW DEFAULT
      cout << "finished inizializing LHAPDF" << endl;
    }
  #endif
  // TFile*feffSF = new TFile(Form("../Zanalysis_controlplots.root",outputdir.Data()),"RECREATE");
  
  TH1D *hWlikePos_VarScaled_1_Gen[WMass::NFitVar][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWlikePos_VarScaled_2_ZGenMassCut[WMass::NFitVar][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWlikePos_VarScaled_3_Mu1GenCut[WMass::NFitVar][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWlikePos_VarScaled_4_Mu2GenCut[WMass::NFitVar][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWlikePos_VarScaled_5_RecoCut[WMass::NFitVar][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWlikePos_VarScaled_6_METCut[WMass::NFitVar][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWlikePos_VarScaled_7_RecoilCut[WMass::NFitVar][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWlikePos_VarScaled_8_JetCut[WMass::NVarRecoilCorr][WMass::PDF_members][WMass::NFitVar][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWlikePos_VarNonScaled_8_JetCut[WMass::NVarRecoilCorr][WMass::PDF_members][WMass::NFitVar][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWlikePos_VarScaled_QCD[WMass::NFitVar][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];

  TH1D *hPDF_weights[WMass::PDF_members];
  for(int h=0; h<WMass::PDF_members; h++)
    hPDF_weights[h]=new TH1D(Form("hPDF_weights_%d_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h),Form("hPDF_weights_%d_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h),1000,0,2);

  /*
  TH1D *hPDF_x1=new TH1D("hPDF_x1","hPDF_x1",1000,-4,0);
  TH1D *hPDF_x1unweighted=new TH1D("hPDF_x1unweighted","hPDF_x1unweighted",1000,-4,0);
  TH1D *hPDF_x2=new TH1D("hPDF_x2","hPDF_x2",1000,-4,0);
  TH1D *hPDF_x2unweighted=new TH1D("hPDF_x2unweighted","hPDF_x2unweighted",1000,-4,0);
  TH1D *hPileUp_Fall11=new TH1D("hPileUp_Fall11","hPileUp_Fall11",50,0,50);
  */
  TH1D *hWlikePos_VarScaled_RWeighted_Templates[WMass::PDF_members][WMass::NFitVar][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1]; // used only to build templates
  TH1D*hWlikePos_R_WdivZ[WMass::PDF_members][WMass::NFitVar][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1]; // used only to build templates
  TFile*finTemplates, *finEffSF, *finPileupSF, *finZPtSF;
  TGraphAsymmErrors*hEffSF_MuId_eta_2011[2],*hEffSF_Iso_eta_2011[2],*hEffSF_HLT_eta_2011/* ,*hEffSF_Iso_vtx_2011A,*hEffSF_Iso_vtx_2011B*/;
  TH1D*hPileupSF,*hZPtSF;
  
  if(useEffSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0)){
    finEffSF = new TFile("../utils/MuonEfficiencies_SF_2011_44X_DataMC.root"); // used only to build templates
    hEffSF_MuId_eta_2011[0]=(TGraphAsymmErrors*)finEffSF->Get("SF_TIGHT_nL8_2011A_eta__pt>20");
    hEffSF_MuId_eta_2011[1]=(TGraphAsymmErrors*)finEffSF->Get("SF_TIGHT_nL8_2011B_eta__pt>20");
    hEffSF_Iso_eta_2011[0]=(TGraphAsymmErrors*)finEffSF->Get("combRelPFISO12_2011A_eta__pt>20");
    hEffSF_Iso_eta_2011[1]=(TGraphAsymmErrors*)finEffSF->Get("combRelPFISO12_2011B_eta__pt>20");
    hEffSF_HLT_eta_2011=(TGraphAsymmErrors*)finEffSF->Get("SF_HLT_MuIso24_2011_eta__pt>30");
    // hEffSF_Iso_vtx_2011A=(TH1D*)finEffSF->Get("combRelPFISO12_2011A_vtx__pt>20_abseta<2.4");
    // hEffSF_Iso_vtx_2011B=(TH1D*)finEffSF->Get("combRelPFISO12_2011B_vtx__pt>20_abseta<2.4");
    if(!finEffSF){
      cout << "file MuonEfficiencies_SF_2011_44X_DataMC.root is missing, impossible to retrieve efficiency scale factors" << endl;
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
  if(usePtSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && sampleName.Contains("DYJetsSig")){
    cout << "REWEIGHTING Z PT" << endl;
    finZPtSF = new TFile(Form("../utils/Zpt_reweighting.root")); // used only to build templates
    if(!finZPtSF){
      cout << "file " << Form("../utils/Zpt_reweighting.root") << " is missing, impossible to retrieve ZPt reweighting factors" << endl;
      return;
    }else{
      hZPtSF=(TH1D*)finZPtSF->Get("hZ_pt_Sig_eta0p6");
    }
  }
  // return;
  
  TH1D *hnvtx[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *Zmass[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *Zmass_zoomed[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH2D *ZmassVsMuPosEta[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  
  TH2D *PosMuonPtCorrVsMuPosEta[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *PosMuonPtCorrVsMuPosPhi[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH2D *NegMuonPtCorrVsMuNegEta[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *NegMuonPtCorrVsMuNegPhi[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  
  TH1D *ZDeltaRMuPosJet[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *ZDeltaEtaMuPosJet[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *ZDeltaPhiMuPosJet[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *ZDeltaRMuNegJet[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *ZDeltaEtaMuNegJet[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *ZDeltaPhiMuNegJet[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *ZlepDeltaEta[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH2D *ZlepEta1VsEta2[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *ZlepDeltaEtaVsEta1[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *ZlepDeltaPt[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH2D *ZlepPt1VsPt2[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *ZlepDeltaPtVsPt1[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *ZlepDeltaPhi[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH2D *ZlepPhi1VsPhi2[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *ZlepDeltaPhiVsPhi1[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *ZlepDeltaR[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  // TH2D *ZlepDeltaPhiVsMass[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *ZlepDeltaEtaVsMass[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  // TH2D *ZlepDeltaPtVsMass[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *ZlepDeltaRVsMass[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];

  TH1D *hpfMET_Z[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hpfMETphi_Z[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hpfMET_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hpfMETphi_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hZ_pt[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hZ_phi[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hZ_mt[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH2D *hZ_ptVsmt[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  
  TH1D *hWlikePos_pt[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hWlikePos_phi[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hWlikePos_mt[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hMupt_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hMueta_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hMuphi_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hMuiso_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hMuMETpt_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hMuMETeta_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hMuMETphi_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hMuMETiso_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hJetpt_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hJeteta_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hJetphi_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hu1_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hu2_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH2D *hu1vsZpt_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hu2vsZpt_WlikePos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  // TH2D *hWlikePos_logiso_vs_logdxy[WMass::etaMuonNSteps];
  // TH2D *hWlikePos_iso_vs_dxy[WMass::etaMuonNSteps];

  static const int nbins=75;
  float bins_scaled[3][nbins+1]={{0.}};
  float bins_Notscaled[3][nbins+1]={{0.}};
  float binsize1=0.01,binsize2=0.04;
  float binsize;
  float xmin=0.6,xmax=1.8, x;
  for(int k=0;k<3;k++){
    x=xmin;
    binsize=binsize1;
    for(int i=0;i<nbins;i++){
      bins_scaled[k][i]=x;
      bins_Notscaled[k][i]=x*80/(k==1 ? 1 : 2); // mT has double range wrt pt, met
      if(x>1.2-binsize) binsize=binsize2;
      x+=binsize;
      // cout << "bins_scaled["<<k<<"]["<<i<<"]= "<< bins_scaled[k][i] << endl;
      // cout << "bins_Notscaled["<<k<<"]["<<i<<"]= "<< bins_Notscaled[k][i] << endl;
    }
    bins_scaled[k][nbins]=xmax;
    bins_Notscaled[k][nbins]=xmax*80/(k==1 ? 1 : 2);
    // cout << "bins_scaled["<<k<<"]["<<nbins<<"]= " <<bins_scaled[k][nbins] << endl;
    // cout << "bins_Notscaled["<<k<<"]["<<nbins<<"]= " <<bins_Notscaled[k][nbins] << endl;
    // cout << endl;
  }
  
  // cout << endl;
  // for(int k=0;k<3;k++){
    // for(int i=0;i<nbins+1;i++){
      // cout << "bins_scaled["<<k<<"]["<<i<<"]= "<< bins_scaled[k][i] << " ";
    // }
    // cout << endl;
    // cout << endl;
    // for(int i=0;i<nbins+1;i++){
      // cout << "bins_Notscaled["<<k<<"]["<<i<<"]= "<< bins_Notscaled[k][i] << " ";
    // }
    // cout << endl;
    // cout << endl;
  // }
  // return;

  for(int i=0; i<WMass::etaMuonNSteps; i++){
    TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
            
    for(int j=0; j<2*WMass::WMassNSteps+1; j++){
      
      if(!sampleName.Contains("DYJetsSig") && WMass::WMassNSteps!=j) continue;
      
      int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV;

      for(int k=0;k<3;k++){
        hWlikePos_VarScaled_1_Gen[k][i][j]=new TH1D(Form("hWlikePos_%sScaled_1_Gen_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),Form("hWlikePos_%sScaled_1_Gen_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),nbins,bins_scaled[k]);
        hWlikePos_VarScaled_1_Gen[k][i][j]->Sumw2();
        hWlikePos_VarScaled_2_ZGenMassCut[k][i][j]=new TH1D(Form("hWlikePos_%sScaled_2_ZGenMassCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),Form("hWlikePos_%sScaled_2_ZGenMassCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),nbins,bins_scaled[k]);
        hWlikePos_VarScaled_2_ZGenMassCut[k][i][j]->Sumw2();
        hWlikePos_VarScaled_3_Mu1GenCut[k][i][j]=new TH1D(Form("hWlikePos_%sScaled_3_Mu1GenCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),Form("hWlikePos_%sScaled_3_Mu1GenCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),nbins,bins_scaled[k]);
        hWlikePos_VarScaled_3_Mu1GenCut[k][i][j]->Sumw2();
        hWlikePos_VarScaled_4_Mu2GenCut[k][i][j]=new TH1D(Form("hWlikePos_%sScaled_4_Mu2GenCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),Form("hWlikePos_%sScaled_4_Mu2GenCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),nbins,bins_scaled[k]);
        hWlikePos_VarScaled_4_Mu2GenCut[k][i][j]->Sumw2();
        hWlikePos_VarScaled_5_RecoCut[k][i][j]=new TH1D(Form("hWlikePos_%sScaled_5_RecoCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),Form("hWlikePos_%sScaled_5_RecoCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),nbins,bins_scaled[k]);
        hWlikePos_VarScaled_5_RecoCut[k][i][j]->Sumw2();
        hWlikePos_VarScaled_6_METCut[k][i][j]=new TH1D(Form("hWlikePos_%sScaled_6_METCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),Form("hWlikePos_%sScaled_6_METCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),nbins,bins_scaled[k]);
        hWlikePos_VarScaled_6_METCut[k][i][j]->Sumw2();
        hWlikePos_VarScaled_7_RecoilCut[k][i][j]=new TH1D(Form("hWlikePos_%sScaled_7_RecoilCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),Form("hWlikePos_%sScaled_7_RecoilCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),nbins,bins_scaled[k]);
        hWlikePos_VarScaled_7_RecoilCut[k][i][j]->Sumw2();
        for(int h=0; h<WMass::PDF_members; h++){
          for(int m=0; m<WMass::NVarRecoilCorr; m++){
            TString toys_str = "";
            if(WMass::NVarRecoilCorr>1) toys_str = Form("_RecoilCorrVar%d",m);
            
            hWlikePos_VarScaled_8_JetCut[m][h][k][i][j]=new TH1D(Form("hWlikePos_%sScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass),Form("hWlikePos_%sScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass),nbins,bins_scaled[k]);
            hWlikePos_VarScaled_8_JetCut[m][h][k][i][j]->Sumw2();
            hWlikePos_VarNonScaled_8_JetCut[m][h][k][i][j]=new TH1D(Form("hWlikePos_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass),Form("hWlikePos_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass),nbins,bins_Notscaled[k]);
            hWlikePos_VarNonScaled_8_JetCut[m][h][k][i][j]->Sumw2();
          }
        }
        hWlikePos_VarScaled_QCD[k][i][j]=new TH1D(Form("hWlikePos_%sScaled_QCD_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),Form("hWlikePos_%sScaled_QCD_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),nbins,bins_scaled[k]);
        hWlikePos_VarScaled_QCD[k][i][j]->Sumw2();
      }
      // hWlikePos_logiso_vs_logdxy[i][j]=new TH2D(Form("hWlikePos_logiso_vs_logdxy_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_logiso_vs_logdxy_eta%s_%d",eta_str.Data(),jWmass),1000,-5,1,1000,-5,1);
      // hWlikePos_iso_vs_dxy[i][j]=new TH2D(Form("hWlikePos_iso_vs_dxy_eta%s_%d",eta_str.Data(),jWmass),Form("hWlikePos_iso_vs_dxy_eta%s_%d",eta_str.Data(),jWmass),1000,-0.001,0.999,1000,0,1);
    
        for(int k=0; k<WMass::nSigOrQCD; k++){
              
          hnvtx[k][i][j]=new TH1D(Form("hnvtx_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hnvtx_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),50,0,50);
          Zmass[k][i][j]=new TH1D(Form("Zmass_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("Zmass_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),300,50,200);
          Zmass_zoomed[k][i][j]=new TH1D(Form("Zmass_zoomed_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("Zmass_zoomed_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),6000,60,120);
          ZmassVsMuPosEta[k][i][j]=new TH2D(Form("ZmassVsMuPosEta_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("ZmassVsMuPosEta_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),50,-2.5,2.5,200,90,92);
          
          PosMuonPtCorrVsMuPosEta[k][i][j]=new TH2D(Form("PosMuonPtCorrVsMuPosEta_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("PosMuonPtCorrVsMuPosEta_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),50,-2.5,2.5,300,60,120);
          PosMuonPtCorrVsMuPosPhi[k][i][j]=new TH2D(Form("PosMuonPtCorrVsMuPosPhi_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("PosMuonPtCorrVsMuPosPhi_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),50,-2*TMath::Pi(),2*TMath::Pi(),300,60,120);
          NegMuonPtCorrVsMuNegEta[k][i][j]=new TH2D(Form("NegMuonPtCorrVsMuNegEta_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("NegMuonPtCorrVsMuNegEta_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),50,-2.5,2.5,300,60,120);
          NegMuonPtCorrVsMuNegPhi[k][i][j]=new TH2D(Form("NegMuonPtCorrVsMuNegPhi_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("NegMuonPtCorrVsMuNegPhi_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),50,-2*TMath::Pi(),2*TMath::Pi(),300,60,120);
          
          ZDeltaRMuPosJet[k][i][j]=new TH1D(Form("ZDeltaRMuPosJet_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("ZDeltaRMuPosJet_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),1000,0,10);
          ZDeltaEtaMuPosJet[k][i][j]=new TH1D(Form("ZDeltaEtaMuPosJet_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("ZDeltaEtaMuPosJet_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),200,-5,5);
          ZDeltaPhiMuPosJet[k][i][j]=new TH1D(Form("ZDeltaPhiMuPosJet_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("ZDeltaPhiMuPosJet_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),200,-2*TMath::Pi(),2*TMath::Pi());
          ZDeltaRMuNegJet[k][i][j]=new TH1D(Form("ZDeltaRMuNegJet_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("ZDeltaRMuNegJet_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),1000,0,10);
          ZDeltaEtaMuNegJet[k][i][j]=new TH1D(Form("ZDeltaEtaMuNegJet_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("ZDeltaEtaMuNegJet_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),200,-5,5);
          ZDeltaPhiMuNegJet[k][i][j]=new TH1D(Form("ZDeltaPhiMuNegJet_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("ZDeltaPhiMuNegJet_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),200,-2*TMath::Pi(),2*TMath::Pi());
          ZlepDeltaEta[k][i][j]=new TH1D(Form("ZlepDeltaEta_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("ZlepDeltaEta_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),200,-5,5);
          ZlepEta1VsEta2[k][i][j]=new TH2D(Form("ZlepEta1VsEta2_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("ZlepEta1VsEta2_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),50,-2.5,2.5,50,-2.5,2.5);
          ZlepDeltaEtaVsEta1[k][i][j]=new TH2D(Form("ZlepDeltaEtaVsEta1_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("ZlepDeltaEtaVsEta1_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),50,-2.5,2.5,100,-5,5);
          ZlepDeltaPt[k][i][j]=new TH1D(Form("ZlepDeltaPt_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("ZlepDeltaPt_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,-50,50);
          ZlepPt1VsPt2[k][i][j]=new TH2D(Form("ZlepPt1VsPt2_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("ZlepPt1VsPt2_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),200,0,200,200,0,200);
          ZlepDeltaPtVsPt1[k][i][j]=new TH2D(Form("ZlepDeltaPtVsPt1_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("ZlepDeltaPtVsPt1_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),200,0,200,100,-50,50);
          ZlepDeltaPhi[k][i][j]=new TH1D(Form("ZlepDeltaPhi_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("ZlepDeltaPhi_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),200,-2*TMath::Pi(),2*TMath::Pi());
          ZlepPhi1VsPhi2[k][i][j]=new TH2D(Form("ZlepPhi1VsPhi2_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("ZlepPhi1VsPhi2_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),200,-2*TMath::Pi(),2*TMath::Pi(),200,-2*TMath::Pi(),2*TMath::Pi());
          ZlepDeltaPhiVsPhi1[k][i][j]=new TH2D(Form("ZlepDeltaPhiVsPhi1_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("ZlepDeltaPhiVsPhi1_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),200,-2*TMath::Pi(),2*TMath::Pi(),200,-2*TMath::Pi(),2*TMath::Pi());
          ZlepDeltaR[k][i][j]=new TH1D(Form("ZlepDeltaR_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("ZlepDeltaR_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),1000,0,10);
          // ZlepDeltaPhiVsMass[k][i][j]=new TH2D(Form("ZlepDeltaPhiVsMass_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("ZlepDeltaPhiVsMass_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),200,0,200,200,-2*TMath::Pi(),2*TMath::Pi());
          // ZlepDeltaEtaVsMass[k][i][j]=new TH2D(Form("ZlepDeltaEtaVsMass_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("ZlepDeltaEtaVsMass_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),200,0,200,50,-2.5,2.5);
          // ZlepDeltaPtVsMass[k][i][j]=new TH2D(Form("ZlepDeltaPtVsMass_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("ZlepDeltaPtVsMass_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),200,0,200,100,-50,50);
          // ZlepDeltaRVsMass[k][i][j]=new TH2D(Form("ZlepDeltaRVsMass_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("ZlepDeltaRVsMass_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),200,0,200,1000,0,10);

          hpfMET_Z[k][i][j]=new TH1D(Form("hpfMET_Z_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hpfMET_Z_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,0,200);
          hpfMETphi_Z[k][i][j]=new TH1D(Form("hpfMETphi_Z_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hpfMETphi_Z_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,-TMath::Pi(),TMath::Pi());
          hpfMET_WlikePos[k][i][j]=new TH1D(Form("hpfMET_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hpfMET_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,0,200);
          hpfMETphi_WlikePos[k][i][j]=new TH1D(Form("hpfMETphi_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hpfMETphi_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,-TMath::Pi(),TMath::Pi());
          hZ_pt[k][i][j]=new TH1D(Form("hZ_pt_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hZ_pt_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,0,25);
          hZ_phi[k][i][j]=new TH1D(Form("hZ_phi_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hZ_phi_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,-TMath::Pi(),TMath::Pi());
          hZ_mt[k][i][j]=new TH1D(Form("hZ_mt_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hZ_mt_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,0,200);
          hZ_ptVsmt[k][i][j]=new TH2D(Form("hZ_ptVsmt_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hZ_ptVsmt_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),150,50,200,100,0,25);
          
          hWlikePos_pt[k][i][j]=new TH1D(Form("hWlikePos_pt_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hWlikePos_pt_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,0,25);
          hWlikePos_phi[k][i][j]=new TH1D(Form("hWlikePos_phi_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hWlikePos_phi_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,-TMath::Pi(),TMath::Pi());
          hWlikePos_mt[k][i][j]=new TH1D(Form("hWlikePos_mt_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hWlikePos_mt_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),200,0,200);
          hMupt_WlikePos[k][i][j]=new TH1D(Form("hMupt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hMupt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),200,0,200);
          hMueta_WlikePos[k][i][j]=new TH1D(Form("hMueta_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hMueta_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,-2.5,2.5);
          hMuphi_WlikePos[k][i][j]=new TH1D(Form("hMuphi_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hMuphi_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,-TMath::Pi(),TMath::Pi());
          hMuiso_WlikePos[k][i][j]=new TH1D(Form("hMuiso_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hMuiso_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),1000,0,10);
          hMuMETpt_WlikePos[k][i][j]=new TH1D(Form("hMuMETpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hMuMETpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,0,200);
          hMuMETeta_WlikePos[k][i][j]=new TH1D(Form("hMuMETeta_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hMuMETeta_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,-2.5,2.5);
          hMuMETphi_WlikePos[k][i][j]=new TH1D(Form("hMuMETphi_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hMuMETphi_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,-TMath::Pi(),TMath::Pi());
          hMuMETiso_WlikePos[k][i][j]=new TH1D(Form("hMuMETiso_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hMuMETiso_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,0,10);
          hJetpt_WlikePos[k][i][j]=new TH1D(Form("hJetpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hJetpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,0,50);
          hJeteta_WlikePos[k][i][j]=new TH1D(Form("hJeteta_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hJeteta_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,-2.5,2.5);
          hJetphi_WlikePos[k][i][j]=new TH1D(Form("hJetphi_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hJetphi_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,-TMath::Pi(),TMath::Pi());
          hu1_WlikePos[k][i][j]=new TH1D(Form("hu1_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hu1_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),360,-30,150);
          hu2_WlikePos[k][i][j]=new TH1D(Form("hu2_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hu2_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),250,-50,50);
          hu1vsZpt_WlikePos[k][i][j]=new TH2D(Form("hu1vsZpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hu1vsZpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,0,25,360,-30,150);
          hu2vsZpt_WlikePos[k][i][j]=new TH2D(Form("hu2vsZpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hu2vsZpt_WlikePos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,0,25,250,-50,50);
        }  
    }
  }

  
  Long64_t first_entry = 0;
  Long64_t nentries = fChain->GetEntriesFast();
  if(IS_MC_CLOSURE_TEST==1 && isMCorDATA==1) first_entry=nentries/2; // in case of closure test, DATA runs from N/2 to N
  if(IS_MC_CLOSURE_TEST==1 && isMCorDATA==0) nentries=nentries/2; // in case of closure test, MC runs from 0 to N/2
  if(IS_MC_CLOSURE_TEST==1) lumi_scaling=lumi_scaling*2; // in case of closure test, scaling must be multiplied by 2
  
  //To get the central value of the momentum correction
  int random_seed_start=67525;
  rochcor_44X_v3 *rmcor44X = WMass::NVarRecoilCorr>1? new rochcor_44X_v3(random_seed_start) : new rochcor_44X_v3();  // make the pointer of rochcor class
  TString MuscleCard = (IS_MC_CLOSURE_TEST || isMCorDATA==0) ? "MuScleFit_2011_MC_44X" : "MuScleFit_2011_DATA_44X";
  TString fitParametersFile = MuscleCard+".txt";
  MuScleFitCorrector *corrector;
  if(useMomentumCorr==2){
    cout << "using MuscleFit card " << fitParametersFile << endl;
    corrector = new MuScleFitCorrector(fitParametersFile);
  }

  //  std::string fileCorrectTo = "../RecoilCode/RecoilCorrector_v6/recoilfits/recoilfit_wjets53X_20pv_njet.root";
  std::string fileCorrectTo = "../RecoilCode/RecoilCorrector_v6/recoilfits/recoilfit_zmm42X_20pv_0jet.root";

  // Original from PHIL
  std::string fileZmmData_P = "../RecoilCode/RecoilCorrector_v6/recoilfits/recoilfit_datamm42X_20pv_0jet.root"; 
  std::string fileZmmMC_P = "../RecoilCode/RecoilCorrector_v6/recoilfits/recoilfit_zmm42X_20pv_njet.root";

  /// these are for inclusive jets and inclusive vtx
  /// MADE by Maria
  //  std::string fileZmmMC_M = "../RecoilCode/recoilfit_Z_inc_1.root";
  std::string fileZmmData_M = "../RecoilCode/recoilfit_DATA.root";
  std::string fileZmmMC_M = "../RecoilCode/recoilfit_genZ_inc_1.root";

  RecoilCorrector::RecoilCorrector*  correctorRecoil_P_;
  RecoilCorrector::RecoilCorrector*  correctorRecoil_Z_;

  ////////
  ////////

  correctorRecoil_Z_ = new RecoilCorrector(fileCorrectTo.c_str(),123456); // this file is used to read the jet mutliplicity, will be a dummy file
  correctorRecoil_Z_->addDataFile(fileZmmData_M.c_str());
  correctorRecoil_Z_->addMCFile(fileZmmMC_M.c_str());

  ////////
  ////////

  correctorRecoil_P_ = new RecoilCorrector(fileCorrectTo.c_str(),123456); // this file is used to read the jet mutliplicity, will be a dummy file
  correctorRecoil_P_->addDataFile(fileZmmData_P.c_str());
  correctorRecoil_P_->addMCFile(fileZmmMC_P.c_str());

  // the following variables are dummy, but necessary to call the corrector.
  double u1_dummy = 0;
  double u2_dummy = 0;
  double fluc_dummy = 0;
  double zero_dummy = 0;
  int jetMult = 0; // set to zero;
  //for the lepPt, lepPhi, 2: lepton is on leg2;
  
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=first_entry; jentry<nentries;jentry++) {
    //  for (Long64_t jentry=0; jentry<1000; jentry++) {
    
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    if(jentry%250000==0) cout <<"Analyzed entry "<<jentry<<"/"<<nentries<<endl;
    if(jentry%50000==0){
      time_t now = time(0);
      TString dt = ctime(&now); dt.ReplaceAll("\n"," ");
      outTXTfile << dt << "\t - \t Analyzed entry "<<jentry<<"/"<<nentries<<endl;
    }
    
    if(nTrgMuons>2) continue; // NOT (YET) REQUIRED WHILE MAKING NTUPLES!!!! <<---------- it sohuld be already fixed in the ntuples now
    
    double evt_weight = lumi_scaling;
    // TO BE CHECKED!!!
    if(useVtxSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && npu>0) evt_weight=lumi_scaling*hPileupSF->GetBinContent(hPileupSF->GetXaxis()->FindBin(npu));
    if(usePtSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && hZPtSF && sampleName.Contains("DYJetsSig")) evt_weight*=hZPtSF->GetBinContent(hZPtSF->GetXaxis()->FindBin(Z_pt>0?Z_pt:ZGen_pt));    

    int runopt = r->Rndm()<0.457451 ? 0 : 1;
    double MuPos_tight_muon_SF = 1;
    double MuNeg_tight_muon_SF = 1;
    
    if(useEffSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0)){
    // TO BE CHECKED!!!
      MuPos_tight_muon_SF = hEffSF_MuId_eta_2011[runopt]->Eval(MuPos_eta)*hEffSF_Iso_eta_2011[runopt]->Eval(MuPos_eta)*hEffSF_HLT_eta_2011->Eval(MuPos_eta);
      MuNeg_tight_muon_SF = hEffSF_MuId_eta_2011[runopt]->Eval(MuNeg_eta)*hEffSF_Iso_eta_2011[runopt]->Eval(MuNeg_eta)*hEffSF_HLT_eta_2011->Eval(MuPos_eta);
    }
    // cout << "MuPos_tight_muon_SF= " << MuPos_tight_muon_SF << endl;
    if((IS_MC_CLOSURE_TEST || isMCorDATA==0) ) plot1D("hPileUp_Fall11",npu,1,h_1d, 50,0,50);

    for(int i=0; i<WMass::etaMuonNSteps; i++){
      for(int j=0; j<2*WMass::WMassNSteps+1; j++){
        
        if(!sampleName.Contains("DYJetsSig") && WMass::WMassNSteps!=j) continue;
        double iWmass = (WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV)/1e3;
        // SELECT EVENT WITH Z/W SCALED CUTS
        // ORIGINAL CUTS:
        // muon pt > 30,
        // pfmetcut > 25,
        // Boson pt (recoilcut) < 20,
        
        // gen stuff to be used only when required
        if(useGenVar){
          if(ZGen_mass>0){
            // double MuPosGen_pt_jacobian = 2*MuPosGen_pt/WMass::ZMassCentral_MeV;
            double MuPosGen_var_jacobian[3] = {2*MuPosGen_pt/WMass::ZMassCentral_MeV,ZGen_mt/iWmass,2*MuNegGen_pt/iWmass,};
            // LUCA ADD TO AVOID OVERFLOW
            for(int k=0;k<3;k++)
              if(MuPosGen_var_jacobian[k]>=xmax) MuPosGen_var_jacobian[k]=xmax-binsize2/2;
            
            for(int k=0;k<3;k++)
              hWlikePos_VarScaled_1_Gen[k][i][j]->Fill(MuPosGen_var_jacobian[k],evt_weight);
            
            if(ZGen_mass>50){
              for(int k=0;k<3;k++)
                hWlikePos_VarScaled_2_ZGenMassCut[k][i][j]->Fill(MuPosGen_var_jacobian[k],evt_weight);
              
              if(TMath::Abs(MuPosGen_eta)<WMass::etaMaxMuons[i]){
                for(int k=0;k<3;k++)
                  hWlikePos_VarScaled_3_Mu1GenCut[k][i][j]->Fill(MuPosGen_var_jacobian[k],evt_weight);
                
                if(TMath::Abs(MuNegGen_eta)<2.4){
                  for(int k=0;k<3;k++)
                    hWlikePos_VarScaled_4_Mu2GenCut[k][i][j]->Fill(MuPosGen_var_jacobian[k],evt_weight);
                }
              }
            }
          }
        } // end gen stuff 
        
        if(!useGenVar || Z_mass>0){ // dummy thing to separate signal and background in DY+Jets (useless)
          for(int m=0; m<WMass::NVarRecoilCorr; m++){
            if( evtHasGoodVtx && evtHasTrg){ // good reco event

              TLorentzVector muPosCorr,muNegCorr, muPosNoCorr,muNegNoCorr, Zcorr; //TLorentzVector of the reconstructed muon
              //Set TLorentzVector of mu, and mubar
              muPosCorr.SetPtEtaPhiM(MuPos_pt,MuPos_eta,MuPos_phi,MuPos_mass);
              muPosNoCorr = muPosCorr;
              muNegCorr.SetPtEtaPhiM(MuNeg_pt,MuNeg_eta,MuNeg_phi,MuNeg_mass);
              muNegNoCorr = muNegCorr;

              if(useMomentumCorr==1){ // use rochester corrections if required
                if(IS_MC_CLOSURE_TEST || isMCorDATA==0){
                  rmcor44X->momcor_mc(muPosCorr, MuPos_charge, smearRochCorrByNsigma/* , runopt */);
                  rmcor44X->momcor_mc(muNegCorr, MuNeg_charge, smearRochCorrByNsigma/* , runopt */);
                }
                else{
                  rmcor44X->momcor_data(muPosCorr, MuPos_charge, smearRochCorrByNsigma , run<175832 ? 0 : 1 );
                  rmcor44X->momcor_data(muNegCorr, MuNeg_charge, smearRochCorrByNsigma , run<175832 ? 0 : 1 );
                }
              }else if(useMomentumCorr==2){ // use Momentum scale corrections if required
                corrector->applyPtCorrection(muPosCorr,MuPos_charge);
                corrector->applyPtCorrection(muNegCorr,MuNeg_charge);
              }
              // cout << "toy= " << m 
                   // << " " << muPosCorr.Pt() << " " << muPosCorr.Eta() << " " << muPosCorr.Phi() 
                   // << " " << muNegCorr.Pt() << " " << muNegCorr.Eta() << " " << muNegCorr.Phi() 
                   // << " " << endl;
              Zcorr = muPosCorr + muNegCorr;
              
              TLorentzVector WlikePos_met,WlikePos_nu,WlikePos;
              WlikePos_met.SetPtEtaPhiM(pfmet,0,pfmet_phi,0);
              WlikePos_nu = muNegCorr + WlikePos_met;
              WlikePos = muPosCorr + WlikePos_nu;

              double MuPos_var_jacobian[3] = {2*muPosCorr.Pt()/WMass::ZMassCentral_MeV,WlikePos.Mt()/WMass::ZMassCentral_MeV,2*WlikePos_nu.Pt()/WMass::ZMassCentral_MeV};
              double MuPos_var_NotScaled[3] = {muPosCorr.Pt(),WlikePos.Mt(),WlikePos_nu.Pt()};
              // LUCA ADD TO AVOID OVERFLOW
              for(int k=0;k<3;k++)
                if(MuPos_var_jacobian[k]>=xmax) MuPos_var_jacobian[k]=xmax-binsize2/2;

              int wmass1 = iWmass*1e3;
              
              // good pair within acceptance cuts for both muons
              if(Zcorr.M()>50
		 && TMath::Abs(muPosCorr.Eta())<WMass::etaMaxMuons[i] && muPosCorr.Pt()>30*WMass::ZMassCentral_MeV/iWmass && MuPosTrg
		 && TMath::Abs(muNegCorr.Eta())<2.4 && muNegCorr.Pt()>10 && MuPos_charge != MuNeg_charge
		 && noTrgExtraMuonsLeadingPt<10
		 ){
                // full ID and tight requirements on the muon
		if(MuPosIsTightAndIso && MuPosRelIso<0.12 && MuPos_dxy<0.02){
		  
		  double pfmet_corr_Z=pfmet;
		  double pfmetphi_corr_Z=pfmet_phi;
		  
		  correctorRecoil_Z_->CorrectType1( pfmet_corr_Z, pfmetphi_corr_Z,
						    ZGen_pt, ZGen_phi, 
						    Z_pt, Z_phi,  // ths is for Z like // for W events need to use the 
						    u1_dummy, u2_dummy, fluc_dummy, zero_dummy,
						    jetMult);
		  
		  ////////
		  double pfmet_corr_P=pfmet;
		  double pfmetphi_corr_P=pfmet_phi;

		  correctorRecoil_P_->CorrectType1( pfmet_corr_P, pfmetphi_corr_P,
						    ZGen_pt, ZGen_phi, 
						    Z_pt, Z_phi, 
						    u1_dummy, u2_dummy, fluc_dummy, zero_dummy,
						    jetMult );
		  

		  double pseudomt=getPseudoMT(pfmet, pfmet_phi);

		  double pseudomt_Z=getPseudoMT(pfmet_corr_Z, pfmetphi_corr_Z);
		  double pseudomt_P=getPseudoMT(pfmet_corr_P, pfmetphi_corr_P);

		  plot1D("h_mt", pseudomt ,       evt_weight*MuPos_tight_muon_SF, h_1d, 150, 0, 150);
		  if(!sampleName.Contains("DATA")) plot1D("h_mt_corr_Z", pseudomt_Z ,       evt_weight*MuPos_tight_muon_SF, h_1d, 150, 0, 150);
		  if(!sampleName.Contains("DATA")) plot1D("h_mt_corr_P", pseudomt_P ,       evt_weight*MuPos_tight_muon_SF, h_1d, 150, 0, 150);

		  plot1D("h_met", pfmet ,       evt_weight*MuPos_tight_muon_SF, h_1d, 100, 0, 100);
		  plot1D("h_metphi", pfmet_phi ,       evt_weight*MuPos_tight_muon_SF, h_1d, 100, -3.15, 3.15);

		  if(!sampleName.Contains("DATA")) plot1D("h_met_corr_Z", pfmet_corr_Z ,       evt_weight*MuPos_tight_muon_SF, h_1d, 100, 0, 100);
		  if(!sampleName.Contains("DATA")) plot1D("h_metphi_corr_Z", pfmetphi_corr_Z ,       evt_weight*MuPos_tight_muon_SF, h_1d, 100, -3.15, 3.15);

		  if(!sampleName.Contains("DATA")) plot1D("h_met_corr_P", pfmet_corr_P ,       evt_weight*MuPos_tight_muon_SF, h_1d, 100, 0, 100);
		  if(!sampleName.Contains("DATA")) plot1D("h_metphi_corr_P", pfmetphi_corr_P ,       evt_weight*MuPos_tight_muon_SF, h_1d, 100, -3.15, 3.15);

		  /////////	      cout << "pfmet " << pfmet << " pfmet_corr " << pfmet_corr << endl;
		  

                  for(int k=0;k<3;k++)
                    if(m==0) hWlikePos_VarScaled_5_RecoCut[k][i][j]->Fill(MuPos_var_jacobian[k],evt_weight*MuPos_tight_muon_SF);

                  if(pfmetWlikePos>25*WMass::ZMassCentral_MeV/iWmass){
                    for(int k=0;k<3;k++)
                      if(m==0) hWlikePos_VarScaled_6_METCut[k][i][j]->Fill(MuPos_var_jacobian[k],evt_weight*MuPos_tight_muon_SF);
                    
                    if(WlikePos_pt<20*WMass::ZMassCentral_MeV/iWmass){
                      for(int k=0;k<3;k++)
                        if(m==0) hWlikePos_VarScaled_7_RecoilCut[k][i][j]->Fill(MuPos_var_jacobian[k],evt_weight*MuPos_tight_muon_SF);
                      
                      if(true){ // for the momet remove jet pt cut
                      
                        double lha_weight = 1;
                        // double lha_weight = LHAPDF::xfx(0,x1,Q,fl1)*LHAPDF::xfx(0,x2,Q,fl2) / (LHAPDF::xfx(1,x1,Q,fl1)*LHAPDF::xfx(1,x2,Q,fl2));
                        double weight_old = 1;
                        #ifdef LHAPDF_ON
                          weight_old = !sampleName.Contains("DATA") ? (LHAPDF::xfx(1,parton1_x,scalePDF,parton1_pdgId)*LHAPDF::xfx(1,parton2_x,scalePDF,parton2_pdgId)) : 1;
                        #endif
                        // cout << "scalePDF= " << scalePDF << " parton1_x= " << parton1_x << " parton1_pdgId= " << parton1_pdgId 
                             // << "parton2_x= " << parton2_x << " parton2_pdgId= " << parton2_pdgId << endl;
                        // cout << " LHAPDF::xfx(0,parton1_x,scalePDF,parton1_pdgId)= LHAPDF::xfx(0,"<<parton1_x<<","<<scalePDF<<","<<parton1_pdgId<<")= " << LHAPDF::xfx(0,parton1_x,scalePDF,parton1_pdgId) << endl;
                        // cout << " LHAPDF::xfx(0,parton2_x,scalePDF,parton2_pdgId)= LHAPDF::xfx(0,"<<parton2_x<<","<<scalePDF<<","<<parton2_pdgId<<")= " << LHAPDF::xfx(0,parton2_x,scalePDF,parton2_pdgId) << endl;
                        // cout << " LHAPDF::xfx(1,parton1_x,scalePDF,parton1_pdgId)= LHAPDF::xfx(1,"<<parton1_x<<","<<scalePDF<<","<<parton1_pdgId<<")= " << LHAPDF::xfx(1,parton1_x,scalePDF,parton1_pdgId) << endl;
                        // cout << " LHAPDF::xfx(1,parton2_x,scalePDF,parton2_pdgId)= LHAPDF::xfx(1,"<<parton2_x<<","<<scalePDF<<","<<parton2_pdgId<<")= " << LHAPDF::xfx(1,parton2_x,scalePDF,parton2_pdgId) << endl;
                        // cout << " lha_weight= " << lha_weight << endl;
                        if(m==0){
			  plot1D("hPDF_x1",TMath::Log10(parton1_x), 1, h_1d, 1000,-4,0);
			  plot1D("hPDF_x1unweighted",TMath::Log10(parton1_x), 1/weight_old, h_1d,1000,-4,0);
			  plot1D("hPDF_x2",TMath::Log10(parton1_x), 1, h_1d, 1000,-4,0);
			  plot1D("hPDF_x2unweighted",TMath::Log10(parton2_x), 1/weight_old, h_1d, 1000,-4,0);

			  /* MARIA
                          hPDF_x1->Fill(TMath::Log10(parton1_x));
                          hPDF_x1unweighted->Fill(TMath::Log10(parton1_x),1/weight_old);
                          hPDF_x2->Fill(TMath::Log10(parton2_x));
                          hPDF_x2unweighted->Fill(TMath::Log10(parton2_x),1/weight_old);
			  */
                        }
                        
                        for(int h=0; h<WMass::PDF_members; h++){
                          if(!sampleName.Contains("DATA") && WMass::PDF_sets>0 && WMass::PDF_sets!=generated_PDF_set && WMass::PDF_members!=generated_PDF_member){
                            double weight_new = 1;
                            #ifdef LHAPDF_ON
                              LHAPDF::usePDFMember(0,h);
                              weight_new = (LHAPDF::xfx(0,parton1_x,scalePDF,parton1_pdgId)*LHAPDF::xfx(0,parton2_x,scalePDF,parton2_pdgId));
                            #endif
                            lha_weight = weight_new/weight_old;
                            if(m==0) hPDF_weights[h]->Fill(lha_weight);
                          }
                          for(int k=0;k<3;k++){
                            hWlikePos_VarScaled_8_JetCut[m][h][k][i][j]->Fill(MuPos_var_jacobian[k],evt_weight*MuPos_tight_muon_SF);
                            // cout << ((muPosCorr.Pt()*iWmass/WMass::ZMassCentral_MeV)<xmax*80/2 ? muPosCorr.Pt()*iWmass/WMass::ZMassCentral_MeV : (xmax-binsize2/2)*80/2) << endl;
                            hWlikePos_VarNonScaled_8_JetCut[m][h][k][i][j]->Fill( (MuPos_var_NotScaled[k]*iWmass/WMass::ZMassCentral_MeV)<xmax*80/(k==1 ? 1 : 2) ? MuPos_var_NotScaled[k]*iWmass/WMass::ZMassCentral_MeV : (xmax-binsize2/2)*80/(k==1 ? 1 : 2) ,evt_weight*MuPos_tight_muon_SF);
                          }
                        
                        }
                        
                        // control plots for different etas but only for central W mass
                        if(m==0){
                          // control distributions 
                          hnvtx[0][i][j]->Fill(nvtx,evt_weight);
                          Zmass[0][i][j]->Fill(Zcorr.M(),evt_weight*MuPos_tight_muon_SF);

                          Zmass_zoomed[0][i][j]->Fill(Zcorr.M(),evt_weight*MuPos_tight_muon_SF);
                          // ZmassVsMuPosEta[0][i][j]->Fill(MuPos_eta,Zcorr.M(),evt_weight*MuPos_tight_muon_SF);
                          ZmassVsMuPosEta[0][i][j]->Fill(muPosCorr.Eta(),Zcorr.M(),evt_weight*MuPos_tight_muon_SF);
                          
                          // PosMuonPtCorrVsMuPosEta[0][i][j]->Fill(MuPos_eta,Zcorr.M(),evt_weight*MuPos_tight_muon_SF);
                          
                          ZlepDeltaEta[0][i][j]->Fill(muPosCorr.Eta()-muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                          ZlepEta1VsEta2[0][i][j]->Fill(muPosCorr.Eta(),muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                          ZlepDeltaEtaVsEta1[0][i][j]->Fill(muPosCorr.Eta(),muPosCorr.Eta()-muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                          // ZlepDeltaEtaVsMass[0][i][j]->Fill(Zcorr.M(),muPosCorr.Eta()-muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                          ZlepDeltaPt[0][i][j]->Fill(muPosCorr.Pt()-muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                          ZlepPt1VsPt2[0][i][j]->Fill(muPosCorr.Pt(),muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                          ZlepDeltaPtVsPt1[0][i][j]->Fill(muPosCorr.Pt(),muPosCorr.Pt()-muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                          // ZlepDeltaPtVsMass[0][i][j]->Fill(Zcorr.M(),muPosCorr.Pt()-muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                          ZlepDeltaPhi[0][i][j]->Fill(muPosCorr.Phi()-muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                          ZlepPhi1VsPhi2[0][i][j]->Fill(muPosCorr.Phi(),muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                          ZlepDeltaPhiVsPhi1[0][i][j]->Fill(muPosCorr.Phi(),muPosCorr.Phi()-muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                          // ZlepDeltaPhiVsMass[0][i][j]->Fill(Zcorr.M(),muPosCorr.Phi()-muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                          ZlepDeltaR[0][i][j]->Fill(TMath::Hypot(muPosCorr.Phi()-muNegCorr.Phi(),muPosCorr.Eta()-muNegCorr.Eta()),evt_weight*MuPos_tight_muon_SF);
                          // ZlepDeltaRVsMass[0][i][j]->Fill(Zcorr.M(),TMath::Hypot(muNegCorr.Phi()-Jet_leading_phi,muNegCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF);
                          ZDeltaEtaMuPosJet[0][i][j]->Fill(muPosCorr.Eta()-Jet_leading_eta,evt_weight*MuPos_tight_muon_SF);
                          ZDeltaPhiMuPosJet[0][i][j]->Fill(muPosCorr.Phi()-Jet_leading_phi,evt_weight*MuPos_tight_muon_SF);
                          ZDeltaRMuPosJet[0][i][j]->Fill(TMath::Hypot(muPosCorr.Phi()-Jet_leading_phi,muPosCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF);
                          ZDeltaEtaMuNegJet[0][i][j]->Fill(muNegCorr.Eta()-Jet_leading_eta,evt_weight*MuPos_tight_muon_SF);
                          ZDeltaPhiMuNegJet[0][i][j]->Fill(muNegCorr.Phi()-Jet_leading_phi,evt_weight*MuPos_tight_muon_SF);
                          ZDeltaRMuNegJet[0][i][j]->Fill(TMath::Hypot(muNegCorr.Phi()-Jet_leading_phi,muNegCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF);
                          hpfMET_Z[0][i][j]->Fill(pfmet,evt_weight*MuPos_tight_muon_SF);
                          hpfMETphi_Z[0][i][j]->Fill(pfmet_phi,evt_weight*MuPos_tight_muon_SF);
                          hZ_pt[0][i][j]->Fill(Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                          hZ_phi[0][i][j]->Fill(Zcorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                          hZ_mt[0][i][j]->Fill(Zcorr.Mt(),evt_weight*MuPos_tight_muon_SF);
                          hZ_ptVsmt[0][i][j]->Fill(Zcorr.Mt(),Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                          hWlikePos_pt[0][i][j]->Fill(WlikePos_pt,evt_weight*MuPos_tight_muon_SF);
                          hWlikePos_phi[0][i][j]->Fill(WlikePos_phi,evt_weight*MuPos_tight_muon_SF);
                          hWlikePos_mt[0][i][j]->Fill(WlikePos_mt,evt_weight*MuPos_tight_muon_SF);
                          hpfMET_WlikePos[0][i][j]->Fill(pfmetWlikePos,evt_weight*MuPos_tight_muon_SF);
                          hpfMETphi_WlikePos[0][i][j]->Fill(pfmetWlikePos_phi,evt_weight*MuPos_tight_muon_SF);
                          hMupt_WlikePos[0][i][j]->Fill( muPosCorr.Pt()<200 ? muPosCorr.Pt() : 199.5,evt_weight*MuPos_tight_muon_SF);
                          hMueta_WlikePos[0][i][j]->Fill(muPosCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                          hMuphi_WlikePos[0][i][j]->Fill(muPosCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                          hMuiso_WlikePos[0][i][j]->Fill(MuPosRelIso,evt_weight*MuPos_tight_muon_SF);
                          hMuMETpt_WlikePos[0][i][j]->Fill(muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                          hMuMETeta_WlikePos[0][i][j]->Fill(muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                          hMuMETphi_WlikePos[0][i][j]->Fill(muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                          hMuMETiso_WlikePos[0][i][j]->Fill(MuNegRelIso,evt_weight*MuPos_tight_muon_SF);
                          hJetpt_WlikePos[0][i][j]->Fill(Jet_leading_pt,evt_weight*MuPos_tight_muon_SF);
                          hJeteta_WlikePos[0][i][j]->Fill(Jet_leading_eta,evt_weight*MuPos_tight_muon_SF);
                          hJetphi_WlikePos[0][i][j]->Fill(Jet_leading_phi,evt_weight*MuPos_tight_muon_SF);
                          hu1_WlikePos[0][i][j]->Fill(u1,evt_weight*MuPos_tight_muon_SF);
                          hu2_WlikePos[0][i][j]->Fill(u2,evt_weight*MuPos_tight_muon_SF);
                          hu1vsZpt_WlikePos[0][i][j]->Fill(Zcorr.Pt(),u1,evt_weight*MuPos_tight_muon_SF);
                          hu2vsZpt_WlikePos[0][i][j]->Fill(Zcorr.Pt(),u2,evt_weight*MuPos_tight_muon_SF);
                        }
                      }
                    }
                  }
                }else if(m==0){
                  
                  // Iso and dxy for muons which fail either tight requirement, isolation or dxy cut
                  if(pfmetWlikePos>25 && WlikePos_pt<20){
                    // if( (TMath::Abs(wmass1 - WMass::WMassCentral_MeV) > 1)) continue;
                    // hWlikePos_logiso_vs_logdxy[i][j]->Fill(TMath::Log10(MuPos_dxy),TMath::Log10(MuPosRelIso),evt_weight*MuPos_tight_muon_SF);
                    // hWlikePos_iso_vs_dxy[i][j]->Fill(MuPos_dxy,MuPosRelIso,evt_weight*MuPos_tight_muon_SF);
                  }

                  // invert cuts to select QCD enriched region, no cuts on met or jet
                  if(
                    MuPosRelIso>0.12 // single muon cuts (inverted iso (is <0.5 for signal) , no tight requirement)
                    && MuPos_dxy>0.02 // single muon cuts (MuPosIsTightAndIso contains dxy < 0.2 cut)
                    && WlikePos_pt<20*WMass::ZMassCentral_MeV/iWmass 
                    ){

                    for(int k=0;k<3;k++)
                      hWlikePos_VarScaled_QCD[k][i][j]->Fill(MuPos_var_jacobian[k],evt_weight*MuPos_tight_muon_SF);
                    
                    // if( (TMath::Abs(wmass1 - WMass::WMassCentral_MeV) > 1)
                        // ) 
                    // continue;

                    // QCD distributions for central W mass
                    hnvtx[1][i][j]->Fill(nvtx,evt_weight);
                    Zmass[1][i][j]->Fill(Zcorr.M(),evt_weight*MuPos_tight_muon_SF);
                    Zmass_zoomed[1][i][j]->Fill(Zcorr.M(),evt_weight*MuPos_tight_muon_SF);
                    ZmassVsMuPosEta[1][i][j]->Fill(muPosCorr.Eta(),Zcorr.M(),evt_weight*MuPos_tight_muon_SF);
                    ZlepDeltaEta[1][i][j]->Fill(muPosCorr.Eta()-muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                    ZlepEta1VsEta2[1][i][j]->Fill(muPosCorr.Eta(),muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                    ZlepDeltaEtaVsEta1[1][i][j]->Fill(muPosCorr.Eta(),muPosCorr.Eta()-muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                    // ZlepDeltaEtaVsMass[1][i][j]->Fill(Zcorr.M(),muPosCorr.Eta()-muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                    ZlepDeltaPt[1][i][j]->Fill(muPosCorr.Pt()-muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                    ZlepPt1VsPt2[1][i][j]->Fill(muPosCorr.Pt(),muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                    ZlepDeltaPtVsPt1[1][i][j]->Fill(muPosCorr.Pt(),muPosCorr.Pt()-muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                    // ZlepDeltaPtVsMass[1][i][j]->Fill(Zcorr.M(),muPosCorr.Pt()-muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                    ZlepDeltaPhi[1][i][j]->Fill(muPosCorr.Phi()-muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                    ZlepPhi1VsPhi2[1][i][j]->Fill(muPosCorr.Phi(),muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                    ZlepDeltaPhiVsPhi1[1][i][j]->Fill(muPosCorr.Phi(),muPosCorr.Phi()-muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                    // ZlepDeltaPhiVsMass[1][i][j]->Fill(Zcorr.M(),muPosCorr.Phi()-muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                    ZlepDeltaR[1][i][j]->Fill(TMath::Hypot(muPosCorr.Phi()-muNegCorr.Phi(),muPosCorr.Eta()-muNegCorr.Eta()),evt_weight*MuPos_tight_muon_SF);
                    // ZlepDeltaRVsMass[1][i][j]->Fill(Zcorr.M(),TMath::Hypot(muNegCorr.Phi()-Jet_leading_phi,muNegCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF);
                    ZDeltaEtaMuPosJet[1][i][j]->Fill(muPosCorr.Eta()-Jet_leading_eta,evt_weight*MuPos_tight_muon_SF);
                    ZDeltaPhiMuPosJet[1][i][j]->Fill(muPosCorr.Phi()-Jet_leading_phi,evt_weight*MuPos_tight_muon_SF);
                    ZDeltaRMuPosJet[1][i][j]->Fill(TMath::Hypot(muPosCorr.Phi()-Jet_leading_phi,muPosCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF);
                    ZDeltaEtaMuNegJet[1][i][j]->Fill(muNegCorr.Eta()-Jet_leading_eta,evt_weight*MuPos_tight_muon_SF);
                    ZDeltaPhiMuNegJet[1][i][j]->Fill(muNegCorr.Phi()-Jet_leading_phi,evt_weight*MuPos_tight_muon_SF);
                    ZDeltaRMuNegJet[1][i][j]->Fill(TMath::Hypot(muNegCorr.Phi()-Jet_leading_phi,muNegCorr.Eta()-Jet_leading_eta),evt_weight*MuPos_tight_muon_SF);
                    hpfMET_Z[1][i][j]->Fill(pfmet,evt_weight*MuPos_tight_muon_SF);
                    hpfMETphi_Z[1][i][j]->Fill(pfmet_phi,evt_weight*MuPos_tight_muon_SF);
                    hZ_pt[1][i][j]->Fill(Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                    hZ_phi[1][i][j]->Fill(Zcorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                    hZ_mt[1][i][j]->Fill(Zcorr.Mt(),evt_weight*MuPos_tight_muon_SF);
                    hZ_ptVsmt[1][i][j]->Fill(Zcorr.Mt(),Zcorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                    hWlikePos_pt[1][i][j]->Fill(WlikePos_pt,evt_weight*MuPos_tight_muon_SF);
                    hWlikePos_phi[1][i][j]->Fill(WlikePos_phi,evt_weight*MuPos_tight_muon_SF);
                    hWlikePos_mt[1][i][j]->Fill(WlikePos_mt,evt_weight*MuPos_tight_muon_SF);
                    hpfMET_WlikePos[1][i][j]->Fill(pfmetWlikePos,evt_weight*MuPos_tight_muon_SF);
                    hpfMETphi_WlikePos[1][i][j]->Fill(pfmetWlikePos_phi,evt_weight*MuPos_tight_muon_SF);
                    hMupt_WlikePos[1][i][j]->Fill(muPosCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                    hMueta_WlikePos[1][i][j]->Fill(muPosCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                    hMuphi_WlikePos[1][i][j]->Fill(muPosCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                    hMuiso_WlikePos[1][i][j]->Fill(MuPosRelIso,evt_weight*MuPos_tight_muon_SF);
                    hMuMETpt_WlikePos[1][i][j]->Fill(muNegCorr.Pt(),evt_weight*MuPos_tight_muon_SF);
                    hMuMETeta_WlikePos[1][i][j]->Fill(muNegCorr.Eta(),evt_weight*MuPos_tight_muon_SF);
                    hMuMETphi_WlikePos[1][i][j]->Fill(muNegCorr.Phi(),evt_weight*MuPos_tight_muon_SF);
                    hMuMETiso_WlikePos[1][i][j]->Fill(MuNegRelIso,evt_weight*MuPos_tight_muon_SF);
                    hJetpt_WlikePos[1][i][j]->Fill(Jet_leading_pt,evt_weight*MuPos_tight_muon_SF);
                    hJeteta_WlikePos[1][i][j]->Fill(Jet_leading_eta,evt_weight*MuPos_tight_muon_SF);
                    hJetphi_WlikePos[1][i][j]->Fill(Jet_leading_phi,evt_weight*MuPos_tight_muon_SF);
                    hu1_WlikePos[1][i][j]->Fill(u1,evt_weight*MuPos_tight_muon_SF);
                    hu2_WlikePos[1][i][j]->Fill(u2,evt_weight*MuPos_tight_muon_SF);
                    hu1vsZpt_WlikePos[1][i][j]->Fill(Zcorr.Pt(),u1,evt_weight*MuPos_tight_muon_SF);
                    hu2vsZpt_WlikePos[1][i][j]->Fill(Zcorr.Pt(),u2,evt_weight*MuPos_tight_muon_SF);
                    
                  } // end if for qcd enriched
                } // end else for muon cuts (sig or qcd enriched)
              } // end if for good pair within acceptance cuts for both muons
            } // end if for good reco event
          } // end toys loop
        } // end if for dummy signal/fake separation
      } // end W mass loop
    } // end muon eta loop
        
  } // end event loop
  
  outTXTfile.close();
  
  TFile*fout = new TFile(Form("%s/Zanalysis_controlplots.root",outputdir.Data()),"RECREATE");
  
  std::map<std::string, TH1F*>::iterator it1d;
  for(it1d=h_1d.begin(); it1d!=h_1d.end(); it1d++) {
    it1d->second->Write(); 
    delete it1d->second;
  }

  std::map<std::string, TH2F*>::iterator it2d;
  for(it2d=h_2d.begin(); it2d!=h_2d.end(); it2d++) {
    it2d->second->Write(); 
    delete it2d->second;
  }
  
  fout->cd();
  for(int h=0; h<WMass::PDF_members; h++)
    hPDF_weights[h]->Write();
  
  /* MARIA
  hPDF_x1->Write();
  hPDF_x1unweighted->Write();
  hPDF_x2->Write();
  hPDF_x2unweighted->Write();
  hPileUp_Fall11->Write();
  */
  
  for(int i=0; i<WMass::etaMuonNSteps; i++){
    for(int j=0; j<2*WMass::WMassNSteps+1; j++){

      if(!sampleName.Contains("DYJetsSig") && WMass::WMassNSteps!=j) continue;
      
        
        // hWlikePos_logiso_vs_logdxy[i]->Write();
        // hWlikePos_iso_vs_dxy[i]->Write();
        
        for(int k=0; k<WMass::nSigOrQCD; k++){
          Zmass[k][i][j]->Write();
          hMupt_WlikePos[k][i][j]->Write();
          hMueta_WlikePos[k][i][j]->Write();
          hMuphi_WlikePos[k][i][j]->Write();
          hMuiso_WlikePos[k][i][j]->Write();
          hMuMETpt_WlikePos[k][i][j]->Write();
          hMuMETeta_WlikePos[k][i][j]->Write();
          hMuMETphi_WlikePos[k][i][j]->Write();
          hMuMETiso_WlikePos[k][i][j]->Write();
          hpfMET_WlikePos[k][i][j]->Write();
          hpfMETphi_WlikePos[k][i][j]->Write();
          if(k==0){
            hnvtx[k][i][j]->Write();
            Zmass_zoomed[k][i][j]->Write();
            ZmassVsMuPosEta[k][i][j]->Write();
            ZlepDeltaEta[k][i][j]->Write();
            ZlepEta1VsEta2[k][i][j]->Write();
            ZlepDeltaEtaVsEta1[k][i][j]->Write();
            ZlepDeltaPt[k][i][j]->Write();
            ZlepPt1VsPt2[k][i][j]->Write();
            ZlepDeltaPtVsPt1[k][i][j]->Write();
            ZlepDeltaPhi[k][i][j]->Write();
            ZlepPhi1VsPhi2[k][i][j]->Write();
            ZlepDeltaPhiVsPhi1[k][i][j]->Write();
            ZlepDeltaR[k][i][j]->Write();
            ZDeltaEtaMuPosJet[k][i][j]->Write();
            ZDeltaPhiMuPosJet[k][i][j]->Write();
            ZDeltaRMuPosJet[k][i][j]->Write();
            ZDeltaEtaMuNegJet[k][i][j]->Write();
            ZDeltaPhiMuNegJet[k][i][j]->Write();
            ZDeltaRMuNegJet[k][i][j]->Write();
            hpfMET_Z[k][i][j]->Write();
            hpfMETphi_Z[k][i][j]->Write();
            hZ_pt[k][i][j]->Write();
            hZ_phi[k][i][j]->Write();
            hZ_mt[k][i][j]->Write();
            hZ_ptVsmt[k][i][j]->Write();
            hWlikePos_pt[k][i][j]->Write();
            hWlikePos_phi[k][i][j]->Write();
            hWlikePos_mt[k][i][j]->Write();
            hJetpt_WlikePos[k][i][j]->Write();
            hJeteta_WlikePos[k][i][j]->Write();
            hJetphi_WlikePos[k][i][j]->Write();
            hu1_WlikePos[k][i][j]->Write();
            hu2_WlikePos[k][i][j]->Write();
            hu1vsZpt_WlikePos[k][i][j]->Write();
            hu2vsZpt_WlikePos[k][i][j]->Write();
          }
          // ZlepDeltaEtaVsMass[k][i][j]->Write();
          // ZlepDeltaPtVsMass[k][i][j]->Write();
          // ZlepDeltaPhiVsMass[k][i][j]->Write();
          // ZlepDeltaRVsMass[k][i][j]->Write();
        }
        // for(int k=0;k<3;k++)
          // for(int h=0; h<WMass::PDF_members; h++)
            // hWlikePos_VarScaled_RWeighted_Templates[h][k][i][j]->Write();
    }
    if(!sampleName.Contains("DYJetsSig")){
      TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
      for(int j=0; j<2*WMass::WMassNSteps+1; j++){
        if(WMass::WMassNSteps!=j){
          int jWmass = (WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV);
          for(int k=0;k<3;k++){
            for(int h=0; h<WMass::PDF_members; h++){
              for(int m=0; m<WMass::NVarRecoilCorr; m++){
                TString toys_str = "";
                if(WMass::NVarRecoilCorr>1) toys_str = Form("_RecoilCorrVar%d",m);
                
                hWlikePos_VarScaled_8_JetCut[m][h][k][i][j]=(TH1D*)hWlikePos_VarScaled_8_JetCut[m][h][k][i][WMass::WMassNSteps]->Clone(Form("hWlikePos_%sScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass));
                hWlikePos_VarScaled_8_JetCut[m][h][k][i][j]->SetName(Form("hWlikePos_%sScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass));
                hWlikePos_VarScaled_8_JetCut[m][h][k][i][j]->SetTitle(Form("hWlikePos_%sScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass));
                hWlikePos_VarScaled_8_JetCut[m][h][k][i][j]->Write();
                hWlikePos_VarNonScaled_8_JetCut[m][h][k][i][j]=(TH1D*)hWlikePos_VarNonScaled_8_JetCut[m][h][k][i][WMass::WMassNSteps]->Clone(Form("hWlikePos_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass));
                hWlikePos_VarNonScaled_8_JetCut[m][h][k][i][j]->SetName(Form("hWlikePos_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass));
                hWlikePos_VarNonScaled_8_JetCut[m][h][k][i][j]->SetTitle(Form("hWlikePos_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass));
                hWlikePos_VarNonScaled_8_JetCut[m][h][k][i][j]->Write();
              }
            }
          }
        }
      }
    }    
  }
  
  fout->Write();
  fout->Close();

}
