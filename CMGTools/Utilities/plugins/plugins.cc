#include "CMGTools/Utilities/plugins/plugins.h"
#include "FWCore/Framework/interface/MakerMacros.h"


DEFINE_FWK_MODULE(RecoilCorrectedMETProducerTauMu); 
DEFINE_FWK_MODULE(RecoilCorrectedMETProducerTauEle); 
DEFINE_FWK_MODULE(RecoilCorrectedMETProducerMuEle); 
DEFINE_FWK_MODULE(RecoilCorrectedMETProducerDiTau); 

DEFINE_FWK_MODULE(MVAMETProducerTauMu);
DEFINE_FWK_MODULE(MVAMETProducerTauEle);
DEFINE_FWK_MODULE(MVAMETProducerDiTau);

DEFINE_FWK_MODULE(MVAMETProducerPreselectedLeptons);

DEFINE_FWK_MODULE(TauESCorrectorProducer);
