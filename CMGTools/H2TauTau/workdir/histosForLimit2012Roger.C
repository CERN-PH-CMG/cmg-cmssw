#include <TF1.h>
#include <TString.h>
#include <TH1F.h>
#include <TFile.h>

#define NCAT 5
#define smearres 20. //resolution svfitmass

void histosForLimit2012Roger(TString path=""){

  gROOT->ProcessLine(".L ./tauMuConfig2011Reload.C");

  //TString path="/data/benitezj/Samples/TauMuV541June2_MVA";
  //TString path="/data/benitezj/Samples/TauMuV541June2_pZ";
  //TString path="/data/benitezj/Samples/TauMuV541June2_PFMET";

  TauMuPlotter*analysis=tauMuConfig("analysis",path);
  analysis->plotvar_="svfitmass";
  analysis->nbins_=20;
  analysis->xmin_=0.;
  analysis->xmax_=500.;
  analysis->Isocat_=1;
  analysis->MTcat_=1;
  analysis->setSmearHistoRes(smearres);

  TString catdirname[NCAT];
  catdirname[0]="muTau_0jet_low";
  catdirname[1]="muTau_0jet_high";
  catdirname[2]="muTau_boost_low";
  catdirname[3]="muTau_boost_high";
  catdirname[4]="muTau_vbf";

  Int_t nbins[NCAT];
  nbins[0]=50;
  nbins[1]=50;
  nbins[2]=50;
  nbins[3]=25;
  nbins[4]=20;

  
  TFile output(path+"/muTauSM.root","recreate");
  for(long sm=0;sm<NCAT;sm++){

    TDirectory* dir = output.mkdir(catdirname[sm]);  

    analysis->extrasel_ = analysis->getSMcut(sm);
    analysis->nbins_=nbins[sm];
    if(!analysis->scaleSamplesLumi())return 0;
    if(!analysis->correctSamplesInc())return 0;
  
    gROOT->cd();

    TH1F* ZTT = analysis->getZToTauTau();
    ZTT->SetName("ZTT");//rename otherwise default name may conflict below

    TH1F* QCD = 0;
    if(sm==0||sm==1||sm==2||sm==3)QCD=analysis->getQCDInc();
    if(sm==4)QCD=analysis->getQCDIsoSM();
    QCD->SetName("QCD");//rename otherwise default name may conflict below

    TH1F* W = 0;
    if(sm==0||sm==1||sm==2||sm==3) W = analysis->getWJetsInc();
    if(sm==4) W = analysis->getWJetsTauIsoSM();
    W->SetName("W");//rename otherwise default name may conflict below

    TH1F* TT = analysis->getTTJetsInc();
    TT->SetName("TT");//rename otherwise default name may conflict below

    TH1F* ZL = analysis->getZToMuMuInc();
    ZL->SetName("ZL");//rename otherwise default name may conflict below

    TH1F* ZJ = analysis->getZToLJetInc();
    ZJ->SetName("ZJ");//rename otherwise default name may conflict below  

    TH1F* VV = analysis->getDiBoson();
    VV->SetName("VV");//rename otherwise default name may conflict below

    //TH1F* data_obs = analysis->getTotalData();
    TH1F* data_obs = ZTT->Clone();//blind for now 
    data_obs->Add(QCD); data_obs->Add(W); data_obs->Add(TT); data_obs->Add(ZL); data_obs->Add(ZJ); data_obs->Add(VV);
    data_obs->Scale(((int)data_obs->Integral())/data_obs->Integral());
    for(Int_t b=1;b<=data_obs->GetXaxis()->GetNbins();b++)
      data_obs->SetBinContent(b,(int)(data_obs->GetBinContent(b)+0.5));
    data_obs->SetName("data_obs");//rename otherwise default name may conflict below


    dir->cd();

    ZTT ->Write();
    ZL->Write();
    ZJ->Write();
    W->Write();
    TT->Write();
    VV->Write();
    QCD->Write();
    data_obs->Write();

    
    gROOT->cd();

    //   ZTT_CMS_scale_tUp ->Write();
    //   ZL_CMS_scale_tUp->Write();
    //   ZJ_CMS_scale_tUp->Write();
    //   W_CMS_scale_tUp->Write();
    //   TT_CMS_scale_tUp->Write();
    //   VV_CMS_scale_tUp->Write();

    //   ZTT_CMS_scale_tDown ->Write();
    //   ZL_CMS_scale_tDown->Write();
    //   ZJ_CMS_scale_tDown->Write();
    //   W_CMS_scale_tDown->Write();
    //   TT_CMS_scale_tDown->Write();
    //   VV_CMS_scale_tDown->Write();

    delete ZTT ;
    delete ZL;
    delete ZJ;
    delete W;
    delete TT;
    delete VV;
    delete QCD;
    delete data_obs;



    //   delete ZTT_CMS_scale_tUp ;
    //   delete ZL_CMS_scale_tUp;
    //   delete ZJ_CMS_scale_tUp;
    //   delete W_CMS_scale_tUp;
    //   delete TT_CMS_scale_tUp;
    //   delete VV_CMS_scale_tUp;

    //   delete ZTT_CMS_scale_tDown ;
    //   delete ZL_CMS_scale_tDown;
    //   delete ZJ_CMS_scale_tDown;
    //   delete W_CMS_scale_tDown;
    //   delete TT_CMS_scale_tDown;
    //   delete VV_CMS_scale_tDown;
  


    for(long ma=110;ma<=145;ma+=5){

      gROOT->cd();
      TH1F* SM = analysis->getSample(TString("HiggsGG")+ma);
      SM->SetName(TString("ggH")+ma);//rename otherwise default name may conflict below

      TH1F* VBF = analysis->getSample(TString("HiggsVBF")+ma);
      VBF->SetName(TString("qqH")+ma);//rename otherwise default name may conflict below

      TH1F* VH = analysis->getSample(TString("HiggsVH")+ma);
      VH->SetName(TString("VH")+ma);//rename otherwise default name may conflict below

      dir->cd();
      VH->Write();
      SM->Write();
      VBF->Write();

      gROOT->cd();

    //   VH_CMS_scale_tUp->Write();
    //   SM_CMS_scale_tUp->Write();
    //   VBF_CMS_scale_tUp->Write();

    //   VH_CMS_scale_tDown->Write();
    //   SM_CMS_scale_tDown->Write();
    //   VBF_CMS_scale_tDown->Write();

      delete VH;
      delete SM;
      delete VBF;

    //   delete VH_CMS_scale_tUp;
    //   delete SM_CMS_scale_tUp;
    //   delete VBF_CMS_scale_tUp;

    //   delete VH_CMS_scale_tDown;
    //   delete SM_CMS_scale_tDown;
    //   delete VBF_CMS_scale_tDown;


    }



  }
  

  output.Close();
  gROOT->ProcessLine(".q");
}



//   VH_CMS_scale_tUp->Scale(VH->Integral()/VH_CMS_scale_tUp->Integral());
//   SM_CMS_scale_tUp->Scale(SM->Integral()/SM_CMS_scale_tUp->Integral());
//   VBF_CMS_scale_tUp->Scale(VBF->Integral()/VBF_CMS_scale_tUp->Integral());
//   ZTT_CMS_scale_tUp->Scale(ZTT->Integral()/ZTT_CMS_scale_tUp->Integral());
//   ZL_CMS_scale_tUp->Scale(ZL->Integral()/ZL_CMS_scale_tUp->Integral());
//   ZJ_CMS_scale_tUp->Scale(ZJ->Integral()/ZJ_CMS_scale_tUp->Integral());
//   W_CMS_scale_tUp->Scale(W->Integral()/W_CMS_scale_tUp->Integral());
//   TT_CMS_scale_tUp->Scale(TT->Integral()/TT_CMS_scale_tUp->Integral());
//   VV_CMS_scale_tUp->Scale(VV->Integral()/VV_CMS_scale_tUp->Integral());

//   VH_CMS_scale_tDown->Scale(VH->Integral()/VH_CMS_scale_tDown->Integral());
//   SM_CMS_scale_tDown->Scale(SM->Integral()/SM_CMS_scale_tDown->Integral());
//   VBF_CMS_scale_tDown->Scale(VBF->Integral()/VBF_CMS_scale_tDown->Integral());
//   ZTT_CMS_scale_tDown->Scale(ZTT->Integral()/ZTT_CMS_scale_tDown->Integral());
//   ZL_CMS_scale_tDown->Scale(ZL->Integral()/ZL_CMS_scale_tDown->Integral());
//   ZJ_CMS_scale_tDown->Scale(ZJ->Integral()/ZJ_CMS_scale_tDown->Integral());
//   W_CMS_scale_tDown->Scale(W->Integral()/W_CMS_scale_tDown->Integral());
//   TT_CMS_scale_tDown->Scale(TT->Integral()/TT_CMS_scale_tDown->Integral());
//   VV_CMS_scale_tDown->Scale(VV->Integral()/VV_CMS_scale_tDown->Integral());
