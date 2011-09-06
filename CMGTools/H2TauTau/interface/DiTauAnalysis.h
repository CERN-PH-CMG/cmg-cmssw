#ifndef CMGTools_H2TauTau_DiTauAnalysis_H
#define CMGTools_H2TauTau_DiTauAnalysis_H

#include "CMGTools/H2TauTau/interface/BaseAnalysis.h"

class DiTauAnalysis : public BaseAnalysis {

public:
  
  DiTauAnalysis();
  DiTauAnalysis(const char * name);
  virtual ~DiTauAnalysis();

  void setInputTag(string tag){inputTag_=tag;}
  
  virtual bool init();
  virtual bool createHistos(TString samplename="RelValZTT");

  bool createTotalMass();


protected:
  virtual bool addHistos(Sample* s);
  virtual bool getHistos(Sample* s);
  virtual bool scaleWeightHistos(Sample* s);
  virtual bool fillHistos(const fwlite::Event * event );


private:
    
  string inputTag_;

  //histos for output
  TH1F* nDiTauHisto_;
  TH1F* diTauMassHisto_;
  TH1F* diTauEtaHisto_;
  TH1F* diTauPtHisto_;
  TH1F* metHisto_;
  TH1F* pZetaVisHisto_;
  TH1F* pZetaMETHisto_;
  TH1F* pZetaHisto_;

  TH1F* diTauMassPUPWeightHisto_;
  TH1F* diTauEtaPUPWeightHisto_;
  TH1F* diTauPtPUPWeightHisto_;
  TH1F* metPUPWeightHisto_;
  TH1F* pZetaVisPUPWeightHisto_;
  TH1F* pZetaMETPUPWeightHisto_;
  TH1F* pZetaPUPWeightHisto_;

  std::vector<std::string> triggerPaths_;

  ClassDef(DiTauAnalysis, 1);
};

#endif 
