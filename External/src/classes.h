#include "CMGTools/External/interface/PileupJetIdentifier.h"
#include "CMGTools/External/interface/PileupJetIdAlgo.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Common/interface/ValueMap.h"

namespace {
  namespace {
	  std::vector<StoredPileupJetIdentifier> vec;
	  edm::ValueMap<StoredPileupJetIdentifier> vmap;
	  edm::Wrapper<edm::ValueMap<StoredPileupJetIdentifier> > vmapw;
  }
}
