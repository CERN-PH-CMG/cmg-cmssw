#ifndef _CMGTools_Common_CollectionSizeProducer_H_
#define _CMGTools_Common_CollectionSizeProducer_H_

// #include "DataFormats/Common/interface/OrphanHandle.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include <iostream>

template<class CollectionClass>
class CollectionSizeProducer : public edm::EDFilter{
 public:

  CollectionSizeProducer(const edm::ParameterSet& ps):
    src_(ps.getParameter<edm::InputTag>("src")), 
    verbose_(ps.getUntrackedParameter<bool>("verbose",false)) {
    produces<int>();
  }

  virtual bool filter(edm::Event&, const edm::EventSetup&);

 private:
  edm::InputTag src_;
  bool verbose_;
};


template<class CollectionClass>
bool CollectionSizeProducer<CollectionClass>::filter(edm::Event& iEvent, const edm::EventSetup& iSetup){

  edm::Handle< CollectionClass > handle; 
  iEvent.getByLabel( src_, handle );
  
  std::auto_ptr<int> output( new int(handle->size()) ); 
  iEvent.put( output );

  return true;
}

#endif
