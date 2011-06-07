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

  //track related   
  reco::PFCandidateRefVector signalChargedHadrCands=input->signalPFChargedHadrCands();
  if(signalChargedHadrCands.isAvailable()){  // isNonnull() returns false always ?
    output->numberChargedHadr_ =  signalChargedHadrCands.size();
  }
  reco::PFCandidateRef leadChargedHadrCand=input->leadPFChargedHadrCand();//lead track
  if(leadChargedHadrCand.isNonnull()){
    output->leadChargedHadrPt_  = leadChargedHadrCand->pt();
    output->leadChargedHadrCharge_  = leadChargedHadrCand->charge();
    output->leadChargedHadrHCalEnergy_    = leadChargedHadrCand->hcalEnergy();
    output->leadChargedHadrECalEnergy_    = leadChargedHadrCand->ecalEnergy();
    output->leadChargedHadrMvaEPi_ = leadChargedHadrCand->mva_e_pi();

    reco::TrackRef chHadrtrk=leadChargedHadrCand->trackRef();
    if(chHadrtrk.isAvailable()){
      output->leadChargedHadrVertex_ = chHadrtrk->referencePoint();
    }

  }

  //neutral related
  reco::PFCandidateRefVector signalGammaCands=input->signalPFGammaCands();
  if(signalGammaCands.isAvailable()){ // isNonnull() returns false always ?
    output->numberGamma_ = signalGammaCands.size() ;
  }
  reco::PFCandidateRef leadNeutralCand=input->leadPFNeutralCand();
  if(leadNeutralCand.isNonnull()){
    output->leadNeutralCandPt_ = leadNeutralCand->pt();
    output->leadNeutralCandECalEnergy_ = leadNeutralCand->ecalEnergy();
  }


  //general variables 
  output->isolation_ = input->particleIso();
  output->decayMode_ = input->decayMode();


  
  //copy the the tauIDs 
  std::vector<pat::Tau::IdPair> tauids = input->tauIDs();
  Int_t i=0;
  for (std::vector<pat::Tau::IdPair>::const_iterator it = tauids.begin() ; it != tauids.end() ; ++it) {
    if( i == NCMGTAUIDS)
      throw cms::Exception( " Trying to add too many tauIDs");
    //save only the names of the ids which are passed
    if(it->second==1.0) 
      output->tauID_[i++]=it->first;
  }

 

}



