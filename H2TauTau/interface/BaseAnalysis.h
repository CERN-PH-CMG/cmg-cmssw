#ifndef CMGTools_H2TauTau_BaseAnalysis_H
#define CMGTools_H2TauTau_BaseAnalysis_H



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


#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "CMGTools/H2TauTau/interface/Sample.h"



class BaseAnalysis : public TNamed {
 
public:
  
  BaseAnalysis();
  BaseAnalysis(const char * name);
  virtual ~BaseAnalysis();

  bool createMCPUPHisto();

  void addSample(Sample* sample){samples_.push_back(sample);}
  
  virtual bool init();

  virtual bool createHistos(TString samplename="RelValZTT");
 

  void setTruncateEvents(int maxEvents){truncateEvents_=maxEvents;}
  void setPrintFreq(int freq){ printFreq_=freq;}

protected:
  virtual bool addHistos(Sample* s);
  virtual bool getHistos(Sample* s);
  virtual bool applySelections(const fwlite::Event * event);
  virtual bool fillHistos(const fwlite::Event * event );
  virtual bool scaleWeightHistos(Sample* s);

  std::vector<Sample*> samples_;
  Sample* sample_;

  int truncateEvents_;
  int printFreq_;
  float mcPUPWeight_;

  ///Histograms for output
  TH1F* runNumberHisto_;
  TH1D* nPUPVertexHisto_;
  TH1F* nVertexHisto_;
  TH2F* vertexXYHisto_;
  TH1F* vertexZHisto_;

  //histograms reweighted for pile-up
  TH1F* nVertexPUPWeightHisto_;

  //
  TH1F* triggerHisto_;
  
private:


  TH1F* mcPUPWeightHisto_;



  ClassDef(BaseAnalysis, 1);
};

#endif 
