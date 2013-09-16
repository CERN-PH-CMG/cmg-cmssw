#include <TString.h>
#include <TFile.h>
#include <TH1F.h>
#include <TDirectory.h>

#include "constants.h"

bool skipCategory(int cat, int sm){
  if(cat==10 && !( sm==8 ) )return 1;//inclusive category
  if(cat==11 && !(sm==0 || sm==1 || sm==2 || sm==3) )return 1;
  if(cat==12 && !(sm==0 || sm==1 || sm==2 || sm==4) )return 1;
  if(cat==13 && !(sm==0 || sm==1 || sm==2 || sm==5) )return 1;
  if(cat==15 && !(sm==0 || sm==1 || sm==2 || sm==6) )return 1;
  if(cat==16 && !(sm==0 || sm==1 || sm==2 || sm==7) )return 1;
  return 0;
}

void plotDataCardSMSummer13(TString file, Int_t channel, Int_t cat=-1){

  TString ChannelName;
  if(channel==1)ChannelName="muTau";
  else if(channel==2)ChannelName="eleTau";
  else return;
  
  TFile nominal(file,"read");
  gROOT->cd();

  TString fname=file;

  TCanvas C;
  C.Print(fname+".ps[");

  for(long sm=0;sm<NCATSUMMER13;sm++){//
    if(skipCategory(cat,sm))continue;

    TH1F* ZTT = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ZTT");
    if(!ZTT)continue;
    TH1F* QCD = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/QCD");
    TH1F*  W = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/W");
    TH1F* TT = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/TT");
    TH1F* ZL = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ZL");
    TH1F* ZJ = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ZJ");
    TH1F* VV = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/VV");
    TH1F* data_obs = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/data_obs");

    //plot
    TH1F*MC=(TH1F*)ZTT->Clone("MC");
    MC->Add(ZL);
    MC->Add(ZJ);
    MC->Add(TT);
    MC->Add(W);
    MC->Add(TT);
    MC->Add(VV);
    MC->Add(QCD);
    MC->SetTitle(ChannelName+"_"+catdirnameSummer13[sm]);


    THStack hMCStack("hBkgStack","BkgStack");//dont't set any of the regular histogram properties on the THStack will crash.
    QCD->SetFillColor(kMagenta-10);
    hMCStack.Add(QCD,"hist");
    TH1F*hEWK=(TH1F*)W->Clone("EWK");
    hEWK->Add(ZL);
    hEWK->Add(ZJ);
    hEWK->Add(VV);
    hEWK->SetFillColor(kRed+2);
    hMCStack.Add(hEWK,"hist");
    TT->SetFillColor(kBlue-8);
    hMCStack.Add(TT,"hist");
    ZTT->SetFillColor(kOrange-4);
    hMCStack.Add(ZTT,"hist");


    ////////////
    C.Clear();
    data_obs->GetXaxis()->SetRangeUser(0,200);
    if(MC->GetMaximum()>data_obs->GetMaximum())
      data_obs->GetYaxis()->SetRangeUser(0,1.2*MC->GetMaximum());
    else
      data_obs->GetYaxis()->SetRangeUser(0,1.2*data_obs->GetMaximum());
    data_obs->SetTitle(catdirnameSummer13[sm]);
    data_obs->Draw("histpe");
    hMCStack.Draw("histsame");
    data_obs->Draw("histpesame");
    C.Print(fname+".ps");
    C.Print(fname+".png");
    delete MC;
 
  }
  
  C.Print(fname+".ps]");
 
  nominal.Close();

  gROOT->ProcessLine(".q");
}
