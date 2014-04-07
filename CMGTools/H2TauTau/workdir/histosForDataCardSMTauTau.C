#include <TString.h>
#include <TFile.h>
#include <TH1F.h>
#include <TDirectory.h>
#include "TauMuPlotter.h"
#include "configTauMu2011.C"
#include "configTauMu2012.C"
#include "configTauMu2012AB.C"
#include "configTauMu2012ABC.C"
#include "configTauMu2012D.C"
#include "configTauMu2012ABCD.C"
#include "TauElePlotter.h"
#include "configTauEle2011.C"
#include "configTauEle2012.C"
#include "configTauEle2012AB.C"
#include "configTauEle2012ABC.C"
#include "configTauEle2012D.C"
#include "configTauEle2012ABCD.C"

#define NMASS 8
long massValues[NMASS]={110,115,120,125,130,135,140,145};

#define NCAT 5
TString catdirname[NCAT]={
 "0jet_low",
 "0jet_high",
 "boost_low",
 "boost_high",
 "vbf"};


/////New fine binning
#define NXBINS 26
Float_t xbinsValues[NXBINS+1]={0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350};
#define NXBINSVBF 13
Float_t xbinsValuesVBF[NXBINSVBF+1]={0,20,40,60,80,100,120,140,160,180,200,250,300,350};



void fix0Bins(TH1F* h){

//   float maxer=0.;
//   for(Int_t b=1;b<=h->GetNbinsX();b++)
//     if( h->GetBinError(b) > maxer) maxer = h->GetBinError(b);
  
//   for(Int_t b=1;b<=h->GetNbinsX();b++)
//     if(h->GetBinContent(b)<=0.){
//       h->SetBinContent(b,0.);
//       h->SetBinError(b,maxer);
//     }

  if(h->Integral()<=0.)
    for(Int_t b=1; b<=h->GetNbinsX(); b++){
      h->SetBinContent(b,1e-6);
      h->SetBinError(b,2e-6);
    }
}

void fixSignal(TH1F*hData,TH1F*hMC,TH1F*hsig){
  for(Int_t b=1;b<=hData->GetNbinsX();b++)
    if(hData->GetBinContent(b)>0.&&hMC->GetBinContent(b)==0.){
      hsig->SetBinContent(b,0.);
      hsig->SetBinError(b,0.);
    }
}

void histosForDataCardSMTauTau(Int_t channel, Int_t year, Int_t dataset, TString mass, TString path, TString tag, Int_t option=0){

  if(tag!="")path=path+"_"+tag;

  TString ChannelName;
  if(channel==1)ChannelName="muTau";
  else if(channel==2)ChannelName="eleTau";
  else return;
  cout<<"ChannelName "<<ChannelName<<endl;
  
  if(channel==1){
    TauMuPlotter * analysis=0;
    
    if(year==2011){
      analysis=configTauMu2011("analysis",path);
    }
    if(year==2012){
      if(dataset==1)analysis=configTauMu2012AB("analysis",path);
      if(dataset==2)analysis=configTauMu2012ABC("analysis",path);
      if(dataset==3)analysis=configTauMu2012D("analysis",path);
      if(dataset==4)analysis=configTauMu2012ABCD("analysis",path);
    }
  }
  if(channel==2){
    TauElePlotter * analysis=0;

    if(year==2011){
      analysis=configTauEle2011("analysis",path);
    }
    if(year==2012){
      if(dataset==1)analysis=configTauEle2012AB("analysis",path);
      if(dataset==2)analysis=configTauEle2012ABC("analysis",path);
      if(dataset==3)analysis=configTauEle2012D("analysis",path);
      if(dataset==4)analysis=configTauEle2012ABCD("analysis",path);
    }
  }
  
  analysis->plotvar_=mass;
  bool Blind=0;
  analysis->nbins_=0;
  analysis->Isocat_=1;
  analysis->MTcat_=1; 
  analysis->Chcat_=1; 

  analysis->scaleSamplesLumi();

  TFile output(ChannelName+"SM"+"_TauTau_"+analysis->plotvar_+"_"+tag+".root","recreate");
  for(long sm=0; sm<NCAT; sm++){//NCAT

    TDirectory* dir = output.mkdir(ChannelName+"_"+catdirname[sm]);  
    gROOT->cd();

    if(sm==0||sm==2) continue;//do not fill low pT categories

    if(sm==4)analysis->setVariableBinning(NXBINSVBF,xbinsValuesVBF);
    else analysis->setVariableBinning(NXBINS,xbinsValues);
    analysis->extrasel_ = analysis->getSMcutTauTau(sm);
    //analysis->extrasel_ = analysis->getSMcutTauTau(sm) + "*(taudecaymode==10)";
    //analysis->extrasel_ = analysis->getSMcutTauTau(sm) + "*(taudecaymode==1)";
    //analysis->extrasel_ = analysis->getSMcutTauTau(sm) + "*(taudecaymode!=1&&taudecaymode!=10)";


    TH1F* QCD = 0;
    if(channel==1){//mu-tau
      if(year==2011){
	if(sm==0 || sm==2 ) QCD=analysis->getQCDInc();
	if(sm==1 || sm==3 ) QCD=analysis->getQCDIncLooseShape();
	if(sm==4)           QCD=analysis->getQCDMike();
      }
      if(year==2012){
	if(sm==0) QCD=analysis->getQCDIncWNJet(); 
	if(sm==1) QCD=analysis->getQCDMuIsoSM(); 
	if(sm==2) QCD=analysis->getQCDIncLowPt(); 
	if(sm==3) QCD=analysis->getQCDIncLooseShape(); //Changed for TauTau
	if(sm==4) QCD=analysis->getQCDVBFHCP();
      }
    }
    if(channel==2){//e-tau
      if(year==2011){
	if(sm==0 || sm==2) QCD=analysis->getQCDInc();
	if(sm==1 || sm==3) QCD=analysis->getQCDIncLooseShape();
	if(sm==4)          QCD=analysis->getQCDMike();
      }
      if(year==2012){
	if(sm==0) QCD=analysis->getQCDIncWNJet(); 
	if(sm==1) QCD=analysis->getQCDMuIsoSM();
	if(sm==2) QCD=analysis->getQCDIncLowPt();
	if(sm==3) QCD=analysis->getQCDIncHighPt();
	if(sm==4) QCD=analysis->getQCDVBFHCP2();
      }
    }
    QCD->SetName("QCD");


    TH1F* W = 0;
    if(channel==1){//mu-tau
      if(year==2011){
	if(sm==0 || sm==1) W = analysis->getWJetsInc();
	if(sm==2 || sm==3) W = analysis->getWJetsInc();
	if(sm==4) W = analysis->getW3JetsVBF();
      }
      if(year==2012){
	if(sm==0) W = analysis->getWJetsNJet();
	if(sm==1) W = analysis->getWJetsNJetNoChCut();
	if(sm==2) W = analysis->getWJetsNJet();
	if(sm==3) W = analysis->getWJetsIncLooseTau();//changed for TauTau
	if(sm==4) W = analysis->getWJetsNJetVBFHCP(); 
      }
    }
    if(channel==2){//e-tau
      if(year==2011){
	if(sm==0 || sm==1) W = analysis->getWJetsInc();
	if(sm==2 || sm==3) W = analysis->getWJetsInc();
	if(sm==4)  W = analysis->getW3JetsVBF();
      }
      if(year==2012){
	if(sm==0) W = analysis->getWJetsNJet();
	if(sm==1) W = analysis->getWJetsNJetNoChCut();
	if(sm==2) W = analysis->getWJetsNJetNoChCut();
	if(sm==3) W = analysis->getWJetsNJetNoChCut();
	if(sm==4) W = analysis->getWJetsNJetVBFHCP(); 
      }
    }
    W->SetName("W");

    TH1F* ZTT = analysis->getZToTauTau();
    ZTT->SetName("ZTT");

    TH1F* TT = 0;
    if(sm==0 || sm==2 || sm==1 || sm==3)        TT=analysis->getTTJetsInc(); 
    if(sm==4)                                   TT=analysis->getTTJetsVBFHCP(); 
    TT->SetName("TT");

    TH1F* ZL =0;
    if(channel==1){//mu-Tau
      if(sm==0 || sm==2 || sm==1 || sm==3) 	ZL =analysis->getZLInc();
      if(sm==4)                            	ZL =analysis->getZLVBFHCP();
    }
    if(channel==2){//e-Tau
      if(sm==0 || sm==2 || sm==1) 	        ZL =analysis->getZL2012();
      if(sm==3)                            	ZL =analysis->getZLBoost();
      if(sm==4)                            	ZL =analysis->getZLVBFHCP();
    }
    ZL->SetName("ZL");

    TH1F* ZJ = 0;
    if(sm==0 || sm==2 || sm==1 || sm==3)        ZJ=analysis->getZToLJetInc();
    if(sm==4)                                   ZJ=analysis->getZToLJetVBFHCP();                   
    ZJ->SetName("ZJ");

    TH1F* VV = 0;
    if(sm==0 || sm==2 || sm==1 || sm==3)        VV=analysis->getDiBoson();
    if(sm==4)                                   VV=analysis->getDiBosonVBFHCP();
    VV->SetName("VV");

    TH1F* ZLL=(TH1F*)ZL->Clone("ZLL");
    ZLL->SetName("ZLL");
    ZLL->Add(ZJ);

    //blind
    TString tmpsel=analysis->extrasel_;
    if(Blind)analysis->extrasel_ += "*(svfitmass<100||160<svfitmass)"; 
    TH1F* data_obs = analysis->getTotalData();
    data_obs->SetName("data_obs");
    analysis->extrasel_ =tmpsel;



    TH1F* MC=(TH1F*)ZTT->Clone("MC");//needed below
    MC->Add(ZL);
    MC->Add(ZJ);
    MC->Add(W);
    MC->Add(TT);
    MC->Add(VV);
    MC->Add(QCD);


    dir->cd();
  

    fix0Bins(ZTT); ZTT->Write();
    fix0Bins(ZL);  ZL->Write();
    fix0Bins(ZJ); ZJ->Write();
    fix0Bins(ZLL);ZLL->Write();
    fix0Bins(W);  W->Write();
    fix0Bins(TT);TT->Write();
    fix0Bins(VV);VV->Write();
    fix0Bins(QCD); QCD->Write();
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
      
      //check for empty histos
      if( SM->Integral()<=0.){  SM->SetBinContent(SM->GetNbinsX()/2,1e-4);    SM->SetBinError(SM->GetNbinsX()/2,1e-4); }
      if( VBF->Integral()<=0.){ VBF->SetBinContent(VBF->GetNbinsX()/2,1e-4);  VBF->SetBinError(VBF->GetNbinsX()/2,1e-4); }
      if( VH->Integral()<=0.){  VH->SetBinContent(VH->GetNbinsX()/2,1e-4);    VH->SetBinError(VH->GetNbinsX()/2,1e-4); }

      dir->cd();
      
     
      
      fixSignal(data_obs,MC,VH);  fix0Bins(VH);   VH->Write();
      fixSignal(data_obs,MC,SM);  fix0Bins(SM);   SM->Write();
      fixSignal(data_obs,MC,VBF); fix0Bins(VBF);  VBF->Write();
      gROOT->cd();

      delete VH;
      delete SM;
      delete VBF;

    }

    delete MC;
    delete data_obs;

  }
  
  output.ls();
  output.Close();
  gROOT->ProcessLine(".q");
}




void plotDataCard(TString file, Int_t channel){

  TString ChannelName;
  if(channel==1)ChannelName="muTau";
  else if(channel==2)ChannelName="eleTau";
  else return;
  
  TFile nominal(file+".root","read");
  gROOT->cd();

  TString fname=TString("plotDataCard_")+file+".ps";

  TCanvas C;
  C.Print(fname+"[");

  for(long sm=0;sm<NCAT;sm++){//

     if(sm==0||sm==2)continue;

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
