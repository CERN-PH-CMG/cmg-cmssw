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
  
  bool init();

  void setOutputPath(TString path){outputpath_=path;}
  void setCrossection(float crossection){crossection_=crossection;} //in pb
  void setSampleLumi(float lumi){lumi_=lumi;}
  void setSampleGenEvents(int Ngen){genEvents_=Ngen;}
  
  void addTrigPath(std::string pathname){trigPaths_.push_back(pathname);}
  void addTrigPaths(std::vector<std::string> * paths){
    if(!paths)return;
    for(std::vector<std::string>::const_iterator p=paths->begin(); p!=paths->end(); ++p) trigPaths_.push_back(*p);
  }
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
    mainsampleHist_.push_back(hist);
    return 1;
  }
  void cloneHistos(TString tag){
    for(std::vector<TH1*>::const_iterator h=mainsampleHist_.begin(); h!=mainsampleHist_.end(); ++h)
      sampleHist_.push_back((TH1*)(*h)->Clone(TString((*h)->GetName())+"_"+tag));
  }

  
  void setDataType(TString type){
    dataType_=type;
  }
  
  void setColor(Int_t color){color_=color;}
  void setLineStyle(Int_t lstyle){lstyle_=lstyle;}
  void setPlotOrder(unsigned int n){plotOrder_=n;}
  void setApplyRecoilCorr(bool applyCorr){applyRecoilCorr_=applyCorr;}
  void setApplyTauRateWeight(bool applyWeight){applyTauRateWeight_=applyWeight;}

  bool scaleLumi(Float_t lumi){
    if(dataType_=="Data" || dataType_=="Data_SS") return 0;
    if(!histFile_){//check the histograms exist
      histFile_=new TFile(outputpath_+"/"+GetName()+"_Sample_Histograms.root","read");
      if(histFile_->IsZombie()) return NULL;
      if(!histFile_->GetListOfKeys()) return NULL;
      if(histFile_->GetListOfKeys()->GetSize()==0) return NULL;
      cout<<" opened file :"<<outputpath_+"/"+GetName()+"_Sample_Histograms.root"<<endl;
    }
    //scale all histograms
    TList* keys=histFile_->GetListOfKeys();
    if(!keys)return 0;
    TIterator* keyiter=keys->MakeIterator();
    for(TKey* histname=(TKey*)keyiter->Next(); histname; histname=(TKey*)keyiter->Next())
      ((TH1*)histFile_->Get(histname->GetName()))->Scale((effCorrFactor_*lumi)/getLumi());
    return 1;
  }


  //access
  TString getPath(){return GetTitle();}
  int getNEvents(){return nEvents_;}
  float getCrossection(){return crossection_;}
  TString getDataType(){return dataType_;}
  int getSampleGenEvents(){return genEvents_;}

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
  bool getApplyRecoilCorr(){return applyRecoilCorr_;}
  bool getApplyTauRateWeight(){return applyTauRateWeight_;}
  

  fwlite::ChainEvent* getEvents();
  
  TH1* getHisto(TString name){    
    for(std::vector<TH1*>::const_iterator h=sampleHist_.begin(); h!=sampleHist_.end(); ++h)
      if(TString((*h)->GetName()) == TString(GetName())+"_"+name) return *h;
    cout<<"Sample "<<GetName()<<" histo "<<name<<" not found "<<endl;
    return NULL;
  }

  
  TH1* getHistoFromFile(TString name){
    
    if(!histFile_){
      histFile_=new TFile(outputpath_+"/"+GetName()+"_Sample_Histograms.root","read");
      if(histFile_->IsZombie()) return NULL;
      if(!histFile_->GetListOfKeys()) return NULL;
      if(histFile_->GetListOfKeys()->GetSize()==0) return NULL;
      cout<<" opened file :"<<outputpath_+"/"+GetName()+"_Sample_Histograms.root"<<endl;
    }
    
    return (TH1*)histFile_->Get(TString(GetName())+"_"+name) ;
  }

  std::vector<std::string> * getTrigPaths(){return &trigPaths_;}
  unsigned int getFirstRun(){return firstrun_;}
  unsigned int getLastRun(){return lastrun_;}

  bool save();


  void print();

protected:


private:

  float crossection_;
  float lumi_;//for Data
  int genEvents_;//for MC
  int nEvents_;
  int chainNEvents_;
  std::vector<std::string> sampleList_; 
  fwlite::ChainEvent* sampleChain_;
  std::vector<TH1*> sampleHist_;//all histos including clones
  std::vector<TH1*> mainsampleHist_;//list of histos added with addHisto()
  std::vector<std::string> trigPaths_;
  unsigned int firstrun_;
  unsigned int lastrun_;
  float effCorrFactor_;

  TString outputpath_;
  TFile* histFile_;

  TString dataType_;
  Int_t color_;
  Int_t lstyle_;
  unsigned int plotOrder_;
  bool applyRecoilCorr_;
  bool applyTauRateWeight_;

  bool init_;
  
    

  ClassDef(Sample, 1);
};

#endif 
