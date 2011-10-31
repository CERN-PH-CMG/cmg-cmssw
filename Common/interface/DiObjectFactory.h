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
#include "TVector3.h"

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

/*         double mT(T const& l1, U const& l2) const; */
	double mT( const reco::Candidate& l1, const reco::Candidate& l2) const;
        double alphaT(T const& l1, U const& l2) const;
        double mR(T const& l1, U const& l2) const;
        double mRT(T const& l1, U const& l2, const reco::Candidate& met) const;
        double lp(T const& l1, cmg::DiObject<T,U>* const obj) const;
	std::pair<double, double> pZeta(T const& l1, U const& l2, 
					const reco::Candidate& met) const;

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
    bool metAvailable = false;
    if( !(metLabel_ == edm::InputTag()) ) {
      metAvailable = true; 
      iEvent.getByLabel(metLabel_,metCands);
    }
    
    

    typename cmg::DiObjectFactory<T,U>::event_ptr result(new collection);
    if( !leg1Cands->size() || !leg2Cands->size() ){
        return result;  
    }
    const bool sameCollection = (leg1Cands.id () == leg2Cands.id());
    for(typename collection1::const_iterator it = leg1Cands->begin(); it != leg1Cands->end(); ++it){
        for(typename collection2::const_iterator jt = leg2Cands->begin(); jt != leg2Cands->end(); ++jt){
            //we skip if its the same object
            if( sameCollection && (*it == *jt) ) continue;
            
            //enable sorting only if we are using the same collection - see Savannah #20217
            cmg::DiObject<T, U> cmgTmp = sameCollection ? cmg::make(*it,*jt) : cmg::DiObject<T,U>(*it,*jt); 
            
            cmg::DiObjectFactory<T, U>::set(std::make_pair(cmgTmp.leg1(),cmgTmp.leg2()),&cmgTmp);
	    /*             if(metAvailable && metCands->size()){ */
	    if(metAvailable && ! metCands->empty() ) 
	      cmg::DiObjectFactory<T, U>::set(std::make_pair(cmgTmp.leg1(),cmgTmp.leg2()),metCands->at(0),&cmgTmp);
	    /*             }    */
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
    obj->mT_ = mT(pair.first, pair.second);  
    obj->alphaT_ = alphaT(pair.first, pair.second);
    if (pair.first.isElectron() || pair.first.isMuon())
      obj->lp_ = lp(pair.first, obj);
    //calculate the Razor variables without MET
    obj->mR_ = mR(pair.first, pair.second);
}

template<typename T, typename U>
void cmg::DiObjectFactory<T, U>::set(const std::pair<T,U>& pair, const reco::Candidate& met, cmg::DiObject<T,U>* const obj) const{
    obj->mT_ = mT(pair.first, pair.second);
    if (pair.first.isElectron() || pair.first.isMuon())
      obj->lp_ = lp(pair.first, obj);
    //calculate the Razor variables with MET
    obj->mRT_ = mRT(pair.first, pair.second, met);
    std::pair<double,double> pZetaVars = pZeta(pair.first, pair.second, met);
    obj->pZetaVis_ = pZetaVars.first;
    obj->pZetaMET_ = pZetaVars.second;    
    obj->mTLeg1_ = mT(pair.first, met);
    obj->mTLeg2_ = mT(pair.second, met);    
}

template<typename T, typename U>
/*   double cmg::DiObjectFactory<T, U>::mT(T const& l1, U const& l2) const */
  double cmg::DiObjectFactory<T, U>::mT( const reco::Candidate& l1, const reco::Candidate& l2) const
{
  // transverse mass
  return std::sqrt( std::pow(l1.pt()+l2.pt(), 2) - 
                    std::pow(l1.px()+l2.px(), 2) - 
                    std::pow(l1.py()+l2.py(), 2) );
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
double cmg::DiObjectFactory<T, U>::mR(T const& ja, U const& jb) const{
    //validated for 2011 variables
    const double A = ja.p();
    const double B = jb.p();
    const double az = ja.pz();
    const double bz = jb.pz();
  
    TVector3 jaT, jbT;
    jaT.SetXYZ(ja.px(),ja.py(),0.0);
    jbT.SetXYZ(jb.px(),jb.py(),0.0);
    const double ATBT = (jaT+jbT).Mag2();

    double temp = sqrt((A+B)*(A+B)-(az+bz)*(az+bz)-
                         (jbT.Dot(jbT)-jaT.Dot(jaT))*(jbT.Dot(jbT)-jaT.Dot(jaT))/(jaT+jbT).Mag2());

    const double mybeta = (jbT.Dot(jbT)-jaT.Dot(jaT))/
        TMath::Sqrt(ATBT*((A+B)*(A+B)-(az+bz)*(az+bz)));
    double mygamma = 1./sqrt(1.-mybeta*mybeta);

    //gamma times MRstar                                                                                                                                                                              
    temp *= mygamma;
    // protect against nan
    if(TMath::IsNaN(temp)) temp = UnSet(Double_t);
    return temp;
}

template<typename T, typename U>
double cmg::DiObjectFactory<T, U>::mRT(T const& ja, U const& jb, const reco::Candidate& met) const{
    ///validated for 2011 variables
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

template<typename T, typename U>
std::pair<double,double> cmg::DiObjectFactory<T, U>::pZeta(T const& tau1, 
							   U const& tau2, 
							   const reco::Candidate& met) const{

  TVector3 tau1P=TVector3(tau1.p4().x(),tau1.p4().y(),tau1.p4().z());
  TVector3 tau2P=TVector3(tau2.p4().x(),tau2.p4().y(),tau2.p4().z());
  TVector3 tau1PT=TVector3(tau1.p4().x(),tau1.p4().y(),0.);
  TVector3 tau2PT=TVector3(tau2.p4().x(),tau2.p4().y(),0.);
  TVector3 metPT=TVector3(met.p4().x(),met.p4().y(),0.);
  TVector3 zetaAxis=(tau1PT.Unit() + tau2PT.Unit()).Unit();
  double pZetaVis=tau1PT*zetaAxis + tau2PT*zetaAxis;
  double pZetaMET=metPT*zetaAxis;
  /*   double tmp=pZetaVis+pZetaMET; */
  /*   return tmp-1.5*pZetaVis; */

  std::pair< double, double> result(pZetaVis, pZetaMET);
  return result;
}

template<typename T, typename U>
  double cmg::DiObjectFactory<T, U>::lp(T const& l1, cmg::DiObject<T,U>* const obj) const
{
  const reco::Candidate::LorentzVector & l1p4 = l1.p4();
  const reco::Candidate::LorentzVector & dip4 = obj->p4();

  return (l1p4.px() * dip4.px() + l1p4.py() * dip4.py()) / 
          (std::pow(dip4.pt(), 2)) ; 

}
#endif /*DIOBJECTFACTORY_H_*/
