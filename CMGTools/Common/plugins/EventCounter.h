// system include files
#include <memory>


// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DQMServices/Core/interface/DQMStore.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


class EventCounter : public edm::EDProducer {
   public:
      explicit EventCounter(const edm::ParameterSet&);
      ~EventCounter();

   private:
      virtual void produce(edm::Event&, const edm::EventSetup&);
      MonitorElement *evCount;
      std::string name_;
};
