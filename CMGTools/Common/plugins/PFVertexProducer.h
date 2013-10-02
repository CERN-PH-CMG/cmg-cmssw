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
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include <string>

class PFVertexProducer : public edm::EDProducer {
 public:

  explicit PFVertexProducer(const edm::ParameterSet&);

  ~PFVertexProducer();
  
  virtual void produce(edm::Event&, const edm::EventSetup&);

  virtual void beginRun(edm::Run& run,const edm::EventSetup & es);

  int chargedHadronVertex( const reco::VertexCollection& vertices, 
			   const reco::PFCandidate& pfcand ) const;

 private:

  /// Input PFCandidates
  edm::InputTag       inputTagPFCandidates_;
  edm::InputTag       inputTagVertices_;

  bool checkClosestZVertex_;

};

#endif
