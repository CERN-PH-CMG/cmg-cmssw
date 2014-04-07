#include "CMGTools/Common/interface/TriggerObjectFactory.h"

#include "DataFormats/Provenance/interface/ProcessHistory.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/Registry.h"

#include <algorithm>
#include <map>
#include <set>
#include <iostream>

cmg::TriggerObjectFactory::event_ptr cmg::TriggerObjectFactory::create(const edm::Event& iEvent, const edm::EventSetup& iSetup){

    const std::string isRealDataString("isRealData");
    const bool isRealData = iEvent.isRealData();

    // Just return empty if no trigger info found.
    cmg::TriggerObjectFactory::event_ptr result(new cmg::TriggerObjectFactory::collection);
    if ( processName_ == "*" || processName_ == "@" )
      return result;

    edm::Handle<edm::TriggerResults> triggerResults;
    edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects;

    // Use the auto-process name.
    const edm::InputTag triggerResultsLabel = edm::InputTag( triggerResultsLabel_.label(), triggerResultsLabel_.instance(), processName_);

    iEvent.getByLabel(triggerResultsLabel, triggerResults);
    iEvent.getByLabel(triggerObjectsLabel_, triggerObjects);

    const edm::TriggerNames& triggerNames = iEvent.triggerNames(*triggerResults);
    edm::TriggerNames::Strings const& names = triggerNames.triggerNames();
    
    int pset = hlt_.prescaleSet(iEvent,iSetup);

    // Store which triggers passed and failed.
    std::map<std::string,bool> triggerMap;
    for(edm::TriggerNames::Strings::const_iterator it = names.begin(); it != names.end(); ++it) {
        const unsigned int i = triggerNames.triggerIndex(*it);
        if(i >= triggerNames.size())
          continue;
        triggerMap[*it] = triggerResults->wasrun(i) && triggerResults->accept(i);
    }

    if(useTriggerObjects_) {

      //copy the pat trigger objects into a cmg::TriggerObject collection
      for(unsigned int index = 0; index < triggerObjects->size(); index++) {
	pat::TriggerObjectStandAlone sa = triggerObjects->at(index);
	pat::TriggerObjectPtr o(triggerObjects, index);
	cmg::TriggerObject to(o,"");

	//check if this object fired at least one path to remove unwanted objects 
	bool fired=0;
	for(std::map<std::string,bool>::const_iterator path = triggerMap.begin(); path != triggerMap.end(); ++path)
	  if(path->second)
	    if(sa.path(path->first, false, false)) {
	      //use path() method with checkLastFilter=false because complex filters are not handled properly
	      //see this posting: https://hypernews.cern.ch/HyperNews/CMS/get/physTools/2651/1.html
	      //last filter requirement can be applied later using the filter labels 
	      fired=1;
	      to.addSelection(path->first,1);//save the HLT paths the object was used in,
	    }
	
	if(!fired) continue;

	//save the filter labels for each trigger object, needed for trigger-matching
	std::vector< std::string > filters=sa.filterLabels();
	for(std::vector< std::string >::const_iterator label=filters.begin();label!=filters.end();label++)
	  to.addSelection(*label,1);

	
	result->push_back(to);
      }
      

    } else {
        // Just store a single trigger object and set which triggers
        // passed.
        cmg::TriggerObject to;
        for(std::map<std::string,bool>::const_iterator jt = triggerMap.begin(); jt != triggerMap.end(); ++jt) {
            const int prescale = (pset < 0) ? pset : hlt_.prescaleValue(pset,jt->first);
            to.addSelectionWithPrescale(jt->first,jt->second,prescale);
            //std::cout << "Prescale: " << jt->first << ":" << prescale << "\t - " << jt->second << " " << to.getPrescale(jt->first) << std::endl;
        }
        to.addSelection(isRealDataString,isRealData);
        result->push_back(to);
    }

    return result;
}

std::string cmg::TriggerObjectFactory::getProcessName(edm::Run& iRun, const std::string& tag) {

    //code taken from PatTriggerEventProducer
    //http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/PhysicsTools/PatAlgos/plugins/PATTriggerEventProducer.cc?revision=1.15&view=markup

    std::string nameProcess = "*";
    // Determine process name from last run module "tag" in process
    // history of input.
    const edm::ProcessHistory& processHistory( iRun.processHistory() );
    edm::ProcessConfiguration processConfiguration;
    edm::ParameterSet processPSet;
    // Unbroken loop, which relies on time ordering (accepts the last
    // found entry).
    for ( edm::ProcessHistory::const_iterator iHist = processHistory.begin(); iHist != processHistory.end(); ++iHist ) {
      if ( processHistory.getConfigurationForProcess( iHist->processName(), processConfiguration )     &&
           edm::pset::Registry::instance()->getMapped( processConfiguration.parameterSetID(), processPSet ) &&
           processPSet.exists( tag )
         ) {
        nameProcess = iHist->processName();
      }
    }
    // If no name was found default to nothing and hope for the best.
    if(nameProcess == "*") {
         edm::LogError( "cmg::TriggerObjectFactory" ) << "trigger::TriggerEvent product with label '"
            << tag << "' not produced according to process history of input data\n"
            << "No trigger information produced.";
        nameProcess = "@";//mark as error state
    }
    return nameProcess;
}

bool cmg::TriggerObjectFactory::beginRun(edm::Run& iRun, edm::EventSetup const& iSetup) {

    if( (processName_ != "") && ( (processName_ == "*") || (processName_ == "@") ) ) {
        processName_ = getProcessName(iRun,tagTriggerEvent_);
    }
    bool changed = false;
    return hlt_.init(iRun,iSetup,processName_,changed);
}
