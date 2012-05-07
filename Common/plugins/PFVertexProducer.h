#ifndef CMGTools_Common_PFVertexProducer_h
#define CMGTools_Common_PFVertexProducer_h

/** \class PFVertexProducer
 *  Producer meant to add the vertex back to charged PF Candidates.
 *  Patrick Janot - 07-May-2012
 */

#include <iostream>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

#include <string>

class PFVertexProducer : public edm::EDProducer {
 public:

  explicit PFVertexProducer(const edm::ParameterSet&);

  ~PFVertexProducer();
  
  virtual void produce(edm::Event&, const edm::EventSetup&);

  virtual void beginRun(edm::Run& run,const edm::EventSetup & es);

 private:

  /// Input PFCandidates
  edm::InputTag       inputTagPFCandidates_;

};

#endif
