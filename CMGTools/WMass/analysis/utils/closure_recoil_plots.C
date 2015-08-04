#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TString.h"
#include "TLegend.h"
#include <iostream>

using namespace std;

double xaxislimit = 17;
int fillstyle = 3001;

void syst_recoil_one(TString recstr="u2")
{
  const int nhists = 6;
  const int ntotsysts = 60;

  int IniVar[nhists] = {0,  9,  0, 0,  9,  0 };
  int NVar[nhists]   = {9, 18, 12, 9, 18, 12 };

  TFile* fcentral = new TFile(Form("0.root"));
  TH1D* hcentral=(TH1D*)fcentral->Get(Form("hWlikePos_%s_8_JetCut_pdf229800-0_eta0p9_91188", recstr.Data()));

  TFile* fmadgraph = new TFile(Form("madnocorr.root"));
  TH1D* hmadgraph = (TH1D*)fmadgraph->Get(Form("hWlikePos_%s_8_JetCut_pdf229800-0_eta0p9_91188", recstr.Data()));
  hmadgraph->SetTitle("Madgraph");
  hmadgraph->SetName("madgraph");

  TH1D* hcentral_noerr = (TH1D*)hcentral->Clone("hcentral_noerr");
  for(int i=1;i<hcentral_noerr->GetNbinsX()+1; i++){
    //hcentral_noerr->SetBinError(i, 0);
  }
  hcentral_noerr ->Scale(1/hcentral->Integral());

  hcentral ->Scale(1/hcentral->Integral());
  hcentral ->Divide(hcentral_noerr);

  hmadgraph->Scale(1/hmadgraph->Integral());
  hmadgraph->Divide(hcentral_noerr);


  TCanvas* c=new TCanvas("c_"+recstr, "c_"+recstr);
  c->cd();

  hcentral->GetYaxis()->SetRangeUser(0.8,1.2);
  hcentral->Draw();

  TFile* fin[nhists];
  TH1D* hsyst[ntotsysts];

  int nsyst=0;
  for(int i=0; i<6; i++){
    fin[i]=new TFile(Form("%d.root", i+1));
    for(int j=IniVar[i]; j<NVar[i]; j++){
      hsyst[nsyst]=(TH1D*)fin[i]->Get(Form("hWlikePos_%s_8_JetCut_pdf229800-0_RecoilCorrVar%d_eta0p9_91188", recstr.Data(), j));
      hsyst[nsyst]->SetName(Form("hWlikePos_%s_8_JetCut_pdf229800-0_RecoilCorrVar%d_eta0p9_91188", recstr.Data(), nsyst));
      hsyst[nsyst]->SetTitle(Form("hWlikePos_%s_8_JetCut_pdf229800-0_RecoilCorrVar%d_eta0p9_91188", recstr.Data(), nsyst));
      hsyst[nsyst]->Scale(1/hsyst[nsyst]->Integral());
      hsyst[nsyst]->Divide(hcentral_noerr);
      hsyst[nsyst]->SetLineColor(nsyst);
      hsyst[nsyst]->SetMarkerStyle(20);
      hsyst[nsyst]->SetMarkerSize(0.01);
      hsyst[nsyst]->SetMarkerColor(nsyst);
      hsyst[nsyst]->Draw("same");
      nsyst++;
    }
  }

  TH1D* hsystfit = (TH1D*)hcentral->Clone("hsystfit");
  for(int i=1;i<hsystfit->GetNbinsX()+1; i++){
    double error = 0;
    for(int j=0; j<ntotsysts; j++){
      error = sqrt(error*error+(hsyst[j]->GetBinContent(i)-1)*(hsyst[j]->GetBinContent(i)-1));
    }
    hsystfit->SetBinError(i, error);
  }

  TH1D* hstat = (TH1D*)hcentral->Clone("hstat");
  for(int i=1;i<hstat->GetNbinsX()+1; i++){
    double errcentral = hcentral ->GetBinError(i);
    double errmad     = hmadgraph->GetBinError(i);
    hstat->SetBinError(i, sqrt(errcentral*errcentral + errmad*errmad));
  }

  TH1D* herr = (TH1D*)hcentral->Clone("herr");
  herr->SetTitle("Pow2Mad closure");
  for(int i=1;i<herr->GetNbinsX()+1; i++){
    double errstat = hstat   ->GetBinError(i);
    double errfit  = hsystfit->GetBinError(i);
    herr->SetBinError(i, sqrt(errstat*errstat + errfit*errfit));
  }

  TCanvas *c_closure = new TCanvas("c_closure_"+recstr, "c_closure_"+recstr);
  c_closure->cd();

  herr->SetAxisRange(-xaxislimit, +xaxislimit, "X");
  herr->SetAxisRange(0.8, 1.2, "Y");
  herr->SetFillColor(kCyan-2);
  herr->SetFillStyle(fillstyle);
  herr->Draw("E3");
  hstat->SetFillColor(kGreen);
  hstat->SetFillStyle(fillstyle);
  hstat->Draw("same E3");
  hcentral->SetFillColor(kRed);
  hcentral->SetFillStyle(fillstyle);
  hcentral->Draw("same E3");

  TLine *line = new TLine(-xaxislimit,1,xaxislimit,1);
  line->Draw("same");


  hmadgraph->SetLineWidth(2);
  hmadgraph->SetLineColor(4);
  hmadgraph->Draw("histo same");


  TLegend *leg = new TLegend(0.1,0.7,0.48,0.9);
  // leg->SetHeader("The Legend Title");
  leg->AddEntry(hmadgraph,"madgraph / (powheg morphed to madgraph)","l");
  leg->AddEntry(hcentral,"powheg stat unc","f");
  leg->AddEntry(hstat,"madgraph stat unc","f");
  leg->AddEntry(herr,"propagation of recoil fit stat unc","f");
  leg->Draw();

   TH1D* hsigmas = (TH1D*)hmadgraph->Clone("hsigmas");
  hsigmas->SetName("hsigmas");
  hsigmas->SetTitle("hsigmas");

  TH1D* hpull = new TH1D("hpull", "hpull", 50, -4, 4);

  for(int i=1; i<hsigmas->GetNbinsX()+1; i++){
    double ratio = (hmadgraph->GetBinContent(i)-1)/herr->GetBinError(i);
    hsigmas->SetBinContent(i, ratio);
    hpull->Fill(ratio);
  }

  TCanvas *c_sigmas = new TCanvas("c_sigmas_"+recstr, "c_sigmas_"+recstr);
  c_sigmas->cd();

  hsigmas->Draw("histo");


  TCanvas *c_pull = new TCanvas("c_pull_"+recstr, "c_pull_"+recstr);
  c_pull->cd();

  hpull->Draw("histo");


  TFile* fout = new TFile(recstr+".root", "RECREATE");
  fout->cd();
  c->Write();
  c_closure->Write();
  c_sigmas->Write();
  c_pull->Write();
}

int closure_recoil_plots()
{
  // syst_recoil_one("u1");
  syst_recoil_one("u2");
  return 0;
}

int main()
{
  closure_recoil_plots();
  return 0;
}
