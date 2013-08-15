#ifndef _CMGTools_Common_StructuredPFJetFactory_h_
#define _CMGTools_Common_StructuredPFJetFactory_h_

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "AnalysisDataFormats/CMGTools/interface/StructuredPFJet.h"
#include "CMGTools/Common/interface/Factory.h"
#include "CMGTools/Common/interface/PFJetFactory.h"

#include <iostream>
#include <memory>

namespace cmg{

class StructuredPFJetFactory : public Factory<cmg::StructuredPFJet>{
  public:
    StructuredPFJetFactory(const edm::ParameterSet& ps);
    
    virtual event_ptr create(const edm::Event&, const edm::EventSetup&);

  private:
    const edm::InputTag jetLabel_;
    const edm::InputTag subJetLabel_;
    const BaseJetFactory baseJetFactory_;
    const PFJetFactory pfJetFactory_;
  };

}


#endif /*_CMGTools_Common_StructuredPFJetFactory_h_*/
