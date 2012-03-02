#ifndef CMGTools_H2TauTau_TauMuPlotter_H
#define CMGTools_H2TauTau_TauMuPlotter_H


#include "CMGTools/H2TauTau/interface/Sample.h"

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

class TauMuPlotter : public TNamed {

public:
  
  TauMuPlotter();
  TauMuPlotter(const char * name);
  virtual ~TauMuPlotter();
  


  //Divide methods to avoid confusion in the categories
  //General Methods 
  void setOutputPath(TString path){outputpath_=path;}
  void addSample(Sample* sample){samples_.push_back(sample);}   
  void setSMCat(Int_t cat){SMcat_=cat;}
  void setQCDOStoSSRatio(Float_t ratio){QCDOStoSSRatio_=ratio;}
  void setQCDColor(Int_t color){QCDColor_=color;}
  void setWJetsColor(Int_t color){WJetsColor_=color;}
  void setTTJetsColor(Int_t color){TTJetsColor_=color;}
  void setZMuMuColor(Int_t color){ZMuMuColor_=color;}
  void setZTauTauColor(Int_t color){ZTauTauColor_=color;}
  void setSignalColor(Int_t color){SignalColor_=color;}

  bool printRawYields(TString selection="");
  bool scaleSamplesLumi();

  ///basic methods which can be used by all categories
  TH1F* getTotalDataSS();//sum of SS Data samples 
  TH1F* getTotalData();
  TH1F* getTotalEmbeddedSS();
  TH1F* getTotalEmbedded();
  TH1F* getTotalMC();//sum of all OS MCs 
  TH1F* getTotalMCSS();//sum of all SS MC's 
  TH1F* getSample(TString samplename);//can be used to get histo for any MC or Data sample
  TH1F* getSampleSS(TString samplename);//can be used to get histo for any MC or Data sample
  TH1F* getDiBoson();
  TH1F* getDiBosonSS();
  TH1F* getZToTauTau();//Z-->tau tau (either from MC or Embedded)
  TH1F* getZToTauTauSS();

 
  //Inclusive/SM0 methods
  bool correctSamplesInc();
  TH1F* getWJetsInc();
  TH1F* getWJetsIncSS();
  TH1F* getQCDInc();//uses the same sign samples
  TH1F* getTotalBackgroundIncSS();//sum of SS backgrounds execpt for the QCD
  //TH1F* getTotalBackgroundInc();//sum of all OS backgrounds 
  bool plotInc(TString variable, Int_t Isocat, Int_t MTcat, Int_t SMcat, Int_t nbins, Float_t xmin, Float_t xmax, TString xlabel="", TString ylabel="", Float_t* legendcoords=0, bool log=0);//OS distributions
  bool plotIncSS(TString variable, Int_t Isocat, Int_t MTcat, Int_t SMcat, Int_t nbins, Float_t xmin, Float_t xmax, TString xlabel="", TString ylabel="", Float_t* legendcoords=0, bool log=0);//SS distributions


  //Boosted/VBF methods
  bool correctSamplesSM();
  //TH1F* getQCDMuIsoSM();//from muon isolation side-band
  TH1F* getWJetsSM();
  TH1F* getWJetsSMSS();
  TString qcdTauIsoRatio_;//formula for the ratio
  TH1F* getQCDTauIsoSM();//from anti-isolated taus
  TH1F* getQCDTauIsoSMSS();//from anti-isolated taus
  TH1F* getTotalMCSM();//sum of OS backgrounds execpt for the QCD
  TH1F* getTotalMCSMSS();//sum of SS backgrounds execpt for the QCD
  bool plotSM(TString variable, Int_t Isocat, Int_t MTcat, Int_t SMcat, Int_t nbins, Float_t xmin, Float_t xmax,  TString extrasel="", TString xlabel="", TString ylabel="", Float_t* legendcoords=0, bool log=0);
  bool plotSMSS(TString variable, Int_t Isocat, Int_t MTcat, Int_t SMcat, Int_t nbins, Float_t xmin, Float_t xmax, TString extrasel="", TString xlabel="", TString ylabel="", Float_t* legendcoords=0, bool log=0);
 

  ////Methods unrelated to the main plots

  //tau fake rate
  void plotTauFakeRate(TString variable, Int_t nbins, Float_t xmin, Float_t xmax, Int_t njets=-1,Float_t ymax=0.2,Bool_t log=0);


  //utilities
  void setSmearHistoRes(float res){smearHistoRes_=res;}
  float smearHistoRes_;
  TH1F* smearHisto(TH1F* h);

  //clean up
  void deleteSamples(){
    for(std::vector<Sample*>::const_iterator s=samples_.begin();s!=samples_.end();s++)
      delete *s;
    samples_.clear();
  }  

protected:
 

private:

  TString outputpath_;
  std::vector<Sample*> samples_;   
  Float_t QCDOStoSSRatio_;
  Float_t QCDMuIsoSideRatio_;
  Float_t QCDOSTauIsoSideRatio_;
  Float_t QCDSSTauIsoSideRatio_;
  Float_t WJetsOSSideCorr_;
  Float_t WJetsSSSideCorr_;
  Float_t WJetsSMOSSideCorr_;
  Float_t WJetsSMSSSideCorr_;


  TString plotvar_;
  Int_t nbins_;
  Float_t xmin_;
  Float_t xmax_;
  TString selection_;
  Int_t Isocat_;
  Int_t MTcat_;
  Int_t SMcat_;
  TString extrasel_;

  //
  Int_t QCDColor_;
  Int_t WJetsColor_;
  Int_t TTJetsColor_;
  Int_t ZMuMuColor_;
  Int_t ZTauTauColor_;
  Int_t SignalColor_;


  ClassDef(TauMuPlotter, 1);
};

#endif 
