#ifndef MULTIOBJECTFACTORY_H_
#define MULTIOBJECTFACTORY_H_

#include "CMGTools/Common/interface/Factory.h"
#include "CMGTools/Common/interface/SettingTool.h"
#include "CMGTools/Common/interface/MultiObjectSettingTool.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "AnalysisDataFormats/CMGTools/interface/MultiObject.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

//--------------------------------------------------------------------

namespace cmg {

  class MultiObjectFactory : public cmg::Factory<cmg::MultiObject>,
    public SettingTool<std::vector<edm::Ptr<reco::Candidate> >, cmg::MultiObject> {
  public:

    MultiObjectFactory(edm::ParameterSet const& ps):
      inputLabel_(ps.getParameter<edm::InputTag>("inputCollection"))
      {
      }

      typedef cmg::Factory<cmg::MultiObject>::event_ptr event_ptr;
      virtual event_ptr create(edm::Event const& iEvent,
                               edm::EventSetup const& iSetup);

      void set(std::vector<edm::Ptr<reco::Candidate> > const& input,
               cmg::MultiObject& object) const;

  private:
      edm::InputTag const inputLabel_;
      MultiObjectSettingTool const multiObjectSettingTool_;

  }; // class MultiObjectFactory

} // namespace cmg

//--------------------------------------------------------------------

#endif /*MULTIOBJECTFACTORY_H_*/

//--------------------------------------------------------------------
