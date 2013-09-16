#include "CMGTools/Common/interface/L1TriggerObjectFactory.h"


cmg::L1TriggerObjectFactory::event_ptr cmg::L1TriggerObjectFactory::create(const edm::Event& iEvent, const edm::EventSetup& iSetup){

    const std::string isRealDataString("isRealData");
    const bool isRealData = iEvent.isRealData();
   
    // Just return empty if no trigger info found.
    cmg::L1TriggerObjectFactory::event_ptr result(new cmg::L1TriggerObjectFactory::collection);
   
    edm::Handle<L1GlobalTriggerReadoutRecord> L1Results;
    
    // Use the process name ?? should be RECO (data), or HLT(MC)
    // Get L1 results.
    const edm::InputTag L1ResultsLabel = edm::InputTag( L1ResultsLabel_.label(), L1ResultsLabel_.instance());

    iEvent.getByLabel(L1ResultsLabel, L1Results);
   
    DecisionWord gtDecisionWord = L1Results->decisionWord();
   
    //get L1 menu from event setup for correspondance algo bits <-> names
    edm::ESHandle<L1GtTriggerMenu> menuRcd;
    iSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;
    const L1GtTriggerMenu* menu = menuRcd.product();

    std::map<std::string,bool> triggerMap;

    for (CItAlgo algo = menu->gtAlgorithmMap().begin(); algo!=menu->gtAlgorithmMap().end(); ++algo) {
      //bit number
      unsigned int itrig        = (algo->second).algoBitNumber();
      //name
      std::string l1triggername = (algo->second).algoAlias() ; //or TString( (algo->second).algoName() );
      //decision & storing
      triggerMap[l1triggername] = gtDecisionWord[itrig];
    }

    // Just store a single trigger object and set which triggers
    // passed.
    cmg::TriggerObject to;
    for(std::map<std::string,bool>::const_iterator jt = triggerMap.begin(); jt != triggerMap.end(); ++jt) {
      const int prescale = 1;
      to.addSelectionWithPrescale(jt->first,jt->second,prescale);
    }
    to.addSelection(isRealDataString,isRealData);
    result->push_back(to);

    return result;
}

