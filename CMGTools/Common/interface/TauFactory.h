#ifndef TAUFACTORY_H_
#define TAUFACTORY_H_

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "AnalysisDataFormats/CMGTools/interface/Tau.h"
#include "AnalysisDataFormats/CMGTools/interface/PatTypes.h"
#include "CMGTools/Common/interface/Factory.h"
#include "CMGTools/Common/interface/SettingTool.h"
#include "CMGTools/Common/interface/LeptonSettingTool.h"

#include <iostream>
#include <memory>
#include <string>

namespace cmg{

  class TauFactory : public Factory<cmg::Tau>, public SettingTool<pat::TauPtr,cmg::Tau>{
  public:

    TauFactory(const edm::ParameterSet& ps):
      tauLabel_(ps.getParameter<edm::InputTag>("inputCollection")),
      leptonFactory_(ps.getParameter<edm::ParameterSet>("leptonFactory")){
    }
    virtual event_ptr create(const edm::Event&, const edm::EventSetup&);
    virtual void set(const pat::TauPtr& input,  cmg::Tau*  const output, const edm::Event&, const edm::EventSetup&) const;
        


  private:
        
    
    const edm::InputTag tauLabel_;
        
    LeptonSettingTool<pat::TauPtr> leptonFactory_;
    

  };

}


#endif /*TAUFACTORY_H_*/
