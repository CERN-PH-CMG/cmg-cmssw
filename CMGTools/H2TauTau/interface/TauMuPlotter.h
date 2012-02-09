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

  Float_t getQCDOStoSSRatio(){return QCDOStoSSRatio_;}

  bool scaleHistos();//contains logic for scaling samples before making plots
  TH1F* getTotalDataSS();//sum of SS Data samples 
  TH1F* getTotalData();
  TH1F* getTotalEmbeddedSS();
  TH1F* getTotalEmbedded();
  TH1F* getTotalMC();//sum of all MCs contributing to the background (only lumi reweighted)
  TH1F* getTotalMCSS();//sum of SS MC's used in the QCD extraction (may exclude additional MC samples declared)
  TH1F* getSample(TString samplename);//can be used to get histo for any MC or Data sample
  TH1F* getSampleSS(TString samplename);//can be used to get histo for any MC or Data sample
  TH1F* getQCD();
  TH1F* getDiBoson();
  TH1F* getZToTauTau();//Z-->tau tau (either from MC or Embedded)
  TH1F* getZToTauTauSS();
  TH1F* getWJets();//WJets from MC and corrected using the high MT sideband
  TH1F* getWJetsSS();
  TH1F* getTotalBackground();//sum of backgrounds determined from MC and data
  TH1F* getTotalBackgroundSS();//sum of backgrounds execpt for the QCD


  bool printRawYields();
  Float_t getWJetsSignalToSBFraction();
  Float_t getWJetsScaleFactor();
  Float_t getWJetsOSCorrFactor(){if(SMcat_<=0)return WJetsOSSideCorr_; else return 1.;}
  Float_t getWJetsSSCorrFactor(){if(SMcat_<=0)return WJetsSSSideCorr_; else return 1.;}
  Float_t getQCDScaleFactor();

  //bool plotDistribution(TString histoname, Int_t rebin, TString xlabel, TString ylabel, Float_t* legendcoords, Float_t* axesrange, bool log=0);
  bool plot(TString variable, Int_t Isocat, Int_t MTcat, Int_t SMcat, Int_t nbins, Float_t xmin, Float_t xmax, TString xlabel="", TString ylabel="", Float_t* legendcoords=0, bool log=0);

  //QCD for SM1/SM2 categories
  TH1F* getQCDSS();
  TH1F* getQCDSide();
  //  TH1F* getQCDFullSide();
   

  void deleteSamples(){
    for(std::vector<Sample*>::const_iterator s=samples_.begin();s!=samples_.end();s++)
      delete *s;
    samples_.clear();
  }  

  void setSmearHistoRes(float res){smearHistoRes_=res;}
  float smearHistoRes_;
  TH1F* smearHisto(TH1F* h);


protected:
 

private:

  TString outputpath_;
  std::vector<Sample*> samples_;   
  Float_t QCDOStoSSRatio_;
  Float_t QCDMuIsoSideRatio_;
  Float_t WJetsOSSideCorr_;
  Float_t WJetsSSSideCorr_;

  TString plotvar_;
  Int_t nbins_;
  Float_t xmin_;
  Float_t xmax_;
  TString selection_;
  Int_t Isocat_;
  Int_t MTcat_;
  Int_t SMcat_;

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
