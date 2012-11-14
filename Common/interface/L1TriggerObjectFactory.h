#ifndef _CMGTools_CommonTools_L1TRIGGEROBJECTFACTORY_H_
#define _CMGTools_CommonTools_L1TRIGGEROBJECTFACTORY_H_

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/Registry.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "AnalysisDataFormats/CMGTools/interface/TriggerObject.h"
#include "CMGTools/Common/interface/Factory.h"
#include "CMGTools/Common/interface/SettingTool.h"

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenuFwd.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"

#include "DataFormats/Provenance/interface/ProcessHistory.h"
#include "FWCore/Framework/interface/Run.h"

#include <algorithm>
#include <map>
#include <iostream>


namespace cmg{

class L1TriggerObjectFactory : public Factory<cmg::TriggerObject>{
    public:
        L1TriggerObjectFactory(const edm::ParameterSet& ps):
	  L1ResultsLabel_(ps.getParameter<edm::InputTag>("L1Results"))
	      {
	      }
	  virtual event_ptr create(const edm::Event&, const edm::EventSetup&);
   
    private:
        const edm::InputTag L1ResultsLabel_;
        
};

}


#endif /*_CMGTools_CommonTools_L1TRIGGEROBJECTFACTORY_H_*/
