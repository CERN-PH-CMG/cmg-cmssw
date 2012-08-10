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

  void setTauLooseIsoCut(Float_t cut=0.7){tauLooseIsoCut_=cut;}
  void setMuLooseIsoCut(Float_t cut=0.5){muLooseIsoCut_=cut;}
  
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
    cout<<"Total Data lumi = "<<totalDataLumi<<endl;
    return totalDataLumi;
  }

  ///basic methods which can be used by all categories
  TH1F* getTotalData();
  TH1F* getTotalEmbedded();
  TH1F* getTotalMC();//sum of all OS MCs 
  TH1F* getSample(TString samplename);//can be used to get histo for any MC or Data sample
  TH1F* getDiBoson();
  TH1F* getZToTauTau();//Z-->tau tau (either from MC or Embedded)
 
  //Inclusive/SM0 methods
  TH1F* getTTJetsInc();
  TH1F* getZToLJetInc();
  TH1F* getZToMuMuInc(){return 0;}//dummy method just make script work for datacard
  TH1F* getZToEEInc();
  TH1F* getWJetsInc();
  TH1F* getWJetsInc2012();
  TH1F* getWJetsIncShape();
  TH1F* getWJetsIncShape2012();
  TH1F* getW3Jets();
  TH1F* getW3JetsVBF();
  TH1F* getW2JetsBJet();
  TH1F* getQCDInc();
  TH1F* getQCDIncLooseShape();
  TH1F* getQCDIncWJetsShape();
  TH1F* getQCDIncFit();
  TH1F* getQCDMike();
  TH1F* getQCDKeti();

  ///
  bool plotInc(TString variable, Int_t nbins, Float_t xmin, Float_t xmax, Int_t Chcat, Int_t Isocat, Int_t MTcat,TString extrasel="", TString blindsel = "",  Int_t QCDType=0, Int_t WJetsType=0, TString xlabel="", TString ylabel="", Float_t* legendcoords=0, int higgs=0,TString filetag="");


  //Boosted/VBF methods
  TH1F* getQCDMuIsoSM();//from muon isolation side-band
  TH1F* getWJetsSM();
  TString qcdTauIsoRatioMuNonIso_;//formula for the ratio
  TString qcdMuIsoRatioTauNonIso_;//formula for the ratio
  TString qcdTauIsoRatio_;//formula for the ratio
  TString qcdMuIsoRatio_;//formula for the ratio
  //TH1F* getQCDTauIsoSM();//from anti-isolated taus
  TH1F* getQCDIsoSM();//from anti-isolated taus or anti iso muons
  TString wjetsTauIsoRatio_;//formula for the ratio for W+jets 
  TH1F* getWJetsTauIsoSM();//from anti-isolated taus
  TString wjetsTauIsoRatioSS_;//formula for the ratio for W+jets 
  TH1F* getTotalMCSM();//sum of OS backgrounds execpt for the QCD


  ///MSSM methods
  void setMSSMFlag(bool flag){MSSMFlag_=flag;}


  ////Methods unrelated to the main plots
  //tau fake rate
  //void plotIsoFakeRate(TString variable, Int_t nbins, Float_t xmin, Float_t xmax, TString extrasel="",TString IsoSel="(tauisodisc>=2)",Float_t ymax=50,Bool_t log=1);//
  void plotTauFakeRateWJets(TString variable, Int_t nbins, Float_t xmin, Float_t xmax, TString extrasel="",Float_t ymax=50,Bool_t log=1);


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

  TString plotvar_;
  Int_t nbins_;
  Float_t xmin_;
  Float_t xmax_;
  
  TString plotvar2_;
  Int_t nbins2_;
  Float_t xmin2_;
  Float_t xmax2_;
  

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
    TString metcut="(metpt>30)";
    TString vbfcut="(njet>=2&&njetingap==0&&vbfmva>0.5)";
    TString notvbfcut=TString("(!")+vbfcut+")";
    TString boostcut=TString("(njet>=1&&nbjet==0)")+"*"+metcut;
    TString notboostcut=TString("(!")+boostcut+")";
    TString bjetcut="(njet<2&&nbjet>=1)";
    TString notbjetcut=TString("(!")+bjetcut+")";
    TString taulowcut="(taupt<40.)";
    TString tauhighcut="(taupt>=40.)";
    TString SMcut[7];
    SMcut[0]=notvbfcut+"*"+notboostcut+"*"+notbjetcut+"*"+metcut+"*"+taulowcut;
    SMcut[1]=notvbfcut+"*"+notboostcut+"*"+notbjetcut+"*"+metcut+"*"+tauhighcut;
    SMcut[2]=notvbfcut+"*"+boostcut+"*"+taulowcut;
    SMcut[3]=notvbfcut+"*"+boostcut+"*"+tauhighcut;
    SMcut[4]=vbfcut;
    SMcut[5]=notvbfcut+"*"+notboostcut+"*"+bjetcut+"*"+taulowcut;
    SMcut[6]=notvbfcut+"*"+notboostcut+"*"+bjetcut+"*"+tauhighcut;
    cout<<"Category selection : "<<SMcut[sm]<<endl;
    return SMcut[sm];
  }


protected:
 

private:

  TString outputpath_;
  std::vector<Sample*> samples_;   
  Float_t QCDOStoSSRatio_;
  Float_t QCDOStoSSRatioErr_;
  Float_t QCDMuIsoSideRatio_;
  Float_t QCDOSTauIsoSideRatio_;
  Float_t QCDSSTauIsoSideRatio_;
  Float_t WJetsOSSideCorr_;
  Float_t WJetsSSSideCorr_;
  Float_t WJetsOSSideCorrErr_;
  Float_t WJetsSSSideCorrErr_;

  TString plotTitle_;

  Int_t ZTTType_;

  Int_t WJetsType_;

  Bool_t MSSMFlag_;
  
  float tauLooseIsoCut_;
  float muLooseIsoCut_;

  Int_t nbinsVariable_;//for variable with binning
  Float_t* xbinsVariable_;

  ///
  void fixFileTag(TString * filetag);

  ClassDef(TauElePlotter, 1);
};

#endif 
