#include "CMGTools/TauTest/interface/TauEfficiencyPlot.h"
#include <TFile.h>
#include <TH1F.h>
#include <TDirectory.h>
#include <TCanvas.h>
#include <iostream>
#include <math.h>
#include <TText.h>
using namespace std;

void TauEfficiencyPlot::plotEfficiencyVsPt(){

  TFile fil(file_,"read");
  if(fil.IsZombie())return;

  addHistId("");
  addHistId("Pt");
  addHistId("Eta");
  //addHistId("DecayModeFinding"); //applied in PF2PAT
  addHistId("AgainstMuonTight");
  addHistId("AgainstElectronLoose");
  //addHistId("Vertex"); //applied in PF2PAT
  addHistId("SumPtIsolation");
  //addHistId("");
  
  TCanvas Canv("TauEfficiencyPlot_plotEfficiencyVsPt");
  TString outfile="TauEfficiencyPlot_plotEfficiencyVsPt.ps";
  Canv.Print(outfile+"[");


  TH1F* HEffnew[NCMGTAUEFFPLOTS];  
  TH1F* HEffnewTrue[NCMGTAUEFFPLOTS];  
  Int_t ncuts=-1;
  for(Int_t i=0;i<NCMGTAUEFFPLOTS;i++){
    HEffnew[i]=0;
    if(histId_[i]!="-"){
      TH1F* h=(TH1F*)fil.Get(TString("cmgTauSel")+histId_[i]+"Histograms/pt");
      if(!h)return;
      HEffnew[i]=new TH1F(*h);
      //HEffnew[i]->Sumw2();
      HEffnew[i]->SetTitle(histId_[i]);
      cout<<i<<" "<<histId_[i]<<endl;

      TH1F* ht=(TH1F*)fil.Get(TString("cmgTauSel")+histId_[i]+"HistogramsTrue/pt");
      if(!ht)return;
      HEffnewTrue[i]=new TH1F(*ht);
      //HEffnew[i]->Sumw2();
      HEffnewTrue[i]->SetTitle(histId_[i]);
      cout<<i<<" "<<histId_[i]<<" True"<<endl;
      ncuts++;
    }
  }
  
  TH1F plotframe("plotframe","",1,0,100);
  plotframe.SetStats(0);
  plotframe.GetYaxis()->SetTitle("Entries / 1 GeV");
  plotframe.GetXaxis()->SetTitle("p_{T}   GeV");

  
  Canv.Clear();
  plotframe.GetYaxis()->SetRangeUser(0,HEffnew[0]->GetMaximum()*1.1);
  plotframe.Draw();
  for(Int_t i=0;i<NCMGTAUEFFPLOTS;i++){
    if(histId_[i]!="-"){
      HEffnew[i]->Draw("hist same");
    }
  }
  Canv.Print(outfile);

  Canv.Clear();
  plotframe.GetYaxis()->SetRangeUser(0,HEffnewTrue[0]->GetMaximum()*1.1);
  plotframe.Draw();
  for(Int_t i=0;i<NCMGTAUEFFPLOTS;i++){
    if(histId_[i]!="-"){
      HEffnewTrue[i]->Draw("hist same");
    }
  }
  Canv.Print(outfile);


  TH1F* HEffratio[NCMGTAUEFFPLOTS];  
  TH1F HAvgEff("HAvgEff","",ncuts,0.5,ncuts+0.5);
  for(Int_t i=1;i<NCMGTAUEFFPLOTS;i++){
    if(histId_[i]!="-"){
      HAvgEff.SetBinContent(i,HEffnew[i]->Integral()/HEffnew[0]->Integral());
      HAvgEff.SetBinError(i,HAvgEff.GetBinContent(i)*sqrt(1.0/HEffnew[i]->Integral()+1.0/HEffnew[0]->Integral()));

      HEffratio[i]=new TH1F(*HEffnew[i]);
      HEffratio[i]->Divide(HEffnew[0]);
    }
  }
  



  Canv.Clear();
  plotframe.SetTitle("Efficiency");
  plotframe.GetYaxis()->SetRangeUser(0,1.1);
  plotframe.Draw();
  for(Int_t i=2;i<NCMGTAUEFFPLOTS;i++){
    if(histId_[i]!="-"){
      HEffratio[i]->Draw("hist l same");
    }
  }
  Canv.Print(outfile);

  TText text;
  text.SetTextSize(.03);
  text.SetTextAngle(-25.);

  Canv.Clear();
  HAvgEff.SetStats(0);
  HAvgEff.GetYaxis()->SetRangeUser(0,HAvgEff.GetMaximum()*1.2);
  HAvgEff.GetYaxis()->SetTitle("relative efficiency");
  HAvgEff.GetXaxis()->SetTitle("");
  HAvgEff.GetXaxis()->SetNdivisions(0);
  HAvgEff.Draw("hist pe");
  for(Int_t i=1;i<NCMGTAUEFFPLOTS;i++)
    if(histId_[i]!="-")
      text.DrawText(i,0.0-0.02*HAvgEff.GetMaximum(),TString(histId_[i]));

  Canv.Print(outfile);


  Canv.Print(outfile+"]");
 
}
