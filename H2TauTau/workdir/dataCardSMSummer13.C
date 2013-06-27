#include <TString.h>
#include <TFile.h>
#include <TH1F.h>
#include <TDirectory.h>

#include "constants.h"

#include "TauMuPlotter.h"
#include "configTauMu2011.C"
#include "configTauMu2012.C"
#include "configTauMu2012AB.C"
#include "configTauMu2012ABC.C"
#include "configTauMu2012D.C"
#include "configTauMu2012ABCD.C"
#include "configTauMu2012Summer13.C"
#include "configTauMu2012Summer13ReReco.C"

#include "TauElePlotter.h"
#include "configTauEle2011.C"
#include "configTauEle2012.C"
#include "configTauEle2012AB.C"
#include "configTauEle2012ABC.C"
#include "configTauEle2012D.C"
#include "configTauEle2012ABCD.C"
#include "configTauEle2012Summer13.C"
#include "configTauEle2012Summer13ReReco.C"

bool skipCategory(int cat, int sm){
  if(cat==9)return 1;
  if(cat==11 && !(sm==0 || sm==1 || sm==2 || sm==3) )return 1;
  if(cat==12 && !(sm==0 || sm==1 || sm==2 || sm==4) )return 1;
  if(cat==13 && !(sm==0 || sm==1 || sm==2 || sm==5) )return 1;
  if(cat==15 && !(sm==0 || sm==1 || sm==2 || sm==6) )return 1;
  if(cat==16 && !(sm==0 || sm==1 || sm==2 || sm==7) )return 1;
  return 0;
}

void dataCardSMSummer13(Int_t channel, Int_t cat, Int_t year, Int_t dataset, TString mass, TString path, TString tag, Int_t option=0){

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
      if(dataset==5)analysis=configTauMu2012Summer13("analysis",path);
      if(dataset==6)analysis=configTauMu2012Summer13ReReco("analysis",path);
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
      if(dataset==5)analysis=configTauEle2012Summer13("analysis",path);
      if(dataset==6)analysis=configTauEle2012Summer13ReReco("analysis",path);
    }
  }
  if(!analysis){
    cout<<"Bad choices for channel/dataset/year"<<endl;
    exit(0);
  }
 
  bool Blind=0;
  analysis->nbins_=0;
  analysis->Isocat_=1;
  analysis->MTcat_=1; 
  analysis->Chcat_=1; 

  cout<<endl<<"optimizaion with option "<<option<<endl;


  analysis->scaleSamplesLumi();
  TFile output(ChannelName+"_SM"+(long)cat+"_"+mass+"_"+tag+".root","recreate");
  for(long sm=0; sm<NCATSUMMER13; sm++){
    if(skipCategory(cat,sm))continue;
    
    TDirectory* dir = output.mkdir(ChannelName+"_"+catdirnameSummer13[sm]);  
    gROOT->cd();

    analysis->plotvar_=mass;
    if(sm==6 || sm==7) analysis->setVariableBinning(NXBINSVBF,xbinsValuesVBF);
    else               analysis->setVariableBinning(NXBINS,xbinsValues);

    ////visible mass in 0-jet
//     if(sm==0 || sm==1){
//       analysis->plotvar_="ditaumass";//switch to visible mass for 0jet
//       analysis->setVariableBinning(NXBINS0JET,xbinsValues0Jet);
//     }else{
//       analysis->plotvar_=mass;
//       if(sm==4)analysis->setVariableBinning(NXBINSVBF,xbinsValuesVBF);
//       else analysis->setVariableBinning(NXBINS,xbinsValues);
//    }
    
    ///Category definition
    analysis->extrasel_ = analysis->getSMCategory(sm);


    TH1F* QCD = 0;
    if(sm==0) QCD=analysis->getQCDIncWNJet();
    else if(sm==1) QCD=analysis->getQCDIncWNJet(); 
    else if(sm==2) QCD=analysis->getQCDIncWNJet();
    else if(sm==3) QCD=analysis->getQCDMuIsoSM();
    else if(sm==4) QCD=analysis->getQCDMuIsoSM();
    else if(sm==5) QCD=analysis->getQCDBoostTight();
    else if(sm==6) QCD=analysis->getQCDVBFLoose(); //note its different from mu-Tau
    else if(sm==7) QCD=analysis->getQCDVBFTight();
    else QCD=analysis->getQCDIncWNJet(); 
    QCD->SetName("QCD");


    TH1F* W = 0;
    if(sm==5)      W = analysis->getWJetsNJetLooseTau();
    else if(sm==6) W = analysis->getWJetsNJetVBFLoose(); 
    else if(sm==7) W = analysis->getWJetsNJetVBFTight(); 
    else           W = analysis->getWJetsNJetNoChCut();
    W->SetName("W");
    
    TH1F* ZTT = 0;
    if(channel==1)ZTT = analysis->getZToTauTau();
    if(channel==2){
      if(sm==7)ZTT = analysis->getZToTauTauVBF();//not enough stats in e-Tau
      else ZTT = analysis->getZToTauTau();
    }
    ZTT->SetName("ZTT");

    TH1F* TT = 0;
    if(sm==6||sm==7) TT=analysis->getTTJetsVBFHCP(); 
    else TT=analysis->getTTJetsInc(); 

    TT->SetName("TT");

    TH1F* ZL =0;
    if(sm==6||sm==7)  ZL =analysis->getZLVBFHCP();
    else              ZL =analysis->getZLInc();
    ZL->SetName("ZL");

    TH1F* ZJ = 0;
    if(sm==6||sm==7)  ZJ=analysis->getZToLJetVBFHCP();                   
    else              ZJ=analysis->getZToLJetInc();
    ZJ->SetName("ZJ");

    TH1F* VV = 0;
    if(sm==6||sm==7)  VV=analysis->getDiBosonVBFHCP();
    else VV=analysis->getDiBoson();
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
    ZTT->SetName("ZTT125"); ZTT->Write();//needed for ZTT fits
    fix0Bins(ZL);  ZL->Write();
    fix0Bins(ZJ);  ZJ->Write();
    fix0Bins(ZLL); ZLL->Write();
    fix0Bins(W);   W->Write();
    fix0Bins(TT);  TT->Write();
    fix0Bins(VV);  VV->Write();
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

      TString EventWeightTmp=analysis->eventWeight_;
      analysis->eventWeight_+= "*(signalWeightHigh/signalWeight)";
      TH1F* SMUp = analysis->getSample(TString("HiggsGG")+ma);
      SMUp->SetName(TString("ggH")+ma+"_QCDscale_ggH1inUp");
      analysis->eventWeight_=EventWeightTmp;

      EventWeightTmp=analysis->eventWeight_;
      analysis->eventWeight_+= "*(signalWeightLow/signalWeight)";
      TH1F* SMDown = analysis->getSample(TString("HiggsGG")+ma);
      SMDown->SetName(TString("ggH")+ma+"_QCDscale_ggH1inDown");
      analysis->eventWeight_=EventWeightTmp;


      TH1F* VBF = analysis->getSample(TString("HiggsVBF")+ma);
      VBF->SetName(TString("qqH")+ma);

      TH1F* VH = analysis->getSample(TString("HiggsVH")+ma);
      VH->SetName(TString("VH")+ma);

      SM->Scale(1./analysis->findSample(TString("HiggsGG")+ma)->getCrossection());
      SMUp->Scale(1./analysis->findSample(TString("HiggsGG")+ma)->getCrossection());
      SMDown->Scale(1./analysis->findSample(TString("HiggsGG")+ma)->getCrossection());
      VBF->Scale(1./analysis->findSample(TString("HiggsVBF")+ma)->getCrossection());
      VH->Scale(1./analysis->findSample(TString("HiggsVH")+ma)->getCrossection());
      
      //check for empty histos
      if( SM->Integral()<=0.){  SM->SetBinContent(SM->GetNbinsX()/2,1e-4);    SM->SetBinError(SM->GetNbinsX()/2,1e-4); }
      if( SMUp->Integral()<=0.){  SMUp->SetBinContent(SMUp->GetNbinsX()/2,1e-4);    SMUp->SetBinError(SMUp->GetNbinsX()/2,1e-4); }
      if( SMDown->Integral()<=0.){  SMDown->SetBinContent(SMDown->GetNbinsX()/2,1e-4);    SMDown->SetBinError(SMDown->GetNbinsX()/2,1e-4); }

      if( VBF->Integral()<=0.){ VBF->SetBinContent(VBF->GetNbinsX()/2,1e-4);  VBF->SetBinError(VBF->GetNbinsX()/2,1e-4); }
      if( VH->Integral()<=0.){  VH->SetBinContent(VH->GetNbinsX()/2,1e-4);    VH->SetBinError(VH->GetNbinsX()/2,1e-4); }

      dir->cd();
      
     
      fixSignal(data_obs,MC,SM);  fix0Bins(SM);   SM->Write();
      fixSignal(data_obs,MC,SMUp);  fix0Bins(SMUp);   SMUp->Write();
      fixSignal(data_obs,MC,SMDown);  fix0Bins(SMDown);   SMDown->Write();

      fixSignal(data_obs,MC,VBF); fix0Bins(VBF);  VBF->Write();
      fixSignal(data_obs,MC,VH);  fix0Bins(VH);   VH->Write();
      gROOT->cd();

      delete SM;
      delete SMUp;
      delete SMDown;
      delete VBF;
      delete VH;
    }

    delete MC;
    delete data_obs;

  }
  
  output.ls();
  output.Close();
  gROOT->ProcessLine(".q");
}




void plotDataCard(TString file, Int_t channel, Int_t cat=-1){

  TString ChannelName;
  if(channel==1)ChannelName="muTau";
  else if(channel==2)ChannelName="eleTau";
  else return;
  
  TFile nominal(file+".root","read");
  gROOT->cd();

  TString fname=file+".ps";

  TCanvas C;
  C.Print(fname+"[");

  for(long sm=0;sm<NCATSUMMER13;sm++){//
    if(skipCategory(cat,sm))continue;

    TH1F* ZTT = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ZTT");
    if(!ZTT)continue;
    TH1F* QCD = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/QCD");
    TH1F* W = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/W");
    TH1F* TT = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/TT");
    TH1F* ZL = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ZL");
    TH1F* ZJ = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ZJ");
    TH1F* VV = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/VV");
    TH1F* data_obs = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/data_obs");

    //plot
    TH1F*MC=(TH1F*)ZTT->Clone("MC");
    MC->Add(ZL);
    MC->Add(ZJ);
    MC->Add(TT);
    MC->Add(W);
    MC->Add(TT);
    MC->Add(VV);
    MC->Add(QCD);
    MC->SetTitle(ChannelName+"_"+catdirnameSummer13[sm]);


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
    data_obs->SetTitle(catdirnameSummer13[sm]);
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
      TH1F* SM = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/ggH"+ma);
      TH1F* VBF = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/qqH"+ma);
      TH1F* VH = (TH1F*)nominal.Get(ChannelName+"_"+catdirnameSummer13[sm]+"/VH"+ma);
      if(massValues[m]==110){SM1=(TH1F*)SM->Clone("SM1"); SM1->Add(VBF); SM1->Add(VH);}
      if(massValues[m]==145){SM2=(TH1F*)SM->Clone("SM2");SM2->Add(VBF);SM2->Add(VH);}
    }
    if(SM1->GetMaximum()>SM2->GetMaximum())
      SM1->GetYaxis()->SetRangeUser(0,1.2*SM1->GetMaximum());
    if(SM1->GetMaximum()<SM2->GetMaximum())
      SM1->GetYaxis()->SetRangeUser(0,1.2*SM2->GetMaximum());

    SM1->SetTitle(catdirnameSummer13[sm]);
    SM1->Draw("hist");
    SM2->Draw("histsame");
    C.Print(fname);
 
  }
  
  C.Print(fname+"]");
 
  nominal.Close();
}
