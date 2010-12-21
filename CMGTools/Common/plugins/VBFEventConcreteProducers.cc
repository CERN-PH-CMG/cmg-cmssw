#include "AnalysisDataFormats/CMGTools/interface/VBFEventT1T2.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"

#include "CMGTools/Common/interface/VBFEventT1T2Producer.h"
#include "CMGTools/Common/plugins/VBFEventConcreteProducers.h"

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(VBFEventJetJetProducer);
DEFINE_FWK_MODULE(VBFEventJetPFCandProducer);
DEFINE_FWK_MODULE(VBFEventJetPFPartProducer);
DEFINE_FWK_MODULE(VBFEventPFJetPFJetProducer);
DEFINE_FWK_MODULE(VBFEventPFJetPFCandProducer);
DEFINE_FWK_MODULE(VBFEventGenJetGenJetProducer);
