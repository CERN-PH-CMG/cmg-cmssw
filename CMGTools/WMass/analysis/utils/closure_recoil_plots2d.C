#include <TStyle.h>
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TString.h"
#include "TGraph.h"
#include "TLegend.h"
#include <iostream>

using namespace std;

double xaxislimit = 17;
int fillstyle = 3001;

void syst_recoil_one(TString recstr="u1")
{
  gStyle->SetOptFit(111);

  const int nhists = 6;
  const int ntotsysts = 60;

  int IniVar[nhists] = {0,  9,  0, 0,  9,  0 };
  int NVar[nhists]   = {9, 18, 12, 9, 18, 12 };

  TFile* fcentral = new TFile(Form("0.root"));
  TH2D* hcentral2 =(TH2D*)fcentral->Get(Form("hWlikePos_%svsZpt_8_JetCut_pdf229800-0_eta0p9_91188", recstr.Data()));
  hcentral2->SetTitle(Form("hWlikePos_%svsZpt_8_JetCut_pdf229800-0_eta0p9_91188_pow2madcentral", recstr.Data()));

  TFile* fmadgraph = new TFile(Form("madnocorr.root"));
  TH2D* hmadgraph2 = (TH2D*)fmadgraph->Get(Form("hWlikePos_%svsZpt_8_JetCut_pdf229800-0_eta0p9_91188", recstr.Data()));
  hmadgraph2->SetTitle(Form("hWlikePos_%svsZpt_8_JetCut_pdf229800-0_eta0p9_91188_madgraph", recstr.Data()));

  TH2D*h2dratio = (TH2D*) hcentral2->Clone("h2dratio");
  TH2D*h2dratio_toterr = (TH2D*) hcentral2->Clone("h2dratio_toterr");
  TH2D*h2dratio_sigmas = (TH2D*) hcentral2->Clone("h2dratio_sigmas");

  TFile* fin[nhists];
  TH2D* hsyst2[ntotsysts];

  int nsyst=0;
  for(int i=0; i<nhists; i++){
    fin[i]=new TFile(Form("%d.root", i+1));
    for(int j=IniVar[i]; j<NVar[i]; j++){
      hsyst2[nsyst]=(TH2D*)fin[i]->Get(Form("hWlikePos_%svsZpt_8_JetCut_pdf229800-0_RecoilCorrVar%d_eta0p9_91188", recstr.Data(), j));
      hsyst2[nsyst]->SetName(Form("hWlikePos_%svsZpt_8_JetCut_pdf229800-0_RecoilCorrVar%d_eta0p9_91188", recstr.Data(), nsyst));
      hsyst2[nsyst]->SetTitle(Form("hWlikePos_%svsZpt_8_JetCut_pdf229800-0_RecoilCorrVar%d_eta0p9_91188", recstr.Data(), nsyst));
      nsyst++;
    }
  }

  int Zptbins = hcentral2->GetNbinsX()+2;
  double means[Zptbins];
  double rmss[Zptbins];

  TFile* fout = new TFile(recstr+"vsZpt.root", "RECREATE");
  hcentral2->Write();
  hmadgraph2->Write();

  for (int bin = 1; bin < hcentral2->GetNbinsX()+2; ++bin){

    TString binstr = Form("%d", bin);

    TH1D* hcentral = hcentral2->ProjectionY("hcentral_bin" + binstr, bin, bin+1, "e");

    TCanvas* cp=new TCanvas("cp_"+recstr+"_bin"+binstr, "cp_"+recstr+"_bin"+binstr);
    cp->cd();
    hcentral->Draw();

    TH1D* hmadgraph = hmadgraph2->ProjectionY("hmadgraph_bin" + binstr, bin, bin+1, "e");
    hmadgraph->SetTitle("Madgraph_bin" + binstr);

    TH1D* hcentral_noerr = (TH1D*)hcentral->Clone("hcentral_noerr_bin" + binstr);
    for(int i=1;i<hcentral_noerr->GetNbinsX()+1; i++){
      hcentral_noerr->SetBinError(i, 0);
    }
    hcentral_noerr ->Scale(1/hcentral->Integral());

    hcentral ->Scale(1/hcentral->Integral());
    hcentral ->Divide(hcentral_noerr);

    hmadgraph->Scale(1/hmadgraph->Integral());
    hmadgraph->Divide(hcentral_noerr);


    TCanvas* c=new TCanvas("c_"+recstr+"_bin"+binstr, "c_"+recstr+"_bin"+binstr);
    c->cd();

    hcentral->GetYaxis()->SetRangeUser(0.8,1.2);
    hcentral->Draw();

    TH1D* hsyst[ntotsysts];

    for(int nsyst=0; nsyst<ntotsysts; nsyst++){
      hsyst[nsyst]=hsyst2[nsyst]->ProjectionY("", bin, bin+1, "e");
      hsyst[nsyst]->SetName(Form("hWlikePos_%svsZpt_8_JetCut_pdf229800-0_RecoilCorrVar%d_eta0p9_91188_bin%d", recstr.Data(), nsyst, bin));
      hsyst[nsyst]->SetTitle(Form("hWlikePos_%svsZpt_8_JetCut_pdf229800-0_RecoilCorrVar%d_eta0p9_91188_bin%d", recstr.Data(), nsyst, bin));
      hsyst[nsyst]->Scale(1/hsyst[nsyst]->Integral());
      hsyst[nsyst]->Divide(hcentral_noerr);
      hsyst[nsyst]->SetLineColor(nsyst);
      hsyst[nsyst]->SetMarkerStyle(20);
      hsyst[nsyst]->SetMarkerSize(0.01);
      hsyst[nsyst]->SetMarkerColor(nsyst);
      hsyst[nsyst]->Draw("same");
    }

    TH1D* hsystfit = (TH1D*)hcentral->Clone("hsystfit_bin" + binstr);
    for(int i=1;i<hsystfit->GetNbinsX()+1; i++){
      double error = 0;
      for(int j=0; j<ntotsysts; j++){
        error = sqrt(error*error+(hsyst[j]->GetBinContent(i)-1)*(hsyst[j]->GetBinContent(i)-1));
      }
      hsystfit->SetBinError(i, error);
    }

    TH1D* hstat = (TH1D*)hcentral->Clone("hstat_bin" + binstr);
    for(int i=1;i<hstat->GetNbinsX()+1; i++){
      double errcentral = hcentral ->GetBinError(i);
      double errmad     = hmadgraph->GetBinError(i);
      hstat->SetBinError(i, sqrt(errcentral*errcentral + errmad*errmad));
    }

    TH1D* herr = (TH1D*)hcentral->Clone("herr_bin" + binstr);
    herr->SetTitle("Pow2Mad closure bin" + binstr + "; " +recstr+ "; N");
    herr->SetStats(kFALSE);
    for(int i=1;i<herr->GetNbinsX()+1; i++){
      double errstat = hstat   ->GetBinError(i);
      double errfit  = hsystfit->GetBinError(i);
      herr->SetBinError(i, sqrt(errstat*errstat + errfit*errfit));
    }
    

    TCanvas *c_closure = new TCanvas("c_closure_"+recstr+"_bin"+binstr, "c_closure_"+recstr+"_bin"+binstr);
    c_closure->cd();

    herr->SetAxisRange(-xaxislimit, +xaxislimit, "X");
    herr->SetAxisRange(0.8, 1.2, "Y");
    herr->SetFillColor(kCyan-2);
    herr->SetFillStyle(fillstyle);
    herr->Draw("E2");
    hstat->SetFillColor(kGreen);
    hstat->SetFillStyle(fillstyle);
    hstat->Draw("same E2");
    hcentral->SetFillColor(kRed);
    hcentral->SetFillStyle(fillstyle);
    hcentral->Draw("same E2");

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

    TH1D* hsigmas = (TH1D*)hmadgraph->Clone("hsigmas_bin" + binstr);
    hsigmas->SetName("hsigmas_bin" + binstr);
    hsigmas->SetTitle("hsigmas_bin" + binstr + "; " + recstr + "; N");

    TH1D* hpull = new TH1D("hpull_bin" + binstr, "bin" + binstr + "; pull value; N", 40, -6, 6);
    hpull->StatOverflows(kTRUE);

    for(int i=1; i<hsigmas->GetNbinsX()+1; i++){
      double ratio = (hmadgraph->GetBinContent(i)-1)/herr->GetBinError(i);
      hsigmas->SetBinContent(i, ratio);
      hpull->Fill(ratio);
      h2dratio->SetBinContent(bin,i,hmadgraph->GetBinContent(i));
      h2dratio_toterr->SetBinContent(bin,i,herr->GetBinError(i));
      h2dratio_sigmas->SetBinContent(bin,i,ratio);
    }

    means[bin] = hpull->GetMean();
    rmss[bin] = hpull->GetRMS();

    TCanvas *c_sigmas = new TCanvas("c_sigmas_"+recstr+"_bin"+binstr, "c_sigmas_"+recstr+"_bin"+binstr);
    c_sigmas->cd();

    hsigmas->Draw("histo");


    TCanvas *c_pull = new TCanvas("c_pull_"+recstr+"_bin"+binstr, "c_pull_"+recstr+"_bin"+binstr);
    c_pull->cd();

    hpull->Fit("gaus", "L");
    
    fout->cd();
    c->Write();
    c_closure->Write();
    c_sigmas->Write();
    c_pull->Write();
    // c->SaveAs(".png");
    // c_closure->SaveAs(".png");
    // c_sigmas->SaveAs(".png");
    // c_pull->SaveAs(".png");

  }
  
  fout->cd();
  h2dratio->Write();
  h2dratio_toterr->Write();
  h2dratio_sigmas->Write();

  double numbers[Zptbins];
  for (int i=0; i<Zptbins; ++i) {
    numbers[i] = i;
  }

  TGraph* gmean =  new TGraph(Zptbins-2, numbers, means); // last 2 make no sense
  gmean->SetTitle("Mean "+recstr+"; Zpt; Mean");

  TCanvas *c_mean = new TCanvas("c_mean_"+recstr, "c_mean_"+recstr);
  c_mean->cd();

  gmean->SetMarkerColor(2);
  gmean->SetMarkerSize(1);
  gmean->SetMarkerStyle(22);

  gmean->Draw("ACP");

  TGraph* grms =  new TGraph(Zptbins-2, numbers, rmss);
  grms->SetTitle("RMS "+recstr+"; Zpt; RMS");

  TCanvas *c_rms = new TCanvas("c_rms_"+recstr, "c_rms_"+recstr);
  c_rms->cd();

  grms->SetMarkerColor(1);
  grms->SetMarkerSize(1);
  grms->SetMarkerStyle(20);

  grms->Draw("ACP");

  c_mean->Write();
  c_rms->Write();
  c_mean->SaveAs(".png");
  c_rms->SaveAs(".png");
  
  int nbindiv = 25;
  for (int i=0; i<Zptbins/nbindiv; ++i) {
    int bin_ini = (i+0)*nbindiv;
    int bin_fin = (i+1)*nbindiv;
    TString binstr = Form("%d-%d", bin_ini, bin_fin);

    TH1D* hcentral = hcentral2->ProjectionY("hcentral_bin" + binstr, bin_ini, bin_fin, "e");

    TCanvas* cp=new TCanvas("cp_"+recstr+"_bin"+binstr, "cp_"+recstr+"_bin"+binstr);
    cp->cd();
    hcentral->Draw();

    TH1D* hmadgraph = hmadgraph2->ProjectionY("hmadgraph_bin" + binstr, bin_ini, bin_fin, "e");
    hmadgraph->SetTitle("Madgraph_bin" + binstr);

    TH1D* hcentral_noerr = (TH1D*)hcentral->Clone("hcentral_noerr_bin" + binstr);
    for(int i=1;i<hcentral_noerr->GetNbinsX()+1; i++){
      hcentral_noerr->SetBinError(i, 0);
    }
    hcentral_noerr ->Scale(1/hcentral->Integral());

    hcentral ->Scale(1/hcentral->Integral());
    hcentral ->Divide(hcentral_noerr);

    hmadgraph->Scale(1/hmadgraph->Integral());
    hmadgraph->Divide(hcentral_noerr);


    TCanvas* c=new TCanvas("c_"+recstr+"_bin"+binstr, "c_"+recstr+"_bin"+binstr);
    c->cd();

    hcentral->GetYaxis()->SetRangeUser(0.8,1.2);
    hcentral->Draw();

    TH1D* hsyst[ntotsysts];

    for(int nsyst=0; nsyst<ntotsysts; nsyst++){
      hsyst[nsyst]=hsyst2[nsyst]->ProjectionY("", bin_ini, bin_fin, "e");
      hsyst[nsyst]->SetName(Form("hWlikePos_%svsZpt_8_JetCut_pdf229800-0_RecoilCorrVar%d_eta0p9_91188_bin%s", recstr.Data(), nsyst, binstr.Data()));
      hsyst[nsyst]->SetTitle(Form("hWlikePos_%svsZpt_8_JetCut_pdf229800-0_RecoilCorrVar%d_eta0p9_91188_bin%s", recstr.Data(), nsyst, binstr.Data()));
      hsyst[nsyst]->Scale(1/hsyst[nsyst]->Integral());
      hsyst[nsyst]->Divide(hcentral_noerr);
      hsyst[nsyst]->SetLineColor(nsyst);
      hsyst[nsyst]->SetMarkerStyle(20);
      hsyst[nsyst]->SetMarkerSize(0.01);
      hsyst[nsyst]->SetMarkerColor(nsyst);
      hsyst[nsyst]->Draw("same");
    }

    TH1D* hsystfit = (TH1D*)hcentral->Clone("hsystfit_bin" + binstr);
    for(int i=1;i<hsystfit->GetNbinsX()+1; i++){
      double error = 0;
      for(int j=0; j<ntotsysts; j++){
        error = sqrt(error*error+(hsyst[j]->GetBinContent(i)-1)*(hsyst[j]->GetBinContent(i)-1));
      }
      hsystfit->SetBinError(i, error);
    }

    TH1D* hstat = (TH1D*)hcentral->Clone("hstat_bin" + binstr);
    for(int i=1;i<hstat->GetNbinsX()+1; i++){
      double errcentral = hcentral ->GetBinError(i);
      double errmad     = hmadgraph->GetBinError(i);
      hstat->SetBinError(i, sqrt(errcentral*errcentral + errmad*errmad));
    }

    TH1D* herr = (TH1D*)hcentral->Clone("herr_bin" + binstr);
    herr->SetTitle("Pow2Mad closure bin" + binstr + "; " +recstr+ "; N");
    herr->SetStats(kFALSE);
    for(int i=1;i<herr->GetNbinsX()+1; i++){
      double errstat = hstat   ->GetBinError(i);
      double errfit  = hsystfit->GetBinError(i);
      herr->SetBinError(i, sqrt(errstat*errstat + errfit*errfit));
    }
    

    TCanvas *c_closure = new TCanvas("c_closure_"+recstr+"_bin"+binstr, "c_closure_"+recstr+"_bin"+binstr);
    c_closure->cd();

    herr->SetAxisRange(-xaxislimit, +xaxislimit, "X");
    herr->SetAxisRange(0.8, 1.2, "Y");
    herr->SetFillColor(kCyan-2);
    herr->SetFillStyle(fillstyle);
    herr->Draw("E2");
    hstat->SetFillColor(kGreen);
    hstat->SetFillStyle(fillstyle);
    hstat->Draw("same E2");
    hcentral->SetFillColor(kRed);
    hcentral->SetFillStyle(fillstyle);
    hcentral->Draw("same E2");

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

    TH1D* hsigmas = (TH1D*)hmadgraph->Clone("hsigmas_bin" + binstr);
    hsigmas->SetName("hsigmas_bin" + binstr);
    hsigmas->SetTitle("hsigmas_bin" + binstr + "; " + recstr + "; N");

    TH1D* hpull = new TH1D("hpull_bin" + binstr, "bin" + binstr + "; pull value; N", 40, -6, 6);
    hpull->StatOverflows(kTRUE);

    for(int i=1; i<hsigmas->GetNbinsX()+1; i++){
      double ratio = (hmadgraph->GetBinContent(i)-1)/herr->GetBinError(i);
      hsigmas->SetBinContent(i, ratio);
      hpull->Fill(ratio);
    }

    TCanvas *c_sigmas = new TCanvas("c_sigmas_"+recstr+"_bin"+binstr, "c_sigmas_"+recstr+"_bin"+binstr);
    c_sigmas->cd();

    hsigmas->Draw("histo");


    TCanvas *c_pull = new TCanvas("c_pull_"+recstr+"_bin"+binstr, "c_pull_"+recstr+"_bin"+binstr);
    c_pull->cd();

    hpull->Fit("gaus", "L");
    
    fout->cd();
    c->Write();
    c_closure->Write();
    c_sigmas->Write();
    c_pull->Write();
    // c->SaveAs(".png");
    // c_closure->SaveAs(".png");
    // c_sigmas->SaveAs(".png");
    // c_pull->SaveAs(".png");

  }
 
}

int closure_recoil_plots2d()
{
  syst_recoil_one("u1");
  syst_recoil_one("u2");
  return 0;
}

int main()
{
  closure_recoil_plots2d();
  return 0;
}
