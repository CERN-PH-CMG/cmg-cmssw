#ifndef __PFSim_PFSimParticleProducer__
#define __PFSim_PFSimParticleProducer__

// system include files
/* #include <memory> */
/* #include <string> */

#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "CMGTools/PFSim/interface/Simulator.h"


class PFSimParticleProducer : public edm::EDProducer {
 public:

  explicit PFSimParticleProducer(const edm::ParameterSet&);

  ~PFSimParticleProducer();
  
  virtual void produce(edm::Event&, const edm::EventSetup&);

  virtual void beginJob();

 private:

  edm::InputTag  hepmcSrc_;
  
  /// verbose ?
  bool   verbose_;

  PFSim::Simulator simulator_;

};

#endif
