#ifndef DIOBJECTUPDATEFACTORY_H_
#define DIOBJECTUPDATEFACTORY_H_

#include "CMGTools/H2TauTau/interface/DiTauObjectFactory.h"
#include "TLorentzVector.h"
#include "DataFormats/Math/interface/deltaR.h"

namespace cmg{

  // T is for example a di-object<U, S>
  template< typename T>
  class DiObjectUpdateFactory : public cmg::Factory< T > {

  public:
    
    DiObjectUpdateFactory(const edm::ParameterSet& ps):
      // diObjectFactory_( ps ),
      diObjectLabel_     (ps.getParameter<edm::InputTag>("diObjectCollection")),
      //metLabel_        (ps.getParameter<edm::InputTag>("metCollection")),
      nSigma_            (ps.getParameter<double>("nSigma")),
      uncertainty_       (ps.getParameter<double>("uncertainty")),
      shift1ProngNoPi0_  (ps.getParameter<double>("shift1ProngNoPi0")),
      shift1Prong1Pi0_   (ps.getParameter<double>("shift1Prong1Pi0")),
      ptDependence1Pi0_  (ps.getParameter<double>("ptDependence1Pi0")),
      shift3Prong_       (ps.getParameter<double>("shift3Prong")),
      ptDependence3Prong_(ps.getParameter<double>("ptDependence3Prong")),
      shiftMet_          (ps.getParameter<bool>("shiftMet")),
      shiftTaus_         (ps.getParameter<bool>("shiftTaus"))
      {}

    //need to override from Factory to insert "typename"
    typedef typename cmg::Factory< T >::event_ptr event_ptr;
    virtual event_ptr create(const edm::Event&, const edm::EventSetup&);
    
  private:

    // const DiObjectFactory< typename T::type1, typename T::type2 > diObjectFactory_;
    const edm::InputTag diObjectLabel_;
    // const edm::InputTag metLabel_;
    double nSigma_;
    double uncertainty_; 
    double shift1ProngNoPi0_; 
    double shift1Prong1Pi0_; 
    double ptDependence1Pi0_; 
    double shift3Prong_; 
    double ptDependence3Prong_; 
    bool   shiftMet_ ;
    bool   shiftTaus_ ;
  };
  
} // namespace cmg


template< typename T >
typename cmg::DiObjectUpdateFactory<T>::event_ptr cmg::DiObjectUpdateFactory<T>::create(const edm::Event& iEvent, const edm::EventSetup&){
    
  typedef std::vector< T > collection;
  
  edm::Handle<collection> diObjects;
  iEvent.getByLabel(diObjectLabel_,diObjects);

  edm::Handle< std::vector<reco::GenParticle> > genparticles;
  iEvent.getByLabel("genParticlesPruned",genparticles);
   
  typename cmg::DiObjectUpdateFactory<T>::event_ptr result(new collection);
  
  unsigned index = 0;
  for(typename collection::const_iterator it = diObjects->begin(); it != diObjects->end(); ++it, ++index ){
    const T& diObject = *it;
    // assert( index < metCands->size() );
    typename T::type1 leg1(diObject.leg1());
    typename T::type2 leg2(diObject.leg2());
    // reco::LeafCandidate met = reco::LeafCandidate( metCands->at(index) );
    reco::LeafCandidate met = diObject.met();

    float shift1 = 0.;
    float shift2 = 0.;
    if(typeid(typename T::type1)==typeid(cmg::Tau))
    {
     	shift1 = (nSigma_ * uncertainty_);
      const cmg::Tau& tau1 = dynamic_cast<const cmg::Tau&>(diObject.leg1());
     	if((tau1.decayMode()==0)&&(shift1ProngNoPi0_!=0))
     	    shift1+=shift1ProngNoPi0_;
        //Also allow decay mode 2 according to synchronisation twiki
     	if((tau1.decayMode()==1 || tau1.decayMode()==2)&&(shift1Prong1Pi0_!=0))
     	    shift1+=shift1Prong1Pi0_+ptDependence1Pi0_*TMath::Min(TMath::Max(diObject.leg1().pt()-45.,0.),10.);
     	if((tau1.decayMode()==10)&&(shift3Prong_!=0))
     	    shift1+=shift3Prong_+ptDependence3Prong_*TMath::Min(TMath::Max(diObject.leg1().pt()-32.,0.),18.);
    }

    if(typeid(typename T::type2)==typeid(cmg::Tau))
    {
     	shift2 = (nSigma_ * uncertainty_);
      const cmg::Tau& tau2 = dynamic_cast<const cmg::Tau&>(diObject.leg2());
     	if((tau2.decayMode()==0)&&(shift1ProngNoPi0_!=0))
     	    shift2+=shift1ProngNoPi0_;
        //Also allow decay mode 2 according to synchronisation twiki
     	if((tau2.decayMode()==1 || tau2.decayMode()==2)&&(shift1Prong1Pi0_!=0))
     	    shift2+=shift1Prong1Pi0_+ptDependence1Pi0_*TMath::Min(TMath::Max(diObject.leg2().pt()-45.,0.),10.);
     	if((tau2.decayMode()==10)&&(shift3Prong_!=0))
     	    shift2+=shift3Prong_+ptDependence3Prong_*TMath::Min(TMath::Max(diObject.leg2().pt()-32.,0.),18.);
    }

    // the tauES shift must be applied to *real* taus only
    bool l1genMatched = false ;
    bool l2genMatched = false ;
    
    for ( size_t i=0; i< genparticles->size(); ++i) 
    {
      const reco::GenParticle &p = (*genparticles)[i];
      int id       = p.pdgId()           ;
      int status   = p.status()          ;
      int motherId = 0                   ; 
      if ( p.numberOfMothers()>0 ) {
        //std::cout << __LINE__ << "]\tnum of mothers " << p.numberOfMothers() << "\tmy mom " << p.mother()->pdgId() << std::endl ;
        motherId = p.mother()->pdgId() ;
      }
      // PDG Id: e 11, mu 13, tau 15, Z 23, h 25, H 35, A 35  
      if ( status == 3 && abs(id) == 15 && (motherId == 23 || motherId == 25 || motherId == 35 || motherId == 36 )){
        // match leg 1
        if(typeid(typename T::type1)==typeid(cmg::Tau)){
          const cmg::Tau& tau1 = dynamic_cast<const cmg::Tau&>(diObject.leg1());
          if (deltaR(tau1.eta(),tau1.phi(),p.eta(),p.phi())<0.3) {
            l1genMatched = true ;
            //std::cout << __LINE__ << "]\tleg1 matched to a tau" << std::endl ;
          }
        }
        // match leg 2
        if(typeid(typename T::type2)==typeid(cmg::Tau)){
          const cmg::Tau& tau2 = dynamic_cast<const cmg::Tau&>(diObject.leg2());
          if (deltaR(tau2.eta(),tau2.phi(),p.eta(),p.phi())<0.3) {
            l2genMatched = true ;
            //std::cout << __LINE__ << "]\tleg2 matched to a tau" << std::endl ;
          }
        }
      }
    }

    reco::Candidate::LorentzVector leg1Vec = diObject.leg1().p4();
    reco::Candidate::LorentzVector leg2Vec = diObject.leg2().p4();
    reco::Candidate::LorentzVector metVec = met.p4();

    float dpx = 0.;
    float dpy = 0.;

    // if genMatched compute the transverse momentum variation 
    dpx = l1genMatched * leg1Vec.px() * shift1 + l2genMatched * leg2Vec.px() * shift2;
    dpy = l1genMatched * leg1Vec.py() * shift1 + l2genMatched * leg2Vec.py() * shift2;

    // if genMatched apply the shift 
    if (l1genMatched) leg1Vec *= (1. + shift1);
    if (l2genMatched) leg2Vec *= (1. + shift2);

    // apply the tranverse momentum correction to the MET 
    math::XYZTLorentzVector deltaTauP4(dpx,dpy,0,0);
    math::XYZTLorentzVector scaledmetP4 = metVec - deltaTauP4;

    TLorentzVector metVecNew;
    metVecNew.SetPtEtaPhiM(scaledmetP4.Pt(),scaledmetP4.Eta(),scaledmetP4.Phi(),0.);
        
    if (shiftTaus_ ){ leg1.setP4(leg1Vec); }
    if (shiftTaus_ ){ leg2.setP4(leg2Vec); }
    if (shiftMet_  ){ met.setP4(reco::Candidate::LorentzVector(metVecNew.Px(),metVecNew.Py(),metVecNew.Pz(),metVecNew.E())); }

    // T diObjectNew = T(leg1,leg2);
    result->push_back(T(diObject));

    // diObjectFactory_.set( std::make_pair(leg1, leg2), met, & result->back() );
    DiTauObjectFactory< typename T::type1, typename T::type2 >::set( std::make_pair(leg1, leg2), met, &result->back() );
  }
  
  return result;
}


#endif /*DIOBJECTUPDATEFACTORY_H_*/
