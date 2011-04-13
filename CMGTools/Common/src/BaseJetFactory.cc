#include "CMGTools/Common/interface/BaseJetFactory.h"

#include <iostream>

cmg::BaseJetFactory::event_ptr cmg::BaseJetFactory::create(const edm::Event& iEvent, 
						   const edm::EventSetup&) const{
	
  edm::Handle<pat::JetCollection> jetCands;
  
  cmg::BaseJetFactory::event_ptr result(new cmg::BaseJetFactory::collection);
  iEvent.getByLabel(jetLabel_,jetCands);

  unsigned index = 0;
  for(pat::JetCollection::const_iterator mi = jetCands->begin();
      mi != jetCands->end(); ++mi, ++index){
  			
    pat::JetPtr jetPtr(jetCands,index);
    cmg::BaseJet jet( jetPtr );
    set(jetPtr,&jet);
 
    result->push_back(jet);
  }
  return result;
}

/// refactor out the setting of properties so that it can be used elsewhere
void cmg::BaseJetFactory::set(const pat::JetPtr& input, cmg::BaseJet* const output) const{
    
    output->btag_ = input->bDiscriminator(btagType_);
    output->rawFactor_ = input->jecFactor(0);
}
