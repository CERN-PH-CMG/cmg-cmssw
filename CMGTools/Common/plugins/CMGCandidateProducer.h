#ifndef __Sim_CMGCandidateProducer__
#define __Sim_CMGCandidateProducer__

#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"


class CMGCandidateProducer : public edm::EDProducer {
 public:

  explicit CMGCandidateProducer(const edm::ParameterSet&);

  ~CMGCandidateProducer();
  
  virtual void produce(edm::Event&, const edm::EventSetup&);

  virtual void beginJob();

 private:

  edm::InputTag  Cands_;
  edm::InputTag  CandsFromPV_;
  
};

#endif
