#ifndef DIOBJECTUPDATEFACTORY_H_
#define DIOBJECTUPDATEFACTORY_H_

#include "CMGTools/Common/interface/DiObjectFactory.h"
#include "TLorentzVector.h"

namespace cmg{

  // T is for example a di-object<U, S>
  template< typename T>
  class DiObjectUpdateFactory : public cmg::Factory< T > {

  public:
    
    DiObjectUpdateFactory(const edm::ParameterSet& ps):
      diObjectFactory_( ps ),
      diObjectLabel_(ps.getParameter<edm::InputTag>("diObjectCollection")),
      metLabel_(ps.getParameter<edm::InputTag>("metCollection")),
      nSigma_(ps.getParameter<double>("nSigma")),
      uncertainty_(ps.getParameter<double>("uncertainty")),
      shift1ProngNoPi0_(ps.getParameter<double>("shift1ProngNoPi0")),
      shift1Prong1Pi0_(ps.getParameter<double>("shift1Prong1Pi0")),
      ptDependence1Pi0_(ps.getParameter<double>("ptDependence1Pi0")),
      shift3Prong_(ps.getParameter<double>("shift3Prong")),
      ptDependence3Prong_(ps.getParameter<double>("ptDependence3Prong"))
      {}

    //need to override from Factory to insert "typename"
    typedef typename cmg::Factory< T >::event_ptr event_ptr;
    virtual event_ptr create(const edm::Event&, const edm::EventSetup&);
    
  private:

    const DiObjectFactory< typename T::type1, typename T::type2 > diObjectFactory_;
    const edm::InputTag diObjectLabel_;
    const edm::InputTag metLabel_;
    double nSigma_;
    double uncertainty_; 
    double shift1ProngNoPi0_; 
    double shift1Prong1Pi0_; 
    double ptDependence1Pi0_; 
    double shift3Prong_; 
    double ptDependence3Prong_; 
  };
  
} // namespace cmg


template< typename T >
typename cmg::DiObjectUpdateFactory<T>::event_ptr cmg::DiObjectUpdateFactory<T>::create(const edm::Event& iEvent, const edm::EventSetup&){
    
  typedef std::vector< T > collection;
  typedef edm::View<reco::LeafCandidate> met_collection;
  
  edm::Handle<collection> diObjects;
  iEvent.getByLabel(diObjectLabel_,diObjects);
   
  edm::Handle<met_collection> metCands;
  iEvent.getByLabel(metLabel_,metCands);
  
/*   if(metCands->empty() ) { */
/*     throw cms::Exception("MET collection is empty."); */
/*   } */
  if(metCands->size() != diObjects->size() ) {
    throw cms::Exception("MET and Di-object collections should have the same size.");
  }

  typename cmg::DiObjectUpdateFactory<T>::event_ptr result(new collection);
  
  unsigned index = 0;
  for(typename collection::const_iterator it = diObjects->begin(); it != diObjects->end(); ++it, ++index ){
    const T& diObject = *it;
    assert( index < metCands->size() );
    typename T::type1 leg1(diObject.leg1());
    typename T::type2 leg2(diObject.leg2());
    reco::LeafCandidate met = reco::LeafCandidate( metCands->at(index) );

    float shift1 = 0;
    float shift2 = 0;
    if(typeid(typename T::type1)==typeid(cmg::Tau))
    {
     	shift1 = (nSigma_ * uncertainty_);
     	if((dynamic_cast<const cmg::Tau&>(diObject.leg1()).decayMode()==0)&&(shift1ProngNoPi0_!=0))
     	    shift1+=shift1ProngNoPi0_;
     	if((dynamic_cast<const cmg::Tau&>(diObject.leg1()).decayMode()==1)&&(shift1Prong1Pi0_!=0))
     	    shift1+=shift1Prong1Pi0_+ptDependence1Pi0_*TMath::Min(TMath::Max(diObject.leg1().pt()-45.,0.),10.);
     	if((dynamic_cast<const cmg::Tau&>(diObject.leg1()).decayMode()==10)&&(shift3Prong_!=0))
     	    shift1+=shift3Prong_+ptDependence3Prong_*TMath::Min(TMath::Max(diObject.leg1().pt()-32.,0.),18.);
    }

    if(typeid(typename T::type2)==typeid(cmg::Tau))
    {
     	shift2 = (nSigma_ * uncertainty_);
     	if((dynamic_cast<const cmg::Tau&>(diObject.leg2()).decayMode()==0)&&(shift1ProngNoPi0_!=0))
     	    shift2+=shift1ProngNoPi0_;
     	if((dynamic_cast<const cmg::Tau&>(diObject.leg2()).decayMode()==1)&&(shift1Prong1Pi0_!=0))
     	    shift2+=shift1Prong1Pi0_+ptDependence1Pi0_*TMath::Min(TMath::Max(diObject.leg2().pt()-45.,0.),10.);
     	if((dynamic_cast<const cmg::Tau&>(diObject.leg2()).decayMode()==10)&&(shift3Prong_!=0))
     	    shift2+=shift3Prong_+ptDependence3Prong_*TMath::Min(TMath::Max(diObject.leg2().pt()-32.,0.),18.);
    }

    reco::Candidate::LorentzVector leg1Vec = diObject.leg1().p4();
    reco::Candidate::LorentzVector leg2Vec = diObject.leg2().p4();
    reco::Candidate::LorentzVector metVec = metCands->at(index).p4();
    metVec -= (leg1Vec * shift1);
    metVec -= (leg2Vec * shift2);
    TLorentzVector leg1VecNew, leg2VecNew, metVecNew;
    leg1VecNew.SetPtEtaPhiM(leg1Vec.Pt()*(1. + shift1),leg1Vec.Eta(),leg1Vec.Phi(),leg1Vec.M());
    leg2VecNew.SetPtEtaPhiM(leg2Vec.Pt()*(1. + shift2),leg2Vec.Eta(),leg2Vec.Phi(),leg2Vec.M());
    metVecNew.SetPtEtaPhiM(metVec.Pt(),metVec.Eta(),metVec.Phi(),0);

    leg1.setP4(reco::Candidate::LorentzVector(leg1VecNew.Px(),leg1VecNew.Py(),leg1VecNew.Pz(),leg1VecNew.E()));
    leg2.setP4(reco::Candidate::LorentzVector(leg2VecNew.Px(),leg2VecNew.Py(),leg2VecNew.Pz(),leg2VecNew.E()));
    met.setP4(reco::Candidate::LorentzVector(metVecNew.Px(),metVecNew.Py(),metVecNew.Pz(),metVecNew.E()));

    T diObjectNew = T(leg1,leg2);
    result->push_back(diObjectNew);
    diObjectFactory_.set( std::make_pair(leg1, leg2), met, & result->back() );
  }
  
  return result;
}


#endif /*DIOBJECTUPDATEFACTORY_H_*/
