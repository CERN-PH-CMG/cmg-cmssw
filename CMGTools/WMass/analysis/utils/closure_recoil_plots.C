#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TString.h"
#include <iostream>

using namespace std;

void syst_recoil_one(TString recstr="u2")
{
  TFile *fin[7];
  TH1D* hcentral,*hmadgraph,*hsyst[60];
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
      hcentral=(TH1D*)fin[i]->Get(Form("hWlikePos_%s_8_JetCut_pdf229800-0_eta0p9_91188", recstr.Data()));
      hcentral->Scale(1/hcentral->Integral());
      hmadgraph = (TH1D*)fmadgraph->Get(Form("hWlikePos_%s_8_JetCut_pdf229800-0_eta0p9_91188", recstr.Data()));
      hmadgraph->SetTitle("madgraph");
      hmadgraph->SetName("madgraph");
      hmadgraph->Scale(1/hmadgraph->Integral());
      hmadgraph->Divide(hcentral);
      hmadgraph->SetLineWidth(2);
      hmadgraph->SetLineColor(4);
    }else{
      for(int j=IniVar[i]; j<NVar[i]; j++){
        hsyst[nsyst]=(TH1D*)fin[i]->Get(Form("hWlikePos_%s_8_JetCut_pdf229800-0_RecoilCorrVar%d_eta0p9_91188", recstr.Data(), j));
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
  
  
  TH1D* hsyst_tot = (TH1D*)hcentral->Clone("hsyst_tot");
  hsyst_tot->SetName("hsyst_tot");
  hsyst_tot->SetTitle("hsyst_tot");
  for(int i=1;i<hsyst_tot->GetNbinsX()+1; i++){
    double error = hsyst_tot->GetBinError(i);
    for(int j=0; j<60; j++){
      error = sqrt(error*error+(1-hsyst[j]->GetBinContent(i))*(1-hsyst[j]->GetBinContent(i)));
    }
    hsyst_tot->SetBinError(i, error);
  }
  TCanvas *c_tot = new TCanvas("c_tot_"+recstr, "c_tot_"+recstr);
  c_tot->cd();
  hsyst_tot->SetFillColor(2);
  hsyst_tot->SetFillStyle(3002);
  hsyst_tot->GetYaxis()->SetRangeUser(0,2);
  hsyst_tot->Draw("E4");
  
  TLine *line = new TLine(-20,1,20,1);
  line->Draw("same");
  
  hmadgraph->Draw("histo same e ");
  
  
  TH1D* hsigmas = (TH1D*)hmadgraph->Clone("hnsigma");
  hsigmas->SetName("hsigmas");
  hsigmas->SetTitle("hsigmas");
  
  for(int i=1; i<hsigmas->GetNbinsX()+1; i++){
    double ratio = abs(hmadgraph->GetBinContent(i)-1)/hsyst_tot->GetBinError(i);
    hsigmas->SetBinContent(i, ratio);
  }
  
  TCanvas *c_sigmas = new TCanvas("c_sigmas_"+recstr, "c_sigmas_"+recstr);
  c_sigmas->cd();
  //hsigmas->GetYaxis()->SetRangeUser(0, 1);
  hsigmas->Draw("E4");
  
  hsigmas->Draw("histo");
  
  TFile* fout = new TFile(recstr+".root", "RECREATE");
  fout->cd();
  c->Write();
  c_tot->Write();
  c_sigmas->Write();
}

int closure_recoil_plots()
{
  syst_recoil_one("u1");
  syst_recoil_one("u2");
  return 0;
}

int main()
{
  closure_recoil_plots();
  return 0;
}
