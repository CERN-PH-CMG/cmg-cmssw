#ifndef _CMGTools_Common_PFJetFactory_h_
#define _CMGTools_Common_PFJetFactory_h_

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "CMGTools/Common/interface/Factory.h"
#include "CMGTools/Common/interface/BaseJetFactory.h"

#include <iostream>
#include <memory>

namespace cmg{

class PFJetFactory : public Factory<cmg::PFJet>{
  public:
    PFJetFactory(const edm::ParameterSet& ps):
      jetLabel_(ps.getParameter<edm::InputTag>("inputCollection")),
      baseJetFactory_(ps.getParameter<edm::ParameterSet>("baseJetFactory")), 
      useConstituents_(ps.getParameter<bool>("useConstituents")) {}

      virtual event_ptr create(const edm::Event&, const edm::EventSetup&);
    
  private:
    const edm::InputTag jetLabel_;
    const BaseJetFactory baseJetFactory_;
    bool  useConstituents_;
  };

}


#endif /*_CMGTools_Common_PFJetFactory_h_*/
