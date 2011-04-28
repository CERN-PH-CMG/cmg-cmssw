#include "CMGTools/Common/interface/TauFactory.h"

cmg::TauFactory::event_ptr cmg::TauFactory::create(const edm::Event& iEvent, const edm::EventSetup& iSetup) const{
	
  edm::Handle<pat::TauCollection> tauCands;
  cmg::TauFactory::event_ptr result(new cmg::TauFactory::collection);
  iEvent.getByLabel(tauLabel_,tauCands);

  unsigned index = 0;
  for(pat::TauCollection::const_iterator m = tauCands->begin(); m != tauCands->end(); ++m, ++index){
    pat::TauPtr tauPtr(tauCands, index);
    cmg::Tau tau(tauPtr);
    set(tauPtr,&tau,iEvent,iSetup);
    result->push_back(tau);
  }
  return result;
}

void cmg::TauFactory::set(const pat::TauPtr& input, cmg::Tau* const output, const edm::Event& iEvent, const edm::EventSetup& iSetup) const{
        
  //set the generic quantities first
  leptonFactory_.set(input,output,iEvent,iSetup);

  //now the tau like ones
  output->decayMode_ = input->decayMode();

}


