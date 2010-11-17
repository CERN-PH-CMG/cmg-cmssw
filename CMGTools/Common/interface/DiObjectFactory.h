#ifndef DIOBJECTFACTORY_H_
#define DIOBJECTFACTORY_H_

#include "AnalysisDataFormats/CMGTools/interface/DiObject.h"
#include "CMGTools/Common/interface/Factory.h"
#include "DataFormats/Common/interface/View.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

namespace cmg{

//used to check whether T == U - used template specialization
template< typename T, typename U > struct same{ enum { yes = 0}; };
template< typename T > struct same<T,T>{ enum { yes = 1}; };

template< typename T, typename U >
class DiObjectFactory : public cmg::Factory< cmg::DiObject<T,U> >{
    public:

        DiObjectFactory(const edm::ParameterSet& ps):
            leg1Label_(ps.getParameter<edm::InputTag>("leg1Collection")),
            leg2Label_(ps.getParameter<edm::InputTag>("leg2Collection")),
            same_type_(cmg::same<T,U>::yes){
        }

        //need to override from Factory to insert "typename"
        typedef typename cmg::Factory<typename cmg::DiObject<T,U> >::event_ptr event_ptr;
        virtual event_ptr create(const edm::Event&, const edm::EventSetup&) const;
        
    private:
        const edm::InputTag leg1Label_;
        const edm::InputTag leg2Label_;
        //is T == U - the types not the objects
        const bool same_type_;
};

}

template< typename T, typename U >
typename cmg::DiObjectFactory<T,U>::event_ptr cmg::DiObjectFactory<T,U>::create(const edm::Event& iEvent, const edm::EventSetup&) const{
    
    typedef typename std::vector<typename cmg::DiObject<T,U> > collection;
    //typedef std::vector<T> collection1;
    //typedef std::vector<U> collection2;
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
    
        const T& l1 = *it;
        for(typename collection2::size_type i = 0; i < (leg2Cands->size()/2); i++){
            const U& l2 = leg2Cands->at(i);
            //we skip if its the same object
            if( sameCollection && (l1 == l2) ) continue;
      
            //if T == U then sort so leg1 is greater than leg2 
            if(same_type_){
                if(l1 >= l2){
                    result->push_back(cmg::DiObject<T,U>(l1,l2));
                }else{
                    result->push_back(cmg::DiObject<T,U>(l2,l1));
                }
            }else{
                //stick to the default ordering
               result->push_back(cmg::DiObject<T,U>(l1,l2)); 
            }
      }
    }
    return result;
    
}

#endif /*DIOBJECTFACTORY_H_*/
