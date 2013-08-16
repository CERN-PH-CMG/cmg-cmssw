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
  if(cat==10 && !( sm==8 ) )return 1;//inclusive category
  if(0<cat&&cat<10 && sm!=cat )return 1;//doesnt work for 0-jet low
  if(cat==13 && !(sm==0 || sm==1 || sm==2 || sm==3) )return 1;
  if(cat==14 && !(sm==0 || sm==1 || sm==2 || sm==4) )return 1;
  if(cat==15 && !(sm==0 || sm==1 || sm==2 || sm==5) )return 1;
  if(cat==16 && !(sm==0 || sm==1 || sm==2 || sm==6) )return 1;
  if(cat==17 && !(sm==0 || sm==1 || sm==2 || sm==7) )return 1;
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
 
  cout<<endl<<"optimizaion with option "<<option<<endl;


  analysis->Isocat_=1;
  analysis->MTcat_=1; 
  analysis->Chcat_=1; 
  analysis->scaleSamplesLumi();
  TFile output(ChannelName+"_SM"+(long)cat+"_"+mass+"_"+tag+".root","recreate");
  for(long sm=0; sm<NCATSUMMER13; sm++){
    if(skipCategory(cat,sm))continue;
    
    TDirectory* dir = 0;
    if(sm!=11 && sm!=12 ) dir=output.mkdir(ChannelName+"_"+catdirnameSummer13[sm]);  
    else dir=(TDirectory*)output.Get(ChannelName+"_"+catdirnameSummer13[sm]); 
    if(!dir){cout<<" Directory not found "<<endl; return 0;}
    gROOT->cd();

    analysis->plotvar_=mass;
    analysis->nbins_=0;

    if(sm==6 || sm==7)          analysis->setVariableBinning(NXBINSVBF,xbinsValuesVBF);
    else if( sm==9  || sm==10 ) analysis->setVariableBinning(NXBINSMSSM,xbinsValuesMSSM);
    else if( sm==11 || sm==12 ){
      analysis->nbins_=400;
      analysis->xmin_=0;
      analysis->xmax_=2000;
    }else                       analysis->setVariableBinning(NXBINS,xbinsValues);

    //////visible mass in 0-jet
//     if(sm==0 || sm==1 || sm==2){//5 GeV bins
//       analysis->plotvar_="ditaumass";//switch to visible mass for 0jet
//       analysis->setVariableBinning(NXBINS0JET,xbinsValues0Jet);
//     }else if(sm==6 || sm==7){//20 GeV bins
//       analysis->plotvar_=mass;
//       analysis->setVariableBinning(NXBINSVBF,xbinsValuesVBF);
//     }else{//10 GeV bins
//       analysis->plotvar_=mass;
//       analysis->setVariableBinning(NXBINS,xbinsValues);
//     }
    
    ///Category definition
    analysis->extrasel_ = analysis->getSMCategory(sm);

    ///fits to ZTT vs Tau pT
    if(option>0){
      //inclusive
      if(0<option&&option<10)analysis->extrasel_ = TString("(")+(long)(20+(option-1)*5)+"<taupt&&taupt<"+(long)(20+(option)*5)+")";
 
      //Barrel vs Endcap
      if(10<option&&option<20)analysis->extrasel_ = TString("(abs(taueta)<1.5&&")+(long)(20+(option-10-1)*5)+"<taupt&&taupt<"+(long)(20+(option-10)*5)+")";
      if(20<option&&option<30)analysis->extrasel_ = TString("(abs(taueta)>1.5&&")+(long)(20+(option-20-1)*5)+"<taupt&&taupt<"+(long)(20+(option-20)*5)+")";

      ///decay mode
      if(option==50)analysis->extrasel_ = TString("(taudecaymode==0)");
      if(option==60)analysis->extrasel_ = TString("(taudecaymode==1)");
      if(option==70)analysis->extrasel_ = TString("(taudecaymode==10)");

      if(50<option&&option<60){
	if(option%10<=6)analysis->extrasel_ = TString("(taudecaymode==0&&")+(long)(20+(option-50-1)*5)+"<taupt&&taupt<"+(long)(20+(option-50)*5)+")";
	else if(option%10==7)analysis->extrasel_ = TString("(taudecaymode==0&&")+(long)(50)+"<taupt&&taupt<"+(long)(60)+")";
	else if(option%10==8)analysis->extrasel_ = TString("(taudecaymode==0&&")+(long)(60)+"<taupt&&taupt<"+(long)(80)+")";
	else if(option%10==9)analysis->extrasel_ = TString("(taudecaymode==0&&")+(long)(80)+"<taupt&&taupt<"+(long)(120)+")";
      }
      if(60<option&&option<70){
	if(option%10<=6)analysis->extrasel_ = TString("(taudecaymode==1&&")+(long)(20+(option-60-1)*5)+"<taupt&&taupt<"+(long)(20+(option-60)*5)+")";
	else if(option%10==7)analysis->extrasel_ = TString("(taudecaymode==1&&")+(long)(50)+"<taupt&&taupt<"+(long)(60)+")";
	else if(option%10==8)analysis->extrasel_ = TString("(taudecaymode==1&&")+(long)(60)+"<taupt&&taupt<"+(long)(80)+")";
	else if(option%10==9)analysis->extrasel_ = TString("(taudecaymode==1&&")+(long)(80)+"<taupt&&taupt<"+(long)(120)+")";
      }
      if(70<option&&option<80){
	if(option%10<=6)analysis->extrasel_ = TString("(taudecaymode==10&&")+(long)(20+(option-70-1)*5)+"<taupt&&taupt<"+(long)(20+(option-70)*5)+")";
	else if(option%10==7)analysis->extrasel_ = TString("(taudecaymode==10&&")+(long)(50)+"<taupt&&taupt<"+(long)(60)+")";
	else if(option%10==8)analysis->extrasel_ = TString("(taudecaymode==10&&")+(long)(60)+"<taupt&&taupt<"+(long)(80)+")";
	else if(option%10==9)analysis->extrasel_ = TString("(taudecaymode==10&&")+(long)(80)+"<taupt&&taupt<"+(long)(120)+")";
      }

    }



    TH1F* W = 0;
    if(sm==5)                W = analysis->getWJetsNJet(2);
    else if(sm==6)           W = analysis->getWJetsNJetVBFLoose(); 
    else if(sm==7)           W = analysis->getWJetsNJetVBFTight();
    else if(sm==9 || sm==11) W = analysis->getWJetsNJet(2); 
    else                     W = analysis->getWJetsNJet(); 
    if( sm==11 || sm==12 )     W->SetName("W_fine_binning");
    else W->SetName("W");
    fix0Bins(W); 

    TH1F* QCD = 0;
    if(sm==2) QCD=analysis->getQCDMuIso();
    else if(sm==3) QCD=analysis->getQCDMuIso();
    else if(sm==4) QCD=analysis->getQCDMuIso();
    else if(sm==5) QCD=analysis->getQCDBoostTight();
    else if(sm==6) QCD=analysis->getQCDVBFLoose(); //note its different from mu-Tau
    else if(sm==7) QCD=analysis->getQCDVBFTight();
    else if(sm==9||sm==11) QCD=analysis->getQCDKetiHCP();//b-tag 
    else QCD=analysis->getQCDIncWNJet(); 
    if( sm==11 || sm==12 )     QCD->SetName("QCD_fine_binning");
    else     QCD->SetName("QCD");
    fix0Bins(QCD);


    TH1F* ZTT = 0;
    if(channel==1)ZTT = analysis->getZToTauTau();
    if(channel==2){
      if(sm==7)ZTT = analysis->getZToTauTauVBF();//not enough stats in e-Tau
      else ZTT = analysis->getZToTauTau();
    }
    if( sm==11 || sm==12 )     ZTT->SetName("ZTT_fine_binning");
    else     ZTT->SetName("ZTT");
    fix0Bins(ZTT);


    TH1F* TT = 0;
    if(sm==6||sm==7) TT=analysis->getTTJetsVBFHCP(); 
    else TT=analysis->getTTJetsInc(); 
    if( sm==11 || sm==12 )     TT->SetName("TT_fine_binning");
    else     TT->SetName("TT");
    fix0Bins(TT);

    TH1F* ZL =0;
    if(sm==6||sm==7)  ZL =analysis->getZLVBFHCP();
    else              ZL =analysis->getZLInc();
    if( sm==11 || sm==12 )     ZL->SetName("ZL_fine_binning");
    else     ZL->SetName("ZL");
    fix0Bins(ZL); 
  
    TH1F* ZJ = 0;
    if(sm==6) ZJ=analysis->getZToLJetVBFHCP();                   
    else if(sm==7)  ZJ=analysis->getZToLJetVBFTightHCP();                   
    else              ZJ=analysis->getZToLJetInc();
    if( sm==11 || sm==12 )     ZJ->SetName("ZJ_fine_binning");
    else     ZJ->SetName("ZJ");
    fix0Bins(ZJ); 

    TH1F* VV = 0;
    if(sm==6)       VV=analysis->getDiBosonVBFHCP();
    else if(sm==7)  VV=analysis->getDiBosonVBFTightHCP();
    else            VV=analysis->getDiBoson();
    if( sm==11 || sm==12 )     VV->SetName("VV_fine_binning");
    else     VV->SetName("VV");
    fix0Bins(VV);

    TH1F* ZLL=(TH1F*)ZL->Clone("ZLL");
    ZLL->Add(ZJ);
    if( sm==11 || sm==12 )     ZLL->SetName("ZLL_fine_binning");
    else     ZLL->SetName("ZLL");
    fix0Bins(ZLL); 


    TH1F* data_obs = analysis->getTotalData();
    data_obs->SetName("data_obs");
    cout<<"Data integral :"<< data_obs->Integral()<<endl;



    dir->cd();
    W->Write();
    QCD->Write();
    ZTT->Write();     ZTT->SetName("ZTT125");    ZTT->Write();
    TT->Write();
    ZL->Write();
    ZJ->Write();    
    VV->Write();
    ZLL->Write();
    data_obs->Write();   
    gROOT->cd();
 

    TH1F* MC=(TH1F*)ZTT->Clone("MC");//needed below
    MC->Add(ZL);
    MC->Add(ZJ);
    MC->Add(W);
    MC->Add(TT);
    MC->Add(VV);
    MC->Add(QCD);
    
    delete ZTT ;
    delete ZL;
    delete ZJ;
    delete ZLL;
    delete W;
    delete TT;
    delete VV;
    delete QCD;


    if( sm<9 ){
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

    }

    if( sm>=9 && sm<=10){///MSSM
      for(Int_t m=0;m<NSUSY;m++){
	long ma=SUSYMass[m];

	TH1F* GG = analysis->getSample(TString("SUSYGG")+ma);
	GG->SetName(TString("ggH")+ma);
	TH1F* BB = analysis->getSample(TString("SUSYBB")+ma);
	BB->SetName(TString("bbH")+ma);

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

    delete MC;
    delete data_obs;

  }
  
  output.ls();
  output.Close();
  gROOT->ProcessLine(".q");
}



