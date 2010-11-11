#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/Common/interface/OwnVector.h"
#include "DataFormats/Common/interface/View.h"

#include "CMGTools/Common/interface/GenericFactory.h"

#include <iostream>

cmg::GenericFactory::event_ptr cmg::GenericFactory::create(const edm::Event& iEvent, const edm::EventSetup&) const{
	
  edm::Handle< edm::View<reco::LeafCandidate> > cands;
  cmg::GenericFactory::event_ptr result(new cmg::GenericFactory::collection);
  iEvent.getByLabel(inputLabel_,cands);
  
  for(edm::View<reco::LeafCandidate>::const_iterator it = cands->begin(); it != cands->end(); ++it){

    cmg::Generic g( *it );
    g.charge_ = it->charge();
    g.pdgId_ =  it->pdgId();
    
    result->push_back(g);
    
  }
  return result;
}
