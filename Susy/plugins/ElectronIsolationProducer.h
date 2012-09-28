#ifndef _CMGTOOLS_SUSY_PLUGINS_ELECTRONISO_H_
#define _CMGTOOLS_SUSY_PLUGINS_ELECTRONISO_H_

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class ElectronIsolationProducer : public edm::EDProducer {
public:
     explicit ElectronIsolationProducer (const edm::ParameterSet&);
     virtual ~ElectronIsolationProducer(){}

private:
  virtual void produce(edm::Event&, const edm::EventSetup&);

  edm::InputTag electronTag_;
  edm::InputTag rhoTag_;
  const double isoCut_;
  const double coneSize_;

};

#endif /*_CMGTOOLS_SUSY_PLUGINS_ELECTRONISO_H_*/

