#ifndef CMGTools_H2TauTau_Sample_H
#define CMGTools_H2TauTau_Sample_H

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
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include <TH1.h>
#include <TList.h>
#include <TIterator.h>
#include <TKey.h>

#include "DataFormats/FWLite/interface/ChainEvent.h"

class Sample : public TNamed {
  
public:
  
  Sample();
  Sample(const char * name, const char * path);
  virtual ~Sample();
  

  void setOutputPath(TString path){outputpath_=path;}
  void setCrossection(float crossection){crossection_=crossection;} //in pb
  void setSampleLumi(float lumi){lumi_=lumi;}
  void setSampleGenEvents(int Ngen){genEvents_=Ngen;}
  void setColor(Int_t color){color_=color;}
  void setLineColor(Int_t color){lcolor_=color;}
  void setLineStyle(Int_t lstyle){lstyle_=lstyle;}
  void setDataType(TString type){    dataType_=type;  }
  void resetScale(){ normFactor_=1.;}
  void scale(Float_t factor){ normFactor_*=factor;}

  //access
  float getCrossection(){return crossection_;}
  TString getDataType(){return dataType_;}
  int getSampleGenEvents(){return genEvents_;}
  float getLumi(){ 
    if(dataType_=="Data" || dataType_=="Data_SS" || dataType_=="Embedded" || dataType_=="Embedded_SS")return lumi_;
    else if(crossection_>0.) return genEvents_/crossection_;
    return 0.;
  }
  float getNorm(){return normFactor_;}
  
  Int_t getColor(){return color_;}
  Int_t getLineColor(){return lcolor_;}
  Int_t getLineStyle(){return lstyle_;}


  TH1F* getHistoNtpFile(TString var,Int_t nbins,Float_t xmin,Float_t xmax,TString selection="");

protected:


private:

  float crossection_;
  float lumi_;//for Data
  int genEvents_;//for MC

  TString outputpath_;

  TChain* ntpChain_;

  TString dataType_;
  Int_t color_;
  Int_t lcolor_;
  Int_t lstyle_;
  float normFactor_;
  bool init_;


  ClassDef(Sample, 1);
};

#endif 
