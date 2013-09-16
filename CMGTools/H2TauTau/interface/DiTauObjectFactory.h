#ifndef DITAUOBJECTFACTORY_H_
#define DITAUOBJECTFACTORY_H_

#include "CMGTools/Common/interface/Factory.h"
#include "CMGTools/Common/interface/SettingTool.h"
#include "AnalysisDataFormats/CMGTools/interface/DiTauObject.h"

#include "CMGTools/Common/interface/DiObjectFactory.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"


#include <algorithm>
#include <set>

namespace cmg{

template<typename T, typename U>
class DiTauObjectFactory : public cmg::Factory< cmg::DiTauObject<T, U> >, public cmg::SettingTool<std::pair<T, U>, cmg::DiTauObject<T, U> >
{

    public:

        DiTauObjectFactory(const edm::ParameterSet& ps) :             
            leg1Label_(ps.getParameter<edm::InputTag>("leg1Collection")),
            leg2Label_(ps.getParameter<edm::InputTag>("leg2Collection")),
            metLabel_(ps.getParameter<edm::InputTag>("metCollection")),
            metSigLabel_(ps.getParameter<edm::InputTag>("metsigCollection"))
        {
        }

        //need to override from Factory to insert "typename"
        //typedef typename cmg::Factory<typename cmg::DiTauObject<T, U> >::event_ptr event_ptr;
        typedef typename cmg::Factory<typename cmg::DiTauObject<T, U> >::event_ptr event_ptr;
        virtual event_ptr create(const edm::Event&, const edm::EventSetup&);
        static void set(const std::pair<T, U>& pair, const reco::LeafCandidate& met, cmg::DiTauObject<T, U>* const obj);
        static void set(const std::pair<T, U>& pair, const reco::LeafCandidate& met, const cmg::METSignificance& metSig, cmg::DiTauObject<T, U>* const obj);

    private:
        const edm::InputTag leg1Label_;
        const edm::InputTag leg2Label_;
        const edm::InputTag metLabel_;
        const edm::InputTag metSigLabel_;
};

///Make when the types are different
template< typename T, typename U >
cmg::DiTauObject<T, U> makeDiTau(const T& l1, const U& l2){
    return cmg::DiTauObject<T, U>(l1,l2);
}

///Make when the types are the same - sorts the legs. It's still ensured below
///that it's only run if the tau collections are identical.
template<typename T>
cmg::DiTauObject<T, T> makeDiTau(const T& l1, const T& l2){
    if(l1 >= l2){
        return cmg::DiTauObject<T, T>(l1,l2);
    }else{
        return cmg::DiTauObject<T, T>(l2,l1);
    }
}


template<typename T, typename U>
void cmg::DiTauObjectFactory<T, U>::set(const std::pair<T, U>& pair, const reco::LeafCandidate& met, cmg::DiTauObject<T, U>* const obj){
  obj->leg1_ = pair.first;
  obj->leg2_ = pair.second;
  obj->setP4(obj->leg1_.p4() + obj->leg2_.p4());
  obj->setCharge(obj->leg1_.charge() + obj->leg2_.charge());
  obj->met_ = met;
  obj->mT_ = cmg::DiObjectFactory<T, U>::mT(pair.first, pair.second);
  if (pair.first.isElectron() || pair.first.isMuon())
    obj->lp_ = cmg::DiObjectFactory<T, U>::lp(pair.first, obj);
  //calculate the Razor variables with MET
  obj->mRT_ = cmg::DiObjectFactory<T, U>::mRT(pair.first, pair.second, met);
  std::pair<double,double> pZetaVars = cmg::DiObjectFactory<T, U>::pZeta(pair.first, pair.second, met);
  obj->pZetaVis_ = pZetaVars.first;
  obj->pZetaMET_ = pZetaVars.second;    
  obj->mTLeg1_ = cmg::DiObjectFactory<T, U>::mT(pair.first, met);
  obj->mTLeg2_ = cmg::DiObjectFactory<T, U>::mT(pair.second, met);    
}

template<typename T, typename U>
void cmg::DiTauObjectFactory<T, U>::set(const std::pair<T, U>& pair, const reco::LeafCandidate& met, const cmg::METSignificance& metSig, cmg::DiTauObject<T, U>* const obj) {
  set(pair, met, obj);
  obj->metSig_ = metSig;
}

template< typename T, typename U>
typename cmg::DiTauObjectFactory<T, U>::event_ptr cmg::DiTauObjectFactory<T, U>::create(const edm::Event& iEvent, const edm::EventSetup&){
    
    typedef typename std::vector<typename cmg::DiTauObject<T, U> > collection;
    typedef typename std::set<typename cmg::DiTauObject<T, U> > set;
    typedef edm::View<T> collection1;
    typedef edm::View<U> collection2;
    typedef edm::View<reco::LeafCandidate> met_collection;
    typedef edm::View<cmg::METSignificance> metSig_collection;
    
    edm::Handle<collection1> leg1Cands;
    iEvent.getByLabel(this->leg1Label_,leg1Cands);
    
    edm::Handle<collection2> leg2Cands;
    iEvent.getByLabel(this->leg2Label_,leg2Cands);
    
    edm::Handle<met_collection> metCands;
    bool metAvailable = false;
    if( !(this->metLabel_ == edm::InputTag()) ) {
      metAvailable = true; 
      iEvent.getByLabel(this->metLabel_,metCands);
    }

    edm::Handle<metSig_collection> metSigCands;
    bool metSigAvailable = false;
    if( !(this->metSigLabel_ == edm::InputTag()) ) {
      metSigAvailable = true; 
      iEvent.getByLabel(this->metSigLabel_,metSigCands);
    }
    
    typename cmg::DiTauObjectFactory<T, U>::event_ptr result(new collection);
    if( !leg1Cands->size() || !leg2Cands->size() ){
        return result;  
    }
    const bool sameCollection = (leg1Cands.id () == leg2Cands.id());
    for(typename collection1::const_iterator it = leg1Cands->begin(); it != leg1Cands->end(); ++it){
        for(typename collection2::const_iterator jt = leg2Cands->begin(); jt != leg2Cands->end(); ++jt){
            //we skip if its the same object
            if( sameCollection && (*it == *jt) ) continue;
            
            //enable sorting only if we are using the same collection - see Savannah #20217
            cmg::DiTauObject<T, U> cmgTmp = sameCollection ? cmg::makeDiTau(*it,*jt) : cmg::DiTauObject<T, U>(*it,*jt); 
            
      /*             if(metAvailable && metCands->size()){ */
            if(metAvailable && ! metCands->empty() ) 
            {
                if (metSigAvailable && !metSigCands->empty())
                    cmg::DiTauObjectFactory<T, U>::set(std::make_pair(cmgTmp.leg1(), cmgTmp.leg2()), metCands->at(0), metSigCands->at(0), &cmgTmp);
                else
                    cmg::DiTauObjectFactory<T, U>::set(std::make_pair(cmgTmp.leg1(), cmgTmp.leg2()), metCands->at(0), &cmgTmp);
	    /*             }    */
                result->push_back(cmgTmp);
            }
        }

    }

    //finally, remove duplicates while preserving order
    if(sameCollection){
      //Jose: code below removes some good mu-tau pairs because of the way 
      // two cmg objects are declared to be equal in  AnalysisDataFormats/CMGTools/src/AbstractPhysicsObject.cc
      // therefore this cleaning should only be done for 2 identical collections.
      // In case one builds di-objects from two collections of same kind of objects but where one of these collections is 
      // modified (by skiming for example) the "sameCollection" flag may not be set correctly, then one must do the cleaning 
      // by hand later.
      
      set diObjects;
      for(typename collection::iterator it = result->begin(); it != result->end(); ++it){
        std::pair<typename set::iterator,bool> set_it = diObjects.insert(*it);
        if(!set_it.second){
	  it = result->erase(it);
	  --it;   
        }
      } 
    }

    return result;
}

}


#endif /*DITAUOBJECTFACTORY_H_*/
