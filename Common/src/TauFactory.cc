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

  //  

  reco::PFCandidateRef leadPFCand=input->leadPFCand();
  if(leadPFCand.isNonnull()){
    output->leadCandHCalEnergy = leadPFCand->hcalEnergy();
    output->leadCandMvaEPi     = leadPFCand->mva_e_pi(); 
  }
  reco::PFCandidateRef leadChargedHadrCand=input->leadPFChargedHadrCand();//lead track
  if(leadChargedHadrCand.isNonnull()){
    output->leadHadrHCalEnergy    = leadChargedHadrCand->hcalEnergy();
    output->leadHadrECalEnergy    = leadChargedHadrCand->ecalEnergy();
    output->leadChargedHadrMvaEPi = leadChargedHadrCand->mva_e_pi();
    output->leadChargedHadrTrkPt  = leadChargedHadrCand->pt();
  }
  reco::PFCandidateRef leadNeutralCand=input->leadPFNeutralCand();
  if(leadNeutralCand.isNonnull()){
    output->leadNeutralCandPt = leadNeutralCand->pt();
  }
  reco::PFCandidateRefVector signalChargedHadrCands=input->signalPFChargedHadrCands();
  if(signalChargedHadrCands.isNonnull()){
    output->numberChargedHadr =  signalChargedHadrCands.size();
  }
  reco::PFCandidateRefVector signalGammaCands=input->signalPFGammaCands();
  if(signalGammaCands.isNonnull()){
    output->numberGamma = signalGammaCands.size() ;
  }

  
  //copy the the tauIDs 
  std::vector<pat::Tau::IdPair> tauids = input->tauIDs();
  for (std::vector<pat::Tau::IdPair>::const_iterator it = tauids.begin() ; it != tauids.end() ; ++it) {
    output->setTauID(it->first,it->second);
  }

  //other variables
  output->decayMode = input->decayMode();

}


