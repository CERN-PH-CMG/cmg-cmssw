#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "AnalysisDataFormats/CMGTools/interface/SimpleParticle.h"

#include <iostream>

class SimpleParticleProducer : public edm::EDProducer{
 public:

  typedef std::vector<cmg::SimpleParticle> OutType;

  SimpleParticleProducer(const edm::ParameterSet& ps):
    src_(ps.getParameter<edm::InputTag>("src")) {
    produces< OutType >();
  }

  virtual void produce(edm::Event&, const edm::EventSetup&);

 private:
  edm::InputTag src_;
};


void SimpleParticleProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){

  edm::Handle< edm::View< reco::LeafCandidate > >  handle; 
  iEvent.getByLabel( src_, handle );
  
  std::auto_ptr< OutType > output( new OutType(handle->size()) ); 
  
  for(unsigned i=0; i<handle->size(); ++i) {
    const reco::LeafCandidate& cand = handle->at(i);
    output->push_back( cmg::SimpleParticle(cand.pdgId(), cand.pt(), cand.eta(), cand.phi(), cand.mass()) );
  }

  iEvent.put( output );
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(SimpleParticleProducer);
