#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TString.h"
#include <iostream>

using namespace std;

void syst_recoil_one(TString recstr)
{
  TFile *fin[7];
  TH2D* hcentral, *hmadgraph, *hsyst[60];
  int IniVar[7] = {0, 0,  9,  0, 0,  9,  0 };
  int NVar[7]   = {1, 9, 18, 12, 9, 18, 12 };
  int nsyst=0;
  TCanvas* c=new TCanvas("c_"+recstr, "c_"+recstr);
  c->cd();
  TFile *fmadgraph =new TFile(Form("madnocorr.root"));
  for(int i=0;i<7;i++){
    fin[i]=new TFile(Form("%d.root",i));
    // fin[i]->Print();
    if(i==0){
      hcentral=(TH2D*)fin[i]->Get(Form("hWlikePos_%s_8_JetCut_pdf229800-0_eta0p9_91188", recstr.Data()));
      hcentral->Scale(1/hcentral->Integral());
      hmadgraph = (TH2D*)fmadgraph->Get(Form("hWlikePos_%s_8_JetCut_pdf229800-0_eta0p9_91188", recstr.Data()));
      hmadgraph->SetTitle("madgraph");
      hmadgraph->SetName("madgraph");
      hmadgraph->Scale(1/hmadgraph->Integral());
      hmadgraph->Divide(hcentral);
      hmadgraph->SetLineWidth(2);
      hmadgraph->SetLineColor(4);
    }else{
      for(int j=IniVar[i]; j<NVar[i]; j++){
        hsyst[nsyst]=(TH2D*)fin[i]->Get(Form("hWlikePos_%s_8_JetCut_pdf229800-0_RecoilCorrVar%d_eta0p9_91188", recstr.Data(), j));
        hsyst[nsyst]->SetName(Form("hWlikePos_%s_8_JetCut_pdf229800-0_RecoilCorrVar%d_eta0p9_91188", recstr.Data(), nsyst));
        hsyst[nsyst]->SetTitle(Form("hWlikePos_%s_8_JetCut_pdf229800-0_RecoilCorrVar%d_eta0p9_91188", recstr.Data(), nsyst));
        hsyst[nsyst]->Scale(1/hsyst[nsyst]->Integral());
        hsyst[nsyst]->Divide(hcentral);
        hsyst[nsyst]->SetLineColor(nsyst);
        hsyst[nsyst]->SetMarkerStyle(20);
        hsyst[nsyst]->SetMarkerSize(0.01);
        hsyst[nsyst]->SetMarkerColor(nsyst);
        // hsyst[nsyst]->Print();
        hsyst[nsyst]->Draw(nsyst==0?"":"same");
        nsyst++;
      }
    }
  }
  hcentral->Divide(hcentral);
  hcentral->Draw("same");
  
  TCanvas *c_mad = new TCanvas("c_mad_"+recstr, "c_mad_"+recstr);
  c_mad->cd();
  
  for(int i=1; i<hmadgraph->GetNbinsX()+1; i++){
    for(int j=1; j<hmadgraph->GetNbinsY()+1; j++){
      hmadgraph->SetBinContent(i, j, hmadgraph->GetBinContent(i, j)-1);
    }
  }
  hmadgraph->GetYaxis()->SetRangeUser(-17, +17);
  hmadgraph->Draw("colz");
  
  
  TH2D* hsyst_tot = (TH2D*)hcentral->Clone("hsyst_tot");
  hsyst_tot->SetName("hsyst_tot");
  hsyst_tot->SetTitle("hsyst_tot");
  hsyst_tot->GetYaxis()->SetRangeUser(-17, +17);
  
  for(int i=1; i<hsyst_tot->GetNbinsX()+1; i++){
    for(int j=1; j<hsyst_tot->GetNbinsY()+1; j++){
      double error = hsyst_tot->GetBinError(i, j);
      for(int k=0; k<60; k++){
        error = sqrt(error*error+(1-hsyst[k]->GetBinContent(i, j))*(1-hsyst[k]->GetBinContent(i, j)));
      }
      hsyst_tot->SetBinError(i, j, error);
    }
  }
  
  TCanvas *c_tot = new TCanvas("c_tot_"+recstr, "c_tot_"+recstr);
  c_tot->cd();
  
  for(int i=1; i<hsyst_tot->GetNbinsX()+1; i++){
    for(int j=1; j<hsyst_tot->GetNbinsY()+1; j++){
      hsyst_tot->SetBinContent(i, j, hsyst_tot->GetBinError(i, j));
    }
  }
  hsyst_tot->Draw("colz");
  
  TH2D* hsigmas = (TH2D*)hmadgraph->Clone("hnsigma");
  hsigmas->SetName("hsigmas");
  hsigmas->SetTitle("hsigmas");
  hsigmas->GetYaxis()->SetRangeUser(-17, +17);
  
  for(int i=1; i<hsigmas->GetNbinsX()+1; i++){
    for(int j=1; j<hsigmas->GetNbinsY()+1; j++){
      double ratio = abs(hmadgraph->GetBinContent(i, j)-1)/hsyst_tot->GetBinContent(i, j);
      hsigmas->SetBinContent(i, j, ratio);
    }
  }
  
  TCanvas *c_sigmas = new TCanvas("c_sigmas_"+recstr, "c_sigmas_"+recstr);
  c_sigmas->cd();
  hsigmas->GetZaxis()->SetRangeUser(0,2);
  hsigmas->Draw("colz");

  TH1D* hpull = new TH1D("hpull", "hpull", 200, 0, 5);
  
  for(int i=1; i<hsigmas->GetNbinsX()+1; i++) {
    for(int j=1; j<hsigmas->GetNbinsY()+1; j++) {
      double y = (double)j/hsigmas->GetNbinsY() * 40 - 20 + 20/hsigmas->GetNbinsY();
      if (abs(y) <16.5)
        hpull->Fill(hsigmas->GetBinContent(i,j));
    }
  }
  
  TCanvas *c_pull = new TCanvas("c_pull_"+recstr, "c_pull_"+recstr);
  c_pull->cd();
  hpull->Draw();
  
  TFile* fout = new TFile(recstr+".root", "RECREATE");
  fout->cd();
  c_mad->Write();
  c_tot->Write();
  c_sigmas->Write();
  c_pull->Write();
}

int closure_recoil_plots2d()
{
  syst_recoil_one("u1vsZpt");
  syst_recoil_one("u2vsZpt");
  return 0;
} 

int main()
{
  closure_recoil_plots2d();
  return 0;
}
