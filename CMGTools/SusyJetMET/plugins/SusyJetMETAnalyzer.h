#ifndef _CMGTools_SusyJetMET_SusyJetMETAnalyzer_h_
#define _CMGTools_SusyJetMET_SusyJetMETAnalyzer_h_

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"

#include <string>
#include <vector>

/**
   All purpose analyzer for printouts, and for everything that can'be done with the common cmg tools
*/

namespace cmg{

  class SusyJetMETAnalyzer : public edm::EDAnalyzer {
  public:
    SusyJetMETAnalyzer(const edm::ParameterSet& ps); 
    
    virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);
    
    void printPaths( const std::vector<std::string>& paths );
  };

}

#endif
