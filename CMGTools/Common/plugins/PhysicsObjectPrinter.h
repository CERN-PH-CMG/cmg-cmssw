#ifndef CMGTOOLS_COMMON_PHYSICSOBJECTPRINTER_H_
#define CMGTOOLS_COMMON_PHYSICSOBJECTPRINTER_H_

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

/// A simple analyzer that prints out a collection
class PhysicsObjectPrinter : public edm::EDAnalyzer{
  public:
    PhysicsObjectPrinter(const edm::ParameterSet& ps):
        inputLabel_(ps.getUntrackedParameter<edm::InputTag>("inputCollection")),
        printSelections_(ps.getUntrackedParameter<bool>("printSelections",false)),
        reportEvery_(ps.getUntrackedParameter<unsigned int>("reportEvery",100)),
        eventCount_(0){
    }
    virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);
     
  private:
    const edm::InputTag inputLabel_;
    const bool printSelections_;
    const unsigned int reportEvery_;
    
    unsigned int eventCount_;

};

}

#endif /*CMGTOOLS_COMMON_PHYSICSOBJECTPRINTER_H_*/
