#include "CMGTools/H2TauTau/plugins/H2TauTauTools.h"
#include "FWCore/Framework/interface/MakerMacros.h"

using namespace cmg;

DEFINE_FWK_MODULE(TauMuWithSVFitProducer); 
DEFINE_FWK_MODULE(TauEleWithSVFitProducer); 
DEFINE_FWK_MODULE(MuEleWithSVFitProducer); 
DEFINE_FWK_MODULE(TauTauWithSVFitProducer); 

DEFINE_FWK_MODULE(TauMuFactory);
DEFINE_FWK_MODULE(TauEleFactory);
DEFINE_FWK_MODULE(MuEleFactory);
DEFINE_FWK_MODULE(DiTauFactory);

DEFINE_FWK_MODULE(TauMuUpdateFactory);
DEFINE_FWK_MODULE(TauEleUpdateFactory);
DEFINE_FWK_MODULE(MuEleUpdateFactory);
DEFINE_FWK_MODULE(DiTauUpdateFactory);
