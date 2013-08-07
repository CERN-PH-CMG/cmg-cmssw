#include "CMGTools/H2TauTau/interface/QCDEstimate.h"

//#include <stdio>
//#include <iomanip>
#include <iostream>
using namespace std;


QCDEstimate::QCDEstimate():
  DataTrees_(0),
  ZTTTree_(0),
  WJetsTree_(0),
  TTbarTree_(0)
{
}

QCDEstimate::QCDEstimate(TTree** DataTrees, unsigned int nDataTrees, TTree* ZTT, float ZTTLumiScale, TTree* WJets, float WJetsLumiScale, TTree* TTbar, float TTbarLumiScale, TString categorySelection, TString MuIsoSelection, TString TauIsoSelection, TString mTSignalSelection, TString mTSidebandSelection, TString mujetptVarName, TString taujetptVarName):
  DataTrees_(DataTrees),
  nDataTrees_(nDataTrees),
  ZTTTree_(ZTT),
  WJetsTree_(WJets),
  TTbarTree_(TTbar),
  ZTTLumiScale_(ZTTLumiScale),
  WJetsLumiScale_(WJetsLumiScale),
  TTbarLumiScale_(TTbarLumiScale),
  categorySelection_(categorySelection),
  MuIsoSelection_(MuIsoSelection),
  TauIsoSelection_(TauIsoSelection),
  mTSignalSelection_(mTSignalSelection),
  mTSidebandSelection_(mTSidebandSelection)
{

  
  qcdMuIsoRatio_ = "((17<mujetpt&&mujetpt<=22)*3.94+(22<mujetpt&&mujetpt<=27)*0.662+(27<mujetpt&&mujetpt<=32)*0.276+(32<mujetpt&&mujetpt<=37)*0.184+(37<mujetpt&&mujetpt<=42)*0.168+(42<mujetpt&&mujetpt<=47)*0.16+(47<mujetpt&&mujetpt<=52)*0.216+(52<mujetpt&&mujetpt<=57)*0.232+(57<mujetpt&&mujetpt<=62)*0.174+(62<mujetpt&&mujetpt<=67)*0.104)";
  qcdMuIsoRatio_.ReplaceAll("mujetpt",mujetptVarName);

  qcdTauIsoRatio_ = "((20<taujetpt&&taujetpt<=25)*3.3+(25<taujetpt&&taujetpt<=30)*1.32+(30<taujetpt&&taujetpt<=35)*0.463+(35<taujetpt&&taujetpt<=40)*0.208+(40<taujetpt&&taujetpt<=45)*0.126+(45<taujetpt&&taujetpt<=50)*0.0767+(50<taujetpt&&taujetpt<=55)*0.0506+(55<taujetpt&&taujetpt<=60)*0.0362+(60<taujetpt&&taujetpt<=65)*0.031+(65<taujetpt&&taujetpt<=70)*0.0256+(70<taujetpt&&taujetpt<=75)*0.0106+(75<taujetpt&&taujetpt<=80)*0.0131+(80<taujetpt&&taujetpt<=85)*0.0253+(85<taujetpt&&taujetpt<=90)*0.00869+(90<taujetpt&&taujetpt<=95)*0.027+(95<taujetpt&&taujetpt<=100)*0.0286)";
  qcdTauIsoRatio_.ReplaceAll("taujetpt",taujetptVarName);    
    
}


TH1F * QCDEstimate::GetQCDNominal(const char * HistoName, TString variable, int nbins, float xmin , float xmax, int QCDTypeOption){
  cout<<"Estimating QCD:"<<endl;

  if(!DataTrees_[0] || nDataTrees_<1 || !ZTTTree_ || !WJetsTree_ || !TTbarTree_){
    cout<<"Invalid input Trees"<<endl;
    return 0;
  }
  for(unsigned int i=0;i<nDataTrees_;i++)
    if(!(DataTrees_[i])){
      cout<<"Data tree "<<i<<" is NULL"<<endl;
      return 0;
    }


  cout<<"Check your inputs:"<<endl;
  cout<<"Total Entries in trees:"<<endl;
  for(unsigned int i=0;i< nDataTrees_;i++)
    cout<<" Data "<<i<<" "<<DataTrees_[i]->GetName()<<": "<<DataTrees_[i]->GetEntriesFast()<<endl;
  cout<<" ZTT: "<<ZTTTree_->GetEntriesFast()<<"   normFactor="<<ZTTLumiScale_<<endl;
  cout<<" WJets: "<<WJetsTree_->GetEntriesFast()<<"   normFactor="<<WJetsLumiScale_<<endl;
  cout<<" TTbar: "<<TTbarTree_->GetEntriesFast()<<"   normFactor="<<TTbarLumiScale_<<endl;

  cout<<"categorySelection: "<<categorySelection_<<endl;
  cout<<"MuIsoSelection: "<<MuIsoSelection_<<endl;
  cout<<"TauIsoSelection: "<<TauIsoSelection_<<endl;
  cout<<"mTSignalSelection: "<<mTSignalSelection_<<endl;
  cout<<"mTSidebandSelection: "<<mTSidebandSelection_<<endl;

  
  TString qcdIsoRatio;
  TString AntiIsoSelection;
  if(QCDTypeOption==1){//QCD estimate from anti-iso muons
    AntiIsoSelection=TString("((")+TauIsoSelection_+"))*(!("+MuIsoSelection_+"))";
    qcdIsoRatio=qcdMuIsoRatio_;
    cout<<"Calculating QCD from anti-isolated muons"<<endl;
  }else if(QCDTypeOption==2){//QCD estimate from anti-iso taus
    AntiIsoSelection=TString("(!(")+TauIsoSelection_+"))*(("+MuIsoSelection_+"))";
    qcdIsoRatio=qcdTauIsoRatio_;    
    cout<<"Calculating QCD from anti-isolated taus"<<endl;
  }else if(QCDTypeOption==3) {//QCD estimate from  sample where both muons and taus are anti-isolated
    AntiIsoSelection=TString("(!(")+TauIsoSelection_+"))*(!("+MuIsoSelection_+"))";
    qcdIsoRatio=qcdTauIsoRatio_+"*"+qcdMuIsoRatio_;
    cout<<"Calculating QCD from anti-isolated taus and muons"<<endl;
  }else{
    cout<<"QCDTypeOption must be 1,2, or 3"<<endl;
    return 0;
  }

  //compute the WJets+TTbar correction factor in mT sideband
  TH1F HSideData("HSideData","",nbins,xmin,xmax);
  TH1F HSideWJets("HSideWJets","",nbins,xmin,xmax);
  TH1F HSideTTbar("HSideTTbar","",nbins,xmin,xmax);
  TH1F HSideMC("HSideMC","",nbins,xmin,xmax);
  for(unsigned int i=0;i<nDataTrees_;i++){
    TH1F HDataTmp("HDataTmp","",nbins,xmin,xmax);
    DataTrees_[i]->Draw(variable+">>HDataTmp",TString("(")+categorySelection_+")*"+AntiIsoSelection+"*("+mTSidebandSelection_+")*"+qcdIsoRatio);
    HSideData.Add(&HDataTmp);
  }
  cout<<"Data in mT sideband "<<HSideData.Integral()<<endl;
  WJetsTree_->Draw(variable+">>HSideWJets",TString("(")+categorySelection_+")*"+AntiIsoSelection+"*("+mTSidebandSelection_+")*"+qcdIsoRatio);
  TTbarTree_->Draw(variable+">>HSideTTbar",TString("(")+categorySelection_+")*"+AntiIsoSelection+"*("+mTSidebandSelection_+")*"+qcdIsoRatio);
  cout<<"WJets in mT sideband "<<HSideWJets.Integral()*WJetsLumiScale_<<endl;
  cout<<"TTbar in mT sideband "<<HSideTTbar.Integral()*TTbarLumiScale_<<endl;
  HSideMC.Add(&HSideWJets,WJetsLumiScale_);
  HSideMC.Add(&HSideTTbar,TTbarLumiScale_);
  float mcCorrFactor=HSideData.Integral()/HSideMC.Integral();
  cout<<"Correcting WJets+TTbar MC with factor "<<mcCorrFactor<<endl;

  
  //determine the QCD 
  TH1F HSignalData("HSignalData","",nbins,xmin,xmax);
  TH1F HSignalZTT("HSignalZTT","",nbins,xmin,xmax);
  TH1F HSignalWJets("HSignalWJets","",nbins,xmin,xmax);
  TH1F HSignalTTbar("HSignalTTbar","",nbins,xmin,xmax);
  TH1F HSignalMC("HSignalMC","",nbins,xmin,xmax);
  for(unsigned int i=0;i<nDataTrees_;i++){
    TH1F HDataTmp("HDataTmp","",nbins,xmin,xmax);
    DataTrees_[i]->Draw(variable+">>HDataTmp",TString("(")+categorySelection_+")*"+AntiIsoSelection+"*("+mTSignalSelection_+")*"+qcdIsoRatio);
    HSignalData.Add(&HDataTmp);
  }
  cout<<"Data in mT signal "<<HSignalData.Integral()<<endl;
  ZTTTree_->Draw(variable+">>HSignalZTT",TString("(")+categorySelection_+")*"+AntiIsoSelection+"*("+mTSignalSelection_+")*"+qcdIsoRatio);
  WJetsTree_->Draw(variable+">>HSignalWJets",TString("(")+categorySelection_+")*"+AntiIsoSelection+"*("+mTSignalSelection_+")*"+qcdIsoRatio);
  TTbarTree_->Draw(variable+">>HSignalTTbar",TString("(")+categorySelection_+")*"+AntiIsoSelection+"*("+mTSignalSelection_+")*"+qcdIsoRatio);
  cout<<"ZTT in mT signal "<<HSignalZTT.Integral()*ZTTLumiScale_<<endl;
  cout<<"WJets in mT signal "<<HSignalWJets.Integral()*WJetsLumiScale_<<endl;
  cout<<"TTbar in mT signal "<<HSignalTTbar.Integral()*TTbarLumiScale_<<endl;
  HSignalMC.Add(&HSignalWJets,mcCorrFactor*WJetsLumiScale_);
  HSignalMC.Add(&HSignalTTbar,mcCorrFactor*TTbarLumiScale_);
  HSignalMC.Add(&HSignalZTT,ZTTLumiScale_);

  TH1F* HQCD=new TH1F(HistoName,"",nbins,xmin,xmax);
  HQCD->Add(&HSignalData);
  HQCD->Add(&HSignalMC,-1);
  
  cout<<"QCD yield = "<<HQCD->Integral()<<endl;

  //smooth histogram here

  return HQCD;
}

