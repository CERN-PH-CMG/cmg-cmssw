#include "CMGTools/Common/interface/DiMuonFactory.h"

cmg::DiMuonFactory::event_ptr cmg::DiMuonFactory::create(const edm::Event& iEvent, const edm::EventSetup& iSetup) const{
	
  cmg::DiMuonFactory::event_ptr result(new cmg::DiMuonFactory::collection);
	
  edm::Handle<cmg::MuonFactory::collection> leg1Cands;
  const edm::InputTag leg1Label_ = leg1ps_.getParameter<edm::InputTag>("inputCollection");
  iEvent.getByLabel(leg1Label_,leg1Cands);

  edm::Handle<cmg::MuonFactory::collection> leg2Cands;
  const edm::InputTag leg2Label_ = leg2ps_.getParameter<edm::InputTag>("inputCollection");
  iEvent.getByLabel(leg2Label_,leg2Cands);

  	
  //bail if we don't have at least 2 muons
  if( !leg1Cands->size() || !leg2Cands->size() ){
    return result;	
  }
	
  for(cmg::MuonFactory::collection::const_iterator it = leg1Cands->begin(); it != leg1Cands->end(); ++it){
    const cmg::Muon& m1 = *it;

    for(cmg::MuonFactory::collection::size_type i = 0; i < (leg2Cands->size()/2); i++){
      const cmg::Muon& m2 = leg2Cands->at(i);
      if( m1 != m2 ){
	cmg::DiMuon d_(*it,m2);
// 	std::cout << "Created a dimuon: " << d_ << std::endl;
	result->push_back(d_);
      }
    }
  }
  return result;
	
}
