#ifndef _CMGTOOLS_SUSY_PLUGINS_INDIRECTTAUVETO_H_
#define _CMGTOOLS_SUSY_PLUGINS_INDIRECTTAUVETO_H_

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class IndirectTauVetoProducer : public edm::EDProducer {
public:
     explicit IndirectTauVetoProducer (const edm::ParameterSet&);
     virtual ~IndirectTauVetoProducer(){}

private:
  virtual void produce(edm::Event&, const edm::EventSetup&);

  edm::InputTag pfjetTag_;
  edm::InputTag metTag_;

};

#endif /*_CMGTOOLS_SUSY_PLUGINS_INDIRECTTAUVETO_H_*/
