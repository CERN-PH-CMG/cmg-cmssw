# include "AnalysisDataFormats/CMGTools/interface/Electron.h"

using namespace cmg;

/// charged hadron vetoes    
reco::isodeposit::AbsVetos Electron::chargedHadronVetos() const {
  reco::isodeposit::AbsVetos vetos;
  reco::isodeposit::Direction Dir = reco::isodeposit::Direction((*(this->sourcePtr()))->eta(), (*(this->sourcePtr()))->phi());
  if( fabs( (*(this->sourcePtr()))->superCluster()->eta() ) > 1.479 ){
    vetos.push_back(new reco::isodeposit::ConeVeto( Dir, 0.015 ));
  }
  return vetos;
}
/// charged particle vetoes (e, mu, h+-)
reco::isodeposit::AbsVetos Electron::chargedAllVetos() const {
  return chargedHadronVetos();
}
/// pile-up charged hadron vetoes, used for dbeta corrections
reco::isodeposit::AbsVetos Electron::puChargedHadronVetos() const {
  return chargedHadronVetos();
}
/// neutral hadron vetoes
reco::isodeposit::AbsVetos Electron::neutralHadronVetos() const {
  reco::isodeposit::AbsVetos vetos;
  return vetos;
}
/// photon vetoes
reco::isodeposit::AbsVetos Electron::photonVetos() const {
  reco::isodeposit::AbsVetos vetos;
  reco::isodeposit::Direction Dir = reco::isodeposit::Direction((*(this->sourcePtr()))->eta(), (*(this->sourcePtr()))->phi());
  if( fabs( (*(this->sourcePtr()))->superCluster()->eta() ) > 1.479 ){
    vetos.push_back(new reco::isodeposit::ConeVeto( Dir, 0.08 ));
  }
  return vetos;
}
