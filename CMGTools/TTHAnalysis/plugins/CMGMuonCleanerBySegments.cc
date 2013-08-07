
//
// $Id: MuonCleanerBySegments.cc,v 1.2 2012/10/02 17:26:27 namapane Exp $
//

/**
  \class    modules::MuonCleanerBySegmentsT MuonCleanerBySegmentsT.h "MuonAnalysis/MuonAssociators/interface/MuonCleanerBySegmentsT.h"
  \brief    Removes duplicates from a muon collection using segment references.

            This module removes duplicates from a muon collection using segment references.

            All muons that don't pass the preselection are discarded first

            Then, for each pair of muons that share at least a given fraction of segments,
            the worse one is flagged as ghost.

            Finally, all muons that are not flagged as ghosts, or which pass a 'passthrough' selection,
            are saved in the output.
            
  \author   Giovanni Petrucciani
  \version  Original version:  MuonCleanerBySegments.cc,v 1.1 2012/08/11 13:00:50 gpetrucc Exp
*/


#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "CMGTools/TTHAnalysis/interface/CMGMuonCleanerBySegmentsAlgo.h"


namespace modules {
  template<typename T>
  class CMGMuonCleanerBySegmentsT : public edm::EDProducer {
    public:
      explicit CMGMuonCleanerBySegmentsT(const edm::ParameterSet & iConfig);
      virtual ~CMGMuonCleanerBySegmentsT() { }

      virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);

    private:
      /// Labels for input collections
      edm::InputTag src_;

      CMGMuonCleanerBySegmentsAlgo algo_;
  };

} // namespace

template<typename T>
modules::CMGMuonCleanerBySegmentsT<T>::CMGMuonCleanerBySegmentsT(const edm::ParameterSet & iConfig) :
    src_(iConfig.getParameter<edm::InputTag>("src")),
    algo_(iConfig.getParameter<double>("fractionOfSharedSegments"),
          iConfig.getParameter<std::string>("preselection"),
          iConfig.getParameter<std::string>("passthrough"))
{
    // this is the basic output (edm::Association is not generic)
    produces<std::vector<T> >(); 
}

template<typename T>
void
modules::CMGMuonCleanerBySegmentsT<T>::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {
    using namespace edm;
    using namespace std;

    Handle<vector<T> > src;
    auto_ptr<vector<T> > out(new vector<T>());

    iEvent.getByLabel(src_, src);
    unsigned int nsrc = src->size();

    vector<bool> good = algo_.clean(*src);

    out->reserve(nsrc);
    for (unsigned int i = 0; i < nsrc; ++i) {
        if (good[i]) out->push_back((*src)[i]);
    }

    iEvent.put(out);
}

namespace modules {
    typedef modules::CMGMuonCleanerBySegmentsT<cmg::Muon> CMGMuonCleanerBySegments;
    typedef modules::CMGMuonCleanerBySegmentsT<pat::Muon> CMGPATMuonCleanerBySegments;
}
#include "FWCore/Framework/interface/MakerMacros.h"
using namespace modules;
DEFINE_FWK_MODULE(CMGMuonCleanerBySegments);
DEFINE_FWK_MODULE(CMGPATMuonCleanerBySegments);
