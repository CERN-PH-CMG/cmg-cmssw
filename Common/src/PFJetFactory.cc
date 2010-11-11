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
    std::vector<reco::PFCandidatePtr> pfCandPtrs = jet.getPFConstituents();
    
    //    float maxPt = 0;
    //     float ptRelLeadingHadron = -1;
    //     float ptRelWLeadingHadron = -1;
  
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
  
    float totalEnergyFromConst = 0; 

    for(unsigned i=0; i<pfCandPtrs.size(); ++i) {
      const reco::PFCandidate& cand = *(pfCandPtrs[i]);
      reco::Candidate::LorentzVector jetWithoutCand = jet.p4() - cand.p4();       

      totalEnergyFromConst += cand.energy();

//       double ptRel = ROOT::Math::VectorUtil::Perp( cand.momentum(), 
// 						   jet.momentum() );
//       double ptRelWithout = ROOT::Math::VectorUtil::Perp( cand.momentum(), 
// 							  jetWithoutCand.Vect() );

//       pat::strbitset result = electronSelector_.getBitTemplate();

      switch( cand.particleId() ) {
      case reco::PFCandidate::h: 
// 	idInfo_.addPtRelHadron( ptRel ); 
// 	idInfo_.addPtRelWHadron( ptRelWithout ); 
// 	if( cand.pt()>maxPt) {
// 	  maxPt = cand.pt();
// 	  ptRelLeadingHadron = ptRel;
// 	  ptRelWLeadingHadron = ptRelWithout;
// 	}
	nCharged++;
	energyCharged += cand.energy(); 
	ptCharged += cand.pt(); 
	break;
      case reco::PFCandidate::e: 
// 	if( checkElectrons_ && electronSelector_(cand, result) ) {
// 	  idInfo_.setPtRelElectron( ptRel ); 
// 	  idInfo_.setPtRelWElectron( ptRelWithout ); 
// 	}
	nElectrons++;
	energyElectrons += cand.energy(); 
	ptElectrons += cand.pt(); 
	break;
      case reco::PFCandidate::mu:
	// use of the muon selector not yet implemented
// 	if( checkMuons_ ) {
// 	  idInfo_.setPtRelMuon( ptRel ); 
// 	  idInfo_.setPtRelWMuon( ptRelWithout ); 
// 	}
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

    // compute and fill energy fractions 

    energyCharged /= totalEnergyFromConst;
    energyElectrons /= totalEnergyFromConst;
    energyMuons /= totalEnergyFromConst;
    energyPhotons /= totalEnergyFromConst;
    energyNeutral /= totalEnergyFromConst;
    energyHFHad /= totalEnergyFromConst;
    energyHFEM /= totalEnergyFromConst;

    cmgjet.components_[reco::PFCandidate::h].setFraction(energyCharged);
    cmgjet.components_[reco::PFCandidate::e].setFraction(energyElectrons);
    cmgjet.components_[reco::PFCandidate::mu].setFraction(energyMuons);
    cmgjet.components_[reco::PFCandidate::gamma].setFraction(energyPhotons);
    cmgjet.components_[reco::PFCandidate::h0].setFraction(energyNeutral);
    cmgjet.components_[reco::PFCandidate::egamma_HF].setFraction(energyHFEM);
    cmgjet.components_[reco::PFCandidate::h_HF].setFraction(energyHFHad);

//     cmgjet.setPtRelLeadingHadron( ptRelLeadingHadron );
//     cmgjet.setPtRelWLeadingHadron( ptRelLeadingHadron );

    result->push_back(cmgjet);
  }
//   for(cmg::PFJetFactory::collection::const_iterator it = result->begin(); it != result->end(); ++it){
//     std::cout << "Created a jet: " << *it << std::endl;
//   }
  return result;
}
