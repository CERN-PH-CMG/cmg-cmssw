#include <TF1.h>
#include <TString.h>
#include <TH1F.h>
#include <TFile.h>

#define NCAT 5
#define smearres 20. //resolution svfitmass
#define taulooseiso -0.75 //For QCD
#define mulooseiso 0.5 //For QCD 
//For W we use inclusive 2-jet sample

void histosForLimit2012Roger(){

  gROOT->ProcessLine(".L ./tauMuConfig2011Reload.C");

  TString path="/data/benitezj/Samples/TauMuV541June2_TrigEff";

  TauMuPlotter * analysis=tauMuConfig("analysis",path);
  //analysis->plotvar_="svfitmass";
  analysis->plotvar_="ditaumass";
  Float_t xbinsValues[13]={0.,20,40,60,80,100,120,140,160,210,260,310,360};
  analysis->setVariableBinning(12,xbinsValues);
  analysis->nbins_=0;
  analysis->Isocat_=1;
  analysis->MTcat_=1; 
  analysis->setTauLooseIsoCut(taulooseiso);//applies to QCD
  analysis->setMuLooseIsoCut(mulooseiso);//applies to QCD

  TauMuPlotter * analysis_tUp=tauMuConfig("analysis_tUp",path+"_tUp");
  analysis_tUp->plotvar_=analysis->plotvar_;
  analysis_tUp->nbins_=analysis->nbins_;
  analysis_tUp->Isocat_= analysis->Isocat_;
  analysis_tUp->MTcat_=analysis->MTcat_;
  analysis_tUp->setVariableBinning(12,xbinsValues);
  analysis_tUp->setTauLooseIsoCut(taulooseiso);//applies to QCD
  analysis_tUp->setMuLooseIsoCut(mulooseiso);//applies to QCD

  TauMuPlotter * analysis_tDown=tauMuConfig("analysis_tDown",path+"_tDown");
  analysis_tDown->plotvar_=analysis->plotvar_;
  analysis_tDown->nbins_=analysis->nbins_;
  analysis_tDown->Isocat_= analysis->Isocat_;
  analysis_tDown->MTcat_=analysis->MTcat_;
  analysis_tDown->setVariableBinning(12,xbinsValues);
  analysis_tDown->setTauLooseIsoCut(taulooseiso);//applies to QCD
  analysis_tDown->setMuLooseIsoCut(mulooseiso);//applies to QCD



  TString catdirname[NCAT];
  catdirname[0]="muTau_0jet_low";
  catdirname[1]="muTau_0jet_high";
  catdirname[2]="muTau_boost_low";
  catdirname[3]="muTau_boost_high";
  catdirname[4]="muTau_vbf";

//   Int_t nbins[NCAT];
//   nbins[0]=50;
//   nbins[1]=50;
//   nbins[2]=50;
//   nbins[3]=25;
//   nbins[4]=20;

  
  TFile output(path+"/muTauSM.root","recreate");
  for(long sm=0;sm<NCAT;sm++){

    TDirectory* dir = output.mkdir(catdirname[sm]);  

    analysis->extrasel_ = analysis->getSMcut(sm);
    if(!analysis->scaleSamplesLumi())return 0;
    if(!analysis->correctSamplesInc())return 0;

    analysis_tUp->extrasel_ = analysis_tUp->getSMcut(sm);
    if(!analysis_tUp->scaleSamplesLumi())return 0;
    if(!analysis_tUp->correctSamplesInc())return 0;

    analysis_tDown->extrasel_ = analysis_tDown->getSMcut(sm);
    if(!analysis_tDown->scaleSamplesLumi())return 0;
    if(!analysis_tDown->correctSamplesInc())return 0;
  
    gROOT->cd();

    /////////////Nominal histos:
    TH1F* ZTT = analysis->getZToTauTau();
    ZTT->SetName("ZTT");//rename otherwise default name may conflict below

    TH1F* QCD = 0;
    if(sm==0||sm==1||sm==2||sm==3)QCD=analysis->getQCDInc();
    if(sm==4)QCD=analysis->getQCDMike();
    QCD->SetName("QCD");//rename otherwise default name may conflict below

    TH1F* W = 0;
    if(sm==0||sm==1||sm==2||sm==3) W = analysis->getWJetsInc();
    if(sm==4) W = analysis->getW3JetsVBF();
    W->SetName("W");//rename otherwise default name may conflict below

    TH1F* TT = analysis->getTTJetsInc();
    TT->SetName("TT");//rename otherwise default name may conflict below

    TH1F* ZL = analysis->getZToMuMuInc();
    ZL->SetName("ZL");//rename otherwise default name may conflict below

    TH1F* ZJ = analysis->getZToLJetInc();
    ZJ->SetName("ZJ");//rename otherwise default name may conflict below  

    TH1F* VV = analysis->getDiBoson();
    VV->SetName("VV");//rename otherwise default name may conflict below

    TH1F* ZLL=(TH1F*)ZL->Clone("ZLL");
    ZLL->SetName("ZLL");
    ZLL->Add(ZJ);

    TH1F* data_obs = analysis->getTotalData();
//     TH1F* data_obs = ZTT->Clone();//blind for now 
//     data_obs->Add(QCD); data_obs->Add(W); data_obs->Add(TT); data_obs->Add(ZL); data_obs->Add(ZJ); data_obs->Add(VV);
//     data_obs->Scale(((int)data_obs->Integral())/data_obs->Integral());
//     for(Int_t b=1;b<=data_obs->GetXaxis()->GetNbins();b++)
//       data_obs->SetBinContent(b,(int)(data_obs->GetBinContent(b)+0.5));
    data_obs->SetName("data_obs");//rename otherwise default name may conflict below


    /////////////tUp histos
    TH1F* ZTT_CMS_scale_tUp = analysis_tUp->getZToTauTau();
    ZTT_CMS_scale_tUp->SetName("ZTT_CMS_scale_tUp");//rename otherwise default name may conflict below

    TH1F* QCD_CMS_scale_tUp = 0;
    if(sm==0||sm==1||sm==2||sm==3)QCD_CMS_scale_tUp=analysis_tUp->getQCDInc();
    if(sm==4)QCD_CMS_scale_tUp=analysis_tUp->getQCDMike();
    QCD_CMS_scale_tUp->SetName("QCD_CMS_scale_tUp");//rename otherwise default name may conflict below

    TH1F* W_CMS_scale_tUp = 0;
    if(sm==0||sm==1||sm==2||sm==3) W_CMS_scale_tUp = analysis_tUp->getWJetsInc();
    if(sm==4) W_CMS_scale_tUp = analysis_tUp->getW3JetsVBF();
    W_CMS_scale_tUp->SetName("W_CMS_scale_tUp");//rename otherwise default name may conflict below

    TH1F* TT_CMS_scale_tUp = analysis_tUp->getTTJetsInc();
    TT_CMS_scale_tUp->SetName("TT_CMS_scale_tUp");//rename otherwise default name may conflict below

    TH1F* ZL_CMS_scale_tUp = analysis_tUp->getZToMuMuInc();
    ZL_CMS_scale_tUp->SetName("ZL_CMS_scale_tUp");//rename otherwise default name may conflict below

    TH1F* ZJ_CMS_scale_tUp = analysis_tUp->getZToLJetInc();
    ZJ_CMS_scale_tUp->SetName("ZJ_CMS_scale_tUp");//rename otherwise default name may conflict below  

    TH1F* VV_CMS_scale_tUp = analysis_tUp->getDiBoson();
    VV_CMS_scale_tUp->SetName("VV_CMS_scale_tUp");//rename otherwise default name may conflict below

    TH1F* ZLL_CMS_scale_tUp=(TH1F*)ZL_CMS_scale_tUp->Clone("ZLL_CMS_scale_tUp");
    ZLL_CMS_scale_tUp->SetName("ZLL_CMS_scale_tUp");
    ZLL_CMS_scale_tUp->Add(ZJ_CMS_scale_tUp);

    /////////////tDown histos
    TH1F* ZTT_CMS_scale_tDown = analysis_tDown->getZToTauTau();
    ZTT_CMS_scale_tDown->SetName("ZTT_CMS_scale_tDown");//rename otherwise default name may conflict below

    TH1F* QCD_CMS_scale_tDown = 0;
    if(sm==0||sm==1||sm==2||sm==3)QCD_CMS_scale_tDown=analysis_tDown->getQCDInc();
    if(sm==4)QCD_CMS_scale_tDown=analysis_tDown->getQCDMike();
    QCD_CMS_scale_tDown->SetName("QCD_CMS_scale_tDown");//rename otherwise default name may conflict below

    TH1F* W_CMS_scale_tDown = 0;
    if(sm==0||sm==1||sm==2||sm==3) W_CMS_scale_tDown = analysis_tDown->getWJetsInc();
    if(sm==4) W_CMS_scale_tDown = analysis_tDown->getW3JetsVBF();
    W_CMS_scale_tDown->SetName("W_CMS_scale_tDown");//rename otherwise default name may conflict below

    TH1F* TT_CMS_scale_tDown = analysis_tDown->getTTJetsInc();
    TT_CMS_scale_tDown->SetName("TT_CMS_scale_tDown");//rename otherwise default name may conflict below

    TH1F* ZL_CMS_scale_tDown = analysis_tDown->getZToMuMuInc();
    ZL_CMS_scale_tDown->SetName("ZL_CMS_scale_tDown");//rename otherwise default name may conflict below

    TH1F* ZJ_CMS_scale_tDown = analysis_tDown->getZToLJetInc();
    ZJ_CMS_scale_tDown->SetName("ZJ_CMS_scale_tDown");//rename otherwise default name may conflict below  

    TH1F* VV_CMS_scale_tDown = analysis_tDown->getDiBoson();
    VV_CMS_scale_tDown->SetName("VV_CMS_scale_tDown");//rename otherwise default name may conflict below

    TH1F* ZLL_CMS_scale_tDown=(TH1F*)ZL_CMS_scale_tDown->Clone("ZLL_CMS_scale_tDown");
    ZLL_CMS_scale_tDown->SetName("ZLL_CMS_scale_tDown");
    ZLL_CMS_scale_tDown->Add(ZJ_CMS_scale_tDown);




    dir->cd();

    ZTT->Write();
    ZL->Write();
    ZJ->Write();
    ZLL->Write();
    W->Write();
    TT->Write();
    VV->Write();
    QCD->Write();
    
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
    ZLL_CMS_scale_tUp->Write();
    W_CMS_scale_tDown->Write();
    TT_CMS_scale_tDown->Write();
    VV_CMS_scale_tDown->Write();
    QCD_CMS_scale_tUp->Write();


    data_obs->Write();

    
    gROOT->cd();

    delete ZTT ;
    delete ZL;
    delete ZJ;
    delete W;
    delete TT;
    delete VV;
    delete QCD;
    delete data_obs;



    delete ZTT_CMS_scale_tUp ;
    delete ZL_CMS_scale_tUp;
    delete ZJ_CMS_scale_tUp;
    delete W_CMS_scale_tUp;
    delete TT_CMS_scale_tUp;
    delete VV_CMS_scale_tUp;
    delete ZLL_CMS_scale_tUp;
    
    delete ZTT_CMS_scale_tDown ;
    delete ZL_CMS_scale_tDown;
    delete ZJ_CMS_scale_tDown;
    delete W_CMS_scale_tDown;
    delete TT_CMS_scale_tDown;
    delete VV_CMS_scale_tDown;
    delete ZLL_CMS_scale_tDown;
    


    for(long ma=110;ma<=145;ma+=5){
      
      gROOT->cd();

      //Nominal histos
      TH1F* SM = analysis->getSample(TString("HiggsGG")+ma);
      SM->SetName(TString("ggH")+ma);//rename otherwise default name may conflict below

      TH1F* VBF = analysis->getSample(TString("HiggsVBF")+ma);
      VBF->SetName(TString("qqH")+ma);//rename otherwise default name may conflict below

      TH1F* VH = analysis->getSample(TString("HiggsVH")+ma);
      VH->SetName(TString("VH")+ma);//rename otherwise default name may conflict below


      //tUp histos
      TH1F* SM_CMS_scale_tUp  = analysis_tUp->getSample(TString("HiggsGG")+ma);
      SM_CMS_scale_tUp ->SetName(TString("ggH_CMS_scale_tUp")+ma);//rename otherwise default name may conflict below

      TH1F* VBF_CMS_scale_tUp  = analysis_tUp->getSample(TString("HiggsVBF")+ma);
      VBF_CMS_scale_tUp ->SetName(TString("qqH_CMS_scale_tUp")+ma);//rename otherwise default name may conflict below

      TH1F* VH_CMS_scale_tUp  = analysis_tUp->getSample(TString("HiggsVH")+ma);
      VH_CMS_scale_tUp ->SetName(TString("VH_CMS_scale_tUp")+ma);//rename otherwise default name may conflict below      


      //tDown histos
      TH1F* SM_CMS_scale_tDown  = analysis_tDown->getSample(TString("HiggsGG")+ma);
      SM_CMS_scale_tDown ->SetName(TString("ggH_CMS_scale_tDown")+ma);//rename otherwise default name may conflict below

      TH1F* VBF_CMS_scale_tDown  = analysis_tDown->getSample(TString("HiggsVBF")+ma);
      VBF_CMS_scale_tDown ->SetName(TString("qqH_CMS_scale_tDown")+ma);//rename otherwise default name may conflict below

      TH1F* VH_CMS_scale_tDown  = analysis_tDown->getSample(TString("HiggsVH")+ma);
      VH_CMS_scale_tDown ->SetName(TString("VH_CMS_scale_tDown")+ma);//rename otherwise default name may conflict below      

      
      //Scale signals to 1/pb      
      SM->Scale(1./analysis->findSample(TString("HiggsGG")+ma)->getCrossection());
      VBF->Scale(1./analysis->findSample(TString("HiggsVBF")+ma)->getCrossection());
      VH->Scale(1./analysis->findSample(TString("HiggsVH")+ma)->getCrossection());
      
      SM_CMS_scale_tUp->Scale(1./analysis_tUp->findSample(TString("HiggsGG")+ma)->getCrossection());
      VBF_CMS_scale_tUp->Scale(1./analysis_tUp->findSample(TString("HiggsVBF")+ma)->getCrossection());
      VH_CMS_scale_tUp->Scale(1./analysis_tUp->findSample(TString("HiggsVH")+ma)->getCrossection());
      
      SM_CMS_scale_tDown->Scale(1./analysis_tDown->findSample(TString("HiggsGG")+ma)->getCrossection());
      VBF_CMS_scale_tDown->Scale(1./analysis_tDown->findSample(TString("HiggsVBF")+ma)->getCrossection());
      VH_CMS_scale_tDown->Scale(1./analysis_tDown->findSample(TString("HiggsVH")+ma)->getCrossection());

      dir->cd();
      VH->Write();
      SM->Write();
      VBF->Write();

      VH_CMS_scale_tUp->Write();
      SM_CMS_scale_tUp->Write();
      VBF_CMS_scale_tUp->Write();

      VH_CMS_scale_tDown->Write();
      SM_CMS_scale_tDown->Write();
      VBF_CMS_scale_tDown->Write();

      gROOT->cd();



      delete VH;
      delete SM;
      delete VBF;

      delete VH_CMS_scale_tUp;
      delete SM_CMS_scale_tUp;
      delete VBF_CMS_scale_tUp;

      delete VH_CMS_scale_tDown;
      delete SM_CMS_scale_tDown;
      delete VBF_CMS_scale_tDown;


    }



  }
  
  output.ls();
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
