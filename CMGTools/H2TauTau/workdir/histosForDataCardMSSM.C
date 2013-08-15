#include <TString.h>
#include <TFile.h>
#include <TH1F.h>
#include <TDirectory.h>
#include "TauMuPlotter.h"
#include "configTauMu2011.C"
#include "TauElePlotter.h"
#include "configTauEle2011.C"

#define NMASS 19
#define NCAT 7


long massValues[NMASS]={90,100,120,130,140,160,180,200,250,300,350,400,450,500,600,700,800,900,1000};

TString catdirname[NCAT]={
 "0jet_low",
 "0jet_high",
 "boost_low",
 "boost_high",
 "vbf",
 "btag_low",
 "btag_high"};

void histosForDataCardMSSM(Int_t channel,TString path,TString tag){

  //TString path="/data/benitezj/Samples/TauMuV541June2_TrigEff";

  if(tag!="")path=path+"_"+tag;

  TString ChannelName;
  if(channel==1)ChannelName="muTau";
  else if(channel==2)ChannelName="eTau";
  else return;
  cout<<"ChannelName "<<ChannelName<<endl;

  Float_t xbinsValues[19+1]={0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,450,500,600,700,800};
  
  
  if(channel==1)TauMuPlotter * analysis=configTauMu2011("analysis",path);
  if(channel==2)TauElePlotter * analysis=configTauEle2011("analysis",path);
  
  //analysis->plotvar_="svfitmass";
  analysis->plotvar_="ditaumass";
  analysis->setVariableBinning(19,xbinsValues);
  analysis->nbins_=0;
  analysis->Isocat_=1;
  analysis->Chcat_=1; 
  analysis->MTcat_=101; 
  analysis->setMSSMFlag(1);
  analysis->setZTTType(2);

  
  TFile output(ChannelName+"MSSM_"+analysis->plotvar_+"_"+tag+".root","recreate");
  for(long sm=0; sm<NCAT; sm++){
    if(sm==4) continue;

    TDirectory* dir = output.mkdir(ChannelName+"_"+catdirname[sm]);  

    analysis->extrasel_ = analysis->getSMcut(sm);
    if(!analysis->scaleSamplesLumi())return;

    gROOT->cd();

    TH1F* ZTT = analysis->getZToTauTau();
    ZTT->SetName("ZTT");


    TH1F* QCD = 0;
    if(channel==1){
      if(sm==0 || sm==1 || sm==2 || sm==3) QCD=analysis->getQCDInc();
      if(sm==5 || sm==6)                   QCD=analysis->getQCDKeti();
    }
    if(channel==2){
      if(sm==0 || sm==1 || sm==2 || sm==3) QCD=analysis->getQCDInc();
      if(sm==5 || sm==6)                   QCD=analysis->getQCDKeti();
    }
    QCD->SetName("QCD");


    TH1F* W = 0;
    if(channel==1){
      if(sm==0 || sm==1 || sm==2 || sm==3) W = analysis->getWJetsInc();
      if(sm==5 || sm==6)                   W = analysis->getW2JetsBJet();
    }
    if(channel==2){
      if(sm==0 || sm==1 || sm==2 || sm==3) W = analysis->getWJetsInc();
      if(sm==5 || sm==6)                   W = analysis->getW2JetsBJet();
    }
    W->SetName("W");

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

    TH1F* data_obs = analysis->getTotalData();
    data_obs->SetName("data_obs");


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

      gROOT->cd();

      //Nominal histos
      TH1F* GG = analysis->getSample(TString("SUSYGG")+ma);
      GG->SetName(TString("ggH")+ma);

      TH1F* BB = analysis->getSample(TString("SUSYBB")+ma);
      BB->SetName(TString("bbH")+ma);

      //Scale signals to 1/pb      
      GG->Scale(1./analysis->findSample(TString("SUSYGG")+ma)->getCrossection());
      BB->Scale(1./analysis->findSample(TString("SUSYBB")+ma)->getCrossection());
      
      
      dir->cd();
      GG->Write();
      BB->Write();
      gROOT->cd();

      delete GG;
      delete BB;

    }

  }
  
  output.ls();
  output.Close();
  gROOT->ProcessLine(".q");
}

//Once the root files are created as above use this function to merge
void mergeDataCardHistosMSSM(Int_t channel, TString tag="svfitmass"){

  TString ChannelName;
  if(channel==1)ChannelName="muTau";
  else if(channel==2)ChannelName="eTau";
  else return;
  

  TFile nominal(ChannelName+"MSSM_"+tag+"_.root","read");
  TFile nominaltUp(ChannelName+"MSSM_"+tag+"_tUp.root","read");
  TFile nominaltDown(ChannelName+"MSSM_"+tag+"_tDown.root","read");

  TFile output(ChannelName+"MSSM_"+tag+".root","recreate");
  for(long sm=0;sm<NCAT;sm++){
    if(sm==4) continue;

    TDirectory* dir = output.mkdir(ChannelName+"_"+catdirname[sm]);  
    dir->cd();


    /////////////Nominal histos:
    TH1F* ZTT = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/ZTT");
    ZTT->SetName("ZTT");

    TH1F* QCD = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/QCD");
    QCD->SetName("QCD");

    TH1F* W = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/W");
    W->SetName("W");

    TH1F* TT = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/TT");
    TT->SetName("TT");

    TH1F* ZL = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/ZL");
    ZL->SetName("ZL");

    TH1F* ZJ = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/ZJ");
    ZJ->SetName("ZJ");

    TH1F* VV = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/VV");
    VV->SetName("VV");

    TH1F* ZLL = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/ZLL");
    ZLL->SetName("ZLL");
 
    TH1F* data_obs = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/data_obs");
    data_obs->SetName("data_obs");


    /////////////tUp histos
    TH1F* ZTT_CMS_scale_tUp =  (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/ZTT");
    ZTT_CMS_scale_tUp->SetName("ZTT_CMS_scale_tUp");

    TH1F* QCD_CMS_scale_tUp = (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/QCD");
    QCD_CMS_scale_tUp->SetName("QCD_CMS_scale_tUp");

    TH1F* W_CMS_scale_tUp =  (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/W");
    W_CMS_scale_tUp->SetName("W_CMS_scale_tUp");

    TH1F* TT_CMS_scale_tUp = (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/TT");
    TT_CMS_scale_tUp->SetName("TT_CMS_scale_tUp");

    TH1F* ZL_CMS_scale_tUp = (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/ZL");
    ZL_CMS_scale_tUp->SetName("ZL_CMS_scale_tUp");

    TH1F* ZJ_CMS_scale_tUp = (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/ZJ");
    ZJ_CMS_scale_tUp->SetName("ZJ_CMS_scale_tUp");

    TH1F* VV_CMS_scale_tUp = (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/VV");
    VV_CMS_scale_tUp->SetName("VV_CMS_scale_tUp");

    TH1F* ZLL_CMS_scale_tUp= (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/ZLL");
    ZLL_CMS_scale_tUp->SetName("ZLL_CMS_scale_tUp");


    /////////////tDown histos
    TH1F* ZTT_CMS_scale_tDown =  (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/ZTT");
    ZTT_CMS_scale_tDown->SetName("ZTT_CMS_scale_tDown");

    TH1F* QCD_CMS_scale_tDown = (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/QCD");
    QCD_CMS_scale_tDown->SetName("QCD_CMS_scale_tDown");

    TH1F* W_CMS_scale_tDown =  (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/W");
    W_CMS_scale_tDown->SetName("W_CMS_scale_tDown");

    TH1F* TT_CMS_scale_tDown = (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/TT");
    TT_CMS_scale_tDown->SetName("TT_CMS_scale_tDown");

    TH1F* ZL_CMS_scale_tDown = (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/ZL");
    ZL_CMS_scale_tDown->SetName("ZL_CMS_scale_tDown");

    TH1F* ZJ_CMS_scale_tDown = (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/ZJ");
    ZJ_CMS_scale_tDown->SetName("ZJ_CMS_scale_tDown");

    TH1F* VV_CMS_scale_tDown = (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/VV");
    VV_CMS_scale_tDown->SetName("VV_CMS_scale_tDown");

    TH1F* ZLL_CMS_scale_tDown= (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/ZLL");
    ZLL_CMS_scale_tDown->SetName("ZLL_CMS_scale_tDown");




    ZTT->Write();
    ZL->Write();
    ZJ->Write();
    ZLL->Write();
    W->Write();
    TT->Write();
    VV->Write();
    QCD->Write();
    data_obs->Write();
 
    ZTT_CMS_scale_tUp->Write();
    ZL_CMS_scale_tUp->Write();
    ZJ_CMS_scale_tUp->Write();
    ZLL_CMS_scale_tUp->Write();
    W_CMS_scale_tUp->Write();
    TT_CMS_scale_tUp->Write();
    VV_CMS_scale_tUp->Write();
    QCD_CMS_scale_tUp->Write();


    ZTT_CMS_scale_tDown->Write();
    ZL_CMS_scale_tDown->Write();
    ZJ_CMS_scale_tDown->Write();
    ZLL_CMS_scale_tDown->Write();
    W_CMS_scale_tDown->Write();
    TT_CMS_scale_tDown->Write();
    VV_CMS_scale_tDown->Write();
    QCD_CMS_scale_tDown->Write();


    for(Int_t m=0;m<NMASS;m++){
      long ma=massValues[m];
      
      cout<<ChannelName+"_"+catdirname[sm]<<" "<<ma<<endl;

      //Nominal histos
      TH1F* GG = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/ggH"+ma);
      GG->SetName(TString("ggH")+ma);

      TH1F* BB =  (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/bbH"+ma);
      BB->SetName(TString("bbH")+ma);
     
      //tUp histos
      TH1F* GG_CMS_scale_tUp  = (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/ggH"+ma);
      GG_CMS_scale_tUp->SetName(TString("ggH")+ma+"_CMS_scale_tUp");
      
      TH1F* BB_CMS_scale_tUp  =  (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/bbH"+ma);
      BB_CMS_scale_tUp->SetName(TString("bbH")+ma+"_CMS_scale_tUp");

     
      //tDown histos
      TH1F* GG_CMS_scale_tDown  = (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/ggH"+ma);
      GG_CMS_scale_tDown->SetName(TString("ggH")+ma+"_CMS_scale_tDown");
      
      TH1F* BB_CMS_scale_tDown  =  (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/bbH"+ma);
      if(!BB_CMS_scale_tDown){cout<<ma<<" "<<ChannelName+"_"+catdirname[sm]+"/bbH"+ma<<endl; return 0;}
      BB_CMS_scale_tDown->SetName(TString("bbH")+ma+"_CMS_scale_tDown");


      GG->Write();
      BB->Write();

      GG_CMS_scale_tUp->Write();
      BB_CMS_scale_tUp->Write();

      GG_CMS_scale_tDown->Write();
      BB_CMS_scale_tDown->Write();


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

  TCanvas C("plotDataCard");
  C.Print("plotDataCard.ps[");

  for(long sm=0;sm<NCAT;sm++){
    if(sm==4) continue;

    TH1F* ZTT = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/ZTT");
    ZTT->SetName("ZTT");

    TH1F* QCD = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/QCD");
    QCD->SetName("QCD");

    TH1F* W = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/W");
    W->SetName("W");

    TH1F* TT = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/TT");
    TT->SetName("TT");

    TH1F* ZL = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/ZL");
    ZL->SetName("ZL");

    TH1F* ZJ = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/ZJ");
    ZJ->SetName("ZJ");

    TH1F* VV = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/VV");
    VV->SetName("VV");

    TH1F* data_obs = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/data_obs");
    data_obs->SetName("data_obs");

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


    C.Clear();
    if(MC->GetMaximum()>data_obs->GetMaximum())
      MC->GetYaxis()->SetRangeUser(0,1.2*MC->GetMaximum());
    else
      MC->GetYaxis()->SetRangeUser(0,1.2*data_obs->GetMaximum());
    MC->Draw("hist");
    data_obs->Draw("histpesame");
    C.Print("plotDataCard.ps");
    delete MC;
 
  }
  
  C.Print("plotDataCard.ps]");
 
  nominal.Close();
}
