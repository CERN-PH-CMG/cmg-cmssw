#ifndef _runinfocounter_h_
#define _runinfocounter_h_

// system include files
#include <memory>

// user include files
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DataFormats/Common/interface/MergeableCounter.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


class NormalizationCounter : public edm::EDAnalyzer {
 public:
  explicit NormalizationCounter(const edm::ParameterSet&);
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  ~NormalizationCounter();

 private:
  void beginLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup);
  void endLuminosityBlock(const edm::LuminosityBlock & iLumi, const edm::EventSetup & iSetup);
  void respondToOpenInputFile(edm::FileBlock const& fb);
  void respondToCloseInputFile(edm::FileBlock const& fb); 
  void endJob();

  bool fileChanged_;
  std::map<std::string, float> ctrs_;
};

#endif
