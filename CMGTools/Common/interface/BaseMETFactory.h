#ifndef _CMGTools_CommonTools_BaseMetFactory_H_
#define _CMGTools_CommonTools_BaseMetFactory_H_

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include "CMGTools/Common/interface/Factory.h"

#include <iostream>
#include <memory>

namespace cmg{

class BaseMETFactory : public Factory<cmg::BaseMET>{
  public:
    BaseMETFactory(const edm::ParameterSet& ps):
      inputLabel_(ps.getParameter<edm::InputTag>("inputCollection")){
    }
    virtual event_ptr create(const edm::Event&, const edm::EventSetup&) const;
    
  private:
    const edm::InputTag inputLabel_;
  };
  
}


#endif /*JETFACTORY_H_*/
