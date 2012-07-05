#include <TString.h>
#include <TFile.h>
#include <TH1F.h>
#include <TDirectory.h>
#include "TauMuPlotter.h"
#include "tauMuConfig2011Reload.C"
#include "TauElePlotter.h"
#include "tauEleConfig2011Reload.C"

long massValues[8]={110,115,120,125,130,135,140,145};

TString catdirname[5]={
 "0jet_low",
 "0jet_high",
 "boost_low",
 "boost_high",
 "vbf"};

void histosForDataCardSM(Int_t channel,TString path,TString tag){

  //TString path="/data/benitezj/Samples/TauMuV541June2_TrigEff";

  if(tag!="")path=path+"_"+tag;

  TString ChannelName;
  if(channel==1)ChannelName="muTau";
  else if(channel==2)ChannelName="eTau";
  else return;
  cout<<"ChannelName "<<ChannelName<<endl;

  Float_t xbinsValues[13+1]={0,20,40,60,80,100,120,140,160,180,200,250,300,350};
  
  if(channel==1)TauMuPlotter * analysis=tauMuConfig2011Reload("analysis",path);
  if(channel==2)TauElePlotter * analysis=tauEleConfig2011Reload("analysis",path);
  
  // analysis->plotvar_="svfitmass";
  analysis->plotvar_="ditaumass";
  analysis->setVariableBinning(13,xbinsValues);
  analysis->nbins_=0;
  analysis->Isocat_=1;
  analysis->MTcat_=1; 
  analysis->setZTTType(2);

  
  TFile output(ChannelName+"SM"+tag+".root","recreate");
  for(long sm=0; sm<5; sm++){

    TDirectory* dir = output.mkdir(ChannelName+"_"+catdirname[sm]);  

    analysis->extrasel_ = analysis->getSMcut(sm);
    if(!analysis->scaleSamplesLumi())return;

    gROOT->cd();

    TH1F* ZTT = analysis->getZToTauTau();
    ZTT->SetName("ZTT");


    TH1F* QCD = 0;
    if(channel==1){//need to sync this channel with e-tau
      if(sm==0 || sm==1) QCD=analysis->getQCDInc();
      if(sm==2 || sm==3) QCD=analysis->getQCDInc();
      if(sm==4)          QCD=analysis->getQCDMike();
    }
    if(channel==2){
      if(sm==0 || sm==1) QCD=analysis->getQCDInc();
      if(sm==2 || sm==3) QCD=analysis->getQCDInc();
      if(sm==4)          QCD=analysis->getQCDMike();
    }
    QCD->SetName("QCD");


    TH1F* W = 0;
    if(channel==1){//need to sync this channel with e-tau
      if(sm==0 || sm==1) W = analysis->getWJetsInc();
      if(sm==2 || sm==3) W = analysis->getWJetsInc();
      if(sm==4)          W = analysis->getW3JetsVBF();
    }
    if(channel==2){
      if(sm==0 || sm==1) W = analysis->getWJetsInc();
      if(sm==2 || sm==3) W = analysis->getWJetsInc();
      if(sm==4)          W = analysis->getW3JetsVBF();
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


    for(Int_t m=0;m<0;m++){
      long ma=massValues[m];


      //Nominal h 
      TH1F* SM = analysis->getSample(TString("HiggsGG")+ma);
      SM->SetName(TString("ggH")+ma);//rename otherwise default name may conflict below

      TH1F* VBF = analysis->getSample(TString("HiggsVBF")+ma);
      VBF->SetName(TString("qqH")+ma);//rename otherwise default name may conflict below

      TH1F* VH = analysis->getSample(TString("HiggsVH")+ma);
      VH->SetName(TString("VH")+ma);//rename otherwise default name may conflict below

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

//Once the root files are created as above use this function to merge
void mergeDataCardHistosMSSM(Int_t channel){

  TString ChannelName;
  if(channel==1)ChannelName="muTau";
  else if(channel==2)ChannelName="eTau";
  else return;
  

  TFile nominal(ChannelName+"MSSM.root","read");
  TFile nominaltUp(ChannelName+"MSSMtUp.root","read");
  TFile nominaltDown(ChannelName+"MSSMtDown.root","read");

  TFile output(ChannelName+"MSSM_Merge.root","recreate");
  for(long sm=0;sm<5;sm++){

    TDirectory* dir = output.mkdir(ChannelName+"_"+catdirname[sm]);  
    dir->cd();


    /////////////Nominal histos:
    TH1F* ZTT = nominal.Get(ChannelName+"_"+catdirname[sm]+"/ZTT");
    ZTT->SetName("ZTT");

    TH1F* QCD = nominal.Get(ChannelName+"_"+catdirname[sm]+"/QCD");
    QCD->SetName("QCD");

    TH1F* W = nominal.Get(ChannelName+"_"+catdirname[sm]+"/W");
    W->SetName("W");

    TH1F* TT = nominal.Get(ChannelName+"_"+catdirname[sm]+"/TT");
    TT->SetName("TT");

    TH1F* ZL = nominal.Get(ChannelName+"_"+catdirname[sm]+"/ZL");
    ZL->SetName("ZL");

    TH1F* ZJ = nominal.Get(ChannelName+"_"+catdirname[sm]+"/ZJ");
    ZJ->SetName("ZJ");

    TH1F* VV = nominal.Get(ChannelName+"_"+catdirname[sm]+"/VV");
    VV->SetName("VV");

    TH1F* ZLL = nominal.Get(ChannelName+"_"+catdirname[sm]+"/ZLL");
    ZLL->SetName("ZLL");
 
    TH1F* data_obs = nominal.Get(ChannelName+"_"+catdirname[sm]+"/data_obs");
    data_obs->SetName("data_obs");


    /////////////tUp histos
    TH1F* ZTT_CMS_scale_tUp =  nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/ZTT");
    ZTT_CMS_scale_tUp->SetName("ZTT_CMS_scale_tUp");

    TH1F* QCD_CMS_scale_tUp = nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/QCD");
    QCD_CMS_scale_tUp->SetName("QCD_CMS_scale_tUp");

    TH1F* W_CMS_scale_tUp =  nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/W");
    W_CMS_scale_tUp->SetName("W_CMS_scale_tUp");

    TH1F* TT_CMS_scale_tUp = nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/TT");
    TT_CMS_scale_tUp->SetName("TT_CMS_scale_tUp");

    TH1F* ZL_CMS_scale_tUp = nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/ZL");
    ZL_CMS_scale_tUp->SetName("ZL_CMS_scale_tUp");

    TH1F* ZJ_CMS_scale_tUp = nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/ZJ");
    ZJ_CMS_scale_tUp->SetName("ZJ_CMS_scale_tUp");

    TH1F* VV_CMS_scale_tUp = nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/VV");
    VV_CMS_scale_tUp->SetName("VV_CMS_scale_tUp");

    TH1F* ZLL_CMS_scale_tUp= nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/ZLL");
    ZLL_CMS_scale_tUp->SetName("ZLL_CMS_scale_tUp");


    /////////////tDown histos
    TH1F* ZTT_CMS_scale_tDown =  nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/ZTT");
    ZTT_CMS_scale_tDown->SetName("ZTT_CMS_scale_tDown");

    TH1F* QCD_CMS_scale_tDown = nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/QCD");
    QCD_CMS_scale_tDown->SetName("QCD_CMS_scale_tDown");

    TH1F* W_CMS_scale_tDown =  nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/W");
    W_CMS_scale_tDown->SetName("W_CMS_scale_tDown");

    TH1F* TT_CMS_scale_tDown = nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/TT");
    TT_CMS_scale_tDown->SetName("TT_CMS_scale_tDown");

    TH1F* ZL_CMS_scale_tDown = nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/ZL");
    ZL_CMS_scale_tDown->SetName("ZL_CMS_scale_tDown");

    TH1F* ZJ_CMS_scale_tDown = nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/ZJ");
    ZJ_CMS_scale_tDown->SetName("ZJ_CMS_scale_tDown");

    TH1F* VV_CMS_scale_tDown = nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/VV");
    VV_CMS_scale_tDown->SetName("VV_CMS_scale_tDown");

    TH1F* ZLL_CMS_scale_tDown= nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/ZLL");
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


    for(Int_t m=0;m<0;m++){
      long ma=massValues[m];
      
      cout<<ChannelName+"_"+catdirname[sm]<<" "<<ma<<endl;

      //Nominal histos
      TH1F* SM = nominal.Get(ChannelName+"_"+catdirname[sm]+"/ggH"+ma);
      SM->SetName(TString("ggH")+ma);

      TH1F* VBF =  nominal.Get(ChannelName+"_"+catdirname[sm]+"/qqH"+ma);
      VBF->SetName(TString("qqH")+ma);
      
      TH1F* VH =  nominal.Get(ChannelName+"_"+catdirname[sm]+"/VH"+ma);
      VH->SetName(TString("VH")+ma);
     
      //tUp histos
      TH1F* SM_CMS_scale_tUp  = nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/ggH"+ma);
      SM_CMS_scale_tUp->SetName(TString("ggH")+ma+"_CMS_scale_tUp");
      
      TH1F* VBF_CMS_scale_tUp  =  nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/qqH"+ma);
      VBF_CMS_scale_tUp->SetName(TString("qqH")+ma+"_CMS_scale_tUp");

      TH1F* VH_CMS_scale_tUp  =  nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/VH"+ma);
      VH_CMS_scale_tUp->SetName(TString("VH")+ma+"_CMS_scale_tUp");

     
      //tDown histos
      TH1F* SM_CMS_scale_tDown  = nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/ggH"+ma);
      SM_CMS_scale_tDown->SetName(TString("ggH")+ma+"_CMS_scale_tDown");
      
      TH1F* VBF_CMS_scale_tDown  =  nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/qqH"+ma);
      VBF_CMS_scale_tDown->SetName(TString("qqH")+ma+"_CMS_scale_tDown");

      TH1F* VH_CMS_scale_tDown  =  nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/VH"+ma);
      VH_CMS_scale_tDown->SetName(TString("VH")+ma+"_CMS_scale_tDown");


      SM->Write();
      VBF->Write();
      VH->Write();

      SM_CMS_scale_tUp->Write();
      VBF_CMS_scale_tUp->Write();
      VH_CMS_scale_tUp->Write();

      SM_CMS_scale_tDown->Write();
      VBF_CMS_scale_tDown->Write();
      VH_CMS_scale_tDown->Write();


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

  for(long sm=0;sm<5;sm++){

    TH1F* ZTT = nominal.Get(ChannelName+"_"+catdirname[sm]+"/ZTT");
    ZTT->SetName("ZTT");

    TH1F* QCD = nominal.Get(ChannelName+"_"+catdirname[sm]+"/QCD");
    QCD->SetName("QCD");

    TH1F* W = nominal.Get(ChannelName+"_"+catdirname[sm]+"/W");
    W->SetName("W");

    TH1F* TT = nominal.Get(ChannelName+"_"+catdirname[sm]+"/TT");
    TT->SetName("TT");

    TH1F* ZL = nominal.Get(ChannelName+"_"+catdirname[sm]+"/ZL");
    ZL->SetName("ZL");

    TH1F* ZJ = nominal.Get(ChannelName+"_"+catdirname[sm]+"/ZJ");
    ZJ->SetName("ZJ");

    TH1F* VV = nominal.Get(ChannelName+"_"+catdirname[sm]+"/VV");
    VV->SetName("VV");

    TH1F* data_obs = nominal.Get(ChannelName+"_"+catdirname[sm]+"/data_obs");
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
