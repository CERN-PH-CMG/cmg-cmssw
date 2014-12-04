#ifndef __PFSim_PFSimParticleProducer__
#define __PFSim_PFSimParticleProducer__

#include <string>
#include <vector>
#include <map>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "CMGTools/PFSim/interface/Simulator.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"


class PFSimParticleProducer : public edm::EDProducer {
 public:
  //TODO template on input
  typedef reco::LeafCandidate OutputParticle; 
  typedef std::vector< OutputParticle > OutputParticles;

  explicit PFSimParticleProducer(const edm::ParameterSet&);

  ~PFSimParticleProducer();
  
  virtual void produce(edm::Event&, const edm::EventSetup&);

  virtual void beginJob();

 private:

  edm::InputTag  hepmcSrc_;

  edm::InputTag  genSrc_;
  
  /// verbose ?
  bool   verbose_;

  bool firstEvent_;  

  edm::ESHandle<HepPDT::ParticleDataTable> pdt_;

  PFSim::Simulator simulator_;
};

#endif
