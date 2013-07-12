#ifndef CMGTools_H2TauTau_TauMuPlotter_H
#define CMGTools_H2TauTau_TauMuPlotter_H


//#include "CMGTools/H2TauTau/interface/Sample.h"
#include "Sample.h"

#include <TVector3.h>
#include <memory>
#include <iostream>
#include <iomanip>
#include <sys/stat.h>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

#include <TROOT.h>
#include <TNamed.h>
#include <TSystem.h>
#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TString.h>
#include <TF1.h>
#include <TGraph.h>


//#include "CMGTools/H2TauTau/interface/TriggerEfficiency.h"

class TauMuPlotter : public TNamed {

public:
  
  TauMuPlotter();
  TauMuPlotter(const char * name);
  virtual ~TauMuPlotter();
  


  //Divide methods to avoid confusion in the categories
  //General Methods 
  void setOutputPath(TString path){outputpath_=path;}
  void addSample(Sample* sample){samples_.push_back(sample);}   
  void setQCDOStoSSRatio(Float_t ratio){QCDOStoSSRatio_=ratio;}
  void setZTTType(int type){ZTTType_=type;}
  void setQCDColor(Int_t color){QCDColor_=color;}
  void setWJetsColor(Int_t color){WJetsColor_=color;}
  void setTTJetsColor(Int_t color){TTJetsColor_=color;}
  void setZMuMuColor(Int_t color){ZMuMuColor_=color;}
  void setZTauTauColor(Int_t color){ZTauTauColor_=color;}
  void setSignalColor(Int_t color){SignalColor_=color;}
  void setTauLooseIsoCut(Float_t cut=0.7){tauLooseIsoCut_=cut;}
  void setMuLooseIsoCut(Float_t cut=0.5){muLooseIsoCut_=cut;}
  
  void setPlotTitle(TString title){plotTitle_=title;}

  ///MSSM methods
  void setMSSMFlag(bool flag){MSSMFlag_=flag;}


  void setVariableBinning(Int_t nbins, Float_t * xbins){
    if(nbins<2||xbins==0)cout<<" bad variable bins"<<endl;
    nbinsVariable_=nbins;
    xbinsVariable_=xbins;
  }
  TH1F * getPlotHisto(TString hname){
    TH1F* h=0;
    if(nbins_>0) h=new TH1F(hname,hname,nbins_,xmin_,xmax_);
    else if(nbinsVariable_>0) h=new TH1F(hname,hname,nbinsVariable_,xbinsVariable_);
    else{ cout<<" histo binning not recognized"<<endl; return 0;}
    h->GetXaxis()->SetTitle(plotvar_);
    h->Sumw2();
    return h;
  }
  bool printRawYields(TString selection="");
  bool scaleSamplesLumi();

  Sample * findSample(TString sampleName){
    for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
      if(TString((*s)->GetName())==sampleName){
	return *s;
      }  
    return NULL;
  }

  Float_t getTotalDataLumi(){
    float totalDataLumi=0;
    for( std::vector<Sample*>::const_iterator s=samples_.begin(); s!=samples_.end(); ++s)
      if((*s)->getDataType()=="Data")
	totalDataLumi+=(*s)->getLumi();
    //cout<<"Total Data lumi = "<<totalDataLumi<<endl;
    return totalDataLumi;
  }

  TString getCurrentSelection();

  ///basic methods which can be used by all categories
  TH1F* getTotalData();
  TH1F* getTotalEmbedded();
  TH1F* getTotalMC();//sum of all OS MCs
  TH1F* getTotalMCSM();//sum of OS backgrounds execpt for the QCD
 
  TH1F* getSample(TString samplename,bool removeLumiNorm=0);//can be used to get histo for any MC or Data sample
  TH1F* getSampleVBFHCPShape(TString sample);//used for the methods below
  TH1F* getSampleVBFHCPShape2(TString sample);//used for the methods below
  TH1F* getSampleVBFHCP(TString sample,int type=0);//used for the methods below
  TH1F* getSampleVBFTightHCPShape(TString sample);//used for the methods below
  TH1F* getSampleVBFTightHCP(TString sample);//used for the methods below

  TH1F* getZNJetSum(TString decay);

  TH1F* getZToTauTau();
  TH1F* getZToTauTauVBF();
 
  TH1F* getZToLJetInc();
  TH1F* getZToLJetVBFHCP();
  TH1F* getZToLJetVBFTightHCP();
  
  TH1F* getZLInc();
  TH1F* getZLVBFHCP();
  TH1F* getZLLVBFHCP();
  TH1F* getZL2012(){return 0;}
  TH1F* getZL2012Type2(){return 0;}
  TH1F* getZLBoost(){return 0;}

  TH1F* getQCDInc(Int_t WType=0,bool cleanNegativeBins=1);
  TH1F* getQCDIncWNJet();
  TH1F* getQCDMuIso();
  TH1F* getQCDMuIsoLooseTau();
  TH1F* getQCDKetiHCP();
  TH1F* getQCDVBFHCP();
  TH1F* getQCDBoostTight();
  TH1F* getQCDVBFLoose();
  TH1F* getQCDVBFTight();
  TH1F* getQCDIncLowPt();
  TH1F* getQCDIncHighPt();
  TH1F* getQCDVBFHCP2(){return 0;}
  TString qcdTauIsoRatioMuNonIso_;//formula for the ratio
  TString qcdMuIsoRatioTauNonIso_;//formula for the ratio
  TString qcdTauIsoRatio_;//formula for the ratio
  TString qcdMuIsoRatio_;//formula for the ratio

  TH1F* getWJetsInc();
  TH1F* getWNJetSum();
  TH1F* getWNJetSumLooseTau();
  TH1F* getWNJetSumNoChCut();
  TH1F* getWNJetSumNoChNoMTCut();
  TH1F* getWJetsNJet(int shape=0);
  TH1F* getWJetsNJetVBFLoose();
  TH1F* getWJetsNJetVBFTight();
  TString wjetsTauIsoRatio_;//formula for the ratio for W+jets 
  TString wjetsTauIsoRatioSS_;//formula for the ratio for W+jets 


  TH1F* getTTJetsInc();
  TH1F* getTTJetsVBFHCP();

  TH1F* getDiBoson();
  TH1F* getDiBosonVBFHCP();
  TH1F* getDiBosonVBFTightHCP();


  bool plotInc(TString variable, Int_t nbins, Float_t xmin, Float_t xmax, Int_t Chcat, Int_t Isocat, Int_t MTcat,TString extrasel="", TString blindsel = "",  Int_t QCDType=0, Int_t WJetsType=0, TString xlabel="", TString ylabel="", Float_t* legendcoords=0, int higgs=0,TString filetag="");

  TH1F* computeTrigEff(TH1F* HPass, TH1F* HFail);
  void plotTauTrigger(Int_t Region, TString tag);
  void plotTauTriggerReal(Int_t Region, TString tag);



  ////Methods unrelated to the main plots
  //tau fake rate
  void plotIsoFakeRate(TString variable, Int_t nbins, Float_t xmin, Float_t xmax, TString extrasel="",TString IsoSel="(tauisodisc>=2)",Float_t ymax=50,Bool_t log=1);//
  void plotTauFakeRateWJets(TString variable, Int_t nbins, Float_t xmin, Float_t xmax, TString extrasel="",Float_t ymax=50,Bool_t log=1);


  //HCP Studies
  void plotQCDSSOSRatio();
  void plotQCDSSOSRatioBTag();
  void compareZTTEmbedded();
  void compareZTTEmbeddedUnfolding();
  void plotZTTShape();

  //
  TH1F* getSignal(long mass);
  void compareSignalShapes();



  //utilities
  void setSmearHistoRes(float res){smearHistoRes_=res;}
  float smearHistoRes_;
  TH1F* smearHisto(TH1F* h);

  void makeDensityHisto(TH1F*h){
    for(Int_t b=1;b<=h->GetNbinsX();b++){
      h->SetBinContent(b,h->GetBinContent(b)/h->GetBinWidth(b));
      h->SetBinError(b,h->GetBinError(b)/h->GetBinWidth(b));//scale the error too
    }
  }

  //clean up
  void deleteSamples(){
    for(std::vector<Sample*>::const_iterator s=samples_.begin();s!=samples_.end();s++)
      delete *s;
    samples_.clear();
  }  


  //selections used for each plot made inside this class

  Int_t Isocat_;
  Int_t Chcat_;
  Int_t MTcat_;
  TString extrasel_;
  TString blindsel_;
  Bool_t MSSMFlag_;

  Float_t QCDOStoSSRatio_;
  Int_t ZTTType_;
  Int_t WJetsType_;
  long mTCut_;
  long leadJetpTCut_;
  long taupTCut_;
  long taupTCut1_;
  long taupTCut2_;
  long higgspTCutLow_;
  long higgspTCutMed_;
  long higgspTCutHigh_;
  TString eventWeight_;
  TString tauIsoCut_;
  TString tauIsoCutQCD_;

  Float_t TTJetsCorrFactor[3];
  float tauLooseIsoCut_;
  float muLooseIsoCut_;

  TString plotvar_;
  Int_t nbins_;
  Float_t xmin_;
  Float_t xmax_;
  
  TString plotvar2_;
  Int_t nbins2_;
  Float_t xmin2_;
  Float_t xmax2_;

  Int_t nbinsVariable_;
  Float_t* xbinsVariable_;

  TString plotTitle_;
  
  Int_t QCDColor_;
  Int_t WJetsColor_;
  Int_t TTJetsColor_;
  Int_t ZMuMuColor_;
  Int_t ZTauTauColor_;
  Int_t SignalColor_;


  TString getSMcut(Int_t sm){
    if(sm<0||6<sm){
      cout<<" Category : "<<sm<<" undefined "<<endl;
      return TString("");
    }

    TString vbfcut="(njet>=2&&njetingap==0&&diJetMass>500.&&abs(diJetDeltaEta)>3.5)";
    TString notvbfcut=TString("(!")+vbfcut+")";
    TString zerojetcut=TString("(njet==0&&nbjet==0)");
    TString onejetcut=TString("(njet>=1&&nbjet==0)");
    TString bjetcut="(njet<2&&nbjet>=1)";
    TString tauhighcut=TString("(taupt>")+taupTCut_+")";
    TString taulowcut=TString("(taupt<")+taupTCut_+")";
    

    TString SMcut[7];
    SMcut[0]=zerojetcut+"*"+taulowcut;
    SMcut[1]=zerojetcut+"*"+tauhighcut;
    SMcut[2]=notvbfcut+"*"+onejetcut+"*"+taulowcut;
    SMcut[3]=notvbfcut+"*"+onejetcut+"*"+tauhighcut;
    SMcut[4]=vbfcut;
    SMcut[5]=bjetcut+"*"+taulowcut;
    SMcut[6]=bjetcut+"*"+tauhighcut;
    cout<<"Category selection : "<<SMcut[sm]<<endl;


    return SMcut[sm];
  }


  TString getSMcutOpt(Int_t sm){
    if(sm<0||6<sm){
      cout<<" Category : "<<sm<<" undefined "<<endl;
      return TString("");
    }

    TString vbfcut="(njet>=2&&njetingap==0&&diJetMass>500.&&abs(diJetDeltaEta)>3.5)";
    TString notvbfcut=TString("(!")+vbfcut+")";
    TString zerojetcut=TString("(njet==0&&nbjet==0)");
    TString onejetcut=TString("(njet>=1&&nbjet==0)");
    TString bjetcut="(njet<2&&nbjet>=1)";
    TString taulowcut=TString("(taupt<")+taupTCut_+")";
    TString tauhighcut=TString("(taupt>")+taupTCut_+")";
    TString higsptcutlow=TString("(ditaumetpt>")+higgspTCutLow_+")";
    TString higsptcuthigh=TString("(ditaumetpt>")+higgspTCutHigh_+")";

     TString notboosthigh=TString("(!(taupt>10000&&ditaumetpt>10000))");
     TString notboosthightau=TString("(taupt<10000)");
//     TString notboosthigh=TString("(!(taupt>45&&ditaumetpt>80))");
//     TString notboosthightau=TString("(taupt<45)");
//     TString notboosthigh=TString("(!(taupt>30&&ditaumetpt>20))");
//     TString notboosthightau=TString("(taupt<30)");

    TString SMcut[7];
    SMcut[0]=zerojetcut+"*"+taulowcut;
    SMcut[1]=zerojetcut+"*"+tauhighcut+"*"+notboosthightau;
    SMcut[2]=notvbfcut+"*"+onejetcut+"*"+taulowcut+"*"+higsptcutlow;
    SMcut[3]=notvbfcut+"*"+onejetcut+"*"+tauhighcut+"*"+higsptcuthigh+"*"+notboosthigh;
    SMcut[4]=vbfcut;
    SMcut[5]=bjetcut+"*"+taulowcut;
    SMcut[6]=bjetcut+"*"+tauhighcut;
    cout<<"Category selection : "<<SMcut[sm]<<endl;

    return SMcut[sm];
  }


  TString getSMcutSummer13(Int_t sm){
    if(sm<0||6<sm){
      cout<<" Category : "<<sm<<" undefined "<<endl;
      return TString("");
    }

    TString vbfcut="(njet>=2&&njetingap==0&&diJetMass>500.&&abs(diJetDeltaEta)>3.5)";
    TString notvbfcut=TString("(!")+vbfcut+")";
    TString zerojetcut=TString("(ditaumetpt<20&&nbjet==0)");
    TString onejetcut=TString("(ditaumetpt>20&&nbjet==0)");
    TString bjetcut="(njet<2&&nbjet>=1)";
    TString taulowcut=TString("(taupt<")+taupTCut_+")";
    TString tauhighcut=TString("(taupt>")+taupTCut_+")";
    TString higsptcutlow=TString("(ditaumetpt>")+higgspTCutLow_+")";
    TString higsptcuthigh=TString("(ditaumetpt>")+higgspTCutHigh_+")";

    TString SMcut[7];
    SMcut[0]=zerojetcut+"*"+taulowcut;
    SMcut[1]=zerojetcut+"*"+tauhighcut;
    SMcut[2]=notvbfcut+"*"+onejetcut+"*"+taulowcut+"*"+higsptcutlow;
    SMcut[3]=notvbfcut+"*"+onejetcut+"*"+tauhighcut+"*"+higsptcuthigh;
    SMcut[4]=vbfcut;
    SMcut[5]=bjetcut+"*"+taulowcut;
    SMcut[6]=bjetcut+"*"+tauhighcut;
    cout<<"Category selection : "<<SMcut[sm]<<endl;

    return SMcut[sm];
  }



  TString getSMcutOpt2(Int_t sm){
    if(sm<0||6<sm){
      cout<<" Category : "<<sm<<" undefined "<<endl;
      return TString("");
    }

    TString vbfcut="(njet>=2&&njetingap==0&&diJetMass>500.&&abs(diJetDeltaEta)>3.5)";
    TString notvbfcut=TString("(!")+vbfcut+")";
    TString zerojetcut=TString("(njet==0&&nbjet==0)");
    TString onejetcut=TString("(njet>=1&&nbjet==0)");

    TString taulowcut=TString("(taupt>20)");//need to add another variable here in case optmization cuts higher, also see below
    TString taumedcut=TString("(taupt>")+taupTCut1_+")";
    TString tauhighcut=TString("(taupt>")+taupTCut2_+")";
    TString higsptcutlow=TString("(ditaumetpt>")+higgspTCutLow_+")";
    TString higsptcutmed=TString("(ditaumetpt>")+higgspTCutMed_+")";
    TString higsptcuthigh=TString("(ditaumetpt>")+higgspTCutHigh_+")";
    
    //Note that optimization of these cuts is done with the boost_high category defined in the previous function
    TString boosthigh=tauhighcut+"*"+higsptcuthigh;
    TString notboosthigh=TString("(!(")+boosthigh+"))";
    TString boostmed=taumedcut+"*"+higsptcutmed+"*"+notboosthigh;
    TString notboostmed=TString("(!(")+boostmed+"))";
    TString boostlow=taulowcut+"*"+higsptcutlow+"*"+notboosthigh+"*"+notboostmed;

    TString SMcut[6];
    SMcut[0]=zerojetcut+"*"+"(taupt>20&&taupt<"+taupTCut1_+")";
    SMcut[1]=zerojetcut+"*"+"(taupt>"+taupTCut1_+"&&taupt<"+taupTCut2_+")";
    SMcut[2]=zerojetcut+"*"+"(taupt>"+taupTCut2_+")";
    SMcut[3]=notvbfcut+"*"+onejetcut+"*"+boostlow;
    SMcut[4]=notvbfcut+"*"+onejetcut+"*"+boostmed;
    SMcut[5]=notvbfcut+"*"+onejetcut+"*"+boosthigh;
    cout<<"Category selection : "<<SMcut[sm]<<endl;
    
    
    return SMcut[sm];
  }



  TString getSMCategory(Int_t sm){
    if(sm<0||12<sm){
      cout<<" Category : "<<sm<<" undefined "<<endl;
      return TString("0");
    }


    TString zerojetcut=TString("(njet==0&&nbjet==0)");
    TString onejetcut=TString("(njet>=1&&nbjet==0)");
    TString vbftightcut="(njet>=2&&njetingap==0&&nbjet==0&&diJetMass>700.&&abs(diJetDeltaEta)>4.0&&ditaumetpt>100)";
    TString notvbftightcut=TString("(!")+vbftightcut+")";
    TString vbfcut="(njet>=2&&njetingap==0&&nbjet==0&&diJetMass>500.&&abs(diJetDeltaEta)>3.5)";
    TString notvbfcut=TString("(!")+vbfcut+")";
    TString btagcut="(nbjet>=1&&njet<2)";
    TString notbtagcut=TString("(!")+btagcut+")";

    TString SMcut[13];
    SMcut[0]=zerojetcut+"*"+"(20<taupt&&taupt<30)";
    SMcut[1]=zerojetcut+"*"+"(30<taupt&&taupt<45)";
    SMcut[2]=zerojetcut+"*"+"(45<taupt)";
    SMcut[3]=notvbfcut+"*"+onejetcut+"*"+"(30<taupt&&taupt<45)";
    SMcut[4]=notvbfcut+"*"+onejetcut+"*"+"(45<taupt&&ditaumetpt<100)";
    SMcut[5]=notvbfcut+"*"+onejetcut+"*"+"(45<taupt&&100<ditaumetpt)";
    SMcut[6]=notvbftightcut+"*"+vbfcut;
    SMcut[7]=vbftightcut;
    SMcut[8]="1";
    SMcut[9]=btagcut;
    SMcut[10]=notbtagcut;
    SMcut[11]=btagcut;//for finebin 
    SMcut[12]=notbtagcut;
    cout<<"Category selection : "<<SMcut[sm]<<endl;
    
    
    return SMcut[sm];
  }





  TString getSMcutTauTau(Int_t sm){
    if(sm<0||6<sm){
      cout<<" Category : "<<sm<<" undefined "<<endl;
      return TString("");
    }

    cout<<" Warning using Boost and VBF selections for TauTau !!!!!!!!!!!!!!! "<<endl;
    mTCut_=50;
    cout<<" set mT cut to 50 !!!!!"<<endl;
    TString tauPtCutTauTau="(taupt>=45.&&mupt>30)";
    TString jetPtCutTauTau="(leadJetPt>50&&abs(leadJetEta)<3.0)";
    TString vbfcutTauTau="(njet>=2&&njetingap==0&&vbfvars3>110&&diJetMass>250.&&abs(diJetDeltaEta)>2.5)";
    TString notvbfcutTauTau=TString("(!")+vbfcutTauTau+")";
    TString boostcutTauTau="(njet>=1&&nbjet==0&&vbfvars3>140)";
    TString zerojetcut="(njet==0&&nbjet==0)";

    TString SMcut[7];
    SMcut[0]="(0)";
    SMcut[1]=tauPtCutTauTau+"*"+zerojetcut;
    SMcut[2]="(0)";
    SMcut[3]=tauPtCutTauTau+"*"+jetPtCutTauTau+"*"+boostcutTauTau+"*"+notvbfcutTauTau;
    SMcut[4]=tauPtCutTauTau+"*"+jetPtCutTauTau+"*"+vbfcutTauTau;

    cout<<"Category selection for TauTau : "<<SMcut[sm]<<endl;

    return SMcut[sm];
  }
  



protected:
 

private:

  TString outputpath_;
  std::vector<Sample*> samples_;   

  Float_t QCDMuIsoSideRatio_;


  void fixFileTag(TString * filetag);

  ClassDef(TauMuPlotter, 1);
};

#endif 
