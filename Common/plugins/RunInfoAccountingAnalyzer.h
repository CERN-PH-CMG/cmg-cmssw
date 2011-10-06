#ifndef _CMGTools_Common_RunInfoAccountingAnalyzer_h_
#define _CMGTools_Common_RunInfoAccountingAnalyzer_h_

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "CMGTools/Common/interface/RunInfoAccounting.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/Event.h"
#include "SimDataFormats/GeneratorProducts/interface/GenFilterInfo.h"
// #include "FWCore/Framework/interface/Frameworkfwd.h"

#include <string>

namespace cmg{

  class RunInfoAccountingAnalyzer : public edm::EDAnalyzer {
  public:
    RunInfoAccountingAnalyzer(const edm::ParameterSet& ps); 
    
    virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);
    virtual void respondToOpenInputFile(edm::FileBlock const& fb);
    virtual void respondToCloseInputFile(edm::FileBlock const& fb);

    virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
    virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

    virtual void beginRun(edm::Run const&, edm::EventSetup const&);
    virtual void endRun(edm::Run const&, edm::EventSetup const&);

  private:
    void printFilterInfo(const edm::LuminosityBlock& iLumi, const GenFilterInfo& filter); 

    std::string  name_;

    cmg::RunInfoAccounting runInfoAccounting_;

    edm::Service<TFileService> fs_;

    edm::RunNumber_t  runNumber_;

    bool  fileChanged_;

    const edm::Run*  currentRun_;

    /// number of events before preselection
    unsigned int nTotal_;
    
    /// number of events after preselection
    unsigned int nPassed_;

    /// number of events generated (for the lumi sections seen)
    unsigned int nGenTotal_;

    /// number of events passing generator filter (for the lumi sections seen)
    unsigned int nGenPassed_;

    /// keeping track of the filtering efficiency after generator
    edm::InputTag  genFilterInfoSrc_;
    
    /// keeping track of another filtering efficiency, e.g. at PF2PAT+PAT+CMG level
    edm::InputTag  filterInfoSrc_;

    /// run on MC? 
    bool  runOnMC_;
    
  };

}

#endif /*HISTOGRAMANALYZER_H_*/
