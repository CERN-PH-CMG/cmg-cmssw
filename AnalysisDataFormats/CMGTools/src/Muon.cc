# include "AnalysisDataFormats/CMGTools/interface/Muon.h"

using namespace cmg;

/// charged hadron vetoes    
reco::isodeposit::AbsVetos Muon::chargedHadronVetos() const {
  reco::isodeposit::AbsVetos vetos;
  reco::isodeposit::Direction Dir = reco::isodeposit::Direction((*(this->sourcePtr()))->eta(), (*(this->sourcePtr()))->phi());
  vetos.push_back(new reco::isodeposit::ConeVeto( Dir, 0.0001 ));
  return vetos;
}
/// charged particle vetoes (e, mu, h+-)
reco::isodeposit::AbsVetos Muon::chargedAllVetos() const {
  return chargedHadronVetos();
}
/// pile-up charged hadron vetoes, used for dbeta corrections
reco::isodeposit::AbsVetos Muon::puChargedHadronVetos() const {
  return chargedHadronVetos();
}
/// neutral hadron vetoes
reco::isodeposit::AbsVetos Muon::neutralHadronVetos() const {
  reco::isodeposit::AbsVetos vetos;
  reco::isodeposit::Direction Dir = reco::isodeposit::Direction((*(this->sourcePtr()))->eta(), (*(this->sourcePtr()))->phi());
  vetos.push_back(new reco::isodeposit::ConeVeto( Dir, 0.01 ));
  vetos.push_back(new reco::isodeposit::ThresholdVeto( 0.5 ));
  return vetos;
}
/// photon vetoes
reco::isodeposit::AbsVetos Muon::photonVetos() const {
  reco::isodeposit::AbsVetos vetos;
  reco::isodeposit::Direction Dir = reco::isodeposit::Direction((*(this->sourcePtr()))->eta(), (*(this->sourcePtr()))->phi());
  vetos.push_back(new reco::isodeposit::ConeVeto( Dir, 0.01 ));
  vetos.push_back(new reco::isodeposit::ThresholdVeto( 0.5 ));
  return vetos;
}
