#include "RecoParticleFlow/PostProcessing/interface/FastJetAlgo.h"

typedef pf2pat::FastJetAlgo< edm::View< reco::Candidate > , reco::PFJet > FastJetAlgoCandPFJet;
typedef pf2pat::FastJetAlgo< std::vector<reco::PFCandidate>, reco::PFJet > FastJetAlgoPFCandPFJet;
typedef pf2pat::FastJetAlgo< std::vector< edm::Ptr< reco::Candidate > > , reco::PFJet > FastJetAlgoVecPtrCandPFJet;


