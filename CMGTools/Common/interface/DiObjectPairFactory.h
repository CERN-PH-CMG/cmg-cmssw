#ifndef DIOBJECTPAIRFACTORY_H_
#define DIOBJECTPAIRFACTORY_H_

#include "AnalysisDataFormats/CMGTools/interface/DiObjectPair.h"
#include "CMGTools/Common/interface/Factory.h"
#include "CMGTools/Common/interface/SettingTool.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "TMath.h"
#include "TVector3.h"

#include <algorithm>
#include <set>
#include <utility>

namespace cmg{

  template< typename T, typename U,typename V, typename W >
    class DiObjectPairFactory : public cmg::Factory< cmg::DiObjectPair<T,U,V,W> >, public cmg::SettingTool<std::pair<DiObject<T,U>,DiObject<V,W> > ,cmg::DiObjectPair<T,U,V,W> >{
    public:

        DiObjectPairFactory(const edm::ParameterSet& ps):
            leg1Label_(ps.getParameter<edm::InputTag>("leg1Collection")),
            leg2Label_(ps.getParameter<edm::InputTag>("leg2Collection")),
            metLabel_(ps.getParameter<edm::InputTag>("metCollection")){
        }

        //need to override from Factory to insert "typename"
	  typedef typename cmg::Factory<typename cmg::DiObjectPair<T,U,V,W> >::event_ptr event_ptr;
        virtual event_ptr create(const edm::Event&, const edm::EventSetup&) ;
        
        ///Set alphaT etc
        virtual void set(const std::pair<DiObject<T,U>,DiObject<V,W> >& pair, cmg::DiObjectPair<T,U,V,W>* const obj) const;
        virtual void set(const std::pair<DiObject<T,U>,DiObject<V,W> >& pair, const reco::Candidate& met, cmg::DiObjectPair<T,U,V,W>* const obj) const;

    private:



        const edm::InputTag leg1Label_;
        const edm::InputTag leg2Label_;
        const edm::InputTag metLabel_;
};



///Make when the types are different
  template< typename T,typename U, typename V, typename W >
   cmg::DiObjectPair<T,U,V,W> make(const DiObject<T,U>& l1, const DiObject<V,W>& l2){
   return cmg::DiObjectPair<T,U,V,W>(l1,l2);
}

///Make when the types are the same - sorts the legs
 template< typename T ,typename U>
   cmg::DiObjectPair<T,U,T,U> make(const DiObject<T,U>& l1, const DiObject<T,U>& l2){
   if(l1.mass()>l2.mass()){
     return cmg::DiObjectPair<T,U,T,U>(l1,l2);
    }else{
     return cmg::DiObjectPair<T,U,T,U>(l2,l1);
    }
}

}

template< typename T, typename U, typename V,typename W>
  typename cmg::DiObjectPairFactory<T,U,V,W>::event_ptr cmg::DiObjectPairFactory<T,U,V,W>::create(const edm::Event& iEvent, const edm::EventSetup&) {
    
  typedef typename std::vector<typename cmg::DiObjectPair<T,U,V,W> > collection;
  typedef typename std::set<typename cmg::DiObjectPair<T,U,V,W> > set;
  typedef edm::View< DiObject<T,U> > collection1;
  typedef edm::View< DiObject<V,W> > collection2;

  typedef edm::View<reco::Candidate> met_collection;
    
    edm::Handle<collection1> leg1Cands;
    iEvent.getByLabel(leg1Label_,leg1Cands);
    
    edm::Handle<collection2> leg2Cands;
    iEvent.getByLabel(leg2Label_,leg2Cands);
    
    
    edm::Handle<met_collection> metCands;
    bool metAvailable = false;
    if( !(metLabel_ == edm::InputTag()) ) {
      metAvailable = true; 
      iEvent.getByLabel(metLabel_,metCands);
    }
    
    

    typename cmg::DiObjectPairFactory<T,U,V,W>::event_ptr result(new collection);
    if( !leg1Cands->size() || !leg2Cands->size() ){
        return result;  
    }
    const bool sameCollection = (leg1Cands.id () == leg2Cands.id());
    for(typename collection1::const_iterator it = leg1Cands->begin(); it != leg1Cands->end(); ++it){
        for(typename collection2::const_iterator jt = leg2Cands->begin(); jt != leg2Cands->end(); ++jt){
            //we skip if its the same object
            if( sameCollection && (*it == *jt) ) continue;
            
            //enable sorting only if we are using the same collection - see Savannah #20217
            cmg::DiObjectPair<T, U,V,W> cmgTmp = sameCollection ? cmg::make(*it,*jt) : cmg::DiObjectPair<T,U,V,W>(*it,*jt); 
            
            cmg::DiObjectPairFactory<T,U,V,W>::set(std::make_pair(cmgTmp.first(),cmgTmp.second()),&cmgTmp);
	    if(metAvailable && ! metCands->empty() ) 
	      cmg::DiObjectPairFactory<T, U,V,W>::set(std::make_pair(cmgTmp.first(),cmgTmp.second()),metCands->at(0),&cmgTmp);


	    //Cross cleaning. The four candidates must be different
	    if( (cmgTmp.first().leg1().p4() != cmgTmp.second().leg1().p4()) &&
		(cmgTmp.first().leg1().p4() != cmgTmp.second().leg2().p4()) &&
		(cmgTmp.first().leg2().p4() != cmgTmp.second().leg1().p4()) &&
		(cmgTmp.first().leg2().p4() != cmgTmp.second().leg2().p4()) )
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

template<typename T, typename U,typename V, typename W>
  void cmg::DiObjectPairFactory<T, U,V,W>::set(const std::pair<DiObject<T,U>, DiObject<V,W> > & pair, cmg::DiObjectPair<T,U,V,W>* const obj) const{


}

template<typename T, typename U,typename V, typename W>
  void cmg::DiObjectPairFactory<T, U,V,W>::set(const std::pair<DiObject<T,U>,DiObject<V,W> >& pair, const reco::Candidate& met, cmg::DiObjectPair<T,U,V,W>* const obj) const{

}


#endif /*DIOBJECTFACTORY_H_*/
