#ifndef _runinfocounter_h_
#define _runinfocounter_h_

// system include files
#include <memory>

// user include files
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DataFormats/Common/interface/MergeableCounter.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


class RunInfoCounter : public edm::EDProducer {
 public:
  explicit RunInfoCounter(const edm::ParameterSet&);
  void endLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup);
  void endJob();
  ~RunInfoCounter();
  
 private:
  
  virtual void produce(edm::Event&, const edm::EventSetup&);

  std::map<std::string, MonitorElement *> ctrs_;
  MonitorElement *crossSection_, *genEff_, *totalLumis_;
};

#endif
