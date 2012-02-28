#include "CMGTools/Common/interface/TauFactory.h"
#include "PhysicsTools/JetMCUtils/interface/JetMCTag.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

cmg::TauFactory::event_ptr cmg::TauFactory::create(const edm::Event& iEvent, const edm::EventSetup& iSetup){
	
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
  reco::PFCandidateRef leadChargedHadrCand=input->leadPFChargedHadrCand();
  if(leadChargedHadrCand.isNonnull()){
    const reco::PFCandidate& cand = *(input->leadPFChargedHadrCand().get());
    output->leadPFChargedHadrCand_ = cmg::Tau::Constituent( cand.pdgId(), 
							    cand.p4().pt(), cand.p4().eta(), 
							    cand.p4().phi(),  cand.p4().mass());

    //COLIN a bit of redundancy here ... should clean up at some point
    output->leadChargedHadrPt_  = leadChargedHadrCand->pt();
    output->leadChargedHadrCharge_  = leadChargedHadrCand->charge();
    output->leadChargedHadrHcalEnergy_    = leadChargedHadrCand->hcalEnergy();
    output->leadChargedHadrEcalEnergy_    = leadChargedHadrCand->ecalEnergy();
    output->leadChargedHadrMvaEPi_ = leadChargedHadrCand->mva_e_pi();

    reco::TrackRef chHadrtrk=leadChargedHadrCand->trackRef();
    if(chHadrtrk.isAvailable()){
      output->leadChargedHadrVertex_ = chHadrtrk->referencePoint();

      //this method sets dxy and dz
      leptonFactory_.set(chHadrtrk,output,iEvent,iSetup);

    }
  }

  //neutral related
  reco::PFCandidateRef leadNeutralCand=input->leadPFNeutralCand();
  if(leadNeutralCand.isNonnull()){
    output->leadNeutralCandPt_ = leadNeutralCand->pt();
    output->leadNeutralCandEcalEnergy_ = leadNeutralCand->ecalEnergy();
  }

  
  for(unsigned int k = 0 ; k < input->signalPFCands().size() ; k++){
    const reco::PFCandidate& cand = *(input->signalPFCands().at(k).get());
    output->signalPFCands_.push_back( cmg::Tau::Constituent( cand.pdgId(), 
							     cand.p4().pt(), cand.p4().eta(), 
							     cand.p4().phi(),  cand.p4().mass() ) );
  }
  
  
  //general variables 
  output->particleIso_ = input->particleIso();
  output->isolationPFChargedHadrCandsPtSum_ = input->isolationPFChargedHadrCandsPtSum();
  output->isolationPFGammaCandsEtSum_ = input->isolationPFGammaCandsEtSum();
  float nhIso = 0;
  for(unsigned int i = 0; i < (input->isolationPFNeutrHadrCands()).size(); i++){
    nhIso += (input->isolationPFNeutrHadrCands()).at(i)->pt();
  }
  output->isolationPFNeutralHadrCandsPtSum_ = nhIso;

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


  //generator info  
  if(input->genJet()){
    //visible part of the generated jet 
    output->genJetp4_ = input->genJet()->p4();
    output->genJetCharge_ = input->genJet()->charge();
    output->genJetDecayMode_ = JetMCTagUtils::genTauDecayMode((const reco::CompositePtrCandidate)*(input->genJet()));

    //find the 4-momentum of the generated tau
    std::vector<const reco::GenParticle *> jetparticles = input->genJet()->getGenConstituents();
    for(unsigned i=0;i<jetparticles.size();i++){
      const reco::GenParticle* part=jetparticles.at(i);

      ///check three generations up 
      if(part->mother()){
	if(abs(part->mother()->pdgId())==15){
	  output->genTaup4_ = part->mother()->p4(); break;
	}
	
	if(part->mother()->mother()){
	  if(abs(part->mother()->mother()->pdgId())==15 ){
	    output->genTaup4_ = part->mother()->mother()->p4(); break;
	  }
	  
	  if(part->mother()->mother()->mother()){
	    if(abs(part->mother()->mother()->mother()->pdgId())==15 ){
	      output->genTaup4_ = part->mother()->mother()->mother()->p4(); break;
	    }
	  }
	  
	}
      }

    }
  }    
 

}



