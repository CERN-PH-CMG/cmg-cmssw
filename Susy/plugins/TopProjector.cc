#include "CommonTools/ParticleFlow/plugins/TopProjector.h"

#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"

typedef TopProjector<cmg::Muon,cmg::Muon> TPMuonOnMuon;
typedef TopProjector<cmg::Electron,cmg::Electron> TPElectronOnElectron;
typedef TopProjector<cmg::PFJet,cmg::PFJet> TPPFJetOnPFJet;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(TPMuonOnMuon);
DEFINE_FWK_MODULE(TPElectronOnElectron);
DEFINE_FWK_MODULE(TPPFJetOnPFJet);