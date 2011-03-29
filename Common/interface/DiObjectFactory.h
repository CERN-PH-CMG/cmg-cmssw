#ifndef DIOBJECTFACTORY_H_
#define DIOBJECTFACTORY_H_

#include "AnalysisDataFormats/CMGTools/interface/DiObject.h"
#include "CMGTools/Common/interface/Factory.h"
#include "CMGTools/Common/interface/SettingTool.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "TMath.h"

#include <algorithm>
#include <set>
#include <utility>

namespace cmg{

template< typename T, typename U >
class DiObjectFactory : public cmg::Factory< cmg::DiObject<T,U> >, public cmg::SettingTool<std::pair<T,U> ,cmg::DiObject<T,U> >{
    public:

        DiObjectFactory(const edm::ParameterSet& ps):
            leg1Label_(ps.getParameter<edm::InputTag>("leg1Collection")),
            leg2Label_(ps.getParameter<edm::InputTag>("leg2Collection")),
            metLabel_(ps.getParameter<edm::InputTag>("metCollection")){
        }

        //need to override from Factory to insert "typename"
        typedef typename cmg::Factory<typename cmg::DiObject<T,U> >::event_ptr event_ptr;
        virtual event_ptr create(const edm::Event&, const edm::EventSetup&) const;
        
        ///Set alphaT etc
        virtual void set(const std::pair<T,U>& pair, cmg::DiObject<T,U>* const obj) const;
        virtual void set(const std::pair<T,U>& pair, const reco::Candidate& met, cmg::DiObject<T,U>* const obj) const;

    private:

        double alphaT(T const& l1, U const& l2) const;
        double betaR(T const& l1, U const& l2) const;
        double mR(T const& l1, U const& l2) const;
        double mRP(T const& l1, U const& l2, const reco::Candidate& met) const;
        double mRT(T const& l1, U const& l2, const reco::Candidate& met) const;

        const edm::InputTag leg1Label_;
        const edm::InputTag leg2Label_;
        const edm::InputTag metLabel_;
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
    typedef edm::View<reco::Candidate> met_collection;
    
    edm::Handle<collection1> leg1Cands;
    iEvent.getByLabel(leg1Label_,leg1Cands);
    
    edm::Handle<collection2> leg2Cands;
    iEvent.getByLabel(leg2Label_,leg2Cands);
    
    edm::Handle<met_collection> metCands;
    iEvent.getByLabel(metLabel_,metCands);
    
    
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
            cmg::DiObjectFactory<T, U>::set(std::make_pair(cmgTmp.leg1(),cmgTmp.leg2()),&cmgTmp);
            if(metCands->size()){
                cmg::DiObjectFactory<T, U>::set(std::make_pair(cmgTmp.leg1(),cmgTmp.leg2()),metCands->at(0),&cmgTmp);
            }
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
void cmg::DiObjectFactory<T, U>::set(const std::pair<T,U>& pair, cmg::DiObject<T,U>* const obj) const{
    obj->alphaT_ = alphaT(pair.first, pair.second);
    //calculate the Razor variables without MET
    obj->betaR_ = betaR(pair.first, pair.second);
    obj->mR_ = mR(pair.first, pair.second);
}

template<typename T, typename U>
void cmg::DiObjectFactory<T, U>::set(const std::pair<T,U>& pair, const reco::Candidate& met, cmg::DiObject<T,U>* const obj) const{
    //calculate the Razor variables with MET
    obj->mRP_ = mRP(pair.first, pair.second, met);
    obj->mRT_ = mRT(pair.first, pair.second, met);
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

template<typename T, typename U>
double cmg::DiObjectFactory<T, U>::betaR(T const& l1, U const& l2) const{
    // compute Razor boost
    //Eqn (4) from CMS-PAS-SUS-10-009
    const double num = l1.p()-l2.p();
    const double den = l1.pz()-l2.pz();
      
    double beta = 1.;
    if(fabs(num) <= fabs(den)){
        // R good, R' bad
        beta = num/den;
    }else{
        // R bad, R' good
        beta = den/num;
    }
    return beta;   
}

template<typename T, typename U>
double cmg::DiObjectFactory<T, U>::mR(T const& ja, U const& jb) const{
  //Eqn (6) from CMS-PAS-SUS-10-009
  double temp = (ja.p()*jb.pz()-jb.p()*ja.pz())*
                (ja.p()*jb.pz()-jb.p()*ja.pz());
  temp /= (ja.pz()-jb.pz())*(ja.pz()-jb.pz())-(ja.p()-jb.p())*(ja.p()-jb.p());

  temp = 2.*sqrt(temp);
  // protect against nan
  if(TMath::IsNaN(temp)) temp = UnSet(Double_t);
  return temp;
}

template<typename T, typename U>
double cmg::DiObjectFactory<T, U>::mRP(T const& ja, U const& jb, const reco::Candidate& met) const{

    double jaP = ja.pt()*ja.pt() +ja.pz()*jb.pz()-ja.p()*jb.p();
    double jbP = jb.pt()*jb.pt() +ja.pz()*jb.pz()-ja.p()*jb.p();
    jbP *= -1.;
    double den = sqrt((ja.p()-jb.p())*(ja.p()-jb.p())-(ja.pz()-jb.pz())*(ja.pz()-jb.pz()));

    jaP /= den;
    jbP /= den;

    const reco::Candidate::LorentzVector metV = met.p4();
    const reco::Candidate::LorentzVector jaV = ja.p4();
    const reco::Candidate::LorentzVector jbV = jb.p4();
    const double metMag = metV.Vect().R();

    double temp = jaP*metV.Vect().Dot(jbV.Vect())/metMag + jbP*metV.Vect().Dot(jaV.Vect())/metMag;
    temp = temp*temp;
    den = (metV.Vect().Dot(jaV.Vect()+jbV.Vect())/metMag)*(metV.Vect().Dot(jaV.Vect()+jbV.Vect())/metMag)-(jaP-jbP)*(jaP-jbP);

    if(den <= 0.0) return -1.;

    temp /= den;
    temp = 2.*sqrt(temp);

    double bR = (jaP-jbP)/(metV.Vect().Dot(jaV.Vect()+jbV.Vect())/metMag);
    double gR = 1./sqrt(1.-bR*bR);

    temp *= gR;
  
    // protect against nan            
    if(TMath::IsNaN(temp)) temp = UnSet(Double_t);                                                                                                                                                   
    return temp;
}

template<typename T, typename U>
double cmg::DiObjectFactory<T, U>::mRT(T const& ja, U const& jb, const reco::Candidate& met) const{
    
    const reco::Candidate::LorentzVector metV = met.p4();
    const reco::Candidate::LorentzVector jaV = ja.p4();
    const reco::Candidate::LorentzVector jbV = jb.p4();    
    const double metMag = metV.Vect().R();
    
    double temp = metMag*(ja.pt()+jb.pt()) - metV.Vect().Dot(jaV.Vect()+jbV.Vect());
    temp /= 2.;

    temp = std::sqrt(temp);
    if(TMath::IsNaN(temp)) temp = UnSet(Double_t);
    return temp;
}

#endif /*DIOBJECTFACTORY_H_*/
