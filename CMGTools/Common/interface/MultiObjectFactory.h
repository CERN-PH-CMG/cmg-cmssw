#ifndef MULTIOBJECTFACTORY_H_
#define MULTIOBJECTFACTORY_H_

#include "CMGTools/Common/interface/Factory.h"
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

class MultiObjectFactory : public cmg::Factory<cmg::MultiObject> {
public:

  MultiObjectFactory(edm::ParameterSet const& ps):
    inputLabel_(ps.getParameter<edm::InputTag>("inputCollection"))
    {
    }

    typedef cmg::Factory<cmg::MultiObject>::event_ptr event_ptr;
    virtual event_ptr create(edm::Event const& iEvent,
                             edm::EventSetup const& iSetup) const;

    double deltaPhiStar(std::vector<edm::Ptr<reco::Candidate> > const&
                        input) const;

    void set(std::vector<edm::Ptr<reco::Candidate> >
             const& input, cmg::MultiObject& object) const;

private:

  const edm::InputTag inputLabel_;

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

  std::vector<edm::Ptr<reco::Candidate> > tmp;
  tmp.resize(cands->size());
  for (size_t i = 0, n = cands->size(); i < n; ++i) {
    tmp.at(i) = edm::Ptr<reco::Candidate>(cands, i);
  }

  cmg::MultiObject object(tmp);
  cmg::MultiObjectFactory::set(tmp, object);
  result->push_back(object);

  return result;
}

//--------------------------------------------------------------------

// This is the delta-phi^star variable as defined in formula 5 (p. 10)
// of CMS AN-2010/242.
// NOTE: This is a delta-phi in [0, pi].
double cmg::MultiObjectFactory::deltaPhiStar(std::vector<
                                             edm::Ptr<reco::Candidate> >
                                             const& input) const
{
  double deltaPhiStar = cmg::unsetD;

  typedef math::XYZVector Vector;
  Vector negSumMom;
  for (std::vector<edm::Ptr<reco::Candidate> >::const_iterator it = input.begin(),
         end = input.end(); it != end; ++it) {
    negSumMom -= (*it)->momentum();
  }

  double deltaPhiMin = std::numeric_limits<double>::max();
  for (std::vector<edm::Ptr<reco::Candidate> >::const_iterator it = input.begin(),
         end = input.end(); it != end; ++it) {
    Vector tmp = negSumMom + (*it)->momentum();
    double deltaPhi = reco::deltaPhi(tmp.phi(), (*it)->phi());
    deltaPhi = abs(deltaPhi);
    if (deltaPhi < deltaPhiMin) {
      deltaPhiMin = deltaPhi;
    }
  }

  deltaPhiStar = deltaPhiMin;
  return deltaPhiStar;
}

//--------------------------------------------------------------------

void cmg::MultiObjectFactory::set(std::vector<edm::Ptr<reco::Candidate> >
                                  const& input, cmg::MultiObject& object) const
{
  object.deltaPhiStar_ = deltaPhiStar(input);
}

//--------------------------------------------------------------------

#endif /*MULTIOBJECTFACTORY_H_*/

//--------------------------------------------------------------------
