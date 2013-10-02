#include "CMGTools/External/interface/PileupJetIdentifier.h"
#include "CMGTools/External/interface/PileupJetIdAlgo.h"
#include "CMGTools/External/interface/PileupJetIdentifierSubstructure.h"
#include "CMGTools/External/interface/PileupJetIdAlgoSubstructure.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Common/interface/ValueMap.h"

namespace {
  namespace {
    std::vector<StoredPileupJetIdentifier> vec;
    edm::ValueMap<StoredPileupJetIdentifier> vmap;
    edm::Wrapper<edm::ValueMap<StoredPileupJetIdentifier> > vmapw;
    std::vector<StoredPileupJetIdentifierSubstructure> vec1;
    edm::ValueMap<StoredPileupJetIdentifierSubstructure> vmap1;
    edm::Wrapper<edm::ValueMap<StoredPileupJetIdentifierSubstructure> > vmapw1;
  }
}
