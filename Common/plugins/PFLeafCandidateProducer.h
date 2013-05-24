#ifndef __PFSim_PFLeafCandidateProducer__
#define __PFSim_PFLeafCandidateProducer__

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


class PFLeafCandidateProducer : public edm::EDProducer {
 public:

  explicit PFLeafCandidateProducer(const edm::ParameterSet&);

  ~PFLeafCandidateProducer();
  
  virtual void produce(edm::Event&, const edm::EventSetup&);

  virtual void beginJob();

 private:

  edm::InputTag  hepmcSrc_;
  
  /// verbose ?
  bool   verbose_;

};

#endif
