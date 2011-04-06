#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "PhysicsTools/PFCandProducer/plugins/TopProjector.h"

typedef TopProjector<cmg::Electron,reco::PFCandidate> TPCmgElectronOnPFCandidate;
typedef TopProjector<cmg::Muon,reco::PFCandidate> TPCmgMuonOnPFCandidate;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(TPCmgElectronOnPFCandidate);
DEFINE_FWK_MODULE(TPCmgMuonOnPFCandidate);