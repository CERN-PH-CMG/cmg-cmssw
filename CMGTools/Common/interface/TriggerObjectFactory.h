#ifndef _CMGTools_CommonTools_TRIGGEROBJECTFACTORY_H_
#define _CMGTools_CommonTools_TRIGGEROBJECTFACTORY_H_

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "AnalysisDataFormats/CMGTools/interface/TriggerObject.h"
#include "CMGTools/Common/interface/Factory.h"
#include "CMGTools/Common/interface/SettingTool.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

namespace cmg{

class TriggerObjectFactory : public Factory<cmg::TriggerObject>{
    public:
        TriggerObjectFactory(const edm::ParameterSet& ps):
            triggerResultsLabel_(ps.getParameter<edm::InputTag>("triggerResults")),
            triggerObjectsLabel_(ps.getParameter<edm::InputTag>("triggerObjects")),
            processName_(ps.getUntrackedParameter<std::string>("processName","")),
            useTriggerObjects_(ps.getUntrackedParameter<bool>("useTriggerObjects",false)),
	    saveAllHLTPathsInObjs_(ps.getUntrackedParameter<bool>("saveAllHLTPathsInObjs",false)),
            tagTriggerEvent_( "hltTriggerSummaryAOD" ){
        }
        virtual event_ptr create(const edm::Event&, const edm::EventSetup&);
        
        static std::string getProcessName(edm::Run&, const std::string&);
        virtual bool beginRun(edm::Run&, edm::EventSetup const&);
        
    private:
        const edm::InputTag triggerResultsLabel_;
        const edm::InputTag triggerObjectsLabel_;
        std::string processName_;
        const bool useTriggerObjects_;
        const bool saveAllHLTPathsInObjs_;
        const std::string tagTriggerEvent_;
        
        //trigger info
        HLTConfigProvider hlt_;
        
};

}


#endif /*_CMGTools_CommonTools_TRIGGEROBJECTFACTORY_H_*/
