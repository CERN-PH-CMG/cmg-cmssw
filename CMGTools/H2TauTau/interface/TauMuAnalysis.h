#ifndef CMGTools_H2TauTau_TauMuAnalysis_H
#define CMGTools_H2TauTau_TauMuAnalysis_H

#include "CMGTools/H2TauTau/interface/BaseAnalysis.h"
#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include "AnalysisDataFormats/CMGTools/interface/Tau.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"

#include "CMGTools/H2TauTau/interface/TriggerEfficiency.h"

class TauMuAnalysis : public BaseAnalysis {

public:
  
  TauMuAnalysis();
  TauMuAnalysis(const char * name);
  virtual ~TauMuAnalysis();

  void setInputTag(string tag){inputTag_=tag;}
  
  virtual bool init();
  virtual bool createHistos(TString samplename="");

  bool plotVisMass();
  bool plotDistribution(TString histname, Int_t rebin, TString xlabel, TString ylabel, Float_t* legendcoords, Float_t* axesrange, bool log=0);
   

protected:
  virtual bool addHistos(Sample* s);
  virtual bool getHistos(Sample* s);
  virtual bool scaleWeightHistos(Sample* s);
  virtual bool applySelections(const fwlite::Event * event);
  virtual bool fillHistos(const fwlite::Event * event );

private:
    
  string inputTag_;
  std::vector<cmg::TauMu> diTauSelList_;

  //histos for output
  TH1F* diTauNHisto_;
  TH1F* diTauMassHisto_;
  TH1F* diTauEtaHisto_;
  TH1F* diTauPtHisto_;
  TH1F* muPtHisto_;
  TH1F* muIsoHisto_;
  TH1F* muDxyHisto_;
  TH1F* muDzHisto_;
  TH1F* tauPtHisto_;
  TH1F* tauIsoHisto_; 
  TH1F* tauDxyHisto_;
  TH1F* tauDzHisto_;
  TH1F* metHisto_;
  TH1F* pZetaVisHisto_;
  TH1F* pZetaMETHisto_;
  TH1F* pZetaHisto_;

  TH1F* diTauMassPUPHisto_;
  TH1F* diTauEtaPUPHisto_;
  TH1F* diTauPtPUPHisto_;
  TH1F* muPtPUPHisto_;
  TH1F* muIsoPUPHisto_;
  TH1F* muDxyPUPHisto_;
  TH1F* muDzPUPHisto_;
  TH1F* tauPtPUPHisto_;
  TH1F* tauIsoPUPHisto_;
  TH1F* tauTrackIsoPUPHisto_;
  TH1F* tauNeutralIsoPUPHisto_;
  TH1F* tauRhoPUPHisto_;
  TH1F* tauDxyPUPHisto_;
  TH1F* tauDzPUPHisto_;
  TH1F* metPUPHisto_;
  TH1F* pZetaVisPUPHisto_;
  TH1F* pZetaMETPUPHisto_;
  TH1F* pZetaPUPHisto_;

  std::vector<std::string> triggerPaths_;

  TriggerEfficiency triggerEff_;

  float computePZeta(const cmg::Tau * tau1, const cmg::Muon * tau2, const cmg::BaseMET * met);
  float computeTauIso(const cmg::Tau * tau);

  ClassDef(TauMuAnalysis, 1);
};

#endif 
