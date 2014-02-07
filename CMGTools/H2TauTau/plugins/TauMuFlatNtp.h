#ifndef CMGH2TAUTAU_TauMuFlatNtp_h
#define CMGH2TAUTAU_TauMuFlatNtp_h

#include "CMGTools/H2TauTau/plugins/BaseFlatNtp.h"

class TauMuFlatNtp : public BaseFlatNtp {


public:
  explicit TauMuFlatNtp(const edm::ParameterSet&);
  ~TauMuFlatNtp();

  virtual void beginJob();
  virtual void analyze(const edm::Event&  iEvent, const edm::EventSetup& iSetup);
  virtual void endJob();

  virtual bool fillVariables(const edm::Event & iEvent, const edm::EventSetup & iSetup);
  virtual bool applySelections();
  virtual bool fill();

protected:

  edm::Handle< std::vector<cmg::TauMu> > diTauList_;
  std::vector<cmg::TauMu> diTauSelList_;
  const cmg::TauMu * diTauSel_;


private:

  bool vetoDiLepton();


  int counterev_;
  int counterveto_;
  int counterpresel_;
  int countertaueop_;
  int countertauvtx_;
  int countertaumuveto_;
  int countertaueveto_;
  int countertauiso_;
  int countertaumatch_;
  int countermuvtx_;
  int countermuid_;
  int countermuiso_;
  int countermumatch_;
  int countertruth_;

};


#endif

