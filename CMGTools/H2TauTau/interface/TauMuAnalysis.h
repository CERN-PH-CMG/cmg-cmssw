#ifndef CMGTools_H2TauTau_TauMuAnalysis_H
#define CMGTools_H2TauTau_TauMuAnalysis_H

#include "CMGTools/H2TauTau/interface/BaseAnalysis.h"
#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include "AnalysisDataFormats/CMGTools/interface/Tau.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/METSignificance.h"

#include "CMGTools/H2TauTau/interface/TriggerEfficiency.h"
//#include "CMGTools/H2TauTau/interface/RecoilCorrector.h"
//#include "CMGTools/Common/interface/RecoilCorrector.h"
#include "CMGTools/H2TauTau/interface/TauRate.h"

#include <TVector3.h>

class TauMuAnalysis : public BaseAnalysis {

public:
  
  TauMuAnalysis();
  TauMuAnalysis(const char * name);
  virtual ~TauMuAnalysis();

  void setInputTag(string tag){inputTag_=tag;}
  void setdiLeptonVetoListName(string diLeptonVetoListName){diLeptonVetoListName_=diLeptonVetoListName;}
  void setQCDOStoSSRatio(Float_t ratio){QCDOStoSSRatio_=ratio;}
  void setQCDColor(Int_t color){QCDColor_=color;}
  void setWJetsColor(Int_t color){WJetsColor_=color;}
  void setTTJetsColor(Int_t color){TTJetsColor_=color;}
  void setZMuMuColor(Int_t color){ZMuMuColor_=color;}
  void setZTauTauColor(Int_t color){ZTauTauColor_=color;}
  void setSignalColor(Int_t color){SignalColor_=color;}


  virtual bool init();
  virtual bool createHistos(TString samplename="");

  void calcSVFit(bool calc){calcsvfit_=calc;}
  void makeAllHistos(bool makeall){makeAllHistos_=makeall;}

  bool scaleHistos();//contains logic for scaling samples before making plots
  //all functions below return ownership of TH1F
  TH1F* getTotalDataSS(TString histoname);//sum of SS Data samples 
  TH1F* getTotalData(TString histoname);
  TH1F* getTotalEmbeddedSS(TString histoname);
  TH1F* getTotalEmbedded(TString histoname);
  TH1F* getMCSS(TString histoname);//sum of SS MC's used in the QCD extraction (may exclude additional MC samples declared)
  TH1F* getSample(TString samplename, TString histoname);//can be used to get histo for any MC or Data sample
  TH1F* getQCD(TString histoname);
  TH1F* getZToTauTau(TString histoname);//Z-->tau tau (either from MC or Embedded)
  TH1F* getTotalBackground(TString histoname);
 

  bool printRawYields(TString histoname);

  //bool plotDistribution(TString histoname, Int_t rebin, TString xlabel, TString ylabel, Float_t* legendcoords, Float_t* axesrange, bool log=0);
  bool plot(TString histoname, Int_t rebin, TString xlabel, TString ylabel, Float_t* legendcoords, Float_t* axesrange, bool log=0);
   

protected:
  virtual bool addHistos(Sample* s);
  virtual bool getHistos(TString tag = "");
  virtual bool fillVariables(const fwlite::Event * event);
  virtual bool applySelections(TString exceptcut = "");
  virtual bool fillHistos(TString tag = "");

private:
     
  string inputTag_;
  edm::Handle< std::vector<cmg::TauMu> > diTauList_;
  std::vector<cmg::TauMu> diTauSelList_;
  const cmg::TauMu * diTauSel_;
  const cmg::BaseMET * met_;
  const TMatrixD * metsig_;  
  string diLeptonVetoListName_;
  edm::Handle< std::vector<cmg::Muon> > diLeptonVetoList_;
  std::vector<cmg::PFJet> pfJetList_;
  std::vector<cmg::PFJet> pfJetListLC_;
  const reco::GenParticle * genBoson_;
  const reco::GenParticle * genBosonL1_;
  const reco::GenParticle * genBosonL2_;
  unsigned int genEventType_;//1=ZtoEE, 3=ZToMuMu, 5=ZToTauTau, 6=ZToOther, 11=WToENu, 13=WToMuNu, 15=WToENu
  unsigned int truthEventType_;//1=ZtoEE, 3=ZToMuMu, 5=ZToTauTau, 6=ZOther 11=WToENu, 13=WToMuNu, 15=WToENu, 16=WOther
  unsigned int eventCategorySM_;//SM search
  const cmg::PFJet * boostedJet_;
  const cmg::PFJet * VBFJet1_;
  const cmg::PFJet * VBFJet2_; 
  bool calcsvfit_;
  bool makeAllHistos_;

  Float_t QCDOStoSSRatio_;
  //RecoilCorrector * recoilCorr_;
  TauRate tauRate_;
  float tauFakeWeight_;
  TriggerEfficiency triggerEff_;
  float triggerEffWeight_;

  float embeddedGenWeight_;



  void fillPFJetListLC(const cmg::TauMu * cand);
  //void applyRecoilCorr(const cmg::TauMu * cand, TVector3 * MET);
  //float computePZeta(const cmg::TauMu * cand);
  //float computeTransverseMass(const cmg::TauMu * cand);
  //float computeTauIso(const cmg::Tau * tau);
  bool computeDiLeptonVeto();


  //
  Int_t QCDColor_;
  Int_t WJetsColor_;
  Int_t TTJetsColor_;
  Int_t ZMuMuColor_;
  Int_t ZTauTauColor_;
  Int_t SignalColor_;

  //histos
  TH1F* diTauNHisto_;
  TH1F* diTauMassHisto_;
  TH1F* diTauMassSVFitHisto_;
  TH1F* diTauEtaHisto_;
  TH1F* diTauPtHisto_;

  TH1F* svFitConvergeHisto_;
  TH1F* svFitCov00Histo_;
  TH1F* svFitEigen0Histo_;
  TH1F* svFitEigen1Histo_;

  TH1F* muPtHisto_;
  TH1F* muEtaHisto_;
  TH1F* muIsoHisto_;
  TH1F* muIsoDBHisto_;
  TH1F* muDxyHisto_;
  TH1F* muDzHisto_;

  TH1F* tauPtHisto_;
  TH1F* tauEtaHisto_;
  TH1F* tauIsoHisto_; 
  TH1F* tauDxyHisto_;
  TH1F* tauDzHisto_;

  TH1F* metHisto_;
  TH1F* metphiHisto_;
  TH1F* pZetaHisto_;
  TH1F* transverseMassHisto_;
  TH1F* njetHisto_;

  TH1F* diTauMassVBFHisto_;
  TH1F* diJetMassVBFHisto_;
  TH1F* diJetDeltaEtaVBFHisto_;
  TH1F* diTauMassBoostedHisto_;
  TH1F* jetPtBoostedHisto_;
  TH1F* diTauMassInclusiveHisto_;


  ///output tree definition
  TTree* tree_;
  float tree_eventweight_;
  float tree_ditaumass_;
  float tree_svfitmass_;
  float tree_taupt_;
  float tree_taueta_;
  float tree_mupt_;
  float tree_mueta_;
  float tree_muiso_;
  float tree_transversemass_;
  float tree_met_;
  int   tree_svfitstatus_;
  float tree_svfitedm_;
  float tree_svfiteigenval0_;
  float tree_svfiteigenval1_;



  ClassDef(TauMuAnalysis, 1);
};

#endif 
