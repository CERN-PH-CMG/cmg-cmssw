#ifndef JETSCALER_H_
#define JETSCALER_H_

#include <iomanip>

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "CMGTools/Common/interface/Factory.h"

namespace cmg {

  template <class T>
  class JetScaler : public Factory<T> {
  public:
    JetScaler(const edm::ParameterSet& ps);
    virtual ~JetScaler() {};
    typedef typename cmg::Factory<T>::event_ptr event_ptr;
    virtual event_ptr create(const edm::Event&, const edm::EventSetup&) const;

  private:
    const edm::InputTag jetLabel_;
    double jecUncDirection_;
  };

}

template <class T>
cmg::JetScaler<T>::JetScaler(const edm::ParameterSet& ps):
  jetLabel_(ps.getParameter<edm::InputTag>("inputCollection")),
  jecUncDirection_(0.)
{
  jecUncDirection_ = ps.getParameter<double>("jecUncDirection");
  edm::LogInfo("JetScaler")
    << "Scaling jets by " << std::setw(2) << jecUncDirection_
    << " times JEC uncertainty" << std::endl;
}

template <class T>
typename cmg::JetScaler<T>::event_ptr cmg::JetScaler<T>::create(const edm::Event& iEvent,
                                                                const edm::EventSetup&) const
{
  typedef typename std::vector<T> collection;
  edm::Handle<collection> jetCands;
  typename cmg::JetScaler<T>::event_ptr result(new collection);
  iEvent.getByLabel(jetLabel_, jetCands);

  size_t index = 0;
  for(typename collection::const_iterator mi = jetCands->begin();
      mi != jetCands->end(); ++mi, ++index) {

    edm::Ref<collection> jetPtr(jetCands, index);
    T jet(*jetPtr);

    reco::Candidate::LorentzVector fourVec = jet.p4();
    float unc = jet.uncOnFourVectorScale();
    fourVec *= (1. + (jecUncDirection_ * unc));
    jet.setP4(fourVec);

    result->push_back(jet);
  }
  return result;
}

#endif /*JETSCALER_H_*/
