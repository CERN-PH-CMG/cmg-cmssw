#ifndef _CMGTools_CommonTools_BASEMETMODIFICATIONFACTORY_H_
#define _CMGTools_CommonTools_BASEMETMODIFICATIONFACTORY_H_

#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"
#include "CMGTools/Common/interface/BaseMETFactory.h"
#include "CMGTools/Common/interface/Factory.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include <string>

namespace cmg{

/**
 * Class that takes a cmg::BaseMet and corrects it with another collection
 */
class BaseMETModificationFactory : public Factory<cmg::BaseMET>{
  
  typedef math::XYZTLorentzVector LorentzVector;
  
  public:
    BaseMETModificationFactory(const edm::ParameterSet& ps):
      inputLabel_(ps.getParameter<edm::InputTag>("inputCollection")),
      metLabel_(ps.getParameter<edm::InputTag>("metCollection")),
      ptThreshold_(ps.getParameter<double>("ptThreshold")),
      operator_(ps.getParameter<std::string>("operator")){
    }
    virtual event_ptr create(const edm::Event&, const edm::EventSetup&);

  private:
    const edm::InputTag inputLabel_;
    const edm::InputTag metLabel_;
    const double ptThreshold_;
    const std::string operator_;
  };
  
}

#endif /*_CMGTools_CommonTools_BASEMETMODIFICATIONFACTORY_H_*/
