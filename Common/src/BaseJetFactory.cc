#include "CMGTools/Common/interface/BaseJetFactory.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"

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
//   for(cmg::BaseJetFactory::collection::const_iterator it = result->begin(); it != result->end(); ++it){
//     std::cout << "Created a jet: " << *it << std::endl;
//   }
  return result;
}

/// refactor out the setting of properties so that it can be used elsewhere
void cmg::BaseJetFactory::set(const pat::JetPtr& input, cmg::BaseJet* const output) const{
    
    const reco::SecondaryVertexTagInfo* info = input->tagInfoSecondaryVertex();
    if(info){
        output->btag_ = input->bDiscriminator(btagType_);
    }
    
}