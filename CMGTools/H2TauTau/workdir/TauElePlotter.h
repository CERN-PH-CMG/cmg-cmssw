#ifndef CMGTools_H2TauTau_TauElePlotter_H
#define CMGTools_H2TauTau_TauElePlotter_H


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


class TauElePlotter : public TNamed {

public:
  
  TauElePlotter();
  TauElePlotter(const char * name);
  virtual ~TauElePlotter();
  


  //Divide methods to avoid confusion in the categories
  //General Methods 
  void setOutputPath(TString path){outputpath_=path;}
  void addSample(Sample* sample){samples_.push_back(sample);}   
  void setSMCat(Int_t cat){SMcat_=cat;}
  void setQCDOStoSSRatio(Float_t ratio){QCDOStoSSRatio_=ratio;}
  void setZTTType(int type){ZTTType_=type;}
  void setQCDColor(Int_t color){QCDColor_=color;}
  void setWJetsColor(Int_t color){WJetsColor_=color;}
  void setTTJetsColor(Int_t color){TTJetsColor_=color;}
  void setZMuMuColor(Int_t color){ZMuMuColor_=color;}
  void setZTauTauColor(Int_t color){ZTauTauColor_=color;}
  void setSignalColor(Int_t color){SignalColor_=color;}

  //void setTauLooseIsoCut(Float_t cut=0.7){tauLooseIsoCut_=cut;}
  //void setMuLooseIsoCut(Float_t cut=0.5){muLooseIsoCut_=cut;}
  
  void setPlotTitle(TString title){plotTitle_=title;}

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

  ///basic methods 
  TH1F* getSample(TString samplename, bool removeLumiNorm=0);
  TH1F* getTotalData();
  TH1F* getTotalEmbedded();
  TH1F* getDiBoson();
  TH1F* getZToTauTau();//Z-->tau tau (either from MC or Embedded)
 
  //Inclusive methods
  TH1F* getTTJetsInc();
  TH1F* getZNJetSum(TString decay);
  TH1F* getZToLJetInc();
  TH1F* getZLInc();

  TH1F* getTotalMCSM(Int_t WType=0);//sum of OS backgrounds execpt for the QCD

  TH1F* getWNJetSum();
  TH1F* getWNJetSumLooseTau();
  TH1F* getWNJetSumNoChCut();
  TH1F* getWNJetSumNoChNoMTCut();
  TH1F* getWJetsInc();
  TH1F* getWJetsNJet(int shape=0);
  TH1F* getWJetsIncShape();
  TH1F* getW2JetsBJet();
  TH1F* getW3JetsVBF();
  TH1F* getWJetsNJetVBFHCP();
  TH1F* getWJetsNJetVBFLoose();
  TH1F* getWJetsNJetVBFTight();

  TH1F* getQCDInc(Int_t WType=0,bool cleanNegativeBins=1);
  TH1F* getQCDIncWNJet();
  TH1F* getQCDMuIso();
  TH1F* getQCDMuIsoLooseTau();
  TH1F* getQCDIncWJetsShape();
  TH1F* getQCDIncFit();
  TH1F* getQCDMike();
  TH1F* getQCDKeti();
  TH1F* getQCDKetiHCP();//
  TH1F* getQCDIncLooseShape();
  TH1F* getQCDIncHighPt();
  TH1F* getQCDIncLowPt();

  ///HCP 2012 Methods
  TH1F* getQCDVBFHCP();
  TH1F* getQCDVBFHCP2();
  TH1F* getSampleVBFHCPShape(TString sample);//used for the methods below
  TH1F* getSampleVBFHCP(TString sample);//used for the methods below
  TH1F* getSampleVBFTightHCPShape(TString sample);//used for the methods below
  TH1F* getSampleVBFTightHCP(TString sample);//used for the methods below
  TH1F* getTTJetsVBFHCP();
  TH1F* getZToLJetVBFHCP();
  TH1F* getZToLJetVBFTightHCP();
  TH1F* getZLVBFHCP();
  TH1F* getZLLVBFHCP();
  TH1F* getDiBosonVBFHCP();
  TH1F* getDiBosonVBFTightHCP();
  TH1F* getZToTauTauVBF();
  TH1F* getZL2012();
  TH1F* getZL2012Type1();
  TH1F* getZL2012Type1LowHigh();
  TH1F* getZL2012Type2();
  TH1F* getZLBoost();

  TH1F* getQCDBoostTight();
  TH1F* getQCDVBFLoose();
  TH1F* getQCDVBFTight();


  ///
  bool plotInc(TString variable, Int_t nbins, Float_t xmin, Float_t xmax, Int_t Chcat, Int_t Isocat, Int_t MTcat,TString extrasel="", TString blindsel = "",  Int_t QCDType=0, Int_t WJetsType=0, TString xlabel="", TString ylabel="", Float_t* legendcoords=0, int higgs=0,TString filetag="");



  ///MSSM methods
  void setMSSMFlag(bool flag){MSSMFlag_=flag;}


  ////Methods unrelated to the main plots
  //tau fake rate
  //void plotIsoFakeRate(TString variable, Int_t nbins, Float_t xmin, Float_t xmax, TString extrasel="",TString IsoSel="(tauisodisc>=2)",Float_t ymax=50,Bool_t log=1);//
  void plotTauFakeRateWJets(TString variable, Int_t nbins, Float_t xmin, Float_t xmax, TString extrasel="",Float_t ymax=50,Bool_t log=1);
  void plotQCDSSOSRatio();
  void compareZTTEmbedded();

  TH1F* computeTrigEff(TH1F* HPass, TH1F* HFail);
  void plotTauTrigger(Int_t Region, TString tag);

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
  Int_t Chcat_;
  Int_t Isocat_;
  Int_t MTcat_;
  Int_t SMcat_;
  TString extrasel_;
  TString blindsel_;

  long mTCut_;
  TString eventWeight_;

  TString plotvar_;
  Int_t nbins_;
  Float_t xmin_;
  Float_t xmax_;
  
  TString plotvar2_;
  Int_t nbins2_;
  Float_t xmin2_;
  Float_t xmax2_;
  
  Float_t QCDOStoSSRatio_;
  Float_t TTJetsCorrFactor[3];

  //
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
    TString metcut="(metpt>30)";//30 for PFMET
    //TString vbfcut="(njet>=2&&njetingap==0&&vbfmva>0.5)";
    //TString vbfcut="(njet>=2&&njetingap==0&&vbfmva2012>0.9)";
    TString vbfcut="(njet>=2&&njetingap==0&&diJetMass>500.&&abs(diJetDeltaEta)>3.5)";
    TString notvbfcut=TString("(!")+vbfcut+")";
    TString boostcut=TString("(njet>=1&&nbjet==0)")+"*"+metcut;
    TString boostcutnomet=TString("(njet>=1&&nbjet==0)");
    TString notboostcut=TString("(!")+boostcut+")";
    TString bjetcut="(njet<2&&nbjet>=1)";
    TString notbjetcut=TString("(!")+bjetcut+")";
    TString zerojetcut=TString("(njet==0&&nbjet==0)");
    TString taulowcut="(taupt<40.)";
    TString tauhighcut="(taupt>=40.)";
    TString SMcut[7];
    SMcut[0]=zerojetcut+"*"+taulowcut;
    SMcut[1]=zerojetcut+"*"+tauhighcut;
    SMcut[2]=notvbfcut+"*"+boostcut+"*"+taulowcut;
    //SMcut[2]=notvbfcut+"*"+boostcutnomet+"*"+taulowcut;
    SMcut[3]=notvbfcut+"*"+boostcut+"*"+tauhighcut;
    SMcut[4]=vbfcut;
    SMcut[5]=bjetcut+"*"+taulowcut;
    SMcut[6]=bjetcut+"*"+tauhighcut;
    cout<<"Category selection : "<<SMcut[sm]<<endl;
    return SMcut[sm];
  }

  TString getSMCategory(Int_t sm){
    if(sm<0||12<sm){
      cout<<" Category : "<<sm<<" undefined "<<endl;
      return TString("0");
    }

    TString metcut="(metpt>30)";
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
    SMcut[3]=notvbfcut+"*"+onejetcut+"*"+"(30<taupt&&taupt<45)"+"*"+metcut;
    SMcut[4]=notvbfcut+"*"+onejetcut+"*"+"(45<taupt&&ditaumetpt<100)"+"*"+metcut;
    SMcut[5]=notvbfcut+"*"+onejetcut+"*"+"(45<taupt&&100<ditaumetpt)"+"*"+metcut;
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


protected:
 

private:

  TString outputpath_;
  std::vector<Sample*> samples_;   

  TString plotTitle_;

  Int_t ZTTType_;

  Int_t WJetsType_;

  Bool_t MSSMFlag_;
  
  Int_t nbinsVariable_;
  Float_t* xbinsVariable_;

  void fixFileTag(TString * filetag);

  ClassDef(TauElePlotter, 1);
};

#endif 
