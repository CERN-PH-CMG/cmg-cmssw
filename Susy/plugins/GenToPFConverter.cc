#include "CMGTools/Susy/plugins/GenToPFConverter.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include <iostream>

using namespace std; 

void GenToPFConverter::produce(edm::Event& iEvent, const edm::EventSetup&) {

  edm::Handle< std::vector<reco::GenParticle> > gens;
  iEvent.getByLabel(src_,gens);
    
  std::auto_ptr< reco::PFCandidateCollection> result( new reco::PFCandidateCollection);
  std::auto_ptr< reco::GenParticleCollection> resultGen( new reco::GenParticleCollection);

  for( unsigned i=0; i<gens->size(); ++i) {
    
    const reco::GenParticle& gen = gens->at(i);


    if (gen.status() != 1 ) continue; 
 
    int pdgId = abs(gen.pdgId());
    int charge = gen.charge(); 
    
    reco::PFCandidate::ParticleType pfId = reco::PFCandidate::X; 

    if( pdgId>100 && pdgId<1000000) { // hadrons, and not a susy particle...
      if(charge==0) pfId = reco::PFCandidate::h0;
      else pfId = reco::PFCandidate::h;
    }
    else if(pdgId==11) 
      pfId = reco::PFCandidate::e;
    else if(pdgId==13)
      pfId = reco::PFCandidate::mu;
    else if(pdgId==22)
      pfId = reco::PFCandidate::gamma;
    else if(pdgId!=12 && pdgId!=14 && pdgId != 16 && pdgId<1000000)
      cout<<"WARNING! "<<pdgId<<" "<<charge<<endl;
    else
      continue; // neutrinos

    result->push_back( reco::PFCandidate( charge, gen.p4(), pfId) );
    resultGen->push_back( gen );
    
  }


  iEvent.put(result);
  iEvent.put(resultGen, "GEN");
 return;
}

#include "FWCore/Framework/interface/MakerMacros.h"


DEFINE_FWK_MODULE(GenToPFConverter);
