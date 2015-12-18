#include "../includes/common.h"
#include "../AnalysisCode/common_stuff.C"

void PlotWdistributionsMCvsDATA(TString folderMC="",TString folderDATA=""){

  cout << "PlotWdistributionsMCvsDATA.C working dir: " << gSystem->WorkingDirectory() << endl;


  TFile *fMC=new TFile(Form("%s/WanalysisOnDATA.root",folderMC.Data()));
  TFile *fDATA=new TFile(Form("%s/WanalysisOnDATA.root",folderDATA.Data()));
  
  fMC->Print();
  fDATA->Print();
      
  common_stuff::plotAndSaveHisto1D(fMC,"hpfMET_WPos_Sig_eta2p1",fDATA,"hpfMET_WPos_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hpfMETphi_WPos_Sig_eta2p1",fDATA,"hpfMETphi_WPos_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hWPos_pt_Sig_eta2p1",fDATA,"hWPos_pt_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hWPos_phi_Sig_eta2p1",fDATA,"hWPos_phi_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hWPos_mt_Sig_eta2p1",fDATA,"hWPos_mt_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hMupt_WPos_Sig_eta2p1",fDATA,"hMupt_WPos_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hMueta_WPos_Sig_eta2p1",fDATA,"hMueta_WPos_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hMuphi_WPos_Sig_eta2p1",fDATA,"hMuphi_WPos_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hJetpt_WPos_Sig_eta2p1",fDATA,"hJetpt_WPos_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hJeteta_WPos_Sig_eta2p1",fDATA,"hJeteta_WPos_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hJetphi_WPos_Sig_eta2p1",fDATA,"hJetphi_WPos_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  
  // TCanvas*cZlepEta1VsEta2_MC=new TCanvas("cZlepEta1VsEta2_MC","cZlepEta1VsEta2_MC");
  // TH2D*ZlepEta1VsEta2_MC=(TH2D*)fMC->Get("ZlepEta1VsEta2");
  // ZlepEta1VsEta2_MC->Draw("colz");
  // cZlepEta1VsEta2_MC->SaveAs("ZlepEta1VsEta2_MC.png");
    
  // TCanvas*chpfMETphi_Z=new TCanvas("chpfMETphi_Z","chpfMETphi_Z");
  // TH1D*ZhpfMETphi_Z=(TH1D*)fMC->Get("hpfMETphi_Z");
  // ZhpfMETphi_Z->SetLineColor(2);
  // TH1D*WPoshpfMETphi_Z=(TH1D*)fDATA->Get("hpfMETphi_WPos");
  // WPoshpfMETphi_Z->GetYaxis()->SetRangeUser(0,WPoshpfMETphi_Z->GetMaximum()*1.2);
  // if(WPoshpfMETphi_Z->GetMaximum() > ZhpfMETphi_Z->GetMaximum()){
    // WPoshpfMETphi_Z->DrawNormalized();
    // ZhpfMETphi_Z->DrawNormalized("same");
  // }else{
    // ZhpfMETphi_Z->DrawNormalized("");
    // WPoshpfMETphi_Z->DrawNormalized("same");
  // }
  // chpfMETphi_Z->SaveAs("hpfMETphi_V.png");

  
}

