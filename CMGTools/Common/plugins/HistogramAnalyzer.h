#ifndef HISTOGRAMANALYZER_H_
#define HISTOGRAMANALYZER_H_

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include <iostream>
#include <memory>

namespace cmg{

template <class HC>
class HistogramAnalyzer : public edm::EDAnalyzer {
  public:
  HistogramAnalyzer(const edm::ParameterSet& ps):
    histo_(new HC(ps)) {
    histo_->init();
  }
  virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
    histo_->fill(iEvent,iSetup);
  }
  private:
  std::auto_ptr<HC> histo_;
};

}

#endif /*HISTOGRAMANALYZER_H_*/
