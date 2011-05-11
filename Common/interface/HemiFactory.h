#ifndef HEMIFACTORY_H_
#define HEMIFACTORY_H_

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/View.h"

#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"
#include "AnalysisDataFormats/CMGTools/interface/Hemisphere.h"
#include "CMGTools/Common/interface/Factory.h"
#include "CMGTools/Common/interface/SettingTool.h"
#include "CMGTools/Common/interface/MultiObjectSettingTool.h"

namespace cmg {

  typedef std::vector<edm::InputTag> VInputTag;

  class HemisphereFactory : public Factory<cmg::Hemisphere>,
    public SettingTool<std::vector<edm::Ptr<reco::Candidate> >, cmg::Hemisphere> {

  public:
    HemisphereFactory(const edm::ParameterSet& ps) :
      hemisphereLabel_(ps.getParameter<VInputTag>("inputCollection")),
      maxNCand_(ps.getParameter<uint32_t>("maxCand"))
        {
        }
    typedef cmg::Factory<cmg::Hemisphere>::event_ptr event_ptr;
    virtual event_ptr create(const edm::Event&,
                             const edm::EventSetup&) const;
    void set(std::vector<edm::Ptr<reco::Candidate> > const& input,
             cmg::Hemisphere& object) const;

  private:
    VInputTag const hemisphereLabel_;
    size_t maxNCand_;
    MultiObjectSettingTool const multiObjectFactory_;
  };

} // namespace cmg

#endif /*HEMIFACTORY_H_*/
