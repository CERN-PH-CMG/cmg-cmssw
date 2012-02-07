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
#include <TF1.h>

#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "AnalysisDataFormats/CMGTools/interface/TriggerObject.h"


#include "CMGTools/H2TauTau/interface/Sample.h"


class BaseAnalysis : public TNamed {
 
public:
  
  BaseAnalysis();
  BaseAnalysis(const char * name);
  virtual ~BaseAnalysis();

  void setVerbosity(unsigned int level){verbosity_=level;}
  void addSample(Sample* sample){samples_.push_back(sample);}  
  void setOutputPath(TString path){outputpath_=path;}
  void setTruncateEvents(int maxEvents){truncateEvents_=maxEvents;}
  void setPrintFreq(int freq){ printFreq_=freq;}
  void setPupWeightName(string weightname){pupWeightName_=weightname;}
  void setSmearHistoRes(float res){smearHistoRes_=res;}

  virtual bool init();
  virtual bool createHistos(TString samplename="RelValZTT");
  
  void deleteSamples(){
    for(std::vector<Sample*>::const_iterator s=samples_.begin();s!=samples_.end();s++)
      delete *s;
    samples_.clear();
  }  
  
  void createMCPileUP();


protected:

  virtual bool addHistos(Sample* s);
  virtual bool getHistos(TString tag = "");
  virtual bool fillVariables(const fwlite::Event * event);
  virtual bool applySelections();
  virtual bool fillHistos(TString tag = "");

  std::vector<Sample*> samples_;
  Sample* sample_;

  unsigned int verbosity_;
  int truncateEvents_;
  int printFreq_;
  string pupWeightName_;
  float eventWeight_;
  float pupWeight_;
  unsigned int runnumber_;
  unsigned int lumiblock_;
  unsigned int eventid_;
  edm::Handle< std::vector<reco::Vertex> > vertices_;
  TString outputpath_;
  bool trigpass_;

  ///Event level histograms Histograms
  TH1F* runNumberHisto_;
  TH1F* nVertexHisto_;
  TH2F* vertexXYHisto_;
  TH1F* vertexZHisto_;

  
  //useful functions
  edm::Handle< std::vector<reco::GenParticle> > genParticles_;
  void printMCGen(edm::Handle< std::vector<reco::GenParticle> > & genList);
  
  edm::Handle< std::vector<cmg::TriggerObject> > trigObjs_;
  bool trigObjMatch(float eta, float phi, std::string path, std::string filter);

  float smearHistoRes_;
  TH1F* smearHisto(TH1F* h);

private:


  ClassDef(BaseAnalysis, 1);
};

#endif 
