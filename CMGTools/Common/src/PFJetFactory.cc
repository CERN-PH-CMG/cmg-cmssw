#include "CMGTools/Common/interface/PFJetFactory.h"

#include <iostream>

using namespace std;

cmg::PFJetFactory::event_ptr cmg::PFJetFactory::create(const edm::Event& iEvent, 
						       const edm::EventSetup&) const{
	
  edm::Handle<pat::JetCollection> jetCands;
  
  cmg::PFJetFactory::event_ptr result(new cmg::PFJetFactory::collection);
  iEvent.getByLabel(jetLabel_,jetCands);

  long unsigned index = 0;
  for(pat::JetCollection::const_iterator mi = jetCands->begin();
      mi != jetCands->end(); ++mi, ++index ){
    
    pat::JetPtr jetPtr( jetCands, index ); 
    cmg::PFJet cmgjet( jetPtr );
    //set the basejet properties first
    baseJetFactory_.set(jetPtr,&cmgjet);
    
    const pat::Jet& jet = *mi;
    
    int nCharged = 0;
    int nPhotons = 0;
    int nNeutral = 0;
    int nElectrons = 0;
    int nMuons = 0;
    int nHFHad = 0;
    int nHFEM = 0;
  
    float energyCharged = 0;
    float energyPhotons = 0;
    float energyNeutral = 0;
    float energyElectrons = 0;
    float energyMuons = 0;
    float energyHFHad = 0;
    float energyHFEM = 0;

    float ptCharged = 0;
    float ptPhotons = 0;
    float ptNeutral = 0;
    float ptElectrons = 0;
    float ptMuons = 0;
    float ptHFHad = 0;
    float ptHFEM = 0;
  
    float fractionCharged = 0;
    float fractionPhotons = 0;
    float fractionNeutral = 0;
    float fractionElectrons = 0;
    float fractionMuons = 0;
    float fractionHFHad = 0;
    float fractionHFEM = 0;
  
    float totalEnergyFromConst = 0; 


    if( useConstituents_ ) {

      std::vector<reco::PFCandidatePtr> pfCandPtrs = jet.getPFConstituents();

      for(unsigned i=0; i<pfCandPtrs.size(); ++i) {
	const reco::PFCandidate& cand = *(pfCandPtrs[i]);
	reco::Candidate::LorentzVector jetWithoutCand = jet.p4() - cand.p4();       

	totalEnergyFromConst += cand.energy();

	switch( cand.particleId() ) {
	case reco::PFCandidate::h: 
	  nCharged++;
	  energyCharged += cand.energy(); 
	  ptCharged += cand.pt(); 
	  break;
	case reco::PFCandidate::e: 
	  nElectrons++;
	  energyElectrons += cand.energy(); 
	  ptElectrons += cand.pt(); 
	  break;
	case reco::PFCandidate::mu:
	  // use of the muon selector not yet implemented
	  nMuons++;
	  energyMuons += cand.energy(); 
	  ptMuons += cand.pt(); 
	  break;
	case reco::PFCandidate::gamma:
	  nPhotons++;
	  energyPhotons += cand.energy(); 
	  ptPhotons += cand.pt(); 
	  break;
	case reco::PFCandidate::h0:
	  nNeutral++;
	  energyNeutral += cand.energy(); 
	  ptNeutral += cand.pt(); 
	  break;
	case reco::PFCandidate::h_HF:
	  nHFHad++;
	  energyHFHad += cand.energy(); 
	  ptHFHad += cand.pt(); 
	  break;
	case reco::PFCandidate::egamma_HF:
	  nHFEM++;
	  energyHFEM += cand.energy(); 
	  ptHFEM += cand.pt(); 
	  break;
	default: break;
	}
      }

      fractionCharged = energyCharged / totalEnergyFromConst;
      fractionElectrons = energyElectrons / totalEnergyFromConst;
      fractionMuons = energyMuons / totalEnergyFromConst;
      fractionPhotons = energyPhotons / totalEnergyFromConst;
      fractionNeutral = energyNeutral / totalEnergyFromConst;
      fractionHFEM = energyHFEM / totalEnergyFromConst;
      fractionHFHad = energyHFHad / totalEnergyFromConst;
    }
    else {
      // using the JetSpecific structure stored in the pat jet

      nCharged = jet.chargedHadronMultiplicity();
      nElectrons = jet.electronMultiplicity();
      nMuons = jet.muonMultiplicity();
      nPhotons = jet.photonMultiplicity();
      nNeutral = jet.neutralHadronMultiplicity();
      nHFEM = jet.HFEMMultiplicity();
      nHFHad = jet.HFHadronMultiplicity();
      
      ptCharged = jet.chargedHadronEnergyFraction() * jet.pt();
      ptElectrons = jet.electronEnergyFraction() * jet.pt();
      ptMuons = jet.muonEnergyFraction() * jet.pt();
      ptPhotons = jet.photonEnergyFraction() * jet.pt();
      ptNeutral = jet.neutralHadronEnergyFraction() * jet.pt();
      ptHFEM = jet.HFEMEnergyFraction() * jet.pt();
      ptHFHad = jet.HFHadronEnergyFraction() * jet.pt();
      
      energyCharged = jet.chargedHadronEnergyFraction() * jet.energy();
      energyElectrons = jet.electronEnergyFraction() * jet.energy();
      energyMuons = jet.muonEnergyFraction() * jet.energy();
      energyPhotons = jet.photonEnergyFraction() * jet.energy();
      energyNeutral = jet.neutralHadronEnergyFraction() * jet.energy();
      energyHFEM = jet.HFEMEnergyFraction() * jet.energy();
      energyHFHad = jet.HFHadronEnergyFraction() * jet.energy();
      
      fractionCharged = jet.chargedHadronEnergyFraction();
      fractionElectrons = jet.electronEnergyFraction();
      fractionMuons = jet.muonEnergyFraction();
      fractionPhotons = jet.photonEnergyFraction();
      fractionNeutral = jet.neutralHadronEnergyFraction();
      fractionHFEM = jet.HFEMEnergyFraction();
      fractionHFHad = jet.HFHadronEnergyFraction();
      
	
    }

    cmgjet.components_[reco::PFCandidate::h].setNumber(nCharged);
    cmgjet.components_[reco::PFCandidate::e].setNumber(nElectrons);
    cmgjet.components_[reco::PFCandidate::mu].setNumber(nMuons);
    cmgjet.components_[reco::PFCandidate::gamma].setNumber(nPhotons);
    cmgjet.components_[reco::PFCandidate::h0].setNumber(nNeutral);
    cmgjet.components_[reco::PFCandidate::egamma_HF].setNumber(nHFEM);
    cmgjet.components_[reco::PFCandidate::h_HF].setNumber(nHFHad);

    cmgjet.components_[reco::PFCandidate::h].setPt(ptCharged);
    cmgjet.components_[reco::PFCandidate::e].setPt(ptElectrons);
    cmgjet.components_[reco::PFCandidate::mu].setPt(ptMuons);
    cmgjet.components_[reco::PFCandidate::gamma].setPt(ptPhotons);
    cmgjet.components_[reco::PFCandidate::h0].setPt(ptNeutral);
    cmgjet.components_[reco::PFCandidate::egamma_HF].setPt(ptHFEM);
    cmgjet.components_[reco::PFCandidate::h_HF].setPt(ptHFHad);

    cmgjet.components_[reco::PFCandidate::h].setEnergy(energyCharged);
    cmgjet.components_[reco::PFCandidate::e].setEnergy(energyElectrons);
    cmgjet.components_[reco::PFCandidate::mu].setEnergy(energyMuons);
    cmgjet.components_[reco::PFCandidate::gamma].setEnergy(energyPhotons);
    cmgjet.components_[reco::PFCandidate::h0].setEnergy(energyNeutral);
    cmgjet.components_[reco::PFCandidate::egamma_HF].setEnergy(energyHFEM);
    cmgjet.components_[reco::PFCandidate::h_HF].setEnergy(energyHFHad);

    cmgjet.components_[reco::PFCandidate::h].setFraction(fractionCharged);
    cmgjet.components_[reco::PFCandidate::e].setFraction(fractionElectrons);
    cmgjet.components_[reco::PFCandidate::mu].setFraction(fractionMuons);
    cmgjet.components_[reco::PFCandidate::gamma].setFraction(fractionPhotons);
    cmgjet.components_[reco::PFCandidate::h0].setFraction(fractionNeutral);
    cmgjet.components_[reco::PFCandidate::egamma_HF].setFraction(fractionHFEM);
    cmgjet.components_[reco::PFCandidate::h_HF].setFraction(fractionHFHad);


    result->push_back(cmgjet);
  }

  return result;
}
