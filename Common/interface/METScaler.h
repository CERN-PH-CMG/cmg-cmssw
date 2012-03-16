#ifndef METSCALER_H_
#define METSCALER_H_

#include <iomanip>

#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/BaseMET.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "CMGTools/Common/interface/Factory.h"

namespace cmg {

  class METScaler : public Factory<cmg::BaseMET> {
  public:
    METScaler(const edm::ParameterSet& ps);
    virtual event_ptr create(const edm::Event&, const edm::EventSetup&);

  private:
    const edm::InputTag jetLabel_;
    const edm::InputTag metLabel_;
    const double jecUncDirection_;
    const bool doType1_;

  };

}
#endif /*METSCALER_H_*/
