#ifndef CMGTools_H2TauTau_TauMuAnalysis_H
#define CMGTools_H2TauTau_TauMuAnalysis_H

#include "CMGTools/H2TauTau/interface/BaseAnalysis.h"
#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include "AnalysisDataFormats/CMGTools/interface/Tau.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/METSignificance.h"

#include "CMGTools/H2TauTau/interface/TriggerEfficiency.h"
#include "CMGTools/H2TauTau/interface/RecoilCorrector.h"

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
  virtual bool getHistos(Sample* s, TString tag = "");
  virtual bool fillVariables(const fwlite::Event * event);
  virtual bool applySelections(TString exceptcut="");
  virtual bool fillHistos(double weight = 1);

private:
    
  string inputTag_;
  edm::Handle< std::vector<cmg::TauMu> > diTauList_;
  std::vector<cmg::TauMu> diTauSelList_;
  edm::Handle< std::vector<cmg::BaseMET> > met_;
  edm::Handle< cmg::METSignificance >  metsig_;
  edm::Handle< std::vector<cmg::Muon> > diLeptonVetoList_;
  const reco::GenParticle * genBoson_;
  bool trigpass_;
  TriggerEfficiency triggerEff_;
  RecoilCorrector recoilCorr_;
  Int_t genEventType_;//1=ZtoEE, 3=ZToMuMu, 5=ZToTauTau, 6=ZToOther, 11=WToENu, 13=WToMuNu, 15=WToENu,


  //histos
  TH1F* diTauNHisto_;
  TH1F* diTauMassHisto_;
  TH1F* diTauMassSVFitHisto_;
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
  TH1F* pZetaHisto_;


  float computePZeta(const cmg::Tau * tau1, const cmg::Muon * tau2);

  float computeTauIso(const cmg::Tau * tau);

  bool computeDiLeptonVeto(const cmg::Muon * muon);

  ClassDef(TauMuAnalysis, 1);
};

#endif 
