#ifndef OBJECTSMEAR_H_
#define OBJECTSMEAR_H_

#include <iomanip>

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "CMGTools/Common/interface/Factory.h"
#include <TRandom2.h>

namespace cmg {

  template <class T>
  class ObjectSmear : public Factory<T> {
  public:
    ObjectSmear(const edm::ParameterSet& ps);
    virtual ~ObjectSmear() {};
    typedef typename cmg::Factory<T>::event_ptr event_ptr;
    virtual event_ptr create(const edm::Event&, const edm::EventSetup&);

  private:
    const edm::InputTag candLabel_;
    double sigma_; 
    TRandom2 randEngine_; 
  };

}

template <class T>
cmg::ObjectSmear<T>::ObjectSmear(const edm::ParameterSet& ps):
  candLabel_(ps.getParameter<edm::InputTag>("inputCollection")),
  sigma_(ps.getParameter<double>("sigma"))
{
  if(sigma_>0.){
    edm::LogInfo("ObjectSmear") << "Smearing cands p4 with Gaussian of sigma =" << std::setw(2) << sigma_ << std::endl;
  }else edm::LogInfo("ObjectSmear") << " Disabled " << std::endl;
}

template <class T>
typename cmg::ObjectSmear<T>::event_ptr cmg::ObjectSmear<T>::create(const edm::Event& iEvent,
                                                                const edm::EventSetup&)
{
  typedef typename std::vector<T> collection;
  edm::Handle<collection> candCands;
  typename cmg::ObjectSmear<T>::event_ptr result(new collection);
  iEvent.getByLabel(candLabel_, candCands);
 
  size_t index = 0;
  for(typename collection::const_iterator mi = candCands->begin();
      mi != candCands->end(); ++mi, ++index) {

    edm::Ref<collection> candPtr(candCands, index);
    T cand(*candPtr);

    reco::Candidate::LorentzVector fourVec = cand.p4();
    if(sigma_>0.) fourVec *= (1. + randEngine_.Gaus(0.,sigma_));
    cand.setP4(fourVec);

    result->push_back(cand);
  }
  return result;
}

#endif /*OBJECTSMEAR_H_*/
