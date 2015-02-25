#ifndef __PFSim_PFSimFastJetProducer__
#define __PFSim_PFSimFastJetProducer__

#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

// #include "AnalysisDataFormats/CMGTools/interface/SimpleParticle.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"

#include <fastjet/ClusterSequence.hh>


class PFSimFastJetProducer : public edm::EDProducer {
 public:
  //TODO template on input
  typedef reco::LeafCandidate Particle; 
  typedef edm::View< Particle > Particles;
  typedef std::vector< Particle > Jets;

  explicit PFSimFastJetProducer(const edm::ParameterSet&);

  ~PFSimFastJetProducer();
  
  virtual void produce(edm::Event&, const edm::EventSetup&);

  virtual void beginJob();

 private:

  edm::InputTag  particleSrc_;
  
  fastjet::JetDefinition  jet_def_;

  /// pt threshold on the output jets
  float jetPtThreshold_;

  /// verbose ?
  bool   verbose_;

};

#endif
