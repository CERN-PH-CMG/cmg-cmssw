#ifndef __CMGTools_Susy_GenToPFConverter_h__
#define __CMGTools_Susy_GenToPFConverter_h__

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"


/**
 */

class GenToPFConverter : public edm::EDProducer {

 public:
  

  explicit GenToPFConverter(const edm::ParameterSet& ps):
    src_(ps.getParameter<edm::InputTag>("src")),
    verbose_(ps.getUntrackedParameter<bool>("verbose",false)){
    produces<reco::PFCandidateCollection>("");

    // will store a collection of GenParticles 
    // selected in the same way as the PFCandidates
    produces<reco::GenParticleCollection>("GEN");
  }

  void produce(edm::Event& iEvent, const edm::EventSetup&);

 private:

  const edm::InputTag src_;

  const bool verbose_ ;

};

#endif 
