#ifndef DIOBJECTFACTORY_H_
#define DIOBJECTFACTORY_H_

#include "AnalysisDataFormats/CMGTools/interface/DiObject.h"
#include "CMGTools/Common/interface/Factory.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include <algorithm>
#include <set>
#include <utility>

namespace cmg{

template< typename T, typename U >
class DiObjectFactory : public cmg::Factory< cmg::DiObject<T,U> >{
    public:

        DiObjectFactory(const edm::ParameterSet& ps):
            leg1Label_(ps.getParameter<edm::InputTag>("leg1Collection")),
            leg2Label_(ps.getParameter<edm::InputTag>("leg2Collection")){
        }

        //need to override from Factory to insert "typename"
        typedef typename cmg::Factory<typename cmg::DiObject<T,U> >::event_ptr event_ptr;
        virtual event_ptr create(const edm::Event&, const edm::EventSetup&) const;

    private:

        double alphaT(T const& l1, U const& l2) const;

        const edm::InputTag leg1Label_;
        const edm::InputTag leg2Label_;
};

///Make when the types are different
template< typename T, typename U >
cmg::DiObject<T,U> make(const T& l1, const U& l2){
    return cmg::DiObject<T,U>(l1,l2);
}
///Make when the types are the same - sorts the legs
template< typename T >
cmg::DiObject<T,T> make(const T& l1, const T& l2){
    if(l1 >= l2){
        return cmg::DiObject<T,T>(l1,l2);
    }else{
        return cmg::DiObject<T,T>(l2,l1);
    }
}

}

template< typename T, typename U >
typename cmg::DiObjectFactory<T,U>::event_ptr cmg::DiObjectFactory<T,U>::create(const edm::Event& iEvent, const edm::EventSetup&) const{
    
    typedef typename std::vector<typename cmg::DiObject<T,U> > collection;
    typedef typename std::set<typename cmg::DiObject<T,U> > set;
    typedef edm::View<T> collection1;
    typedef edm::View<U> collection2;
    
    edm::Handle<collection1> leg1Cands;
    iEvent.getByLabel(leg1Label_,leg1Cands);
    
    edm::Handle<collection2> leg2Cands;
    iEvent.getByLabel(leg2Label_,leg2Cands);
    
    typename cmg::DiObjectFactory<T,U>::event_ptr result(new collection);
    if( !leg1Cands->size() || !leg2Cands->size() ){
        return result;  
    }
    const bool sameCollection = (leg1Cands.id () == leg2Cands.id());
    for(typename collection1::const_iterator it = leg1Cands->begin(); it != leg1Cands->end(); ++it){
        for(typename collection2::const_iterator jt = leg2Cands->begin(); jt != leg2Cands->end(); ++jt){
            //we skip if its the same object
            if( sameCollection && (*it == *jt) ) continue;
            cmg::DiObject<T, U> cmgTmp = cmg::make(*it,*jt);
            cmgTmp.alphaT_ = alphaT(*it, *jt);
            result->push_back(cmgTmp);
      }
    }
    //finally, remove duplicates while preserving order
    set diObjects;
    for(typename collection::iterator it = result->begin(); it != result->end(); ++it){
        std::pair<typename set::iterator,bool> set_it = diObjects.insert(*it);
        if(!set_it.second){
            it = result->erase(it);
            --it;   
        }
    } 
    return result;
}

template<typename T, typename U>
  double cmg::DiObjectFactory<T, U>::alphaT(T const& l1, U const& l2) const
{
  // Alpha_T as defined on p. 3 of AN2008-114-v1.
  double num = std::min(l1.et(), l2.et());
  double deltaPhi = reco::deltaPhi(l1.p4().Phi(),
                                   l2.p4().Phi());
  double den = sqrt(2. * l1.et() * l2.et() *
                    (1. - cos(deltaPhi)));
  return num / den;
}

#endif /*DIOBJECTFACTORY_H_*/
