#ifndef DIOBJECT_H_
#define DIOBJECT_H_

#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"

#include "AnalysisDataFormats/CMGTools/interface/UnSet.h"

#include <vector>

namespace cmg {

template <typename T, typename U> class DiObject;
template <typename T, typename U> class DiObjectFactory;

template< typename T, typename U >
class DiObject : public AbstractPhysicsObject{
  public:

    typedef T type1;
    typedef U type2;

    /// default constructor
    DiObject():
      AbstractPhysicsObject(), alphaT_(UnSet(Double_t)) {
    }

    /// constructs from leg1 and leg2  
    DiObject(const T& leg1, const U& leg2):
        AbstractPhysicsObject(reco::LeafCandidate(leg1.charge()+leg2.charge(),leg1.p4()+leg2.p4())),
      leg1_(leg1),
      leg2_(leg2),
      mT_(UnSet(Double_t)),
      alphaT_(UnSet(Double_t)),
      mR_(UnSet(Double_t)),
      mRT_(UnSet(Double_t)),
      lp_(UnSet(Double_t)), 
      pZetaMET_(UnSet(Double_t)),
      pZetaVis_(UnSet(Double_t)),
      mTLeg1_(UnSet(Double_t)),
      mTLeg2_(UnSet(Double_t)),
      massSVFit_(UnSet(Double_t))
	{
    }
    
    /// copy constructor
    DiObject(const DiObject<T,U>& other):
      AbstractPhysicsObject(other),
      leg1_(other.leg1()),
      leg2_(other.leg2()),
      met_(other.met()),
      mT_(other.mT_),
      alphaT_(other.alphaT_),
      mR_(other.mR_),
      mRT_(other.mRT_),
      lp_(other.lp_), 
      pZetaMET_(other.pZetaMET_), 
      pZetaVis_(other.pZetaVis_), 
      mTLeg1_(other.mTLeg1_), 
      mTLeg2_(other.mTLeg2_),
      massSVFit_(other.massSVFit_) {
    }

    virtual ~DiObject(){}

    const T& leg1() const{ return leg1_; }

    const U& leg2() const{ return leg2_; }

    const reco::LeafCandidate& met() const { return met_;}

    virtual double mT() const { return mT_; }

    ///The RA1 variable alpha_T
    Double_t const alphaT() const { return alphaT_; }

    ///The Razor variable M_R
    Double_t mR() const { return mR_; }

    ///The Razor variable M_T^R
    Double_t mRT() const { return mRT_; }

    ///The Razor variable R
    Double_t R() const { return (mR() > 0) ? mRT()/mR() : UnSet(Double_t); }

    ///The Razor variable Rsq
    Double_t Rsq() const { return (mR() > 0) ? (mRT()/mR())*(mRT()/mR()) : UnSet(Double_t); }
 
    ///The lepton projection (LP) variable
    Double_t lp() const { return lp_; }
    
    ///The pzeta variable, for H->tau tau
    double pZeta() const {return pZetaMET_ + pZetaVis_;}

    ///The pzeta-based discriminator variable, for H->tau tau
    double pZetaDisc() const { return pZetaVis_+pZetaMET_ - 1.5*pZetaVis_;}

    ///The pzeta-MET variable, for H->tau tau
    double pZetaMET() const { return pZetaMET_;}

    ///The pzeta-Vis variable, for H->tau tau
    double pZetaVis() const { return pZetaVis_;}

    ///transverse mass M_T between the first leg and the MET
    double mTLeg1() const { return mTLeg1_; }

    ///transverse mass M_T between the second leg and the MET 
    double mTLeg2() const { return mTLeg2_; }

    ///mass from SVFit
    double massSVFit() const { return massSVFit_; }

    ///set mass from SVFit 
    void setMassSVFit(double mass) { massSVFit_ = mass; }
    
    virtual void accept(AbstractPhysicsObjectVisitor* v) const{
      v->visit(this);
      v->visit(&leg1_);
      v->visit(&leg2_);
    }

   ///Overides the methods reco::Candidate so that it can be used with the TopProjector
   virtual reco::Candidate::size_type numberOfSourceCandidatePtrs() const{
    return leg1().numberOfSourceCandidatePtrs() + leg2().numberOfSourceCandidatePtrs();
   }
   ///Overides the methods reco::Candidate so that it can be used with the TopProjector
   virtual reco::CandidatePtr sourceCandidatePtr( reco::Candidate::size_type i ) const{
    reco::Candidate::size_type leg1Size = leg1().numberOfSourceCandidatePtrs();
    reco::Candidate::size_type leg2Size = leg2().numberOfSourceCandidatePtrs();
    if(i < leg1Size){
     return leg1().sourceCandidatePtr(i);
    }else if(i < (leg1Size + leg2Size) ){
     return leg2().sourceCandidatePtr(i-leg1Size);
    }else{
     return reco::Candidate::sourceCandidatePtr(i);
    }
   }

   ///Overrides the methods for read only access to daughters as in CompositeCandidate
   virtual reco::Candidate::size_type numberOfDaughters() const{
        return 2;
   }
   /// return daughter at a given position, i = 0, ... numberOfDaughters() - 1 (read only mode)
   virtual const reco::Candidate * daughter( reco::Candidate::size_type i ) const{
        const reco::Candidate* result = 0;
        switch(i){
         case 0:
            result = &leg1_;
            break;
         case 1:
            result = &leg2_;
            break;
         default:
            result = 0;
        };
        return result;
   }

  private:

    T leg1_;
    U leg2_;
    reco::LeafCandidate met_;
    
    /// M_T between the two legs (one of them can be the MET)
    Double_t mT_;
    Double_t alphaT_;

    /// The Razor MR
    Double_t mR_; 

    /// The Razor M_T^R
    Double_t mRT_;
    
    /// The lepton projection (LP)
    Double_t lp_; 

    /// the pzeta-MET variable (for H->tau tau)
    double   pZetaMET_; 

    /// the pzeta-vis variable (for H->tau tau)
    double   pZetaVis_;

    /// M_T between the first leg and the MET
    /// filled when the MET collection is specified
    double   mTLeg1_;

    /// M_T between the second leg and the MET
    /// filled when the MET collection is specified 
    double   mTLeg2_;

    /// SVFit mass
    double   massSVFit_;

    friend class cmg::DiObjectFactory<T,U>;

};

} // namespace cmg

#endif /*DIOBJECT_H_*/
