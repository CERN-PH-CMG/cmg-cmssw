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

#include "DataFormats/FWLite/interface/ChainEvent.h"

class Sample : public TNamed {
  
public:
  
  Sample();
  Sample(const char * name, const char * path);
  virtual ~Sample();
  
  //set
  bool init();

  void setCrossection(float crossection){sampleCrossection_=crossection;} 

  bool addHisto(TH1* hist) {
    if(!hist)return 0;
    sampleHist_.push_back(hist);
    return 1;
  }

  //access
  TString getPath(){return GetTitle();}

  int getNEvents(){return sampleNevents_;}

  float getCrossection(){return sampleCrossection_;}

  fwlite::ChainEvent* getEvents(){
    if(!sampleChain_)cout<<"Error event chain not set"<<endl;
    return sampleChain_;
  }
  
  TH1* getHisto(TString name){
    int hi=0;
    for(std::vector<TH1*>::const_iterator h=sampleHist_.begin(); h!=sampleHist_.end(); ++h, hi++){
      if(TString(sampleHist_[hi]->GetName())==name) return sampleHist_[hi];
    }
    cout<<"Sample "<<GetName()<<" histo "<<name<<" not found "<<endl;
    return NULL;
  }

  bool save(TFile* file=NULL);


  void print();

protected:


private:

  float sampleCrossection_;
  int sampleNevents_;
  int sampleChainNevents_;
  std::vector<std::string> sampleList_; 
  fwlite::ChainEvent* sampleChain_;
  std::vector<TH1*> sampleHist_;

  bool init_;

  ClassDef(Sample, 1);
};

#endif 
