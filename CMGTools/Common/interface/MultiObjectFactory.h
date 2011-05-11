#ifndef MULTIOBJECTFACTORY_H_
#define MULTIOBJECTFACTORY_H_

#include "CMGTools/Common/interface/Factory.h"
#include "CMGTools/Common/interface/SettingTool.h"
#include "CMGTools/Common/interface/MultiObjectSettingTool.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "AnalysisDataFormats/CMGTools/interface/UnSet.h"
#include "AnalysisDataFormats/CMGTools/interface/MultiObject.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/Vector.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include <limits>
#include <cmath>

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
                               edm::EventSetup const& iSetup) const;

      void set(std::vector<edm::Ptr<reco::Candidate> > const& input,
               cmg::MultiObject& object) const;

  private:
      edm::InputTag const inputLabel_;
      MultiObjectSettingTool const multiObjectFactory_;

  }; // class MultiObjectFactory

} // namespace cmg

//--------------------------------------------------------------------

cmg::MultiObjectFactory::event_ptr
cmg::MultiObjectFactory::create(edm::Event const& iEvent,
                                edm::EventSetup const& iSetup) const
{
  typedef std::vector<cmg::MultiObject> collection;
  cmg::MultiObjectFactory::event_ptr result(new collection);

  // There really is not much to be done here, create a multi-object
  // from the stuff in the input collection, then fill the few things
  // in it and done.

  edm::Handle<edm::View<reco::Candidate> > cands;
  iEvent.getByLabel(inputLabel_, cands);

  if (cands->size() > 0) {
    std::vector<edm::Ptr<reco::Candidate> > tmp;
    tmp.resize(cands->size());
    for (size_t i = 0, n = cands->size(); i < n; ++i) {
      tmp.at(i) = edm::Ptr<reco::Candidate>(cands, i);
    }

    cmg::MultiObject object(tmp);
    cmg::MultiObjectFactory::set(tmp, object);
    result->push_back(object);
  }

  return result;
}

//--------------------------------------------------------------------

void cmg::MultiObjectFactory::set(std::vector<edm::Ptr<reco::Candidate> >
                                  const& input, cmg::MultiObject& object) const
{
  multiObjectFactory_.set(input, &object);
}

//--------------------------------------------------------------------

#endif /*MULTIOBJECTFACTORY_H_*/

//--------------------------------------------------------------------
