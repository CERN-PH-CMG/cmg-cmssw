#include "../includes/common.h"
#include "../AnalysisCode/common_stuff.C"

void PlotZdistributionsMCvsDATA(TString folderMC="",TString folderDATA=""){

  cout << "PlotWZdistributionsDATA.C working dir: " << gSystem->WorkingDirectory() << endl;


  TFile *fMC=new TFile(Form("%s/ZanalysisOnDATA.root",folderMC.Data()));
  TFile *fDATA=new TFile(Form("%s/ZanalysisOnDATA.root",folderDATA.Data()));
  
  fMC->Print();
  fDATA->Print();
      
  common_stuff::plotAndSaveHisto1D(fMC,"hnvtx_Sig_eta0p6",fDATA,"hnvtx_Sig_eta0p6", /*logxyz*/ 0,0,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"Zmass_Sig_eta2p1",fDATA,"Zmass_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"ZlepDeltaEta_Sig_eta2p1",fDATA,"ZlepDeltaEta_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  
  TCanvas*cZlepEta1VsEta2_MC=new TCanvas("cZlepEta1VsEta2_MC","cZlepEta1VsEta2_MC");
  TH2D*ZlepEta1VsEta2_MC=(TH2D*)fMC->Get("ZlepEta1VsEta2_Sig_eta2p1");
  ZlepEta1VsEta2_MC->Draw("colz");
  cZlepEta1VsEta2_MC->SaveAs("ZlepEta1VsEta2_MC.png");
  
  TCanvas*cZlepEta1VsEta2_DATA=new TCanvas("cZlepEta1VsEta2_DATA","cZlepEta1VsEta2_DATA");
  TH2D*ZlepEta1VsEta2_DATA=(TH2D*)fDATA->Get("ZlepEta1VsEta2_Sig_eta2p1");
  ZlepEta1VsEta2_DATA->Draw("colz");
  cZlepEta1VsEta2_DATA->SaveAs("ZlepEta1VsEta2_DATA.png");
  
  common_stuff::plotAndSaveHisto1D(fMC,"ZlepDeltaPt_Sig_eta2p1",fDATA,"ZlepDeltaPt_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);

  
  TCanvas*cZlepPt1VsPt2_MC=new TCanvas("cZlepPt1VsPt2_MC","cZlepPt1VsPt2_MC");
  TH2D*ZlepPt1VsPt2_MC=(TH2D*)fMC->Get("ZlepPt1VsPt2_Sig_eta2p1");
  ZlepPt1VsPt2_MC->GetXaxis()->SetRangeUser(30,80);
  ZlepPt1VsPt2_MC->GetYaxis()->SetRangeUser(0,80);
  ZlepPt1VsPt2_MC->DrawNormalized("colz");
  cZlepPt1VsPt2_MC->SetLogz();
  cZlepPt1VsPt2_MC->SaveAs("ZlepPt1VsPt2_MC.png");
  
  TCanvas*cZlepPt1VsPt2_DATA=new TCanvas("cZlepPt1VsPt2_DATA","cZlepPt1VsPt2_DATA");
  TH2D*ZlepPt1VsPt2_DATA=(TH2D*)fDATA->Get("ZlepPt1VsPt2_Sig_eta2p1");
  ZlepPt1VsPt2_DATA->GetXaxis()->SetRangeUser(30,80);
  ZlepPt1VsPt2_DATA->GetYaxis()->SetRangeUser(0,80);
  ZlepPt1VsPt2_DATA->DrawNormalized("colz");
  cZlepPt1VsPt2_DATA->SetLogz();
  cZlepPt1VsPt2_DATA->SaveAs("ZlepPt1VsPt2_DATA.png");
  
  common_stuff::plotAndSaveHisto1D(fMC,"hpfMET_Z_Sig_eta2p1",fDATA,"hpfMET_Z_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);

  common_stuff::plotAndSaveHisto1D(fMC,"hpfMET_WlikePos_Sig_eta2p1",fDATA,"hpfMET_WlikePos_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  
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

  common_stuff::plotAndSaveHisto1D(fMC,"hpfMETphi_WlikePos_Sig_eta2p1",fDATA,"hpfMETphi_WlikePos_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hZ_pt_Sig_eta2p1",fDATA,"hZ_pt_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hZ_phi_Sig_eta2p1",fDATA,"hZ_phi_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hZ_mt_Sig_eta2p1",fDATA,"hZ_mt_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hWlikePos_pt_Sig_eta2p1",fDATA,"hWlikePos_pt_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hWlikePos_phi_Sig_eta2p1",fDATA,"hWlikePos_phi_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hWlikePos_mt_Sig_eta2p1",fDATA,"hWlikePos_mt_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hMupt_WlikePos_Sig_eta2p1",fDATA,"hMupt_WlikePos_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hMueta_WlikePos_Sig_eta2p1",fDATA,"hMueta_WlikePos_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hMuphi_WlikePos_Sig_eta2p1",fDATA,"hMuphi_WlikePos_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hMuMETpt_WlikePos_Sig_eta2p1",fDATA,"hMuMETpt_WlikePos_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hMuMETeta_WlikePos_Sig_eta2p1",fDATA,"hMuMETeta_WlikePos_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hMuMETphi_WlikePos_Sig_eta2p1",fDATA,"hMuMETphi_WlikePos_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hJetpt_WlikePos_Sig_eta2p1",fDATA,"hJetpt_WlikePos_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hJeteta_WlikePos_Sig_eta2p1",fDATA,"hJeteta_WlikePos_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hJetphi_WlikePos_Sig_eta2p1",fDATA,"hJetphi_WlikePos_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hu1_WlikePos_Sig_eta2p1",fDATA,"hu1_WlikePos_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fMC,"hu2_WlikePos_Sig_eta2p1",fDATA,"hu2_WlikePos_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  
  // TH2D *hZ_ptVsmt=new TH2D("hZ_ptVsmt","hZ_ptVsmt",150,50,200,100,0,25);
  return;
 
  // TCanvas*chrecoil1=new TCanvas("chrecoil1","chrecoil1");
  // TH1D*Zhrecoil1=(TH1D*)fMC->Get("hu1_WPos");
  // Zhrecoil1->Draw("");
  // chrecoil1->SaveAs("hZrecoil1.png");
 
  // TCanvas*chrecoil2=new TCanvas("chrecoil2","chrecoil2");
  // TH1D*Zhrecoil2=(TH1D*)fMC->Get("hu2_WPos");
  // Zhrecoil2->Draw("");
  // chrecoil2->SaveAs("hZrecoil2.png");
  
}

