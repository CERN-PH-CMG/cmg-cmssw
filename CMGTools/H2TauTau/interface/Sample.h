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
  
  //set
  bool init();

  void setCrossection(float crossection){crossection_=crossection;} //in pb
  void setSampleLumi(float lumi){lumi_=lumi*getProcessEff();}//for Data //correct for batch job failures
  void setSampleGenEvents(int Ngen){genEvents_=Ngen*getProcessEff();}//for MC //correct for batch job failures
  
  void addTrigPath(std::string pathname){trigPaths_.push_back(pathname);}
  void setRunRange(unsigned int first, unsigned int last){firstrun_=first; lastrun_=last;}
  void setEffCorrFactor(float factor){effCorrFactor_=factor;}

  bool addHisto(TH1* hist) {
    if(!hist)return 0;
    if(color_>0){
      hist->SetFillColor(color_);
      hist->SetLineColor(color_);
    }
    if(lstyle_>0) hist->SetLineStyle(lstyle_);
    sampleHist_.push_back(hist);
    return 1;
  }
  
  bool setDataType(TString type){
    dataType_=type;
    return 1;
  }
  
  void setColor(Int_t color){color_=color;}
  void setLineStyle(Int_t lstyle){lstyle_=lstyle;}
  void setPlotOrder(unsigned int n){plotOrder_=n;}

  bool scaleLumi(Float_t lumi){
    if(!histFile_){
      histFile_=new TFile(TString(GetTitle())+"/Sample_Histograms.root","read");
      if(histFile_->IsZombie()) return NULL;
      if(!histFile_->GetListOfKeys()) return NULL;
      if(histFile_->GetListOfKeys()->GetSize()==0) return NULL;
      cout<<" opened file :"<<TString(GetTitle())+"/Sample_Histograms.root"<<endl;
    }
    TList* keys=histFile_->GetListOfKeys();
    if(!keys)return 0;
    TIterator* keyiter=keys->MakeIterator();
    for(TKey* histname=(TKey*)keyiter->Next(); histname; histname=(TKey*)keyiter->Next()){
      TH1* hist=(TH1*)histFile_->Get(histname->GetName());
      hist->Scale(effCorrFactor_);//correct for difference between Data and MC efficiency
      hist->Scale(lumi/getLumi());
    }
    return 1;
  }


  //access
  TString getPath(){return GetTitle();}
  int getNEvents(){return nEvents_;}
  float getCrossection(){return crossection_;}
  float getProcessEff(){return processeff_;}  
  TString getDataType(){return dataType_;}

  float getLumi(){ 
    if(dataType_=="Data" || dataType_=="Data_SS")return lumi_;
    else {
      if(crossection_<1e-6)return 0.;
      return genEvents_/crossection_;
    }
  }
  
  Int_t getColor(){return color_;}
  Int_t getLineStyle(){return lstyle_;}
  unsigned int getPlotOrder(){return plotOrder_;}
  
  fwlite::ChainEvent* getEvents();
  
  TH1* getHisto(TString name){
    std::vector<TH1*>::const_iterator h=sampleHist_.begin();
    for(int i=0; h!=sampleHist_.end(); ++h, i++){
      if(TString(sampleHist_[i]->GetName())==TString(GetName())+"_"+name) return sampleHist_[i];
    }

    cout<<"Sample "<<GetName()<<" histo "<<name<<" not found "<<endl;
    return NULL;
  }

  
  TH1* getHistoFromFile(TString name){
    
    if(!histFile_){
      histFile_=new TFile(TString(GetTitle())+"/Sample_Histograms.root","read");
      if(histFile_->IsZombie()) return NULL;
      if(!histFile_->GetListOfKeys()) return NULL;
      if(histFile_->GetListOfKeys()->GetSize()==0) return NULL;
      cout<<" opened file :"<<TString(GetTitle())+"/Sample_Histograms.root"<<endl;
    }
    
    return (TH1*)histFile_->Get(TString(GetName())+"_"+name) ;
  }

  std::vector<std::string> getTrigPaths(){return trigPaths_;}
  unsigned int getFirstRun(){return firstrun_;}
  unsigned int getLastRun(){return lastrun_;}

  bool save(TFile* file=NULL);


  void print();

protected:


private:

  float crossection_;
  float lumi_;//for Data
  int genEvents_;//for MC
  int nEvents_;
  int chainNEvents_;
  float processeff_;
  std::vector<std::string> sampleList_; 
  fwlite::ChainEvent* sampleChain_;
  std::vector<TH1*> sampleHist_;
  std::vector<std::string> trigPaths_;
  unsigned int firstrun_;
  unsigned int lastrun_;
  float effCorrFactor_;

  TFile* histFile_;

  TString dataType_;
  Int_t color_;
  Int_t lstyle_;
  unsigned int plotOrder_;
  bool init_;
  

  ClassDef(Sample, 1);
};

#endif 
