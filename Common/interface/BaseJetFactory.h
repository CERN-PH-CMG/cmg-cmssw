#ifndef JETFACTORY_H_
#define JETFACTORY_H_

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "AnalysisDataFormats/CMGTools/interface/BaseJet.h"
#include "CMGTools/Common/interface/Factory.h"
#include "CMGTools/Common/interface/SettingTool.h"

#include <iostream>
#include <memory>
#include <string>

class JetCorrectionUncertainty;

namespace cmg {

  class BaseJetFactory : public Factory<cmg::BaseJet>,
    public SettingTool<pat::JetPtr, cmg::BaseJet> {
  public:
    BaseJetFactory(const edm::ParameterSet& ps);
    virtual ~BaseJetFactory();

    virtual event_ptr create(const edm::Event&, const edm::EventSetup&);
    virtual void set(const pat::JetPtr& input, cmg::BaseJet* const output) const;

  private:
    const edm::InputTag jetLabel_;
    const std::vector<std::string> btagType_;

    const bool fillJec_;
    const bool fillJecUncertainty_;
    std::string jecPath_;
/* COLIN not needed ?    int jecUncDirection_; */

    JetCorrectionUncertainty* JES_;
  };

}

#endif /*JETFACTORY_H_*/
