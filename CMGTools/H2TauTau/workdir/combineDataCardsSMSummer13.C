#include <TString.h>
#include <TFile.h>
#include <TH1F.h>
#include <TDirectory.h>
#include "dataCardSMSummer13.C"

void combineDataCardsSMSummer13(Int_t channel, Int_t cat, TString mass){

  TString ChannelName;
  if(channel==1)ChannelName="muTau";
  else if(channel==2)ChannelName="eleTau";
  else return;
  

  TFile nominal(ChannelName+"_SM"+(long)cat+"_"+mass+"_.root","read");
  TFile nominaltUp(ChannelName+"_SM"+(long)cat+"_"+mass+"_tUp.root","read");
  TFile nominaltDown(ChannelName+"_SM"+(long)cat+"_"+mass+"_tDown.root","read");


  TString scaleUp="CMS_scale_t_mutau_8TeVUp";
  TString scaleDown="CMS_scale_t_mutau_8TeVDown";
  if(channel==2)scaleUp="CMS_scale_t_etau_8TeVUp";
  if(channel==2)scaleDown="CMS_scale_t_etau_8TeVDown";


  TFile output(ChannelName+"_SM"+(long)cat+"_"+mass+".root","recreate");
  for(long sm=0;sm<NCATSUMMER13;sm++){
    if(skipCategory(cat,sm))continue;
    cout<<catdirnameSummer13[sm]<<endl;

    TDirectory* dir = 0;
    if(sm!=11 && sm!=12 ) dir=output.mkdir(ChannelName+"_"+catdirnameSummer13[sm]);  
    else dir=(TDirectory*)output.Get(ChannelName+"_"+catdirnameSummer13[sm]); 
    if(!dir){cout<<" Directory not found "<<endl; return 0;}
    dir->cd();

    TString tag="";
    if(sm==11 || sm==12)tag="_fine_binning";

    /////////////Nominal histos:
    TH1F* ZTT = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ZTT"+tag);
    ZTT->Write(); 
    TH1F* QCD = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/QCD"+tag);
    QCD->Write();
    TH1F* W = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/W"+tag);
    W->Write();
    TH1F* TT = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/TT"+tag);
    TT->Write(); 
    TH1F* ZL = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ZL"+tag);
    ZL->Write(); 
    TH1F* ZJ = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ZJ"+tag);
    ZJ->Write();
    TH1F* VV = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/VV"+tag);
    VV->Write();
    TH1F* ZLL = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ZLL"+tag);
    ZLL->Write();

    if(sm==11 || sm==12)continue;

    TH1F* data_obs = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/data_obs");
    data_obs->Write();

    ///QCD and W shape systematics
    TH1F* QCDUp=(TH1F*)QCD->Clone(TString("QCD_CMS_htt_QCDShape_mutau_")+catdirnameSummer13[sm]+"_8TeVUp");
    TH1F* QCDDown=(TH1F*)QCD->Clone(TString("QCD_CMS_htt_QCDShape_mutau_")+catdirnameSummer13[sm]+"_8TeVDown");
    if(channel==2)QCDUp->SetName(TString("QCD_CMS_htt_QCDShape_etau_")+catdirnameSummer13[sm]+"_8TeVUp");
    if(channel==2)QCDDown->SetName(TString("QCD_CMS_htt_QCDShape_etau_")+catdirnameSummer13[sm]+"_8TeVDown");
    for(Int_t b=1;b<=QCD->GetNbinsX();b++){
      if(QCD->GetBinCenter(b)<=70.){
	QCDUp->SetBinContent(b,1.15*QCDUp->GetBinContent(b));
	QCDDown->SetBinContent(b,0.85*QCDDown->GetBinContent(b));
      }
    }
    QCDUp->Write();
    QCDDown->Write();

    TH1F* WUp=(TH1F*)W->Clone(TString("W_CMS_htt_WShape_mutau_")+catdirnameSummer13[sm]+"_8TeVUp");
    TH1F* WDown=(TH1F*)W->Clone(TString("W_CMS_htt_WShape_mutau_")+catdirnameSummer13[sm]+"_8TeVDown");
    if(channel==2)WUp->SetName(TString("W_CMS_htt_WShape_etau_")+catdirnameSummer13[sm]+"_8TeVUp");
    if(channel==2)WDown->SetName(TString("W_CMS_htt_WShape_etau_")+catdirnameSummer13[sm]+"_8TeVDown");
    for(Int_t b=1;b<=W->GetNbinsX();b++){
      if(W->GetBinCenter(b)<=70.){
	WUp->SetBinContent(b,1.15*WUp->GetBinContent(b));
	WDown->SetBinContent(b,0.85*WDown->GetBinContent(b));
      }
    }
    WUp->Write();
    WDown->Write();


    /////////////tUp histos
    TH1F* ZTT_CMS_scale_tUp =  (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ZTT");
    ZTT_CMS_scale_tUp->SetName(TString("ZTT_")+scaleUp);
    //ZTT_CMS_scale_tUp->Scale(ZTT->Integral()/ZTT_CMS_scale_tUp->Integral());

    TH1F* QCD_CMS_scale_tUp = (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/QCD");
    QCD_CMS_scale_tUp->SetName(TString("QCD_")+scaleUp);

    TH1F* W_CMS_scale_tUp =  (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/W");
    W_CMS_scale_tUp->SetName(TString("W_")+scaleUp);

    TH1F* TT_CMS_scale_tUp = (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/TT");
    TT_CMS_scale_tUp->SetName(TString("TT_")+scaleUp);

    TH1F* ZL_CMS_scale_tUp = (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ZL");
    ZL_CMS_scale_tUp->SetName(TString("ZL_")+scaleUp);

    TH1F* ZJ_CMS_scale_tUp = (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ZJ");
    ZJ_CMS_scale_tUp->SetName(TString("ZJ_")+scaleUp);

    TH1F* VV_CMS_scale_tUp = (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/VV");
    VV_CMS_scale_tUp->SetName(TString("VV_")+scaleUp);

    TH1F* ZLL_CMS_scale_tUp= (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ZLL");
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
    TH1F* ZTT_CMS_scale_tDown =  (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ZTT");
    ZTT_CMS_scale_tDown->SetName(TString("ZTT_")+scaleDown);
    //ZTT_CMS_scale_tDown->Scale(ZTT->Integral()/ZTT_CMS_scale_tDown->Integral());

    TH1F* QCD_CMS_scale_tDown = (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/QCD");
    QCD_CMS_scale_tDown->SetName(TString("QCD_")+scaleDown);

    TH1F* W_CMS_scale_tDown =  (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/W");
    W_CMS_scale_tDown->SetName(TString("W_")+scaleDown);

    TH1F* TT_CMS_scale_tDown = (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/TT");
    TT_CMS_scale_tDown->SetName(TString("TT_")+scaleDown);

    TH1F* ZL_CMS_scale_tDown = (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ZL");
    ZL_CMS_scale_tDown->SetName(TString("ZL_")+scaleDown);

    TH1F* ZJ_CMS_scale_tDown = (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ZJ");
    ZJ_CMS_scale_tDown->SetName(TString("ZJ_")+scaleDown);

    TH1F* VV_CMS_scale_tDown = (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/VV");
    VV_CMS_scale_tDown->SetName(TString("VV_")+scaleDown);

    TH1F* ZLL_CMS_scale_tDown= (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ZLL");
    ZLL_CMS_scale_tDown->SetName(TString("ZLL_")+scaleDown);


    ZTT_CMS_scale_tDown->Write();
    ZL_CMS_scale_tDown->Write();
    ZJ_CMS_scale_tDown->Write();
    ZLL_CMS_scale_tDown->Write();
    W_CMS_scale_tDown->Write();
    TT_CMS_scale_tDown->Write();
    VV_CMS_scale_tDown->Write();
    QCD_CMS_scale_tDown->Write();

    
    if( sm<9 ){
      for(Int_t m=0;m<NMASS;m++){
	long ma=massValues[m];
      
	cout<<ChannelName+"_"+catdirnameSummer13[sm]<<" "<<ma<<endl;

	//Nominal histos
	TH1F* SM = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ggH"+ma);
	TH1F* SMUp = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ggH"+ma+"_QCDscale_ggH1inUp");
	TH1F* SMDown = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ggH"+ma+"_QCDscale_ggH1inDown");
	TH1F* VBF =  (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/qqH"+ma);
	TH1F* VH =  (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/VH"+ma);
     
	//tUp histos
	TH1F* SM_CMS_scale_tUp  = (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ggH"+ma);
	SM_CMS_scale_tUp->SetName(TString("ggH")+ma+"_"+scaleUp);
      
	TH1F* VBF_CMS_scale_tUp  =  (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/qqH"+ma);
	VBF_CMS_scale_tUp->SetName(TString("qqH")+ma+"_"+scaleUp);

	TH1F* VH_CMS_scale_tUp  =  (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/VH"+ma);
	VH_CMS_scale_tUp->SetName(TString("VH")+ma+"_"+scaleUp);

     
	//tDown histos
	TH1F* SM_CMS_scale_tDown  = (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ggH"+ma);
	SM_CMS_scale_tDown->SetName(TString("ggH")+ma+"_"+scaleDown);
      
	TH1F* VBF_CMS_scale_tDown  =  (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/qqH"+ma);
	VBF_CMS_scale_tDown->SetName(TString("qqH")+ma+"_"+scaleDown);

	TH1F* VH_CMS_scale_tDown  =  (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/VH"+ma);
	VH_CMS_scale_tDown->SetName(TString("VH")+ma+"_"+scaleDown);


	SM->Write();
	SMUp->Write();
	SMDown->Write();
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


    if( sm>=9 && sm<=10){
      for(Int_t m=0;m<NSUSY;m++){
	long ma=SUSYMass[m];

	cout<<ChannelName+"_"+catdirnameSummer13[sm]<<" "<<ma<<endl;

	//Nominal histos
	TH1F* GG = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ggH"+ma);
 	TH1F* BB =  (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/bbH"+ma);
     
	//tUp histos
	TH1F* GG_CMS_scale_tUp  = (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ggH"+ma);
	GG_CMS_scale_tUp->SetName(TString("ggH")+ma+"_"+scaleUp);
      
	TH1F* BB_CMS_scale_tUp  =  (TH1F*)nominaltUp.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/bbH"+ma);
	BB_CMS_scale_tUp->SetName(TString("bbH")+ma+"_"+scaleUp);

     
	//tDown histos
	TH1F* GG_CMS_scale_tDown  = (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ggH"+ma);
	GG_CMS_scale_tDown->SetName(TString("ggH")+ma+"_"+scaleDown);
      
	TH1F* BB_CMS_scale_tDown  =  (TH1F*)nominaltDown.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/bbH"+ma);
	BB_CMS_scale_tDown->SetName(TString("bbH")+ma+"_"+scaleDown);


	GG->Write();
	BB->Write();

	GG_CMS_scale_tUp->Write();
	BB_CMS_scale_tUp->Write();

	GG_CMS_scale_tDown->Write();
	BB_CMS_scale_tDown->Write();

      }

    }


  }
  

  output.ls();
  output.Close();


  gROOT->ProcessLine(".q");
}


