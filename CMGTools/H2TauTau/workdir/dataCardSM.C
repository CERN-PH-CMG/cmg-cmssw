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

void fix0Bins(TH1F* h){

  //determine if this is a low stats shape
  float binUnc=0.;
  int nNon0bins=0;
  for(Int_t i=1;i<=h->GetNbinsX();i++){
    if(h->GetBinContent(i)>0.){
      nNon0bins++;
      binUnc+=h->GetBinError(i)/h->GetBinContent(i);
    }
  }
  binUnc=binUnc/nNon0bins;

  //if the average bin uncertainty is greater than 30% then smooth
  if(binUnc>0.5){

    //TH1F* hs=new TH1F(TString(h->GetName())+"smeared",h->GetTitle(),h->GetXaxis()->GetNbins(),h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
    TH1F* hs=h->Clone("hs");
    TF1 gaus("gauss","[0]*exp(-0.5*(x-[1])**2/[2]**2)",h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
    gaus.SetParameter(2,10);

    for(Int_t b=1;b<=h->GetXaxis()->GetNbins();b++){
      gaus.SetParameter(0,h->GetBinContent(b));
      gaus.SetParameter(1,h->GetBinCenter(b));
      for(Int_t bs=1;bs<=h->GetXaxis()->GetNbins();bs++){
        hs->AddBinContent(bs,hs->GetBinWidth(bs)*gaus.Eval(hs->GetBinCenter(bs))) ;
      }
    }
    for(Int_t bs=1;bs<=h->GetXaxis()->GetNbins();bs++){
      hs->SetBinError(bs,0.);//not sure this is necessary
    }
    
    if(hs->Integral()>0.)
      hs->Scale(h->Integral()/hs->Integral());//make sure the output histo has the same integral
    else hs->Scale(0.);

    for(Int_t b=1;b<=h->GetXaxis()->GetNbins();b++){
      h->SetBinContent(b,hs->GetBinContent(b));
      h->SetBinError(b,hs->GetBinContent(b)*0.4);
    }
    delete hs;
  }


  //fix empty distributions for limit to run
  if(h->Integral()<=0.){
    for(Int_t i=1;i<=h->GetNbinsX();i++){
      h->SetBinContent(i,0.001);
      h->SetBinError(i,0.001);
    }
  }

}

void fixSignal(TH1F*hData,TH1F*hMC,TH1F*hsig){
  for(Int_t b=1;b<=hData->GetNbinsX();b++)
    if(hData->GetBinContent(b)>0.&&hMC->GetBinContent(b)==0.){
      hsig->SetBinContent(b,0.);
      hsig->SetBinError(b,0.);
    }
}

void rebinHisto(TH1F* hinput,TH1F* houtput){
  for(Int_t bo=1;bo<=houtput->GetNbinsX();bo++){
    float err=0.;
    for(Int_t bi=1;bi<=hinput->GetNbinsX();bi++){
      if( houtput->GetBinLowEdge(bo) < hinput->GetBinCenter(bi)
	  && hinput->GetBinCenter(bi) < houtput->GetBinLowEdge(bo)+houtput->GetBinWidth(bo)){
	houtput->AddBinContent(bo,hinput->GetBinContent(bi));
	err+=hinput->GetBinError(bi)*hinput->GetBinError(bi);
      }
    } 
    houtput->SetBinError(bo,sqrt(err));
  }
}


void dataCardSM(Int_t channel, Int_t cat, Int_t year, Int_t dataset, TString mass, TString path, TString tag, Int_t option=0){

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
    }
  }
  
 
  bool Blind=0;
  analysis->nbins_=0;
  analysis->Isocat_=1;
  analysis->MTcat_=1; 
  analysis->Chcat_=1; 

  cout<<endl<<"optimizaion with option "<<option<<endl;

//   ///Simple isolation scan
//   char isocutoption[100];
//   if(option<10){
//     sprintf(isocutoption,"(tauisomva2raw>%.3f)",0.80+0.02*option);
//     analysis->tauIsoCutQCD_="(tauisomva2raw>0.5)";
//   }else if(option<20){
//     sprintf(isocutoption,"(tauiso3hitraw<%.3f)",2.2-0.2*(option-10));
//     analysis->tauIsoCutQCD_="(tauiso3hitraw<6.0)";
//   }
//   analysis->tauIsoCut_=isocutoption;

  
//   ////////////////////////optimization of mT cut
//   if(option>0){
//     //use the digits of the option for each variable
//     analysis->mTCut_=5+abs(option)*5;
//   }
//   //////////////////////////////////////////////////

  ////////////////////////optimization of 1Jet : with 2 tau pT categories
  if(option>0){

    //optimize tau pT without higgs pT cut
    if((option/10)==0)analysis->taupTCut_ = 20+(option)*5;//1->6
    ///optimize higgs pT cut at low Tau pT
    if((option/10)==1)analysis->higgspTCutLow_=(option%10-1)*20;//11->18
    ///optimize higgs pT cut at high Tau pT
    if((option/10)==2)analysis->higgspTCutHigh_=(option%10-1)*20;//21->28

    //optimize tau and higgs pT cuts at the same time
    if((option/100)==1){
      analysis->taupTCut_ = 20+((option%100)/10 - 1)*5;//first digit: 1->6
      analysis->higgspTCutHigh_=(option%10-1)*20;//second digit: 1->8
    }

  }
  //////////////////////////////////////////////////

  analysis->scaleSamplesLumi();
  TFile output(ChannelName+"_SM"+(long)cat+"_"+mass+"_"+tag+".root","recreate");
  for(long sm=0; sm<NCAT; sm++){
    if(cat==9)continue;
    if(cat==13 && (sm==2 || sm==4))continue;//skip boost_low and vbf
    if(cat==20 && (sm==3 || sm==4))continue;//skip boost_high and vbf
    if(cat==23 && (sm==4))continue;//skip vbf
    if(cat==15 && (sm==2 || sm==3))continue;//skip boost_low and boost_high

    TDirectory* dir = output.mkdir(ChannelName+"_"+catdirname[sm]);  
    gROOT->cd();

//     if(sm==0 || sm==1){
//       analysis->plotvar_="ditaumass";//switch to visible mass for 0jet
//       analysis->setVariableBinning(NXBINS0JET,xbinsValues0Jet);
//     }else{
    analysis->plotvar_=mass;
    if(sm==4)analysis->setVariableBinning(NXBINSVBF,xbinsValuesVBF);
    else analysis->setVariableBinning(NXBINS,xbinsValues);
    //    }
    
      ///Category definition
    analysis->extrasel_ = analysis->getSMcut(sm);
    //analysis->extrasel_ = analysis->getSMcutOpt(sm);//possible higgs pT cuts
    //analysis->extrasel_ = analysis->getSMcutSummer13(sm);//use higgs pT < 20 in control regions


//     if(option==0)analysis->extrasel_ = analysis->getSMcut(sm) + "*(taudecaymode==0)";
//     if(option==1)analysis->extrasel_ = analysis->getSMcut(sm) + "*(taudecaymode==1)";
//     if(option==10)analysis->extrasel_ = analysis->getSMcut(sm) + "*(taudecaymode==10)";

//     //for ZTT decaymode ratios
//     analysis->mTCut_=40;
//     if(20<=option&&option<30) analysis->extrasel_ = TString("(metpt<40&&njet<=2)*")+"(taudecaymode==0)"+"*("+(long)(20+(option-20)*5)+"<taupt&&taupt<"+(long)(20+(option-20+1)*5)+")";
//     if(30<=option&&option<40) analysis->extrasel_ = TString("(metpt<40&&njet<=2)*")+"(taudecaymode==1)"+"*("+(long)(20+(option-30)*5)+"<taupt&&taupt<"+(long)(20+(option-30+1)*5)+")";
//     if(40<=option&&option<50) analysis->extrasel_ = TString("(metpt<40&&njet<=2)*")+"(taudecaymode==10)"+"*("+(long)(20+(option-40)*5)+"<taupt&&taupt<"+(long)(20+(option-40+1)*5)+")";


    TH1F* QCD = 0;
    if(channel==1){//mu-tau
      if(year==2011){
	if(sm==0 || sm==2 ) QCD=analysis->getQCDInc();
	if(sm==1 || sm==3 ) QCD=analysis->getQCDIncLooseShape();
	if(sm==4)           QCD=analysis->getQCDMike();
      }
      if(year==2012){
	if(sm==0) QCD=analysis->getQCDIncWNJet(); 
	if(sm==1) QCD=analysis->getQCDMuIsoSM(); 
	if(sm==2) QCD=analysis->getQCDIncLowPt(); 
	if(sm==3) QCD=analysis->getQCDMuIsoSM(); 
	if(sm==4) QCD=analysis->getQCDVBFHCP();
      }
    }
    if(channel==2){//e-tau
      if(year==2011){
	if(sm==0 || sm==2) QCD=analysis->getQCDInc();
	if(sm==1 || sm==3) QCD=analysis->getQCDIncLooseShape();
	if(sm==4)          QCD=analysis->getQCDMike();
      }
      if(year==2012){
	if(sm==0) QCD=analysis->getQCDIncWNJet(); 
	if(sm==1) QCD=analysis->getQCDMuIsoSM();
	if(sm==2) QCD=analysis->getQCDIncLowPt();//getQCDIncWNJet(); //
	if(sm==3) QCD=analysis->getQCDIncHighPt();
	if(sm==4) QCD=analysis->getQCDVBFHCP2();
      }
    }
    QCD->SetName("QCD");


    TH1F* W = 0;
    if(channel==1){//mu-tau
      if(year==2011){
	if(sm==0 || sm==1) W = analysis->getWJetsInc();
	if(sm==2 || sm==3) W = analysis->getWJetsInc();
	if(sm==4) W = analysis->getW3JetsVBF();
      }
      if(year==2012){
	if(sm==0) W = analysis->getWJetsNJet();
	if(sm==1) W = analysis->getWJetsNJetNoChCut();
	if(sm==2) W = analysis->getWJetsNJet();
	if(sm==3) W = analysis->getWJetsNJetNoChCut();
	if(sm==4) W = analysis->getWJetsNJetVBFHCP(); 
      }
    }
    if(channel==2){//e-tau
      if(year==2011){
	if(sm==0 || sm==1) W = analysis->getWJetsInc();
	if(sm==2 || sm==3) W = analysis->getWJetsInc();
	if(sm==4)  W = analysis->getW3JetsVBF();
      }
      if(year==2012){
	if(sm==0) W = analysis->getWJetsNJet();
	if(sm==1) W = analysis->getWJetsNJetNoChCut();
	if(sm==2) W = analysis->getWJetsNJetNoChCut();
	if(sm==3) W = analysis->getWJetsNJetNoChCut();
	if(sm==4) W = analysis->getWJetsNJetVBFHCP(); 
      }
    }
    W->SetName("W");
    
    TH1F* ZTT = 0;
    ZTT = analysis->getZToTauTau();
    ZTT->SetName("ZTT");

    TH1F* TT = 0;
    if(sm==0 || sm==2 || sm==1 || sm==3)        TT=analysis->getTTJetsInc(); 
    if(sm==4)                                   TT=analysis->getTTJetsVBFHCP(); 
    TT->SetName("TT");

    TH1F* ZL =0;
    if(channel==1){//mu-Tau
      if(sm==0 || sm==2 || sm==1 || sm==3) 	ZL =analysis->getZLInc();
      if(sm==4)                            	ZL =analysis->getZLVBFHCP();
    }
    if(channel==2){//e-Tau
      if(sm==0 || sm==2 || sm==1) 	        ZL =analysis->getZL2012();//analysis->getZL2012Type2();//
      if(sm==3)                            	ZL =analysis->getZLBoost();
      if(sm==4)                            	ZL =analysis->getZLVBFHCP();
    }
    ZL->SetName("ZL");

    TH1F* ZJ = 0;
    if(sm==0 || sm==2 || sm==1 || sm==3)        ZJ=analysis->getZToLJetInc();
    if(sm==4)                                   ZJ=analysis->getZToLJetVBFHCP();                   
    ZJ->SetName("ZJ");

    TH1F* VV = 0;
    if(sm==0 || sm==2 || sm==1 || sm==3)        VV=analysis->getDiBoson();
    if(sm==4)                                   VV=analysis->getDiBosonVBFHCP();
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

      TH1F* VBF = analysis->getSample(TString("HiggsVBF")+ma);
      VBF->SetName(TString("qqH")+ma);

      TH1F* VH = analysis->getSample(TString("HiggsVH")+ma);
      VH->SetName(TString("VH")+ma);

      SM->Scale(1./analysis->findSample(TString("HiggsGG")+ma)->getCrossection());
      VBF->Scale(1./analysis->findSample(TString("HiggsVBF")+ma)->getCrossection());
      VH->Scale(1./analysis->findSample(TString("HiggsVH")+ma)->getCrossection());
      
      //check for empty histos
      if( SM->Integral()<=0.){  SM->SetBinContent(SM->GetNbinsX()/2,1e-4);    SM->SetBinError(SM->GetNbinsX()/2,1e-4); }
      if( VBF->Integral()<=0.){ VBF->SetBinContent(VBF->GetNbinsX()/2,1e-4);  VBF->SetBinError(VBF->GetNbinsX()/2,1e-4); }
      if( VH->Integral()<=0.){  VH->SetBinContent(VH->GetNbinsX()/2,1e-4);    VH->SetBinError(VH->GetNbinsX()/2,1e-4); }

      dir->cd();
      
     
      
      fixSignal(data_obs,MC,VH);  fix0Bins(VH);   VH->Write();
      fixSignal(data_obs,MC,SM);  fix0Bins(SM);   SM->Write();
      fixSignal(data_obs,MC,VBF); fix0Bins(VBF);  VBF->Write();
      gROOT->cd();

      delete VH;
      delete SM;
      delete VBF;

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

  //TString fname=TString("plotDataCard_")+file+".ps";
  TString fname=file+".ps";

  TCanvas C;
  C.Print(fname+"[");

  for(long sm=0;sm<NCAT;sm++){//
    if(cat==13 && (sm==2 || sm==4))continue;//skip boost_low and vbf
    if(cat==20 && (sm==3 || sm==4))continue;//skip boost_high and vbf
    if(cat==23 && (sm==4))continue;//skip vbf
    if(cat==15 && (sm==2 || sm==3))continue;//skip boost_low and boost_high

    TH1F* ZTT = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/ZTT");
    if(!ZTT)continue;
    TH1F* QCD = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/QCD");
    TH1F* W = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/W");
    TH1F* TT = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/TT");
    TH1F* ZL = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/ZL");
    TH1F* ZJ = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/ZJ");
    TH1F* VV = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/VV");
    TH1F* data_obs = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/data_obs");

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
    data_obs->SetTitle(catdirname[sm]);
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
      TH1F* SM = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/ggH"+ma);
      TH1F* VBF = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/qqH"+ma);
      TH1F* VH = (TH1F*)nominal.Get(ChannelName+"_"+catdirname[sm]+"/VH"+ma);
      if(massValues[m]==110){SM1=(TH1F*)SM->Clone("SM1"); SM1->Add(VBF); SM1->Add(VH);}
      if(massValues[m]==145){SM2=(TH1F*)SM->Clone("SM2");SM2->Add(VBF);SM2->Add(VH);}
    }
    if(SM1->GetMaximum()>SM2->GetMaximum())
      SM1->GetYaxis()->SetRangeUser(0,1.2*SM1->GetMaximum());
    if(SM1->GetMaximum()<SM2->GetMaximum())
      SM1->GetYaxis()->SetRangeUser(0,1.2*SM2->GetMaximum());

    SM1->SetTitle(catdirname[sm]);
    SM1->Draw("hist");
    SM2->Draw("histsame");
    C.Print(fname);
 
  }
  
  C.Print(fname+"]");
 
  nominal.Close();
}
