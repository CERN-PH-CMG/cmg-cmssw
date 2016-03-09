#include <iostream>
#include <vector>

#include "TStyle.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "THStack.h"
#include "TMath.h"
#include "TLatex.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLine.h"
#include "TSystem.h"

void compute_polarization_weights(){
  
  TString foldername = "FEB19_pol";
  double norm2d[3][2][2];
  double norm1d[3][2][1];
  TH2D *histopol2d[3][2][2];
  TH1D *histopol1d[3][2][1];
  TString sample_names[3] = {"DATA","EWKTT","DYJetsPow"};
  enum {  DATA  ,  EWKTT  ,  DYJetsPow  };
  TString charges[2] = {"Pos","Neg"};
  
  TFile *fout = new TFile("Zpol_output_DYJetsPow_pdf229800-0_eta0p9_91188.root","RECREATE");
  
  for(int icharge=0; icharge<2; icharge++){
    for(int isample=0; isample<3; isample++){
      TFile *finput = new TFile(Form("../JobOutputs/%s_mu%s_tkmet_ewk0_KalmanCorrParam_RecoilCorr2_EffHeinerSFCorr_PtSFCorr0_PileupSFCorr/output_%s/ZanalysisOnDATA.root",foldername.Data(),charges[icharge].Data(),sample_names[isample].Data()));
      histopol2d[isample][icharge][0] = (TH2D*)finput->Get(Form("hWlike%s_Zrap_vs_costh_CS_8_JetCut_pdf229800-0_eta0p9_91188",charges[icharge].Data()));
      norm2d[isample][icharge][0] = histopol2d[isample][icharge][0]->Integral();
      histopol2d[isample][icharge][1] = (TH2D*)finput->Get(Form("hWlike%s_phi_CS_vs_costh_CS_8_JetCut_pdf229800-0_eta0p9_91188",charges[icharge].Data()));
      norm2d[isample][icharge][1] = histopol2d[isample][icharge][1]->Integral();
      histopol1d[isample][icharge][0] = (TH1D*)finput->Get(Form("hWlike%s_costh_CS_8_JetCut_pdf229800-0_eta0p9_91188",charges[icharge].Data()));
      norm1d[isample][icharge][0] = histopol1d[isample][icharge][0]->Integral();
    }
    fout->cd();
    
    histopol2d[DATA][icharge][0]->Scale((norm2d[EWKTT][icharge][0]+norm2d[DYJetsPow][icharge][0])/histopol2d[DATA][icharge][0]->Integral());
    histopol2d[DATA][icharge][1]->Scale((norm2d[EWKTT][icharge][1]+norm2d[DYJetsPow][icharge][1])/histopol2d[DATA][icharge][1]->Integral());
    histopol1d[DATA][icharge][0]->Scale((norm1d[EWKTT][icharge][0]+norm1d[DYJetsPow][icharge][0])/histopol1d[DATA][icharge][0]->Integral());
    
    TH2D Zrap_vs_costh_CS = ((*histopol2d[DATA][icharge][0])-(*histopol2d[EWKTT][icharge][0]));
    Zrap_vs_costh_CS.Divide(histopol2d[DYJetsPow][icharge][0]);
    Zrap_vs_costh_CS.Write();
    TH2D phi_CS_vs_costh_CS = ((*histopol2d[DATA][icharge][1])-(*histopol2d[EWKTT][icharge][1]));
    phi_CS_vs_costh_CS.Divide(histopol2d[DYJetsPow][icharge][1]);
    phi_CS_vs_costh_CS.Write();
    TH1D costh_CS = ((*histopol1d[DATA][icharge][0])-(*histopol1d[EWKTT][icharge][0]));
    costh_CS.Divide(histopol1d[DYJetsPow][icharge][0]);
    costh_CS.Write();
    
  }
  
  fout->Write();
  
  
}

int main()
{
  compute_polarization_weights();
  return 0;
}
