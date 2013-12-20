#include <string>
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "CMGTools/Common/interface/TriggerObjectFactory.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "AnalysisDataFormats/CMGTools/interface/TriggerPrescaleList.h"

class CMGTriggerPrescaleProducer : public edm::EDProducer {
    public:
        explicit CMGTriggerPrescaleProducer(const edm::ParameterSet&);
        ~CMGTriggerPrescaleProducer();

        virtual void produce(edm::Event&, const edm::EventSetup&);
        virtual void beginRun(edm::Run &, const edm::EventSetup&);
    private:
        edm::InputTag triggerResultsLabel_;
        std::string   processName_;
        std::string   tagTriggerEvent_;
        //trigger info
        HLTConfigProvider hlt_;
};
CMGTriggerPrescaleProducer::CMGTriggerPrescaleProducer(const edm::ParameterSet& iConfig) :
    triggerResultsLabel_(iConfig.getParameter<edm::InputTag>("triggerResults")),
    processName_(iConfig.getParameter<std::string>("processName")),
    tagTriggerEvent_( "hltTriggerSummaryAOD" )
{
  produces<cmg::TriggerPrescaleList>();
}

CMGTriggerPrescaleProducer::~CMGTriggerPrescaleProducer() {}

void CMGTriggerPrescaleProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
    const edm::InputTag triggerResultsLabel = edm::InputTag( triggerResultsLabel_.label(), triggerResultsLabel_.instance(), processName_);

    edm::Handle<edm::TriggerResults> triggerResults;
    iEvent.getByLabel(triggerResultsLabel, triggerResults);
    std::auto_ptr<cmg::TriggerPrescaleList> outPtr(new cmg::TriggerPrescaleList(triggerResults));

    const edm::TriggerNames & names = iEvent.triggerNames(*triggerResults);
    const std::vector<std::string> vnames = names.triggerNames();

    cmg::TriggerPrescaleList &out = *outPtr;
    int pset = hlt_.prescaleSet(iEvent,iSetup);
    for (unsigned int i = 0, n = vnames.size(); i < n; ++i) { 
        const int prescale = (pset < 0) ? UnSet(int) : hlt_.prescaleValue(pset,vnames[i]);
        out.addPrescaledTrigger(i, prescale);
    }

    iEvent.put( outPtr  );
}
void CMGTriggerPrescaleProducer::beginRun(edm::Run& iRun, edm::EventSetup const& iSetup) {

    if( (processName_ != "") && ( (processName_ == "*") || (processName_ == "@") ) ) {
        processName_ = cmg::TriggerObjectFactory::getProcessName(iRun,tagTriggerEvent_);
    }
    bool changed = false;
    hlt_.init(iRun,iSetup,processName_,changed);
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(CMGTriggerPrescaleProducer);
