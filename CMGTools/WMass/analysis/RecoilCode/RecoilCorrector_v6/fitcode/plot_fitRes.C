void plot_fitRes(){

  gStyle->SetOptStat(000000000000);
  
  TFile *fmc = new TFile("recoilfit_genZ_inc.root");
  TFile *fdata = new TFile("recoilfit_DATA_inc.root");

  TF1 *mcPFu1Mean_0 = (TF1*) fmc->Get("PFu1Mean_0");
  mcPFu1Mean_0->SetLineColor(2);
  TF1 *mcPFu2Mean_0 = (TF1*) fmc->Get("PFu2Mean_0");
  mcPFu2Mean_0->SetLineColor(2);
  TF1 *mcPFu1MeanRMS_0 = (TF1*) fmc->Get("PFu1MeanRMS_0");
  mcPFu1MeanRMS_0->SetLineColor(2);
  TF1 *mcPFu1RMS1_0 = (TF1*) fmc->Get("PFu1RMS1_0");
  mcPFu1RMS1_0->SetLineColor(2);
  TF1 *mcPFu1RMS2_0 = (TF1*) fmc->Get("PFu1RMS2_0");
  mcPFu1RMS2_0->SetLineColor(2);
  TF1 *mcPFu2MeanRMS_0 = (TF1*) fmc->Get("PFu2MeanRMS_0");
  mcPFu2MeanRMS_0->SetLineColor(2);
  TF1 *mcPFu2RMS1_0 = (TF1*) fmc->Get("PFu2RMS1_0");
  mcPFu2RMS1_0->SetLineColor(2);
  TF1 *mcPFu2RMS2_0 = (TF1*) fmc->Get("PFu2RMS2_0");
  mcPFu2RMS2_0->SetLineColor(2);
  
  TF1 *dataPFu1Mean_0 = (TF1*) fdata->Get("PFu1Mean_0");
  TF1 *dataPFu2Mean_0 = (TF1*) fdata->Get("PFu2Mean_0");
  TF1 *dataPFu1MeanRMS_0 = (TF1*) fdata->Get("PFu1MeanRMS_0");
  TF1 *dataPFu1RMS1_0 = (TF1*) fdata->Get("PFu1RMS1_0");
  TF1 *dataPFu1RMS2_0 = (TF1*) fdata->Get("PFu1RMS2_0");
  TF1 *dataPFu2MeanRMS_0 = (TF1*) fdata->Get("PFu2MeanRMS_0");
  TF1 *dataPFu2RMS1_0 = (TF1*) fdata->Get("PFu2RMS1_0");
  TF1 *dataPFu2RMS2_0 = (TF1*) fdata->Get("PFu2RMS2_0");
  
  TCanvas *cPFu1Mean_0 = new TCanvas("cPFu1Mean_0","cPFu1Mean_0");
  TH2D *hPFu1Mean_0 = new TH2D("hPFu1Mean_0","hPFu1Mean_0",50,0,50,70,-60,10);
  hPFu1Mean_0->Draw();
  dataPFu1Mean_0->SetRange(0,50);
  dataPFu1Mean_0->Draw("same");
  mcPFu1Mean_0->SetRange(0,50);
  mcPFu1Mean_0->Draw("same");

  TCanvas *cPFu1MeanRMS_0 = new TCanvas("cPFu1MeanRMS_0","cPFu1MeanRMS_0");
  TH2D *hPFu1MeanRMS_0 = new TH2D("hPFu1MeanRMS_0","hPFu1MeanRMS_0",50,0,50,70,-5,50);
  hPFu1MeanRMS_0->Draw();
  dataPFu1MeanRMS_0->SetRange(0,50);
  dataPFu1MeanRMS_0->Draw("same");
  mcPFu1MeanRMS_0->SetRange(0,50);
  mcPFu1MeanRMS_0->Draw("same");

  TCanvas *cPFu1RMS1_0 = new TCanvas("cPFu1RMS1_0","cPFu1RMS1_0");
  TH2D *hPFu1RMS1_0 = new TH2D("hPFu1RMS1_0","hPFu1RMS1_0",50,0,50,70,-5,50);
  hPFu1RMS1_0->Draw();
  dataPFu1RMS1_0->SetRange(0,50);
  dataPFu1RMS1_0->Draw("same");
  mcPFu1RMS1_0->SetRange(0,50);
  mcPFu1RMS1_0->Draw("same");

  TCanvas *cPFu1RMS2_0 = new TCanvas("cPFu1RMS2_0","cPFu1RMS2_0");
  TH2D *hPFu1RMS2_0 = new TH2D("hPFu1RMS2_0","hPFu1RMS2_0",50,0,50,70,-5,50);
  hPFu1RMS2_0->Draw();
  dataPFu1RMS2_0->SetRange(0,50);
  dataPFu1RMS2_0->Draw("same");
  mcPFu1RMS2_0->SetRange(0,50);
  mcPFu1RMS2_0->Draw("same");

  TCanvas *cPFu2Mean_0 = new TCanvas("cPFu2Mean_0","cPFu2Mean_0");
  TH2D *hPFu2Mean_0 = new TH2D("hPFu2Mean_0","hPFu2Mean_0",50,0,50,70,-5,50);
  hPFu2Mean_0->Draw();
  dataPFu2Mean_0->SetRange(0,50);
  dataPFu2Mean_0->Draw("same");
  mcPFu2Mean_0->SetRange(0,50);
  mcPFu2Mean_0->Draw("same");

  TCanvas *cPFu2MeanRMS_0 = new TCanvas("cPFu2MeanRMS_0","cPFu2MeanRMS_0");
  TH2D *hPFu2MeanRMS_0 = new TH2D("hPFu2MeanRMS_0","hPFu2MeanRMS_0",50,0,50,70,-5,50);
  hPFu2MeanRMS_0->Draw();
  dataPFu2MeanRMS_0->SetRange(0,50);
  dataPFu2MeanRMS_0->Draw("same");
  mcPFu2MeanRMS_0->SetRange(0,50);
  mcPFu2MeanRMS_0->Draw("same");

  TCanvas *cPFu2RMS1_0 = new TCanvas("cPFu2RMS1_0","cPFu2RMS1_0");
  TH2D *hPFu2RMS1_0 = new TH2D("hPFu2RMS1_0","hPFu2RMS1_0",50,0,50,70,-5,50);
  hPFu2RMS1_0->Draw();
  dataPFu2RMS1_0->SetRange(0,50);
  dataPFu2RMS1_0->Draw("same");
  mcPFu2RMS1_0->SetRange(0,50);
  mcPFu2RMS1_0->Draw("same");

  TCanvas *cPFu2RMS2_0 = new TCanvas("cPFu2RMS2_0","cPFu2RMS2_0");
  TH2D *hPFu2RMS2_0 = new TH2D("hPFu2RMS2_0","hPFu2RMS2_0",50,0,50,70,-5,50);
  hPFu2RMS2_0->Draw();
  dataPFu2RMS2_0->SetRange(0,50);
  dataPFu2RMS2_0->Draw("same");
  mcPFu2RMS2_0->SetRange(0,50);
  mcPFu2RMS2_0->Draw("same");

}