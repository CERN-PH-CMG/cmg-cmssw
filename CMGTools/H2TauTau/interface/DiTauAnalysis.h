#ifndef CMGTools_H2TauTau_DiTauAnalysis_H
#define CMGTools_H2TauTau_DiTauAnalysis_H

#include "CMGTools/H2TauTau/interface/BaseAnalysis.h"

class DiTauAnalysis : public BaseAnalysis {

public:
  
  DiTauAnalysis();
  DiTauAnalysis(const char * name);
  virtual ~DiTauAnalysis();
  
  virtual bool init();
  virtual bool analyze(TString samplename="RelValZTT");
  virtual bool end();

  void setInputTag(string tag){inputTag_=tag;}

protected:
  virtual bool getHistos(Sample* s);
  virtual bool scaleWeightHistos(Sample* s);
  virtual bool fill( edm::EventBase const & event );


private:
    
  string inputTag_;

  //histos for output
  TH1F* nDiTauHisto_;
  TH1F* diTauMassHisto_;
  TH1F* diTauEtaHisto_;
  
  ClassDef(DiTauAnalysis, 1);
};

#endif 
