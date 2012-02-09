#ifndef CMGH2TAUTAU_TauMuFlatNtp_h
#define CMGH2TAUTAU_TauMuFlatNtp_h

#include "CMGTools/H2TauTau/plugins/BaseFlatNtp.h"
#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include "AnalysisDataFormats/CMGTools/interface/Tau.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/METSignificance.h"

#include "CMGTools/H2TauTau/interface/TriggerEfficiency.h"
#include "CMGTools/H2TauTau/interface/SelectionEfficiency.h"
#include "CMGTools/H2TauTau/interface/TauRate.h"


class TauMuFlatNtp : public BaseFlatNtp {


public:
  explicit TauMuFlatNtp(const edm::ParameterSet&);
  ~TauMuFlatNtp();

  virtual void beginJob() ;
  virtual void analyze(const edm::Event&  iEvent, const edm::EventSetup& iSetup);
  virtual void endJob() ;

  virtual bool fillVariables(const edm::Event & iEvent, const edm::EventSetup & iSetup);
  virtual bool applySelections();
  virtual bool fill();

protected:

  edm::InputTag diTauTag_;
  edm::Handle< std::vector<cmg::TauMu> > diTauList_;
  std::vector<cmg::TauMu> diTauSelList_;
  const cmg::TauMu * diTauSel_;
  std::vector<cmg::PFJet> pfJetListLC_;

  TriggerEfficiency triggerEff_;
  float triggerEffWeight_;
  SelectionEfficiency selectionEff_;
  float selectionEffWeight_;

  float embeddedGenWeight_;//for tau embedded samples

  float ditaumass_;
  int ditaucharge_;
  float ditaueta_;
  float ditaupt_;
  float svfitmass_;

  float taupt_;
  float taueta_;
  int   tautruth_;
  float tauehop_;
  float taueop_;
  int   taudecaymode_;
  float taudz_;
  float taudxy_;

  float mupt_;
  float mueta_;
  float muiso_;
  float mudz_;
  float mudxy_;

  float transversemass_;
  float metpt_;
  float metphi_;
  
  int njet_;
  float leadJetPt_;
  float leadJetEta_;
  float diJetMass_;
  float diJetDeltaEta_;
  float diJetEta1Eta2_;

  int categoryCh_;//
  int categoryMT_;//
  int categoryIso_;//
  int categorySM_;//SM search 

private:
  

  int sampleGenEventType_;
  int sampleTruthEventType_;
  int genEventType_;//1=ZtoEE, 3=ZToMuMu, 5=ZToTauTau, 6=ZToOther, 11=WToENu, 13=WToMuNu, 15=WToENu
  int truthEventType_;//1=ZtoEE, 3=ZToMuMu, 5=ZToTauTau, 6=ZOther 11=WToENu, 13=WToMuNu, 15=WToENu, 16=WOther
  float deltaRTruth_;

  const reco::GenParticle * genBoson_;
  const reco::GenParticle * genBosonL1_;
  const reco::GenParticle * genBosonL2_;

  edm::Handle< std::vector<cmg::Muon> > diLeptonVetoList_;

  void fillPFJetListLC(const cmg::TauMu * cand);
  bool vetoDiLepton();
  int truthMatchTau();

  int counterall_;
  int counterev_;
  int countergen_;
  int counterveto_;
  int countereop_;
  int countertaumatch_;
  int countermumatch_;
  int counterditau_;
  int countertruth_;
  int counter_;

};


#endif

