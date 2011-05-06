#include "CMGTools/Common/interface/TriggerObjectFactory.h"

#include "DataFormats/Provenance/interface/ProcessHistory.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/Registry.h"

cmg::TriggerObjectFactory::event_ptr cmg::TriggerObjectFactory::create(const edm::Event& iEvent, const edm::EventSetup& iSetup) const{
    
    //just return empty if no trigger info found
    cmg::TriggerObjectFactory::event_ptr result(new cmg::TriggerObjectFactory::collection);
    if ( processName_ == "*" || processName_ == "@" ) return result;
    
    edm::Handle<edm::TriggerResults> triggerResults;
    edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects;
    
    //use the auto-process name
    const edm::InputTag triggerResultsLabel = edm::InputTag( triggerResultsLabel_.label(), triggerResultsLabel_.instance(), processName_);
    const edm::InputTag triggerObjectsLabel = edm::InputTag( triggerObjectsLabel.label(), triggerObjectsLabel.instance(), processName_);
    
    iEvent.getByLabel(triggerResultsLabel,triggerResults);
    iEvent.getByLabel(triggerObjectsLabel,triggerObjects);
    
    const edm::TriggerNames& triggerNames = iEvent.triggerNames(*triggerResults);
    edm::TriggerNames::Strings const& names = triggerNames.triggerNames();

    for(edm::TriggerNames::Strings::const_iterator it = names.begin(); it != names.end(); ++it){
        const unsigned int i = triggerNames.triggerIndex(*it);
        if(i >= triggerNames.size()) continue;
        if(triggerResults->wasrun(i) && triggerResults->accept(i)){
            std::cout << "Trigger passed: " << *it << std::endl;
            for(unsigned int index = 0; index < triggerObjects->size(); index++){
                pat::TriggerObjectStandAlone sa = triggerObjects->at(index);
                if(sa.path(*it)){
                    std::cout << "match found: " << sa.pt() << std::endl;
                    pat::TriggerObjectPtr o(triggerObjects, index);
                    result->push_back(cmg::TriggerObject(o,*it));
                }
            }
        }
    }
    return result;
}

std::string cmg::TriggerObjectFactory::getProcessName(edm::Run& iRun, const std::string& tag){
    
    //code taken from PatTriggerEventProducer
    //http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/PhysicsTools/PatAlgos/plugins/PATTriggerEventProducer.cc?revision=1.15&view=markup
    
    std::string nameProcess = "*";
    // determine process name from last run module "tag" in process history of input
    const edm::ProcessHistory& processHistory( iRun.processHistory() );
    edm::ProcessConfiguration processConfiguration;
    edm::ParameterSet processPSet;
    // unbroken loop, which relies on time ordering (accepts the last found entry)
    for ( edm::ProcessHistory::const_iterator iHist = processHistory.begin(); iHist != processHistory.end(); ++iHist ) {
      if ( processHistory.getConfigurationForProcess( iHist->processName(), processConfiguration )     &&
           edm::pset::Registry::instance()->getMapped( processConfiguration.parameterSetID(), processPSet ) &&
           processPSet.exists( tag )
         ) {
        nameProcess = iHist->processName();
      }
    }
    //if no name was found default to nothing and hope for the best
    if(nameProcess == "*"){
         edm::LogError( "cmg::TriggerObjectFactory" ) << "trigger::TriggerEvent product with label '"
            << tag << "' not produced according to process history of input data\n"
            << "No trigger information produced.";
        nameProcess = "@";//mark as error state
    }
    return nameProcess;
    
}

bool cmg::TriggerObjectFactory::beginRun(edm::Run& iRun, edm::EventSetup const&){
    
    if( (processName_ != "") && ( (processName_ == "*") || (processName_ == "@") ) ){
        processName_ = getProcessName(iRun,tagTriggerEvent_);
    }
    return true;
}