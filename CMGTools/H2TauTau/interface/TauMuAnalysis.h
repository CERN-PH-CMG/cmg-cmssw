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
#include "CMGTools/H2TauTau/interface/TauRate.h"

#include <TVector3.h>

class TauMuAnalysis : public BaseAnalysis {

public:
  
  TauMuAnalysis();
  TauMuAnalysis(const char * name);
  virtual ~TauMuAnalysis();

  void setInputTag(string tag){inputTag_=tag;}

  virtual bool init();
  virtual bool createHistos(TString samplename="");

  void calcSVFit(bool calc){calcsvfit_=calc;}

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
  const cmg::TauMu * diTauSel_;
  edm::Handle< std::vector<cmg::BaseMET> > met_;
  edm::Handle< cmg::METSignificance >  metsig_;  
  edm::Handle< std::vector<cmg::Muon> > diLeptonVetoList_;
  std::vector<cmg::PFJet> pfJetList_;
  const reco::GenParticle * genBoson_;
  const reco::GenParticle * genBosonL1_;
  const reco::GenParticle * genBosonL2_;
  Int_t genEventType_;//1=ZtoEE, 3=ZToMuMu, 5=ZToTauTau, 6=ZToOther, 11=WToENu, 13=WToMuNu, 15=WToENu
  Int_t truthEventType_;//1=ZtoEE, 3=ZToMuMu, 5=ZToTauTau, 6=ZOther 11=WToENu, 13=WToMuNu, 15=WToENu, 16=WOther
  bool trigpass_;
  bool calcsvfit_;


  TriggerEfficiency triggerEff_;
  RecoilCorrector recoilCorr_;
  TauRate tauRate_;



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
  TH1F* metphiHisto_;
  TH1F* pZetaHisto_;
  TH1F* transverseMassHisto_;
  TH1F* njetHisto_;


  void applyRecoilCorr(const cmg::TauMu * cand, TVector3 * MET);
  float computePZeta(const cmg::TauMu * cand);
  float computeTransverseMass(const cmg::TauMu * cand);

  float computeTauIso(const cmg::Tau * tau);

  bool computeDiLeptonVeto(const cmg::Muon * muon);

  ClassDef(TauMuAnalysis, 1);
};

#endif 
