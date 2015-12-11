#include <iostream>

#include <TStyle.h>
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TString.h"
#include "TGraph.h"
#include "TLegend.h"

using namespace std;

double xaxislimit = 17;
int fillstyle = 3001;

void syst_recoil_one(TString recstr="u1")
{
  gStyle->SetOptFit(111);
  gStyle->SetLegendBorderSize(0);

  const int nhists = 6;

  int IniVar[] = {0,  9,  0, 0,  9,  0, 0,  9,  0, 0,  9,  0};
  int NVars[]  = {9, 21, 15, 9, 21, 15, 9, 21, 15, 9, 21, 15};

  int ntotsysts = 0;
  for (int i=0; i<nhists; ++i) ntotsysts+=(NVars[i]-IniVar[i]);

  TFile* fcentral = new TFile(Form("0.root"));
  TFile* frookeys = new TFile(Form("rookeys.root"));
  TFile* fmadgraph = new TFile(Form("madnocorr.root"));
  
  TH2D* hcentral2 = (TH2D*)fcentral->Get(Form("hWlikePos_%svsZpt_8_JetCut_pdf229800-0_eta0p9_91188", recstr.Data()));
  TH2D* hrookeys2 = (TH2D*)frookeys->Get(Form("hWlikePos_%svsZpt_8_JetCut_pdf229800-0_eta0p9_91188", recstr.Data()));
  TH2D* hmadgraph2 = (TH2D*)fmadgraph->Get(Form("hWlikePos_%svsZpt_8_JetCut_pdf229800-0_eta0p9_91188", recstr.Data()));


  TH2D*h2dratio = (TH2D*) hcentral2->Clone("h2dratio");
  TH2D*h2dratio_toterr = (TH2D*) hcentral2->Clone("h2dratio_toterr");
  TH2D*h2dratio_sigmas = (TH2D*) hcentral2->Clone("h2dratio_sigmas");

  TFile* fin[nhists];
  TH2D* hsyst2[ntotsysts];

  int nsyst=0;
  for(int i=0; i<nhists; i++){
    fin[i]=new TFile(Form("%d.root", i+1));
    for(int j=IniVar[i]; j<NVars[i]; j++){
      hsyst2[nsyst]=(TH2D*)fin[i]->Get(Form("hWlikePos_%svsZpt_8_JetCut_pdf229800-0_RecoilCorrVar%d_eta0p9_91188", recstr.Data(), j));
      nsyst++;
    }
  }

  int Zptbins = hcentral2->GetNbinsX()+1;
  double ncentral[Zptbins];
  double nmadgraph[Zptbins];
  double means[Zptbins];
  double rmss[Zptbins];

  TFile* fout = new TFile(recstr+"vsZpt.root", "RECREATE");
  hcentral2->Write();
  hmadgraph2->Write();

  for (int bin = 1; bin < hcentral2->GetNbinsX()+1; ++bin){

    TString binstr = Form("%d", bin);

    TH1D* hcentral = hcentral2->ProjectionY("hcentral_bin" + binstr, bin, bin+1, "e");

    TCanvas* cp=new TCanvas("cp_"+recstr+"_bin"+binstr, "cp_"+recstr+"_bin"+binstr);
    cp->cd();
    hcentral->Draw();

    TH1D* hmadgraph = hmadgraph2->ProjectionY("hmadgraph_bin" + binstr, bin, bin+1, "e");

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
    hcentral->SetStats(kFALSE);
    hcentral->Draw();

    TH1D* hsyst[ntotsysts];

    for(int nsyst=0; nsyst<ntotsysts; nsyst++){
      hsyst[nsyst]=hsyst2[nsyst]->ProjectionY("", bin, bin+1, "e");
      hsyst[nsyst]->Scale(1/hsyst[nsyst]->Integral());
      hsyst[nsyst]->Divide(hcentral_noerr);
      hsyst[nsyst]->SetLineColor(nsyst);
      hsyst[nsyst]->SetMarkerStyle(20);
      hsyst[nsyst]->SetMarkerSize(0.01);
      hsyst[nsyst]->SetMarkerColor(nsyst);
      hsyst[nsyst]->Draw("same");
    }

    TH1D* hrookeys = hrookeys2->ProjectionY("hrookeys_bin" + binstr, bin, bin+1, "e");

    hrookeys->Scale(1/hrookeys->Integral());
    hrookeys->Divide(hcentral_noerr);

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
    for(int i=1;i<herr->GetNbinsX()+1; i++){
      double errstat = hstat   ->GetBinError(i);
      double errfit  = hsystfit->GetBinError(i);
      herr->SetBinError(i, sqrt(errstat*errstat + errfit*errfit));
    }
    
    TH1D* hclosure = (TH1D*)hcentral->Clone("hclosure_bin" + binstr);
    hclosure->SetTitle("Pow2Mad closure; " +recstr+ "; N");
    for(int i=1;i<hclosure->GetNbinsX()+1; i++){
      double errstatfit = herr->GetBinError(i);
      double errclosure = hrookeys->GetBinContent(i)-1;
      hclosure->SetBinError(i, sqrt(errstatfit*errstatfit + errclosure*errclosure));
    }


    TCanvas *c_closure = new TCanvas("c_closure_"+recstr+"_bin"+binstr, "c_closure_"+recstr+"_bin"+binstr);
    c_closure->cd();

    hclosure->SetAxisRange(-xaxislimit, +xaxislimit, "X");
    hclosure->SetAxisRange(0.8, 1.2, "Y");
    hclosure->SetFillColor(kBlue+1);
    hclosure->SetFillStyle(fillstyle);
    hclosure->Draw("E2");
    herr->SetFillColor(kCyan-2);
    herr->SetFillStyle(fillstyle);
    herr->Draw("same E2");
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
    leg->AddEntry(hmadgraph, "madgraph / (powheg morphed to madgraph 3G->3G)", "l");
    leg->AddEntry(hcentral,  "powheg stat unc",                                "f");
    leg->AddEntry(hstat,     "madgraph stat unc",                              "f");
    leg->AddEntry(herr,      "3G->3G statistical uncertainty",                 "f");
    leg->AddEntry(hclosure,  "difference against RK->RK morphing",             "f");
    leg->Draw();

    TH1D* hsigmas = (TH1D*)hmadgraph->Clone("hsigmas_bin" + binstr);
    hsigmas->SetName("hsigmas_bin" + binstr);
    hsigmas->SetTitle("hsigmas_bin" + binstr + "; " + recstr + "; N");

    TH1D* hpull = new TH1D("hpull_bin" + binstr, "bin" + binstr + "; pull value; N", 40, -6, 6);
    hpull->StatOverflows(kTRUE);

    for(int i=1; i<hsigmas->GetNbinsX()+1; i++){
      double ratio = (hmadgraph->GetBinContent(i)-1)/hclosure->GetBinError(i);
      hsigmas->SetBinContent(i, ratio);
      hpull->Fill(ratio);
      h2dratio->SetBinContent(bin,i,hmadgraph->GetBinContent(i));
      h2dratio_toterr->SetBinContent(bin,i,hclosure->GetBinError(i));
      h2dratio_sigmas->SetBinContent(bin,i,ratio);
    }

    ncentral[bin] = hcentral->GetEntries()/hcentral2->GetEntries();
    nmadgraph[bin] = hmadgraph->GetEntries()/hmadgraph2->GetEntries();
    means[bin] = hpull->GetMean();
    rmss[bin] = hpull->GetRMS();

    TCanvas *c_pull = new TCanvas("c_pull_"+recstr+"_bin"+binstr, "c_pull_"+recstr+"_bin"+binstr);
    c_pull->cd();

    hpull->Fit("gaus", "L");
    
    fout->cd();
    // c->Write();
    c_closure->Write();
    hsigmas->Write();
    c_pull->Write();
    // c->SaveAs(".png");
    // c_closure->SaveAs(".png");
    // hsigmas->SaveAs(".png");
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

  TGraph* gcentral  = new TGraph(Zptbins-1, numbers+1, ncentral+1);
  TGraph* gmadgraph = new TGraph(Zptbins-1, numbers+1, nmadgraph+1);
  gmadgraph->SetTitle("Nevt "+recstr+"; Zpt; Nevt");

  TCanvas *c_nevt = new TCanvas("c_nevt_"+recstr, "c_nevt_"+recstr);
  c_nevt->cd();

  gcentral->SetMarkerColor(3);
  gcentral->SetLineColor(3);
  gcentral->SetMarkerSize(1);
  gcentral->SetMarkerStyle(23);
  gmadgraph->SetMarkerColor(2);
  gmadgraph->SetLineColor(2);
  gmadgraph->SetMarkerSize(1);
  gmadgraph->SetMarkerStyle(22);

  gmadgraph->Draw("APL");
  gcentral->Draw("PL");

  TLegend *legevts = new TLegend(0.7,0.7,0.9,0.9);
  // leg->SetHeader("The Legend Title");
  legevts->AddEntry(gmadgraph,"madgraph","p");
  legevts->AddEntry(gcentral,"powheg","p");
  legevts->Draw();

  TGraph* gmean =  new TGraph(Zptbins-1, numbers+1, means+1);
  gmean->SetTitle("Mean "+recstr+"; Zpt; Mean");

  TCanvas *c_mean = new TCanvas("c_mean_"+recstr, "c_mean_"+recstr);
  c_mean->cd();

  gmean->SetMarkerColor(2);
  gmean->SetMarkerSize(1);
  gmean->SetMarkerStyle(22);

  gmean->Draw("ACP");

  TGraph* grms =  new TGraph(Zptbins-1, numbers+1, rmss+1);
  grms->SetTitle("RMS "+recstr+"; Zpt; RMS");

  TCanvas *c_rms = new TCanvas("c_rms_"+recstr, "c_rms_"+recstr);
  c_rms->cd();

  grms->SetMarkerColor(1);
  grms->SetMarkerSize(1);
  grms->SetMarkerStyle(20);

  grms->Draw("ACP");


  c_nevt->Write();
  c_mean->Write();
  c_rms ->Write();
  // c_nevt->SaveAs(".png");
  // c_mean->SaveAs(".png");
  // c_rms->SaveAs(".png");
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
