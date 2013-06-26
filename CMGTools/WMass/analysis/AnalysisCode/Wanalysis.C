// UNCOMMENT TO USE PDF REWEIGHTING
//#define LHAPDF_ON

#ifdef LHAPDF_ON
  #include "LHAPDF/LHAPDF.h"
#endif 

#define Wanalysis_cxx
#include "Wanalysis.h"
#include "../includes/common.h"
// #include "rochcor_42X.h"
#include "rochcor_44X_v3.h"
#include "MuScleFitCorrector.h"
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <TMath.h>
#include <TCanvas.h>
#include <vector>
#include <TGraphAsymmErrors.h>
#include <ctime>
#include <time.h>

void Wanalysis::Loop(int IS_MC_CLOSURE_TEST, int isMCorDATA, TString outputdir, int useMomentumCorr, int smearRochCorrByNsigma, int useEffSF, int usePtSF, int useVtxSF, int controlplots, TString sampleName, int generated_PDF_set, int generated_PDF_member, int contains_PDF_reweight)
{

    cout << "generated_PDF_set= "<<generated_PDF_set
         << " generated_PDF_member= " << generated_PDF_member
         << " contains_PDF_reweight= " << contains_PDF_reweight
         << " WMass::NtoysMomCorr= " << WMass::NtoysMomCorr
         << endl;

  if (fChain == 0) return;
  
  TRandom3 *r = new TRandom3(0);
  
  if(!outputdir.Contains("../")) outputdir = "../"+outputdir;
  cout << "output filename= " << Form("%s/Wanalysis.root",outputdir.Data()) << endl;
  
  ofstream outTXTfile;
  outTXTfile.open(Form("%s/Wanalysis_EVlog.log",outputdir.Data()));
  
  #ifdef LHAPDF_ON
    // LHAPDF::initPDFSet(1,"CT10nnlo.LHgrid");
    if(!sampleName.Contains("DATA")){
      cout << "inizializing LHAPDF::initPDFSet(1)" << endl;
      LHAPDF::initPDFSet(1,generated_PDF_set,generated_PDF_member); // CMSSW DEFAULT
      cout << "finished inizializing LHAPDF" << endl;

      cout << "inizializing LHAPDF::initPDFSet(0)" << endl;
      // LHAPDF::initPDFSet();
      if(WMass::PDF_sets==11200)
        // LHAPDF::initPDFSet(0,"CT10nnlo.LHgrid");
        LHAPDF::initPDFSet(0,11200,0);
      else if(WMass::PDF_sets==232000)
        // LHAPDF::initPDFSet(0,"NNPDF23_nnlo_as_0118.LHgrid");
        LHAPDF::initPDFSet(0,232000,0);
      else if(WMass::PDF_sets==21200)
        // LHAPDF::initPDFSet(0,"MSTW2008nnlo68cl.LHgrid");
        LHAPDF::initPDFSet(0,21200,0);
      else if(WMass::PDF_sets==21241)
        LHAPDF::initPDFSet(0,21241,0);  // else if(WMass::PDF_sets<0)
        // LHAPDF::initPDFSet(0,generated_PDF_set,generated_PDF_member);
    }
    
  #endif 
  
  TH1D *hWPos_VarScaled_1_Gen[WMass::NFitVar][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_VarScaled_2_ZGenMassCut[WMass::NFitVar][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_VarScaled_3_Mu1GenCut[WMass::NFitVar][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_VarScaled_4_Mu2GenCut[WMass::NFitVar][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_VarScaled_5_RecoCut[WMass::NFitVar][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_VarScaled_6_METCut[WMass::NFitVar][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_VarScaled_7_RecoilCut[WMass::NFitVar][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_VarScaled_8_JetCut[WMass::NtoysMomCorr][WMass::PDF_members][WMass::NFitVar][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_VarNonScaled_8_JetCut[WMass::NtoysMomCorr][WMass::PDF_members][WMass::NFitVar][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_VarScaled_QCD[WMass::NFitVar][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];

  TH1D *hPDF_weights[WMass::PDF_members];
  for(int h=0; h<WMass::PDF_members; h++)
    hPDF_weights[h]=new TH1D(Form("hPDF_weights_%d_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h),Form("hPDF_weights_%d_%d",WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h),1000,0,2);
    
  TH1D *hPDF_x1=new TH1D("hPDF_x1","hPDF_x1",1000,-4,0);
  TH1D *hPDF_x1unweighted=new TH1D("hPDF_x1unweighted","hPDF_x1unweighted",1000,-4,0);
  TH1D *hPDF_x2=new TH1D("hPDF_x2","hPDF_x2",1000,-4,0);
  TH1D *hPDF_x2unweighted=new TH1D("hPDF_x2unweighted","hPDF_x2unweighted",1000,-4,0);
  TH1D *hPileUp_Fall11=new TH1D("hPileUp_Fall11","hPileUp_Fall11",50,0,50);
  TH1D *hnvtx[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1],*hnoTrgMuonsLeadingPt[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hpfMET_WPos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hpfMETphi_WPos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hWPos_pt[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hWPos_phi[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hWPos_mt[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hMupt_WPos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hMueta_WPos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hMuphi_WPos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hMulogiso_WPos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hJetpt_WPos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  TH1D *hJeteta_WPos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hJetphi_WPos[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1], *hWPos_MuDRgen[WMass::nSigOrQCD][WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  // TH2D *hWPos_logiso_vs_logdxy[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  // TH2D *hWPos_iso_vs_dxy[WMass::etaMuonNSteps][2*WMass::WMassNSteps+1];
  
  TFile*finEffSF, *finPileupSF,*finPtSF;
  TGraphAsymmErrors*hEffSF_MuId_eta_2011[2],*hEffSF_Iso_eta_2011[2],*hEffSF_HLT_eta_2011/* ,*hEffSF_Iso_vtx_2011A,*hEffSF_Iso_vtx_2011B*/;
  TH1D*hPileupSF,*hWPtSFPos,*hWPtSFNeg;

  // retrieve efficiencies SF
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
  // retrieve pileup SF
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
  // retrieve pileup SF
  if(usePtSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0)){
    // cout << "APPLYING W PT RESCALING" << endl;
    finPtSF = new TFile(Form("../utils/Wpt_reweighting.root")); // used only to build templates
    if(!finPtSF){
      cout << "file " << Form("../utils/Wpt_reweighting.root") << " is missing, impossible to retrieve W pt reweighting factors" << endl;
      // return;
    }else{
      hWPtSFPos=(TH1D*)finPtSF->Get("hWPos_pt_Sig_eta0p6");
      hWPtSFNeg=(TH1D*)finPtSF->Get("hWNeg_pt_Sig_eta0p6");
    }
  }
  

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
  
  // analysis can be automatically repeated for several |eta| ranges
  for(int i=0; i<WMass::etaMuonNSteps; i++){
    TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");

    // and several W mass hypotheses
    for(int j=0; j<2*WMass::WMassNSteps+1; j++){
      
      if(!sampleName.Contains("WJetsSig") && WMass::WMassNSteps!=j) continue;
      
      int jWmass = (WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV);
      cout << "histos i= " << i << " eta_str= "<< eta_str <<" j= " << j << " jWmass= " << jWmass << endl;
      for(int k=0;k<3;k++){
        hWPos_VarScaled_1_Gen[k][i][j]=new TH1D(Form("hWPos_%sScaled_1_Gen_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),Form("hWPos_%sScaled_1_Gen_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),nbins,bins_scaled[k]);
        hWPos_VarScaled_1_Gen[k][i][j]->Sumw2();
        hWPos_VarScaled_2_ZGenMassCut[k][i][j]=new TH1D(Form("hWPos_%sScaled_2_ZGenMassCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),Form("hWPos_%sScaled_2_ZGenMassCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),nbins,bins_scaled[k]);
        hWPos_VarScaled_2_ZGenMassCut[k][i][j]->Sumw2();
        hWPos_VarScaled_3_Mu1GenCut[k][i][j]=new TH1D(Form("hWPos_%sScaled_3_Mu1GenCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),Form("hWPos_%sScaled_3_Mu1GenCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),nbins,bins_scaled[k]);
        hWPos_VarScaled_3_Mu1GenCut[k][i][j]->Sumw2();
        hWPos_VarScaled_4_Mu2GenCut[k][i][j]=new TH1D(Form("hWPos_%sScaled_4_Mu2GenCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),Form("hWPos_%sScaled_4_Mu2GenCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),nbins,bins_scaled[k]);
        hWPos_VarScaled_4_Mu2GenCut[k][i][j]->Sumw2();
        hWPos_VarScaled_5_RecoCut[k][i][j]=new TH1D(Form("hWPos_%sScaled_5_RecoCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),Form("hWPos_%sScaled_5_RecoCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),nbins,bins_scaled[k]);
        hWPos_VarScaled_5_RecoCut[k][i][j]->Sumw2();
        hWPos_VarScaled_6_METCut[k][i][j]=new TH1D(Form("hWPos_%sScaled_6_METCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),Form("hWPos_%sScaled_6_METCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),nbins,bins_scaled[k]);
        hWPos_VarScaled_6_METCut[k][i][j]->Sumw2();
        hWPos_VarScaled_7_RecoilCut[k][i][j]=new TH1D(Form("hWPos_%sScaled_7_RecoilCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),Form("hWPos_%sScaled_7_RecoilCut_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),nbins,bins_scaled[k]);
        hWPos_VarScaled_7_RecoilCut[k][i][j]->Sumw2();
        for(int h=0; h<WMass::PDF_members; h++){
          for(int m=0; m<WMass::NtoysMomCorr; m++){
            TString toys_str = "";
            if(WMass::NtoysMomCorr>1) toys_str = Form("_MomCorrToy%d",m);

            hWPos_VarScaled_8_JetCut[m][h][k][i][j]=new TH1D(Form("hWPos_%sScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass),Form("hWPos_%sScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass),nbins,bins_scaled[k]);
            hWPos_VarScaled_8_JetCut[m][h][k][i][j]->Sumw2();
            hWPos_VarNonScaled_8_JetCut[m][h][k][i][j]=new TH1D(Form("hWPos_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass),Form("hWPos_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass),nbins,bins_Notscaled[k]);
            hWPos_VarNonScaled_8_JetCut[m][h][k][i][j]->Sumw2();
          }
        }
        hWPos_VarScaled_QCD[k][i][j]=new TH1D(Form("hWPos_%sScaled_QCD_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),Form("hWPos_%sScaled_QCD_eta%s_%d",WMass::FitVar_str[k].Data(),eta_str.Data(),jWmass),nbins,bins_scaled[k]);
        hWPos_VarScaled_QCD[k][i][j]->Sumw2();
      }
      // hWPos_logiso_vs_logdxy[i][j]=new TH2D(Form("hWPos_logiso_vs_logdxy_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_logiso_vs_logdxy_eta%s_%d",eta_str.Data(),jWmass),1000,-5,1,1000,-5,1);
      // hWPos_iso_vs_dxy[i][j]=new TH2D(Form("hWPos_iso_vs_dxy_eta%s_%d",eta_str.Data(),jWmass),Form("hWPos_iso_vs_dxy_eta%s_%d",eta_str.Data(),jWmass),1000,-0.001,0.999,1000,0,1);
      
      // relevant histograms are created for Signal and QCD enriched phase-space
      if(controlplots){
        for(int k=0; k<WMass::nSigOrQCD; k++){
          hnvtx[k][i][j]=new TH1D(Form("hnvtx_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hnvtx_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),200,0,200);
          hnoTrgMuonsLeadingPt[k][i][j]=new TH1D(Form("hnoTrgMuonsLeadingPt_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hnoTrgMuonsLeadingPt_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),300,-100,200);
          hpfMET_WPos[k][i][j]=new TH1D(Form("hpfMET_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hpfMET_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),200,0,200);
          hpfMETphi_WPos[k][i][j]=new TH1D(Form("hpfMETphi_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hpfMETphi_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,-TMath::Pi(),TMath::Pi());
          hWPos_pt[k][i][j]=new TH1D(Form("hWPos_pt_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hWPos_pt_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,0,25);
          hWPos_phi[k][i][j]=new TH1D(Form("hWPos_phi_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hWPos_phi_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,-TMath::Pi(),TMath::Pi());
          hWPos_mt[k][i][j]=new TH1D(Form("hWPos_mt_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hWPos_mt_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),200,0,200);
          hMupt_WPos[k][i][j]=new TH1D(Form("hMupt_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hMupt_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),200,0,200);
          hMueta_WPos[k][i][j]=new TH1D(Form("hMueta_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hMueta_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,-2.5,2.5);
          hMuphi_WPos[k][i][j]=new TH1D(Form("hMuphi_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hMuphi_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,-TMath::Pi(),TMath::Pi());
          hMulogiso_WPos[k][i][j]=new TH1D(Form("hMulogiso_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hMulogiso_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),1000,-5,3);
          hJetpt_WPos[k][i][j]=new TH1D(Form("hJetpt_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hJetpt_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,0,50);
          hJeteta_WPos[k][i][j]=new TH1D(Form("hJeteta_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hJeteta_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,-2.5,2.5);
          hJetphi_WPos[k][i][j]=new TH1D(Form("hJetphi_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hJetphi_WPos_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),100,-TMath::Pi(),TMath::Pi());
          hWPos_MuDRgen[k][i][j]=new TH1D(Form("hWPos_MuDRgen_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),Form("hWPos_MuDRgen_%s_eta%s_%d",WMass::nSigOrQCD_str[k].Data(),eta_str.Data(),jWmass),1000,-6,1);
        }
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
  rochcor_44X_v3 *rmcor44X = WMass::NtoysMomCorr>1? new rochcor_44X_v3(random_seed_start) : new rochcor_44X_v3();  // make the pointer of rochcor class
  TString MuscleCard = (IS_MC_CLOSURE_TEST || isMCorDATA==0) ? "MuScleFit_2011_MC_44X" : "MuScleFit_2011_DATA_44X";
  TString fitParametersFile = MuscleCard+".txt";
  MuScleFitCorrector *corrector;
  if(useMomentumCorr==2){
    cout << "using MuscleFit card " << fitParametersFile << endl;
    corrector = new MuScleFitCorrector(fitParametersFile);
  }
  
  // first_entry=469;
  // nentries=470;
  
  // start the actual event loop
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=first_entry; jentry<nentries;jentry++) {
  // for (Long64_t jentry=0; jentry<1e5;jentry++) { // for testing purposes
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    if(jentry%250000==0) cout <<"Analyzed entry "<<jentry<<"/"<<nentries<<endl;
    // if(jentry%500==0) cout <<"Analyzed entry "<<jentry<<"/"<<nentries<<endl;
    if(jentry%50000==0){
      time_t now = time(0);
      TString dt = ctime(&now); dt.ReplaceAll("\n"," ");
      outTXTfile << dt << "\t - \t Analyzed entry "<<jentry<<"/"<<nentries<<endl;
    }

    
    double evt_weight_original = lumi_scaling;
    if(useVtxSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && npu>0) evt_weight_original=lumi_scaling*hPileupSF->GetBinContent(hPileupSF->GetXaxis()->FindBin(npu));
    if(usePtSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0) && sampleName.Contains("WJetsSig")){
      // cout << "TRYING TO APPLY W PT RESCALING" << endl;
      // if(MuGen_charge>0||Mu_charge>0) evt_weight_original*=hWPtSFPos->GetBinContent(hWPtSFPos->GetXaxis()->FindBin(W_pt>0?W_pt:WGen_pt));
      // else if(MuGen_charge<0||Mu_charge<0) evt_weight_original*=hWPtSFNeg->GetBinContent(hWPtSFNeg->GetXaxis()->FindBin(W_pt>0?W_pt:WGen_pt));
    }
    
    int runopt = r->Rndm()<0.457451 ? 0 : 1;
    double MuPos_tight_muon_SF = 1;
    // THIS MUST BE CHECKED !!!!!
    if(useEffSF && (IS_MC_CLOSURE_TEST || isMCorDATA==0)){
      MuPos_tight_muon_SF = hEffSF_MuId_eta_2011[runopt]->Eval(Mu_eta)*hEffSF_Iso_eta_2011[runopt]->Eval(Mu_eta)*hEffSF_HLT_eta_2011->Eval(Mu_eta);
    }
    // cout << "MuPos_tight_muon_SF= " << MuPos_tight_muon_SF << endl;
    // if(!(IS_MC_CLOSURE_TEST || isMCorDATA==0) && run<175832) continue; // TEMPORARY TO TEST ROCHESTER CORRECTIONS
    
    if((IS_MC_CLOSURE_TEST || isMCorDATA==0) && controlplots) hPileUp_Fall11->Fill(npu);
    
    for(int i=0; i<WMass::etaMuonNSteps; i++){
      for(int j=0; j<2*WMass::WMassNSteps+1; j++){
        
        if(!sampleName.Contains("WJetsSig") && WMass::WMassNSteps!=j) continue;
        double iWmass = (WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV)/1e3;
        
        // cout << "events i= " << i << " j= " << j << " iWmass= " << iWmass<< " evt_weight_original= " << evt_weight_original << endl;
        
        // BW REWEIGHTING
        double shat=0,gamma=2.141 /*HARD CODED TO PDG VALUE*/,mw0=0,mw_i=0,weight_i=1;
        if(useGenVar){
          shat=WGen_m*WGen_m;
          mw0=WMass::WMassCentral_MeV/1e3;
          mw_i=iWmass;
          // ((shat - mw0^2)^2 + gamma^2 mw0^2) / ((shat - mw_i^2)^2 + gamma^2 mw_i^2)
          weight_i=(TMath::Power(shat - mw0*mw0,2) + TMath::Power(gamma*mw0,2)) / (TMath::Power(shat - mw_i*mw_i,2) + TMath::Power(gamma*mw_i,2));
          // cout << "WGen_m = " << WGen_m << " mw0= " << mw0 << " mw_i= " << mw_i << " weight_i= " << weight_i << endl;
        }
        double evt_weight=evt_weight_original*weight_i;
          
        // GEN STUFF IF REQUESTED
        if(useGenVar){
          
          if(WGen_m>0){ // only for signal event
            if(MuGen_charge>0){ // only for positive muons
              double MuPosGen_var_jacobian[3] = {2*MuGen_pt/iWmass,WGen_mt/iWmass,2*NuGen_pt/iWmass,};
              // AVOID OVERFLOW BIN TO BE FILLED
              for(int k=0;k<3;k++)
                if(MuPosGen_var_jacobian[k]>=xmax) MuPosGen_var_jacobian[k]=xmax-binsize2/2;
              
              for(int k=0;k<3;k++){
                hWPos_VarScaled_1_Gen[k][i][j]->Fill(MuPosGen_var_jacobian[k],evt_weight);
                
                // mass cut not meaningful for W case
                hWPos_VarScaled_2_ZGenMassCut[k][i][j]->Fill(MuPosGen_var_jacobian[k],evt_weight);
                }
                
                if(TMath::Abs(MuGen_eta)<WMass::etaMaxMuons[i]){
                  for(int k=0;k<3;k++){
                    hWPos_VarScaled_3_Mu1GenCut[k][i][j]->Fill(MuPosGen_var_jacobian[k],evt_weight);
                    
                    // second lepton (i.e. neutrino) not detected
                    // if(TMath::Abs(MuNegGen_eta)<2.4){
                      hWPos_VarScaled_4_Mu2GenCut[k][i][j]->Fill(MuPosGen_var_jacobian[k],evt_weight);
                    // }
                  }
                }
              // }
            }
          }
        }
          
        if(!useGenVar || W_mt>0){ // dummy thing to separate between sig and bkg in W+Jets (useless)
          for(int m=0; m<WMass::NtoysMomCorr; m++){
            
            // good reco event selection
            if(evtHasGoodVtx && evtHasTrg && Mu_charge>0){
              TLorentzVector mu; //TLorentzVector of the reconstructed muon
              //Set TLorentzVector of mu
              mu.SetPtEtaPhiM(Mu_pt,Mu_eta,Mu_phi,Mu_mass);
              // mu.Print();
              // cout << "before roch= " << m << " " << Mu_pt << " " << Mu_eta << " " << Mu_phi << " "<< Mu_mass << endl;
              //use rochester correction if required
              if(useMomentumCorr==1){ // use Rochester Momentum scale corrections if required
                if(IS_MC_CLOSURE_TEST || isMCorDATA==0){
                  // IN THE LAST VERSION NO RUN DEPENDENCE IN MC (TO be CHECKED)
                  // int runopt = r->Rndm()<0.457451 ? 0 : 1; // smear MC according to Run2011A and Run2011B statistics (if cut on pileup the 0.457... must be changed accordingly!!!
                  rmcor44X->momcor_mc(mu, Mu_charge, smearRochCorrByNsigma/* , runopt */);
                }
                else{
                  rmcor44X->momcor_data(mu, Mu_charge, smearRochCorrByNsigma, run<175832 ? 0 : 1 );
                }
              }else if(useMomentumCorr==2){ // use MuscleFit Momentum scale corrections if required
                corrector->applyPtCorrection(mu,Mu_charge);
              }
              // cout << "after roch= " << m << " " << mu.Pt() << " " << mu.Eta() << " " << mu.Phi() << " "<< Mu_mass << endl;
              TLorentzVector nu,W; //TLorentzVector of the reconstructed W
              nu.SetPtEtaPhiM(pfmet,0,pfmet_phi,0);
              W = mu + nu;

              double MuPos_var_jacobian[3] = {2*mu.Pt()/iWmass,W.Mt()/iWmass,2*nu.Pt()/iWmass}; // SCALED VARIABLE
              double MuPos_var_NotScaled[3] = {mu.Pt(),W.Mt(),nu.Pt()}; // SCALED VARIABLE
              // LUCA ADD TO AVOID OVERFLOW
              for(int k=0;k<3;k++)
                if(MuPos_var_jacobian[k]>=xmax) MuPos_var_jacobian[k]=xmax-binsize2/2;
              
              int wmass1 = iWmass*1e3;

              // good event with mu from W candidate within acceptance
              if( /* Z_mass>50  not possible to apply something similar to Z mass cut*/
                  TMath::Abs(mu.Eta())<WMass::etaMaxMuons[i] && mu.Pt()>30
                ){
                // muon candidate is passing tight, iso, dxy requirements
                if(MuIsTightAndIso && MuRelIso<0.12 && Mu_dxy<0.02 && noTrgMuonsLeadingPt<10 ){
                  for(int k=0;k<3;k++)
                    if(m==0) hWPos_VarScaled_5_RecoCut[k][i][j]->Fill(MuPos_var_jacobian[k],evt_weight*MuPos_tight_muon_SF);

                  if(pfmet>25){
                    for(int k=0;k<3;k++)
                      if(m==0) hWPos_VarScaled_6_METCut[k][i][j]->Fill(MuPos_var_jacobian[k],evt_weight*MuPos_tight_muon_SF);
                    
                    if(W_pt<20){
                      for(int k=0;k<3;k++)
                        if(m==0) hWPos_VarScaled_7_RecoilCut[k][i][j]->Fill(MuPos_var_jacobian[k],evt_weight*MuPos_tight_muon_SF);
                      
                      if(true){ // no jet pt cut at the moment
                      // if(Jet_leading_pt<30){
                        // cout << (mu.Pt()<xmax*80/2 ? mu.Pt() : (xmax-binsize2/2)*80/2 )<< endl;
                        
                        // VERY DUMMY REWEIGHTING
                        // hWPos_VarNonScaled_8_JetCut[i][j]->Fill(mu.Pt()*iWmass/(WMass::WMassCentral_MeV/1e3)<xmax*80/2 ? mu.Pt()*iWmass/(WMass::WMassCentral_MeV/1e3) : (xmax-binsize2/2)*80/2 ,evt_weight*MuPos_tight_muon_SF);
                        
                        // std::cout << "event= " << jentry << " mw0= " << mw0 << " iWmass= " << iWmass << " WGen_m= " << WGen_m << " weight_i= " << weight_i << std::endl;
                        // cout << "filling pt= " << (mu.Pt()*iWmass/(WMass::WMassCentral_MeV/1e3)<xmax*80/2 ? mu.Pt() : (xmax-binsize2/2)*80/2) <<" evt_weight= " << evt_weight << " MuPos_tight_muon_SF= " << MuPos_tight_muon_SF << endl;
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
                          hPDF_x1->Fill(TMath::Log10(parton1_x));
                          hPDF_x1unweighted->Fill(TMath::Log10(parton1_x),1/weight_old);
                          hPDF_x2->Fill(TMath::Log10(parton2_x));
                          hPDF_x2unweighted->Fill(TMath::Log10(parton2_x),1/weight_old);
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
                            hWPos_VarScaled_8_JetCut[m][h][k][i][j]->Fill(MuPos_var_jacobian[k],evt_weight*MuPos_tight_muon_SF*lha_weight);
                            hWPos_VarNonScaled_8_JetCut[m][h][k][i][j]->Fill(MuPos_var_NotScaled[k]*iWmass/(WMass::WMassCentral_MeV/1e3)<xmax*80/(k==1 ? 1 : 2) ? MuPos_var_NotScaled[k] : (xmax-binsize2/2)*80/(k==1 ? 1 : 2) ,evt_weight*MuPos_tight_muon_SF*lha_weight);
                          }
                        }
                        
                        // hWPos_VarNonScaled_8_JetCut[i][j]->Fill(mu.Pt()*iWmass/(WMass::WMassCentral_MeV/1e3)<xmax*80/2 ? mu.Pt() : (xmax-binsize2/2)*80/2 ,evt_weight*MuPos_tight_muon_SF);

                        // cout << wmass1 << " " << WMass::WMassCentral_MeV << " " << (wmass1 - WMass::WMassCentral_MeV) << endl;
                        // cout << WMass::etaMaxMuons[i]  << " " << 2.1 << " " << ((WMass::etaMaxMuons[i] - 2.1)) << endl;
                        // if( (TMath::Abs(wmass1 - WMass::WMassCentral_MeV) > 1)
                          // // || (TMath::Abs(WMass::etaMaxMuons[i] - 2.1) > 1e-3 ) 
                          // ) 
                          // continue;
                          
                        // cout << "wmass1 " << wmass1 << " WMass::etaMaxMuons[i]= " << WMass::etaMaxMuons[i] << endl;

                        if(controlplots){
                          if(useGenVar) hWPos_MuDRgen[0][i][j]->Fill(TMath::Log10(MuDRGenP),evt_weight*MuPos_tight_muon_SF);
                          hnvtx[0][i][j]->Fill(nvtx,evt_weight); // TO FIT MET IN THE WHOLE RANGE!!!!
                          hnoTrgMuonsLeadingPt[0][i][j]->Fill(noTrgMuonsLeadingPt,evt_weight*MuPos_tight_muon_SF); // TO FIT MET IN THE WHOLE RANGE!!!!
                          hpfMETphi_WPos[0][i][j]->Fill(pfmet_phi,evt_weight*MuPos_tight_muon_SF);
                          hWPos_pt[0][i][j]->Fill(W_pt,evt_weight*MuPos_tight_muon_SF);
                          hWPos_phi[0][i][j]->Fill(W_phi,evt_weight*MuPos_tight_muon_SF);
                          hWPos_mt[0][i][j]->Fill(W_mt,evt_weight*MuPos_tight_muon_SF);
                          hMupt_WPos[0][i][j]->Fill(mu.Pt(),evt_weight*MuPos_tight_muon_SF);
                          hMueta_WPos[0][i][j]->Fill(mu.Eta(),evt_weight*MuPos_tight_muon_SF);
                          hMuphi_WPos[0][i][j]->Fill(mu.Phi(),evt_weight*MuPos_tight_muon_SF);
                          hMulogiso_WPos[0][i][j]->Fill(TMath::Log10(MuRelIso),evt_weight*MuPos_tight_muon_SF);
                          hJetpt_WPos[0][i][j]->Fill(Jet_leading_pt,evt_weight*MuPos_tight_muon_SF);
                          hJeteta_WPos[0][i][j]->Fill(Jet_leading_eta,evt_weight*MuPos_tight_muon_SF);
                          hJetphi_WPos[0][i][j]->Fill(Jet_leading_phi,evt_weight*MuPos_tight_muon_SF);
                        }
                      }
                    }
                  }
                  if(controlplots && W_pt<20 && m==0){ // do not cut on MET to let the fit have an handle
                          
                    // if(TMath::Abs(wmass1 - WMass::WMassCentral_MeV) > 1) continue;
                      
                    hpfMET_WPos[0][i][j]->Fill(pfmet,evt_weight*MuPos_tight_muon_SF); // TRICK TO FIT MET IN THE WHOLE RANGE!!!!
                      
                  }
                }else if(controlplots && m==0){ // muon candidate is failing either tight ID, iso or dxy: QCD enriched region
                  
                  if(pfmet>25 && W_pt<20){
                    // if( (TMath::Abs(wmass1 - WMass::WMassCentral_MeV) > 1)) continue;
                    // hWPos_logiso_vs_logdxy[i][j]->Fill(TMath::Log10(Mu_dxy),TMath::Log10(MuRelIso),evt_weight*MuPos_tight_muon_SF);
                    // hWPos_iso_vs_dxy[i][j]->Fill(Mu_dxy,MuRelIso,evt_weight*MuPos_tight_muon_SF);
                  }
                    
                  if(
                    MuRelIso>0.12 // single muon cuts (inverted iso (is <0.5 for signal) , no tight requirement)
                    && Mu_dxy>0.02 // single muon cuts (inverted iso (is <0.5 for signal) , no tight requirement)
                    && W_pt<20
                  ){
                  
                    hpfMET_WPos[1][i][j]->Fill(pfmet,evt_weight*MuPos_tight_muon_SF);  // TO FIT MET IN THE WHOLE RANGE!!!!  
                    
                    if(pfmet>25){
                      for(int k=0;k<3;k++)
                        hWPos_VarScaled_QCD[k][i][j]->Fill(MuPos_var_jacobian[k],evt_weight*MuPos_tight_muon_SF);
                      
                      // if( (TMath::Abs(wmass1 - WMass::WMassCentral_MeV) > 1)
                        // )
                        // continue;
                      
                      if(useGenVar) hWPos_MuDRgen[1][i][j]->Fill(TMath::Log10(MuDRGenP),evt_weight*MuPos_tight_muon_SF);
                      hnvtx[1][i][j]->Fill(nvtx,evt_weight);  // TO FIT MET IN THE WHOLE RANGE!!!!  
                      hnoTrgMuonsLeadingPt[1][i][j]->Fill(noTrgMuonsLeadingPt,evt_weight);  // TO FIT MET IN THE WHOLE RANGE!!!!  
                      hpfMETphi_WPos[1][i][j]->Fill(pfmet_phi,evt_weight*MuPos_tight_muon_SF);
                      hWPos_pt[1][i][j]->Fill(W_pt,evt_weight*MuPos_tight_muon_SF);
                      hWPos_phi[1][i][j]->Fill(W_phi,evt_weight*MuPos_tight_muon_SF);
                      hWPos_mt[1][i][j]->Fill(W_mt,evt_weight*MuPos_tight_muon_SF);
                      hMupt_WPos[1][i][j]->Fill(mu.Pt(),evt_weight*MuPos_tight_muon_SF);
                      hMueta_WPos[1][i][j]->Fill(mu.Eta(),evt_weight*MuPos_tight_muon_SF);
                      hMuphi_WPos[1][i][j]->Fill(mu.Phi(),evt_weight*MuPos_tight_muon_SF);
                      hMulogiso_WPos[1][i][j]->Fill(TMath::Log10(MuRelIso),evt_weight*MuPos_tight_muon_SF);
                      hJetpt_WPos[1][i][j]->Fill(Jet_leading_pt,evt_weight*MuPos_tight_muon_SF);
                      hJeteta_WPos[1][i][j]->Fill(Jet_leading_eta,evt_weight*MuPos_tight_muon_SF);
                      hJetphi_WPos[1][i][j]->Fill(Jet_leading_phi,evt_weight*MuPos_tight_muon_SF);
                    }
                  }
                }
              } // end if for good muon in acceptance
            } // end good reco event
          } // end momentum correction toys
        } // end dummy separation sig/bkg
      } // end W mass loop
    } // end muon eta loop
  } // end event loop
  // }
  
  outTXTfile.close();
  
  TFile*fout = new TFile(Form("%s/Wanalysis.root",outputdir.Data()),"RECREATE");
  fout->cd();
  for(int h=0; h<WMass::PDF_members; h++)
    hPDF_weights[h]->Write();
    
  hPDF_x1->Write();
  hPDF_x1unweighted->Write();
  hPDF_x2->Write();
  hPDF_x2unweighted->Write();
  if(controlplots) hPileUp_Fall11->Write();
  for(int i=0; i<WMass::etaMuonNSteps; i++){
  
    // hWPos_logiso_vs_logdxy[i]->Write();
    // hWPos_iso_vs_dxy[i]->Write();
    
    for(int j=0; j<2*WMass::WMassNSteps+1; j++){
      if(!sampleName.Contains("WJetsSig") && WMass::WMassNSteps!=j) continue;
      if(controlplots){
        for(int k=0; k<WMass::nSigOrQCD; k++){
          hWPos_MuDRgen[k][i][j]->Write();
          hnvtx[k][i][j]->Write();
          hnoTrgMuonsLeadingPt[k][i][j]->Write();
          hpfMET_WPos[k][i][j]->Write();
          hpfMETphi_WPos[k][i][j]->Write();
          hWPos_pt[k][i][j]->Write();
          hWPos_phi[k][i][j]->Write();
          hWPos_mt[k][i][j]->Write();
          hMupt_WPos[k][i][j]->Write();
          hMueta_WPos[k][i][j]->Write();
          hMuphi_WPos[k][i][j]->Write();
          hMulogiso_WPos[k][i][j]->Write();
          hJetpt_WPos[k][i][j]->Write();
          hJeteta_WPos[k][i][j]->Write();
          hJetphi_WPos[k][i][j]->Write();
        }
      }
      for(int k=0;k<3;k++){
        hWPos_VarScaled_1_Gen[k][i][j]->Write();
        hWPos_VarScaled_2_ZGenMassCut[k][i][j]->Write();
        hWPos_VarScaled_3_Mu1GenCut[k][i][j]->Write();
        hWPos_VarScaled_4_Mu2GenCut[k][i][j]->Write();
        hWPos_VarScaled_5_RecoCut[k][i][j]->Write();
        hWPos_VarScaled_6_METCut[k][i][j]->Write();
        hWPos_VarScaled_7_RecoilCut[k][i][j]->Write();
        for(int h=0; h<WMass::PDF_members; h++){
          for(int m=0; m<WMass::NtoysMomCorr; m++){
            hWPos_VarScaled_8_JetCut[m][h][k][i][j]->Write();
            hWPos_VarNonScaled_8_JetCut[m][h][k][i][j]->Write();
          }
        }
        if(controlplots)
          hWPos_VarScaled_QCD[k][i][j]->Write();
      }
    }
    
    if(!sampleName.Contains("WJetsSig")){
      TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
      for(int j=0; j<2*WMass::WMassNSteps+1; j++){
        if(WMass::WMassNSteps!=j){
          int jWmass = (WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV);
          for(int k=0;k<3;k++){
            for(int h=0; h<WMass::PDF_members; h++){
              for(int m=0; m<WMass::NtoysMomCorr; m++){
                TString toys_str = "";
                if(WMass::NtoysMomCorr>1) toys_str = Form("_MomCorrToy%d",m);
                
                hWPos_VarScaled_8_JetCut[m][h][k][i][j]=(TH1D*)hWPos_VarScaled_8_JetCut[m][h][k][i][WMass::WMassNSteps]->Clone(Form("hWPos_%sScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass));
                hWPos_VarScaled_8_JetCut[m][h][k][i][j]->SetName(Form("hWPos_%sScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass));
                hWPos_VarScaled_8_JetCut[m][h][k][i][j]->SetTitle(Form("hWPos_%sScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass));
                hWPos_VarScaled_8_JetCut[m][h][k][i][j]->Write();
                hWPos_VarNonScaled_8_JetCut[m][h][k][i][j]=(TH1D*)hWPos_VarNonScaled_8_JetCut[m][h][k][i][WMass::WMassNSteps]->Clone(Form("hWPos_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass));
                hWPos_VarNonScaled_8_JetCut[m][h][k][i][j]->SetName(Form("hWPos_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass));
                hWPos_VarNonScaled_8_JetCut[m][h][k][i][j]->SetTitle(Form("hWPos_%sNonScaled_8_JetCut_pdf%d-%d%s_eta%s_%d",WMass::FitVar_str[k].Data(),WMass::PDF_sets<0?generated_PDF_set:WMass::PDF_sets,h,toys_str.Data(),eta_str.Data(),jWmass));
                hWPos_VarNonScaled_8_JetCut[m][h][k][i][j]->Write();
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
