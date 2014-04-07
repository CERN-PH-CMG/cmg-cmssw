#ifndef CMGH2TAUTAU_TauEleFlatNtp_h
#define CMGH2TAUTAU_TauEleFlatNtp_h

#include "CMGTools/H2TauTau/plugins/BaseFlatNtp.h"


//for electron iso
#include "CMGTools/Common/interface/ElectronFactory.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"



class TauEleFlatNtp : public BaseFlatNtp {


public:
  explicit TauEleFlatNtp(const edm::ParameterSet&);
  ~TauEleFlatNtp();

  virtual void beginJob() ;
  virtual void analyze(const edm::Event&  iEvent, const edm::EventSetup& iSetup);
  virtual void endJob() ;

  virtual bool fillVariables(const edm::Event & iEvent, const edm::EventSetup & iSetup);
  virtual bool applySelections();
  virtual bool fill();

protected:



  edm::Handle< std::vector<cmg::TauEle> > diTauList_;
  std::vector<cmg::TauEle> diTauSelList_;
  const cmg::TauEle * diTauSel_;


  float mumvaid_;
  float tauantiemva_;


private:

  edm::Handle< std::vector<cmg::Electron> > diLeptonVetoList_;
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

