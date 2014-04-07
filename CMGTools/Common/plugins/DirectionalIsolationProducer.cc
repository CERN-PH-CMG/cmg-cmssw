#include "CMGTools/Common/plugins/DirectionalIsolationProducer.h"

#include "FWCore/Framework/interface/MakerMacros.h"
typedef DirectionalIsolationProducer<pat::Electron> DirectionalIsolationProducerElectron;
DEFINE_FWK_MODULE(DirectionalIsolationProducerElectron);

typedef DirectionalIsolationProducer<pat::Muon> DirectionalIsolationProducerMuon;
DEFINE_FWK_MODULE(DirectionalIsolationProducerMuon);
