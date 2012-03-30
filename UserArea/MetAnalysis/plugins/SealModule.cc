#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CMG/MetAnalysis/plugins/TrackMetProducer.h"
#include "CMG/MetAnalysis/plugins/NoPUMetProducer.h"
#include "CMG/MetAnalysis/plugins/PUMetProducer.h"

DEFINE_FWK_MODULE (TrackMetProducer);
DEFINE_FWK_MODULE (NoPUMetProducer);
DEFINE_FWK_MODULE (PUMetProducer);


