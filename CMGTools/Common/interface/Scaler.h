#ifndef SCALER_H_
#define SCALER_H_

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
  class Scaler : public Factory<T> {
  public:
    Scaler(const edm::ParameterSet& ps);
    virtual ~Scaler() {};
    typedef typename cmg::Factory<T>::event_ptr event_ptr;
    virtual event_ptr create(const edm::Event&, const edm::EventSetup&);

  private:
    const edm::InputTag candLabel_;
    double nSigma_;
    double uncertainty_; 
  };

}

template <class T>
cmg::Scaler<T>::Scaler(const edm::ParameterSet& ps):
  candLabel_(ps.getParameter<edm::InputTag>("inputCollection")),
  nSigma_(ps.getParameter<double>("nSigma")),
  uncertainty_(ps.getParameter<double>("uncertainty"))
{
  edm::LogInfo("Scaler")
    << "Scaling cands by " << std::setw(2) << nSigma_
    << " times uncertainty "<<uncertainty_ << std::endl;
}

template <class T>
typename cmg::Scaler<T>::event_ptr cmg::Scaler<T>::create(const edm::Event& iEvent,
                                                                const edm::EventSetup&)
{
  typedef typename std::vector<T> collection;
  edm::Handle<collection> candCands;
  typename cmg::Scaler<T>::event_ptr result(new collection);
  iEvent.getByLabel(candLabel_, candCands);

  size_t index = 0;
  for(typename collection::const_iterator mi = candCands->begin();
      mi != candCands->end(); ++mi, ++index) {

    edm::Ref<collection> candPtr(candCands, index);
    T cand(*candPtr);

    reco::Candidate::LorentzVector fourVec = cand.p4();
    float unc = uncertainty_;
    if( unc<0 ) {
      // uncertainty not specified in cfg, taking it from cand itself. 
      //COLIN: need to generalize the uncertainty to all physics objects
      unc = cand.uncOnFourVectorScale();
    }
    fourVec *= (1. + (nSigma_ * unc));
    cand.setP4(fourVec);

    result->push_back(cand);
  }
  return result;
}

#endif /*SCALER_H_*/
