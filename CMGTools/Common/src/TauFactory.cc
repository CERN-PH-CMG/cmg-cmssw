#include "CMGTools/Common/interface/TauFactory.h"
#include "CMGTools/Common/interface/LeptonSettingTool.h"
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


  reco::PFCandidatePtr leadChargedHadrCand=input->leadPFChargedHadrCand();
  if(leadChargedHadrCand.isNonnull()){
    const reco::PFCandidate& cand = *(input->leadPFChargedHadrCand().get());
    output->leadPFChargedHadrCand_ = cmg::Tau::Constituent( cand.pdgId(), 
							    cand.p4().pt(), cand.p4().eta(), 
							    cand.p4().phi(),  cand.p4().mass());

    //leading charged particle vertex
    if(cand.trackRef().isNonnull() && cand.trackRef().isAvailable()){
      output->leadChargedHadrVertex_ = cand.trackRef()->vertex();
      leptonFactory_.set(cand.trackRef(),output,iEvent,iSetup);   //dz and dxy w.r.t primary vertex in offlinePrimaryVertices
    } else if(cand.gsfTrackRef().isNonnull() && cand.gsfTrackRef().isAvailable()){
      //when there is no pion track check if there is an electron track
      output->leadChargedHadrVertex_ = cand.gsfTrackRef()->vertex();
      leptonFactory_.set(cand.gsfTrackRef(),output,iEvent,iSetup);  
    }


    //COLIN a bit of redundancy here ... should clean up at some point
    output->leadChargedHadrPt_  = leadChargedHadrCand->pt();
    output->leadChargedHadrCharge_  = leadChargedHadrCand->charge();
    output->leadChargedHadrHcalEnergy_    = leadChargedHadrCand->hcalEnergy();
    output->leadChargedHadrEcalEnergy_    = leadChargedHadrCand->ecalEnergy();
    output->leadChargedHadrMvaEPi_ = leadChargedHadrCand->mva_e_pi();
  }



  //neutral related
  reco::PFCandidatePtr leadNeutralCand=input->leadPFNeutralCand();
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
  
  //info about the original PFJet
  if(input->isPFTau()){
    if(input->pfJetRef().isAvailable())
      output->jetRefp4_=input->pfJetRef()->p4();			 
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
  int i=0;
  for (std::vector<pat::Tau::IdPair>::const_iterator it = tauids.begin() ; it != tauids.end() ; ++it,i++) {
    if( i == NCMGTAUIDS)
      throw cms::Exception( " Trying to add more tauIDs than reserved slots in cmg::Tau");
    output->tauIDLabels_.push_back( it->first);
    output->tauID_.push_back( it->second);
  }


  //generator info  
  if(input->genJet()){
    //visible part of the generated jet 
    output->genJetp4_ = input->genJet()->p4();
    output->genJetCharge_ = input->genJet()->charge();
    output->genJetDecayMode_ = JetMCTagUtils::genTauDecayMode((const reco::CompositePtrCandidate)*(input->genJet()));
  }    
 

}



