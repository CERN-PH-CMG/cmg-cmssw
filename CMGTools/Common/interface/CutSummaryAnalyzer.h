#ifndef CUTSUMMARYANALYZER_H_
#define CUTSUMMARYANALYZER_H_

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>

namespace cmg{

class CutSummaryAnalyzer : public edm::EDAnalyzer{
  public:
    CutSummaryAnalyzer(const edm::ParameterSet& ps):
        inputLabel_(ps.getParameter<edm::InputTag>("inputCollection")){
    }
    virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);
    virtual void endJob();

    typedef std::map< std::string, std::pair<unsigned int, unsigned int> > CutCounts;
    
  private:

    const edm::InputTag inputLabel_;
    CutCounts counter_;
    
};

}

#endif /*CUTSUMMARYANALYZER_H_*/
