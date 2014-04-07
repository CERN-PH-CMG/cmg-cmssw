#ifndef PHYSICSOBJECTFACTORY_H_
#define PHYSICSOBJECTFACTORY_H_

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/View.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"
#include "CMGTools/Common/interface/Factory.h"

#include <iostream>
#include <memory>

namespace cmg {

template <class PATPtr>
class PhysicsObjectFactory : public cmg::Factory<cmg::PhysicsObjectWithPtr<PATPtr> > {
    public:
        PhysicsObjectFactory(const edm::ParameterSet& ps):
            inputLabel_(ps.getParameter<edm::InputTag>("inputCollection")){
        }
        typedef typename cmg::Factory<cmg::PhysicsObjectWithPtr<PATPtr> >::event_ptr event_ptr;
        
        virtual event_ptr create(const edm::Event&, const edm::EventSetup&);
        
    private:
        const edm::InputTag inputLabel_;
};

}

template <class PATPtr>
typename cmg::PhysicsObjectFactory<PATPtr>::event_ptr cmg::PhysicsObjectFactory<PATPtr>::create(const edm::Event& iEvent, const edm::EventSetup&){
    
  edm::Handle< edm::View<typename PATPtr::value_type> > cands;
  typedef typename std::vector<typename cmg::PhysicsObjectWithPtr<PATPtr> > collection;
  typename cmg::PhysicsObjectFactory<PATPtr>::event_ptr result(new collection);
  iEvent.getByLabel(inputLabel_,cands);
  
  unsigned int index = 0;
  for(typename edm::View<typename PATPtr::value_type>::const_iterator it = cands->begin(); it != cands->end(); ++it, ++index){

    const PATPtr p(cands, index);
    cmg::PhysicsObjectWithPtr<PATPtr> g(p);
    result->push_back(g);
    
  }
  return result;
}

#endif /*PHYSICSOBJECTFACTORY_H_*/
