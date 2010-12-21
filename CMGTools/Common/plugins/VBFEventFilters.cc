#include "AnalysisDataFormats/CMGTools/interface/VBFEventT1T2.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"

#include "CMGTools/Common/plugins/VBFEventFilters.h"

#include "FWCore/Framework/interface/MakerMacros.h"

typedef VBFEventFilter<pat::Jet, pat::Jet> VBFEventJetJetFilter;
typedef VBFEventFilter<reco::PFJet, reco::PFJet> VBFEventPFJetPFJetFilter;
typedef VBFEventFilter<reco::GenJet, reco::GenJet> VBFEventGenJetGenJetFilter;


DEFINE_FWK_MODULE(VBFEventJetJetFilter);
DEFINE_FWK_MODULE(VBFEventPFJetPFJetFilter);
DEFINE_FWK_MODULE(VBFEventGenJetGenJetFilter);


