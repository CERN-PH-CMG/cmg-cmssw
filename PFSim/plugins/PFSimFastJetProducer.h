#ifndef __PFSim_PFSimFastJetProducer__
#define __PFSim_PFSimFastJetProducer__

#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "AnalysisDataFormats/CMGTools/interface/SimpleParticle.h"

#include <fastjet/ClusterSequence.hh>


class PFSimFastJetProducer : public edm::EDProducer {
 public:
  //TODO template on input
  typedef std::vector< cmg::SimpleParticle> Particles;
  //TODO for now using SimpleParticle as a Jet class... later use CMG::PFJets?
  typedef std::vector< cmg::SimpleParticle> Jets;

  explicit PFSimFastJetProducer(const edm::ParameterSet&);

  ~PFSimFastJetProducer();
  
  virtual void produce(edm::Event&, const edm::EventSetup&);

  virtual void beginJob();

 private:

  edm::InputTag  particleSrc_;
  
  fastjet::JetDefinition  jet_def_;

  /// verbose ?
  bool   verbose_;

};

#endif
