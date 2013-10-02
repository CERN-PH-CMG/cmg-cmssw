#include <TString.h>
#include <TFile.h>
#include <TH1F.h>
#include <TDirectory.h>
#include "constants.h"

void mergeDataCardsSM(Int_t channel, TString mass){

  TString ChannelName;
  if(channel==1)ChannelName="muTau";
  else if(channel==2)ChannelName="eleTau";
  else return;
  

  TFile nominal(ChannelName+"SM_"+mass+"_.root","read");
  TFile nominaltUp(ChannelName+"SM_"+mass+"_tUp.root","read");
  TFile nominaltDown(ChannelName+"SM_"+mass+"_tDown.root","read");


  TString scaleUp="CMS_scale_t_mutau_8TeVUp";
  TString scaleDown="CMS_scale_t_mutau_8TeVDown";
  if(channel==2)scaleUp="CMS_scale_t_etau_8TeVUp";
  if(channel==2)scaleDown="CMS_scale_t_etau_8TeVDown";


  TFile output(ChannelName+"SM_"+mass+".root","recreate");
  for(long sm=0;sm<NCAT;sm++){

    cout<<catdirname[sm]<<endl;

    TDirectory* dir = output.mkdir(ChannelName+"_"+catdirname[sm]);  
    dir->cd();


    /////////////Nominal histos:
    TH1F* ZTT = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/ZTT");
    TH1F* QCD = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/QCD");
    TH1F* W = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/W");
    TH1F* TT = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/TT");
    TH1F* ZL = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/ZL");
    TH1F* ZJ = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/ZJ");
    TH1F* VV = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/VV");
    TH1F* ZLL = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/ZLL");
    TH1F* data_obs = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/data_obs");

    ZTT->Write();
    ZL->Write();
    ZJ->Write();
    ZLL->Write();
    W->Write();
    TT->Write();
    VV->Write();
    QCD->Write();
    data_obs->Write();

    //copy resolution shifts
    TH1F* ZTT_resDown = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/ZTT_resDown");
    TH1F* ZTT_resUp = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/ZTT_resUp");
    ZTT_resDown->Write();
    ZTT_resUp->Write();

    /////////////tUp histos
    TH1F* ZTT_CMS_scale_tUp =  (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/ZTT");
    ZTT_CMS_scale_tUp->SetName(TString("ZTT_")+scaleUp);

    TH1F* QCD_CMS_scale_tUp = (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/QCD");
    QCD_CMS_scale_tUp->SetName(TString("QCD_")+scaleUp);

    TH1F* W_CMS_scale_tUp =  (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/W");
    W_CMS_scale_tUp->SetName(TString("W_")+scaleUp);

    TH1F* TT_CMS_scale_tUp = (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/TT");
    TT_CMS_scale_tUp->SetName(TString("TT_")+scaleUp);

    TH1F* ZL_CMS_scale_tUp = (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/ZL");
    ZL_CMS_scale_tUp->SetName(TString("ZL_")+scaleUp);

    TH1F* ZJ_CMS_scale_tUp = (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/ZJ");
    ZJ_CMS_scale_tUp->SetName(TString("ZJ_")+scaleUp);

    TH1F* VV_CMS_scale_tUp = (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/VV");
    VV_CMS_scale_tUp->SetName(TString("VV_")+scaleUp);

    TH1F* ZLL_CMS_scale_tUp= (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/ZLL");
    ZLL_CMS_scale_tUp->SetName(TString("ZLL_")+scaleUp);

    ZTT_CMS_scale_tUp->Write();
    ZL_CMS_scale_tUp->Write();
    ZJ_CMS_scale_tUp->Write();
    ZLL_CMS_scale_tUp->Write();
    W_CMS_scale_tUp->Write();
    TT_CMS_scale_tUp->Write();
    VV_CMS_scale_tUp->Write();
    QCD_CMS_scale_tUp->Write();


    /////////////tDown histos
    TH1F* ZTT_CMS_scale_tDown =  (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/ZTT");
    ZTT_CMS_scale_tDown->SetName(TString("ZTT_")+scaleDown);

    TH1F* QCD_CMS_scale_tDown = (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/QCD");
    QCD_CMS_scale_tDown->SetName(TString("QCD_")+scaleDown);

    TH1F* W_CMS_scale_tDown =  (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/W");
    W_CMS_scale_tDown->SetName(TString("W_")+scaleDown);

    TH1F* TT_CMS_scale_tDown = (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/TT");
    TT_CMS_scale_tDown->SetName(TString("TT_")+scaleDown);

    TH1F* ZL_CMS_scale_tDown = (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/ZL");
    ZL_CMS_scale_tDown->SetName(TString("ZL_")+scaleDown);

    TH1F* ZJ_CMS_scale_tDown = (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/ZJ");
    ZJ_CMS_scale_tDown->SetName(TString("ZJ_")+scaleDown);

    TH1F* VV_CMS_scale_tDown = (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/VV");
    VV_CMS_scale_tDown->SetName(TString("VV_")+scaleDown);

    TH1F* ZLL_CMS_scale_tDown= (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/ZLL");
    ZLL_CMS_scale_tDown->SetName(TString("ZLL_")+scaleDown);


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
      SM_CMS_scale_tUp->SetName(TString("ggH")+ma+"_"+scaleUp);
      
      TH1F* VBF_CMS_scale_tUp  =  (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/qqH"+ma);
      VBF_CMS_scale_tUp->SetName(TString("qqH")+ma+"_"+scaleUp);

      TH1F* VH_CMS_scale_tUp  =  (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirname[sm]+"/VH"+ma);
      VH_CMS_scale_tUp->SetName(TString("VH")+ma+"_"+scaleUp);

     
      //tDown histos
      TH1F* SM_CMS_scale_tDown  = (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/ggH"+ma);
      SM_CMS_scale_tDown->SetName(TString("ggH")+ma+"_"+scaleDown);
      
      TH1F* VBF_CMS_scale_tDown  =  (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/qqH"+ma);
      VBF_CMS_scale_tDown->SetName(TString("qqH")+ma+"_"+scaleDown);

      TH1F* VH_CMS_scale_tDown  =  (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirname[sm]+"/VH"+ma);
      VH_CMS_scale_tDown->SetName(TString("VH")+ma+"_"+scaleDown);


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
  


  //for 1Jet_low need to add a shape systematic for QCD 
  TH1F* QCD = (TH1F*)nominal.Get(ChannelName+"_boost_low/QCD");
  TH1F* QCDUp=(TH1F*)QCD->Clone("QCD_CMS_htt_QCDShape_mutau_boost_low_8TeVUp");
  TH1F* QCDDown=(TH1F*)QCD->Clone("QCD_CMS_htt_QCDShape_mutau_boost_low_8TeVDown");
  for(Int_t b=1;b<=QCD->GetNbinsX();b++){
    if(QCD->GetBinCenter(b)<=70.){
      QCDUp->SetBinContent(b,1.15*QCDUp->GetBinContent(b));
      QCDDown->SetBinContent(b,0.85*QCDDown->GetBinContent(b));
    }
  }
  output.cd("muTau_boost_low");
  QCDUp->Write();
  QCDDown->Write();

  output.ls();
  output.Close();


  gROOT->ProcessLine(".q");
}


