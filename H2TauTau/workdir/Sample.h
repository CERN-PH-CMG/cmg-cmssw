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
#include <TH2.h>
#include <TList.h>
#include <TIterator.h>
#include <TKey.h>

//#include "DataFormats/FWLite/interface/ChainEvent.h"

class Sample : public TNamed {
  
public:
  
  Sample();
  Sample(const char * name, const char * path);
  virtual ~Sample();
  

  void setOutputPath(TString path){outputpath_=path;}
  void addRootFiles(TString name){addFileNames[nNames_]=name; nNames_++;}
  void setCrossection(float crossection){crossection_=crossection;} //in pb
  void setSampleLumi(float lumi){lumi_=lumi;}
  void setSampleGenEvents(float Ngen){genEvents_=Ngen;}
  void setColor(Int_t color){color_=color;}
  void setLineColor(Int_t color){lcolor_=color;}
  void setLineStyle(Int_t lstyle){lstyle_=lstyle;}
  void setDataType(TString type){    dataType_=type;  }
  void resetScale(){ normFactor_=1.;}
  void scale(Float_t factor){if(!ntpChain_) openNtpFile(); normFactor_ *= factor;}

  void setSignalMass(Float_t mass){signalMass_=mass;}
  float getSignalMass(){return signalMass_;}

  //access
  TChain * getTChain(){if(!ntpChain_) openNtpFile(); return ntpChain_; }
  float getCrossection(){return crossection_;}
  TString getDataType(){return dataType_;}
  int getSampleGenEvents(){return genEvents_;}
  float getLumi(){ 
    if(dataType_=="Data"  || dataType_=="Embedded" )
      return lumi_;    
    else if(crossection_>0.){
      if(!ntpChain_) if(!openNtpFile()) return 0.;
      return genEvents_/crossection_;
    }

    return 0.;
  }
  float getNorm(){return normFactor_;}
  
  Int_t getColor(){return color_;}
  Int_t getLineColor(){return lcolor_;}
  Int_t getLineStyle(){return lstyle_;}
  Int_t getTreeEntries(){
    if(!ntpChain_) 
      if(!openNtpFile()) return 0;
    return ntpChain_->GetEntries();
  }


  TH1F* getHistoNtpFile(TString xvar,Int_t xnbins,Float_t xmin,Float_t xmax,TString selection="");
  TH1F* getHistoNtpFile(TString xvar,Int_t Nbins, Float_t * Xbins,TString selection="");
  
  TH2F* getHistoNtpFile(TString xvar,Int_t xnbins,Float_t xmin,Float_t xmax,TString yvar,Int_t ynbins,Float_t ymin,Float_t ymax,TString selection="");


protected:


private:

  float crossection_;
  float lumi_;//for Data
  float genEvents_;//for MC

  TString outputpath_;
  Int_t  nNames_;
  TString addFileNames[5];


  TChain* ntpChain_;
  Int_t NMAXFILES_;

  TString dataType_;
  Int_t color_;
  Int_t lcolor_;
  Int_t lstyle_;
  float normFactor_;
  bool init_;

  Float_t signalMass_;
  
  bool openNtpFile();

  int getNProcEvents(TString logfile);

  ClassDef(Sample, 1);
};

#endif 
