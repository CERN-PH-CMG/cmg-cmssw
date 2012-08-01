#include <TString.h>
#include <TFile.h>
#include <TH1F.h>
#include <TDirectory.h>
#include "TauMuPlotter.h"
#include "configTauMu2011.C"
#include "TauElePlotter.h"
#include "configTauEle2011.C"

#define NMASS 8
#define NCAT 5

long massValues[NMASS]={110,115,120,125,130,135,140,145};

TString catdirname[NCAT]={
 "0jet_low",
 "0jet_high",
 "boost_low",
 "boost_high",
 "vbf"};


void mergeDataCardsSM(Int_t channel, TString tag){

  TString ChannelName;
  if(channel==1)ChannelName="muTau";
  else if(channel==2)ChannelName="eTau";
  else return;
  

  TFile nominal(ChannelName+"SM_"+tag+"_.root","read");
  TFile nominaltUp(ChannelName+"SM_"+tag+"_tUp.root","read");
  TFile nominaltDown(ChannelName+"SM_"+tag+"_tDown.root","read");

  TFile output(ChannelName+"SM_"+tag+".root","recreate");
  for(long sm=0;sm<NCAT;sm++){

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
      TH1F* SM = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/ggH"+ma);
      SM->SetName(TString("ggH")+ma);

      TH1F* VBF =  (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/qqH"+ma);
      VBF->SetName(TString("qqH")+ma);
      
      TH1F* VH =  (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/VH"+ma);
      VH->SetName(TString("VH")+ma);
     
      //tUp histos
      TH1F* SM_CMS_scale_tUp  = (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/ggH"+ma);
      SM_CMS_scale_tUp->SetName(TString("ggH")+ma+"_CMS_scale_tUp");
      
      TH1F* VBF_CMS_scale_tUp  =  (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/qqH"+ma);
      VBF_CMS_scale_tUp->SetName(TString("qqH")+ma+"_CMS_scale_tUp");

      TH1F* VH_CMS_scale_tUp  =  (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/VH"+ma);
      VH_CMS_scale_tUp->SetName(TString("VH")+ma+"_CMS_scale_tUp");

     
      //tDown histos
      TH1F* SM_CMS_scale_tDown  = (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/ggH"+ma);
      SM_CMS_scale_tDown->SetName(TString("ggH")+ma+"_CMS_scale_tDown");
      
      TH1F* VBF_CMS_scale_tDown  =  (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/qqH"+ma);
      VBF_CMS_scale_tDown->SetName(TString("qqH")+ma+"_CMS_scale_tDown");

      TH1F* VH_CMS_scale_tDown  =  (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/VH"+ma);
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


