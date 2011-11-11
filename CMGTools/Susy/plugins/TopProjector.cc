#include "CommonTools/ParticleFlow/plugins/TopProjector.h"

#include "AnalysisDataFormats/CMGTools/interface/Electron.h"
#include "AnalysisDataFormats/CMGTools/interface/Muon.h"

typedef TopProjector<cmg::Muon,cmg::Muon> TPMuonOnMuon;
typedef TopProjector<cmg::Electron,cmg::Electron> TPElectronOnElectron;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(TPMuonOnMuon);
DEFINE_FWK_MODULE(TPElectronOnElectron);