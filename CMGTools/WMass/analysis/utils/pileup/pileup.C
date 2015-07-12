#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include <stdio.h>

void pileup(){

  gStyle->SetPaintTextFormat("1.f");

  TFile*f7TeV=new TFile("pileup_2011_JSON_pixelLumi.root");
  TFile*f8TeV=new TFile("pileup_JSON_DCSONLY_190389-201678_corr.root");
  TFile *fout=new TFile("pileup.root","RECREATE");
  
  TH1D*pileup7TeV=(TH1D*)f7TeV->Get("pileup");
  pileup7TeV->SetName("pileup7TeV");
  pileup7TeV->SetTitle("pileup7TeV");
  TH1D*pileup7TeVintegral=(TH1D*)pileup7TeV->Clone("pileup7TeVintegral");
  TH1D*pileup8TeV=(TH1D*)f8TeV->Get("pileup");
  pileup8TeV->SetName("pileup8TeV");
  pileup8TeV->SetTitle("pileup8TeV");
  TH1D*pileup8TeVintegral=(TH1D*)pileup8TeV->Clone("pileup8TeVintegral");  
  
  TCanvas *c7TeV=new TCanvas("c7TeV","c7TeV");
  pileup7TeV->Scale(5.05/pileup7TeV->Integral());
  pileup7TeV->Draw("h text");

  TCanvas *c8TeV=new TCanvas("c8TeV","c8TeV");
  pileup8TeV->Scale(9.708/pileup8TeV->Integral());
  pileup8TeV->Draw("h text");
  
  pileup7TeVintegral->SetBinContent(0,0);
  pileup7TeVintegral->SetBinContent(1,0);
  pileup7TeVintegral->SetBinContent(51,0);
  pileup8TeVintegral->SetBinContent(0,0);
  pileup8TeVintegral->SetBinContent(1,0);
  pileup8TeVintegral->SetBinContent(51,0);
  for(int i=2;i<51;i++){
  // cout << "pileup7TeV->GetBinContent("<<i<<")"<<pileup7TeV->GetBinContent(i)<<endl;
    double integral7TeV=pileup7TeV->Integral(1,pileup7TeV->GetBinCenter(i));
    // cout << i<<" integral7TeV= " << integral7TeV << endl;
    pileup7TeVintegral->SetBinContent(i,integral7TeV);
  // cout << "pileup8TeV->GetBinContent("<<i<<")"<<pileup8TeV->GetBinContent(i)<<endl;
    double integral8TeV=pileup8TeV->Integral(1,pileup8TeV->GetBinCenter(i));
    // cout << i<<" integral8TeV= " << integral8TeV << endl;
    pileup8TeVintegral->SetBinContent(i,integral8TeV);
  }

  TCanvas *c7TeVint=new TCanvas("c7TeVintegral","c7TeVintegral");
  c7TeVint->SetGridx();
  c7TeVint->SetGridy();
  pileup7TeVintegral->Draw("h text");

  TCanvas *c8TeVint=new TCanvas("c8TeVintegral","c8TeVintegral");
  c8TeVint->SetGridx();
  c8TeVint->SetGridy();
  pileup8TeVintegral->Draw("h text");
  
  //NUMBER OF Z AND W to leptons!!!!!!!
  // 7 TeV: 9.22 nb for W, 0.882 nb for Z
  // 8 TeV: 11.88 nb for W, 1.12 nb for Z
  
  fout->cd();
  pileup7TeV->Write();
  pileup8TeV->Write();
  c7TeVint->Write();
  c8TeVint->Write();
  // pileup7TeVintegral->Write();
  // pileup8TeVintegral->Write();
  fout->Write();
  fout->Close();
  
}