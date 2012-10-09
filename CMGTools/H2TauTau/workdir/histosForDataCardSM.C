#include <TString.h>
#include <TFile.h>
#include <TH1F.h>
#include <TDirectory.h>
#include "TauMuPlotter.h"
#include "configTauMu2011.C"
#include "configTauMu2012.C"
#include "TauElePlotter.h"
#include "configTauEle2011.C"
#include "configTauEle2012.C"

#define NMASS 8
#define NCAT 5
long massValues[NMASS]={110,115,120,125,130,135,140,145};

//#define NMASS 20
//#define NCAT 7
//long massValues[NMASS]={90,100,110,120,130,140,160,180,200,250,300,350,400,450,500,600,700,800,900,1000};

#define NXBINS 22
//Float_t xbinsValues[13+1]={0,20,40,60,80,100,120,140,160,180,200,250,300,350};
float xbinsValues[NXBINS]={0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,450,500,600,700,800,900,1000};


TString catdirname[NCAT]={
 "0jet_low",
 "0jet_high",
 "boost_low",
 "boost_high",
 "vbf"};

void histosForDataCardSM(Int_t channel,Int_t year,TString path,TString tag){

  if(tag!="")path=path+"_"+tag;

  TString ChannelName;
  if(channel==1)ChannelName="muTau";
  else if(channel==2)ChannelName="eTau";
  else return;
  cout<<"ChannelName "<<ChannelName<<endl;
  
  if(channel==1){
    if(year==2011)TauMuPlotter * analysis=configTauMu2011("analysis",path);
    if(year==2012)TauMuPlotter * analysis=configTauMu2012("analysis",path);
  }
  if(channel==2){
    if(year==2011)TauElePlotter * analysis=configTauEle2011("analysis",path);
    if(year==2012)TauElePlotter * analysis=configTauEle2012("analysis",path);
  }
  
  //analysis->plotvar_="svfitmass";
  //analysis->plotvar_="ditaumass";
  analysis->plotvar_="mupt";
  analysis->setVariableBinning(NXBINS-1,xbinsValues);
  analysis->nbins_=0;
  analysis->Isocat_=1;
  analysis->MTcat_=1; 
  analysis->Chcat_=1; 

  analysis->printRawYields("eventweight*(categoryIso==1&&abs(ditaucharge)==0)");//This might be needed to avoid some nan values


  TFile output(ChannelName+"SM"+"_"+analysis->plotvar_+"_"+tag+".root","recreate");
  for(long sm=0; sm<NCAT; sm++){

    TDirectory* dir = output.mkdir(ChannelName+"_"+catdirname[sm]);  

    analysis->extrasel_ = analysis->getSMcut(sm);
    if(!analysis->scaleSamplesLumi())return;

    gROOT->cd();


    TH1F* QCD = 0;
    if(channel==1){//mu-tau
      if(year==2011){
	if(sm==0)                    QCD=analysis->getQCDInc();
	if(sm==1 || sm==2 || sm==3 ) QCD=analysis->getQCDIncLooseShape();
	if(sm==4)                    QCD=analysis->getQCDMike();
      }
      if(year==2012){
	if(sm==0 || sm==1 || sm==2 || sm==3 ) QCD=analysis->getQCDInc();
	if(sm==4)                             QCD=analysis->getQCDHCP();
      }
    }
    if(channel==2){//e-tau
      if(year==2011){
	if(sm==0 || sm==1) QCD=analysis->getQCDIncLooseShape();//getQCDInc();
	if(sm==2 || sm==3) QCD=analysis->getQCDIncLooseShape();//getQCDInc();
	if(sm==4)          QCD=analysis->getQCDMike();
      }
      if(year==2012){
	if(sm==0 || sm==1 || sm==2 || sm==3 ) QCD=analysis->getQCDInc();
	if(sm==4)                             QCD=analysis->getQCDHCP();
      }
    }
    QCD->SetName("QCD");


    TH1F* W = 0;
    if(channel==1){//mu-tau
      if(year==2011){
	if(sm==0 || sm==1) W = analysis->getWJetsInc();
	if(sm==2 || sm==3) W = analysis->getWJetsInc();
	if(sm==4)          W = analysis->getW3JetsVBF();
      }
      if(year==2012){
	if(sm==0 || sm==1 ) W = analysis->getWJetsInc();
	if(sm==2 || sm==3 ) W = analysis->getWJetsNJet();
	if(sm==4)           W = analysis->getWJetsNJet(); 
      }
    }
    if(channel==2){//e-tau
      if(year==2011){
	if(sm==0 || sm==1) W = analysis->getWJetsInc();
	if(sm==2 || sm==3) W = analysis->getWJetsInc();
	if(sm==4)          W = analysis->getW3JetsVBF();
      }
      if(year==2012){
	if(sm==0 || sm==1) W = analysis->getWJetsInc();
	if(sm==2 || sm==3) W = analysis->getWJetsNJet();
	if(sm==4)          W = analysis->getWJetsNJet(); 
      }
    }
    W->SetName("W");

    TH1F* ZTT = analysis->getZToTauTau();
    ZTT->SetName("ZTT");
    //cout<<" ZTT "<<ZTT->Integral()<<endl;

    TH1F* TT = analysis->getTTJetsInc();
    TT->SetName("TT");

    //cout<<"channel "<<channel<<endl;
    TH1F* ZL =0;
    if(channel==1){  ZL =analysis->getZToMuMuInc();}
    if(channel==2){  ZL =analysis->getZToEEInc();}
    ZL->SetName("ZL");

    TH1F* ZJ = analysis->getZToLJetInc();
    ZJ->SetName("ZJ");

    TH1F* VV = analysis->getDiBoson();
    VV->SetName("VV");

    TH1F* ZLL=(TH1F*)ZL->Clone("ZLL");
    ZLL->SetName("ZLL");
    ZLL->Add(ZJ);

    //blind
    TString tmpsel=analysis->extrasel_;
    //analysis->extrasel_ += "*(svfitmass<100||160<svfitmass)"; 
    TH1F* data_obs = analysis->getTotalData();
    data_obs->SetName("data_obs");
    analysis->extrasel_ =tmpsel;


    dir->cd();

    ZTT->Write();
    ZL->Write();
    ZJ->Write();
    ZLL->Write();
    W->Write();
    TT->Write();
    VV->Write();
    QCD->Write();
    data_obs->Write();
 
    gROOT->cd();

    delete ZTT ;
    delete ZL;
    delete ZJ;
    delete ZLL;
    delete W;
    delete TT;
    delete VV;
    delete QCD;
    delete data_obs;


    for(Int_t m=0;m<NMASS;m++){
      long ma=massValues[m];

      //Nominal h 
      TH1F* SM = analysis->getSample(TString("HiggsGG")+ma);
      SM->SetName(TString("ggH")+ma);

      TH1F* VBF = analysis->getSample(TString("HiggsVBF")+ma);
      VBF->SetName(TString("qqH")+ma);

      TH1F* VH = analysis->getSample(TString("HiggsVH")+ma);
      VH->SetName(TString("VH")+ma);

      SM->Scale(1./analysis->findSample(TString("HiggsGG")+ma)->getCrossection());
      VBF->Scale(1./analysis->findSample(TString("HiggsVBF")+ma)->getCrossection());
      VH->Scale(1./analysis->findSample(TString("HiggsVH")+ma)->getCrossection());
      
      
      dir->cd();
      VH->Write();
      SM->Write();
      VBF->Write();
      gROOT->cd();

      delete VH;
      delete SM;
      delete VBF;

    }

  }
  
  output.ls();
  output.Close();
  gROOT->ProcessLine(".q");
}




void plotDataCard(TString file, Int_t channel){

  TString ChannelName;
  if(channel==1)ChannelName="muTau";
  else if(channel==2)ChannelName="eTau";
  else return;
  
  TFile nominal(file,"read");
  gROOT->cd();

  TString fname=TString("plotDataCard_")+ChannelName+".ps";

  TCanvas C;
  C.Print(fname+"[");

  for(long sm=0;sm<NCAT;sm++){

    TH1F* ZTT = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/ZTT");
    if(!ZTT)continue;
    TH1F* QCD = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/QCD");
    TH1F* W = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/W");
    TH1F* TT = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/TT");
    TH1F* ZL = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/ZL");
    TH1F* ZJ = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/ZJ");
    TH1F* VV = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/VV");
    TH1F* data_obs = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/data_obs");

    //plot
    TH1F*MC=(TH1F*)ZTT->Clone("MC");
    MC->Add(ZL);
    MC->Add(ZJ);
    MC->Add(TT);
    MC->Add(W);
    MC->Add(TT);
    MC->Add(VV);
    MC->Add(QCD);
    MC->SetTitle(ChannelName+"_"+catdirname[sm]);


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
    if(MC->GetMaximum()>data_obs->GetMaximum())
      data_obs->GetYaxis()->SetRangeUser(0,1.2*MC->GetMaximum());
    else
      data_obs->GetYaxis()->SetRangeUser(0,1.2*data_obs->GetMaximum());
    data_obs->SetTitle(catdirname[sm]);
    data_obs->Draw("histpe");
    hMCStack.Draw("histsame");
    data_obs->Draw("histpesame");
    C.Print(fname);
    delete MC;

    ////////Draw the first and last Higgs signal
    C.Clear();
    TH1F * SM1=0;
    TH1F * SM2=0;
    for(Int_t m=0;m<NMASS;m++){
      long ma=massValues[m];
      TH1F* SM = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/ggH"+ma);
      TH1F* VBF = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/qqH"+ma);
      TH1F* VH = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/VH"+ma);
      if(m==0){SM1=(TH1F*)SM->Clone("SM1"); SM1->Add(VBF); SM1->Add(VH);}
      if(m==NMASS-1){SM2=(TH1F*)SM->Clone("SM2");SM2->Add(VBF);SM2->Add(VH);}
    }
    if(SM1->GetMaximum()>SM2->GetMaximum())
      SM1->GetYaxis()->SetRangeUser(0,1.2*SM1->GetMaximum());
    if(SM1->GetMaximum()<SM2->GetMaximum())
      SM1->GetYaxis()->SetRangeUser(0,1.2*SM2->GetMaximum());

    SM1->SetTitle(catdirname[sm]);
    SM1->Draw("hist");
    SM2->Draw("histsame");
    C.Print(fname);
 
  }
  
  C.Print(fname+"]");
 
  nominal.Close();
}
