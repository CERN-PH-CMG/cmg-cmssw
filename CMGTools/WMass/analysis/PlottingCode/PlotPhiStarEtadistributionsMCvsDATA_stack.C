#include "../includes/common.h"
#include "../AnalysisCode/common_stuff.C"

void PlotPhiStarEtadistributionsMCvsDATA_stack(TString folderMCsig="",TString folderMCEWK="",TString folderMCTT="",TString folderMCQCD="",TString folderDATA=""){

  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  
  
  cout << "PlotPhiStarEtadistributionsMCvsDATA_stack.C working dir: " << gSystem->WorkingDirectory() << endl;

  TFile *fMCsig=new TFile(Form("%s/PhiStarEtaAnalysisOnDATA.root",folderMCsig.Data()));
  TFile *fMCEWK=new TFile(Form("%s/PhiStarEtaAnalysisOnDATA.root",folderMCEWK.Data()));
  TFile *fMCTT=new TFile(Form("%s/PhiStarEtaAnalysisOnDATA.root",folderMCTT.Data()));
  TFile *fMCQCD=new TFile(Form("%s/PhiStarEtaAnalysisOnDATA.root",folderMCQCD.Data()));
  TFile *fDATA=new TFile(Form("%s/PhiStarEtaAnalysisOnDATA.root",folderDATA.Data()));
  
  fMCsig->Print();
  fMCEWK->Print();
  fMCTT->Print();
  fDATA->Print();

  TString LegendEvTypeTeX="Z#rightarrow#mu#mu";
  
  for(int i=0; i<WMass::etaMuonNSteps; i++){
    TString eta_str = Form("%.1f",WMass::etaMaxMuons[i]); eta_str.ReplaceAll(".","p");
    // for(int j=0; j<2*WMass::WMassNSteps+1; j++){
    // for(int j=WMass::WMassNSteps; j<WMass::WMassNSteps+1; j++){
      // int jWmass = WMass::WMassCentral_MeV-(WMass::WMassNSteps-j)*WMass::WMassStep_MeV;
      
      // common_stuff::plotAndSaveHisto1D_stack(LegendEvTypeTeX,fMCsig,fMCEWK,fMCTT,fMCQCD,fDATA,Form("hWPos_METNonScaled_8_JetCut_pdf10042-0_eta%s_%d",eta_str.Data(),jWmass),0,0,0,1,";MET [GeV];Counts",-1,-1,1);
      // common_stuff::plotAndSaveHisto1D_stack(LegendEvTypeTeX,fMCsig,fMCEWK,fMCTT,fMCQCD,fDATA,Form("hnvtx_Sig_eta%s_%d",eta_str.Data(),jWmass),0,0,0,1,";number of vertices;Counts",0,50,1);
      // TH1D*h1 = (TH1D*)fMCsig->Get(
      common_stuff::plotAndSaveHisto1D_stack(LegendEvTypeTeX,fMCsig,fMCEWK,fMCTT,fMCQCD,fDATA,
                                              Form("PhiStarEta_pdf10042-0_eta%s",eta_str.Data())
                                              ,1,1,0,1,";#phi^{*};Counts",0.01,1,1,1);

    // }

  }
  
  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

void plotAndSaveHisto1D(TFile*f1, TString str1, TFile*f2, TString str2, int logx, int logy, int logz, int scaleMCtoDATA){

  TH1D*h1=(TH1D*)f1->Get(str1.Data());
  h1->SetLineColor(2);
  TH1D*h2=(TH1D*)f2->Get(str2.Data());
  
  TCanvas *c1 = new TCanvas("c"+str1);
  c1->SetLogx(logx);
  c1->SetLogy(logy);
  c1->SetLogz(logz);
  
  if(scaleMCtoDATA){
    h1->DrawNormalized();
    h2->DrawNormalized("same");
  }else{
    h1->Draw();
    h2->Draw("same");    
  }
  
  c1->SaveAs(str1+".png");
  
}

