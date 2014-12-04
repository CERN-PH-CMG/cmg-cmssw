#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"

#include <iostream>

class SimpleParticleProducer : public edm::EDProducer{
 public:

  typedef std::vector<reco::LeafCandidate> OutType;

  SimpleParticleProducer(const edm::ParameterSet& ps):
    src_(ps.getParameter<edm::InputTag>("src")) {
    produces< OutType >();
  }

  virtual void produce(edm::Event&, const edm::EventSetup&);

 private:
  edm::InputTag src_;
};


void SimpleParticleProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){

  edm::Handle< edm::View< reco::Candidate > >  handle; 
  iEvent.getByLabel( src_, handle );
  
  std::auto_ptr< OutType > output( new OutType(handle->size()) ); 
  
  for(unsigned i=0; i<handle->size(); ++i) {
    const reco::Candidate& cand = handle->at(i);
    output->push_back( reco::LeafCandidate(cand) );
  }

  iEvent.put( output );
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(SimpleParticleProducer);
