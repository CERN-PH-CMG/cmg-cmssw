#include <string>
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

class CMGVertexSlimmer : public edm::EDProducer {
    public:
        explicit CMGVertexSlimmer(const edm::ParameterSet&);
        ~CMGVertexSlimmer();

        virtual void produce(edm::Event&, const edm::EventSetup&);
    private:
        edm::InputTag src_;
};
CMGVertexSlimmer::CMGVertexSlimmer(const edm::ParameterSet& iConfig) :
    src_(iConfig.getParameter<edm::InputTag>("src"))
{
  produces<std::vector<reco::Vertex> >();
}

CMGVertexSlimmer::~CMGVertexSlimmer() {}

void CMGVertexSlimmer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
    edm::Handle<std::vector<reco::Vertex> > vertices;
    iEvent.getByLabel(src_, vertices);
    std::auto_ptr<std::vector<reco::Vertex> > outPtr(new std::vector<reco::Vertex>());

    outPtr->reserve(vertices->size());
    for (unsigned int i = 0, n = vertices->size(); i < n; ++i) { 
        const reco::Vertex &v = (*vertices)[i];
        outPtr->push_back(reco::Vertex(v.position(), v.error(), v.chi2(), v.ndof(), 0));
    }

    iEvent.put(outPtr);
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(CMGVertexSlimmer);
