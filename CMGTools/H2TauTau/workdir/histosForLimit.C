#include <TF1.h>
#include <TString.h>
#include <TH1F.h>
#include <TFile.h>

void histosForLimit(long sm=0, long mass=0){


  //gROOT->ProcessLine(".L ./tauMuConfig.C");
  gROOT->ProcessLine(".L ./tauMuConfig2011Reload.C");
  TauMuPlotter *analysis =tauMuConfig("analysis");
  analysis->plotvar_="svfitmass";
  analysis->nbins_=30;
  analysis->xmin_=0.;
  analysis->xmax_=300.;
  analysis->Isocat_=1;
  //analysis->Isocat_=0; analysis->extrasel_="(((abs(mueta)<1.479)*(muisomva>0.96)+(abs(mueta)>1.479)*(muisomva>0.95))&&tauisodisc>=2)";
  //analysis->Isocat_=0; analysis->extrasel_="(((abs(mueta)<1.479)*(muisomva>0.96)+(abs(mueta)>1.479)*(muisomva>0.95))&&tauisomva>0.884)";
  analysis->MTcat_=1;
  analysis->SMcat_=sm;
  if(!analysis->scaleSamplesLumi())return 0;
  if(!analysis->correctSamplesInc())return 0;

  
  TH1F* ZTT = analysis->getZToTauTau();
  ZTT->SetName("ZTT");//rename otherwise default name may conflict below

  TH1F* ZL = analysis->getZToMuMuInc();
  ZL->SetName("ZL");//rename otherwise default name may conflict below

  TH1F* ZJ = analysis->getZToLJetInc();
  ZJ->SetName("ZJ");//rename otherwise default name may conflict below

  TH1F* W = 0;
  if(sm==0) W = analysis->getWJetsInc();
  if(sm==1 || sm==2) W = analysis->getWJetsTauIsoSM();
  W->SetName("W");//rename otherwise default name may conflict below

  TH1F* TT = analysis->getTTJetsInc();
  TT->SetName("TT");//rename otherwise default name may conflict below

  TH1F* VV = analysis->getDiBoson();
  VV->SetName("VV");//rename otherwise default name may conflict below

  TH1F* QCD = 0;
  if(sm==0)QCD=analysis->getQCDInc();
  if(sm==1 || sm ==2)QCD=analysis->getQCDIsoSM();
  QCD->SetName("QCD");//rename otherwise default name may conflict below
  
  TH1F* data_obs = analysis->getTotalData();;
  data_obs->SetName("data_obs");//rename otherwise default name may conflict below


  for(long ma=110;ma<=145;ma+=5){
    if(ma==mass||mass==0){

      TH1F* VH = analysis->getSample(TString("HiggsVH")+ma);
      VH->SetName(TString("VH")+ma);//rename otherwise default name may conflict below

      TH1F* SM = analysis->getSample(TString("HiggsGG")+ma);
      SM->SetName(TString("SM")+ma);//rename otherwise default name may conflict below

      TH1F* VBF = analysis->getSample(TString("HiggsVBF")+ma);
      VBF->SetName(TString("VBF")+ma);//rename otherwise default name may conflict below
  
      TString rootfilename=TString("muTau/muTau_SM")+(long)sm+"_mH"+(long)ma+".root";
      //create root file and data card 
      TFile output(rootfilename,"recreate");
      TDirectory* dir = output.mkdir((TString("muTau_SM")+sm).Data());  
      dir->cd();

      VH->Write();
      SM->Write();
      VBF->Write();
      ZTT ->Write();
      ZL->Write();
      ZJ->Write();
      W->Write();
      TT->Write();
      VV->Write();
      QCD->Write();
      data_obs->Write();

      //   ////Not sure the reprocessings used the same number of events ==> normalize to nominal for now
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

      //   VH_CMS_scale_tUp->Write();
      //   SM_CMS_scale_tUp->Write();
      //   VBF_CMS_scale_tUp->Write();
      //   ZTT_CMS_scale_tUp ->Write();
      //   ZL_CMS_scale_tUp->Write();
      //   ZJ_CMS_scale_tUp->Write();
      //   W_CMS_scale_tUp->Write();
      //   TT_CMS_scale_tUp->Write();
      //   VV_CMS_scale_tUp->Write();

      //   VH_CMS_scale_tDown->Write();
      //   SM_CMS_scale_tDown->Write();
      //   VBF_CMS_scale_tDown->Write();
      //   ZTT_CMS_scale_tDown ->Write();
      //   ZL_CMS_scale_tDown->Write();
      //   ZJ_CMS_scale_tDown->Write();
      //   W_CMS_scale_tDown->Write();
      //   TT_CMS_scale_tDown->Write();
      //   VV_CMS_scale_tDown->Write();

      output.Close();

      delete VH;
      delete SM;
      delete VBF;
    }
  }
  

//   delete analysis;
//   delete analysisUp;
//   delete analysisDown;

//   delete ZTT ;
//   delete ZL;
//   delete ZJ;
//   delete W;
//   delete TT;
//   delete VV;
//   delete QCD;
//   delete data_obs;
//   delete totalbkg;

//   delete VH_CMS_scale_tUp;
//   delete SM_CMS_scale_tUp;
//   delete VBF_CMS_scale_tUp;
//   delete ZTT_CMS_scale_tUp ;
//   delete ZL_CMS_scale_tUp;
//   delete ZJ_CMS_scale_tUp;
//   delete W_CMS_scale_tUp;
//   delete TT_CMS_scale_tUp;
//   delete VV_CMS_scale_tUp;

//   delete VH_CMS_scale_tDown;
//   delete SM_CMS_scale_tDown;
//   delete VBF_CMS_scale_tDown;
//   delete ZTT_CMS_scale_tDown ;
//   delete ZL_CMS_scale_tDown;
//   delete ZJ_CMS_scale_tDown;
//   delete W_CMS_scale_tDown;
//   delete TT_CMS_scale_tDown;
//   delete VV_CMS_scale_tDown;
  
  gROOT->ProcessLine(".q");
}


