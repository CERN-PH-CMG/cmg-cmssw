#ifndef _AnalysisDataFormats_CMGTools_PatTypes_h_
#define _AnalysisDataFormats_CMGTools_PatTypes_h_

#include "DataFormats/PatCandidates/interface/GenericParticle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/TriggerObject.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

namespace pat {

  typedef pat::GenericParticleRef PATPtr;
  typedef pat::JetRef JetPtr;
  typedef pat::MuonRef MuonPtr;
  typedef pat::ElectronRef ElectronPtr;
  typedef pat::METRef METPtr;
  typedef pat::TauRef TauPtr;
  typedef pat::TriggerObjectStandAloneRef TriggerObjectPtr;
  //typedef pat::PhotonRef PhotonPtr;
  typedef reco::PFCandidateRef PhotonPtr;

}

#endif
