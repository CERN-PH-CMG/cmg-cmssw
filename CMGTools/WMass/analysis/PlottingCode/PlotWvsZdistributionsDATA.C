#include "../includes/common.h"
#include "../AnalysisCode/common_stuff.C"

void PlotWvsZdistributionsDATA(TString folderZ="",TString folderW=""){

  cout << "PlotWZdistributionsDATA.C working dir: " << gSystem->WorkingDirectory() << endl;


  TFile *fZ=new TFile(Form("%s/ZanalysisOnDATA.root",folderZ.Data()));
  TFile *fW=new TFile(Form("%s/WanalysisOnDATA.root",folderW.Data()));
  fZ->Print();
  fW->Print();
  
  // TCanvas*cZlepDeltaPt=new TCanvas("cZlepDeltaPt","cZlepDeltaPt");
  // TH1D*ZlepDeltaPt=(TH1D*)fZ->Get("ZlepDeltaPt");
  // ZlepDeltaPt->Draw();
  // cZlepDeltaPt->SaveAs("ZlepDeltaPt.png");
  
  // TCanvas*cZlepPt1VsPt2=new TCanvas("cZlepPt1VsPt2","cZlepPt1VsPt2");
  // TH2D*ZlepPt1VsPt2=(TH2D*)fZ->Get("ZlepPt1VsPt2");
  // ZlepPt1VsPt2->GetXaxis()->SetRangeUser(30,80);
  // ZlepPt1VsPt2->GetYaxis()->SetRangeUser(0,80);
  // ZlepPt1VsPt2->Draw("colz");
  // cZlepPt1VsPt2->SetLogz();
  // cZlepPt1VsPt2->SaveAs("ZlepPt1VsPt2.png");

  // common_stuff::plotAndSaveHisto1D(fZ,"hpfMET_Z",fW,"hpfMET_WPos", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  common_stuff::plotAndSaveHisto1D(fZ,"hpfMET_WlikePos_Sig_eta2p1",fW,"hpfMET_WPos_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
    
  common_stuff::plotAndSaveHisto1D(fZ,"hpfMETphi_Z_Sig_eta2p1",fW,"hpfMETphi_WPos_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);

  common_stuff::plotAndSaveHisto1D(fZ,"hpfMETphi_Z_Sig_eta2p1",fW,"hpfMETphi_WPos_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
    
  TCanvas*chWpt=new TCanvas("chWpt","chWpt");
  TH1D*hWlikePos_pt=(TH1D*)fZ->Get("hWlikePos_pt_Sig_eta2p1");
  hWlikePos_pt->SetLineColor(2);
  hWlikePos_pt->Scale(1/hWlikePos_pt->Integral(0,hWlikePos_pt->GetXaxis()->FindBin(20)));
  TH1D*WhWpt=(TH1D*)fW->Get("hWPos_pt_Sig_eta2p1");
  WhWpt->Scale(1/WhWpt->Integral(0,WhWpt->GetXaxis()->FindBin(20)));
  WhWpt->GetXaxis()->SetRangeUser(0,19.999);
  WhWpt->GetYaxis()->SetRangeUser(0,WhWpt->GetMaximum()*1.2);
  // WhWpt->GetXaxis()->SetRangeUser(0,30);
  WhWpt->Draw("");
  hWlikePos_pt->Draw("same");
  chWpt->SaveAs("hWVsWlikePos_pt.png");
    
  common_stuff::plotAndSaveHisto1D(fZ,"hWlikePos_phi_Sig_eta2p1",fW,"hWPos_phi_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  
  
  // TCanvas*chWmt=new TCanvas("chWmt","chWmt");
  // TH1D*hWlikePos_mt=(TH1D*)fZ->Get("hWlikePos_mt");
  // hWlikePos_mt->SetLineColor(2);
  // TH1D*WhWmt=(TH1D*)fW->Get("hWPos_mt");
  // WhWmt->DrawNormalized("");
  // hWlikePos_mt->DrawNormalized("same");
  // chWmt->SaveAs("hWVsWlikePos_mt.png");

  common_stuff::plotAndSaveHisto1D(fZ,"hWlikePos_mt_Sig_eta2p1",fW,"hWPos_mt_Sig_eta2p1", /*logxyz*/ 0,1,0, /*normalized*/ 1);
  
  TCanvas*chmuWpt=new TCanvas("chmuWpt","chmuWpt");
  chmuWpt->SetLogy();
  TH1D*ZhmuWpt=(TH1D*)fZ->Get("hMupt_WlikePos_Sig_eta2p1");
  ZhmuWpt->SetLineColor(2);
  TH1D*hmuWpt=(TH1D*)fW->Get("hMupt_WPos_Sig_eta2p1");
  hmuWpt->GetXaxis()->SetRangeUser(25,100);
  hmuWpt->DrawNormalized("");
  ZhmuWpt->DrawNormalized("same");
  chmuWpt->SaveAs("hMupt_WvsWPos.png");
  
  TCanvas*chmuWeta=new TCanvas("chmuWeta","chmuWeta");
  chmuWeta->SetLogy();
  TH1D*ZhmuWeta=(TH1D*)fZ->Get("hMueta_WlikePos_Sig_eta2p1");
  ZhmuWeta->SetLineColor(2);
  TH1D*hmuWeta=(TH1D*)fW->Get("hMueta_WPos_Sig_eta2p1");
  hmuWeta->GetXaxis()->SetRangeUser(25,100);
  hmuWeta->DrawNormalized("");
  ZhmuWeta->DrawNormalized("same");
  chmuWeta->SaveAs("hMueta_WvsWPos_Sig.png");
  
  TCanvas*chmuWphi=new TCanvas("chmuWphi","chmuWphi");
  chmuWphi->SetLogy();
  TH1D*ZhmuWphi=(TH1D*)fZ->Get("hMuphi_WlikePos_Sig_eta2p1");
  ZhmuWphi->SetLineColor(2);
  TH1D*hmuWphi=(TH1D*)fW->Get("hMuphi_WPos_Sig_eta2p1");
  hmuWphi->GetXaxis()->SetRangeUser(25,100);
  hmuWphi->DrawNormalized("");
  ZhmuWphi->DrawNormalized("same");
  chmuWphi->SaveAs("hMuphi_WvsWPos_Sig.png");
    
  // TCanvas*chmuMETpt=new TCanvas("chmuMETpt","chmuMETpt");
  // chmuMETpt->SetLogy();
  // TH1D*ZhmuWpt=(TH1D*)fZ->Get("hMupt_WlikePos");
  // ZhmuWpt->SetLineColor(2);
  // TH1D*hmuMETpt=(TH1D*)fZ->Get("hMuMETpt_WlikePos");
  // hmuMETpt->Scale(1/hmuMETpt->Integral(hmuMETpt->GetXaxis()->FindBin(30*91.1876/80.385),-1));
  // if(hmuMETpt->GetMaximum() > ZhmuWpt->GetMaximum()){
  // hmuMETpt->Draw("");
  // ZhmuWpt->DrawNormalized("same");
  // }else{
  // ZhmuWpt->DrawNormalized("");
  // hmuMETpt->Draw("same");
  // }
  // chmuMETpt->SaveAs("hMuVsMuMET_pt.png");
  
  // TCanvas*chmuMETeta=new TCanvas("chmuMETeta","chmuMETeta");
  // TH1D*ZhmuWeta=(TH1D*)fZ->Get("hMueta_WlikePos");
  // ZhmuWeta->SetLineColor(2);
  // TH1D*hmuMETeta=(TH1D*)fZ->Get("hMuMETeta_WlikePos");
  // ZhmuWeta->GetYaxis()->SetRangeUser(0,ZhmuWeta->GetMaximum()*1.1);
  // ZhmuWeta->DrawNormalized("");
  // hmuMETeta->DrawNormalized("same");
  // chmuMETeta->SaveAs("hMuVsMuMET_eta.png");
  
  // TCanvas*chmuMETphi=new TCanvas("chmuMETphi","chmuMETphi");
  // TH1D*ZhmuWphi=(TH1D*)fZ->Get("hMuphi_WlikePos");
  // ZhmuWphi->SetLineColor(2);
  // TH1D*hmuMETphi=(TH1D*)fZ->Get("hMuMETphi_WlikePos");
  // ZhmuWphi->GetYaxis()->SetRangeUser(0,ZhmuWphi->GetMaximum()*1.1);
  // ZhmuWphi->DrawNormalized("");
  // hmuMETphi->DrawNormalized("same");
  // chmuMETphi->SaveAs("hMuVsMuMET_phi.png");
  
  TCanvas*chjetpt=new TCanvas("chjetpt","chjetpt");
  TH1D*Zhjetpt=(TH1D*)fZ->Get("hJetpt_WlikePos_Sig_eta2p1");
  Zhjetpt->SetLineColor(2);
  TH1D*Whjetpt=(TH1D*)fW->Get("hJetpt_WPos_Sig_eta2p1");
  Whjetpt->GetXaxis()->SetRangeUser(5,35);
  Whjetpt->DrawNormalized("");
  Zhjetpt->DrawNormalized("same");
  chjetpt->SaveAs("hjetpt_Sig.png");
 
  TCanvas*chjeteta=new TCanvas("chjeteta","chjeteta");
  TH1D*Zhjeteta=(TH1D*)fZ->Get("hJeteta_WlikePos_Sig_eta2p1");
  Zhjeteta->SetLineColor(2);
  TH1D*Whjeteta=(TH1D*)fW->Get("hJeteta_WPos_Sig_eta2p1");
  Whjeteta->GetXaxis()->SetRangeUser(5,35);
  Whjeteta->DrawNormalized("");
  Zhjeteta->DrawNormalized("same");
  chjeteta->SaveAs("hjeteta_Sig.png");
 
  TCanvas*chjetphi=new TCanvas("chjetphi","chjetphi");
  TH1D*Zhjetphi=(TH1D*)fZ->Get("hJetphi_WlikePos_Sig_eta2p1");
  Zhjetphi->SetLineColor(2);
  TH1D*Whjetphi=(TH1D*)fW->Get("hJetphi_WPos_Sig_eta2p1");
  Whjetphi->GetXaxis()->SetRangeUser(5,35);
  Whjetphi->GetYaxis()->SetRangeUser(0,Whjetphi->GetMaximum()*1.2);
  Whjetphi->DrawNormalized("");
  Zhjetphi->DrawNormalized("same");
  chjetphi->SaveAs("hjetphi_Sig.png");
 
  // TCanvas*chrecoil1=new TCanvas("chrecoil1","chrecoil1");
  // TH1D*Zhrecoil1=(TH1D*)fZ->Get("hu1_WPos");
  // Zhrecoil1->Draw("");
  // chrecoil1->SaveAs("hZrecoil1.png");
 
  // TCanvas*chrecoil2=new TCanvas("chrecoil2","chrecoil2");
  // TH1D*Zhrecoil2=(TH1D*)fZ->Get("hu2_WPos");
  // Zhrecoil2->Draw("");
  // chrecoil2->SaveAs("hZrecoil2.png");
 
 
  
}
