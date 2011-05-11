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

    DiObject():
      AbstractPhysicsObject(), alphaT_(UnSet(Double_t)) {
    }
    DiObject(const T& leg1, const U& leg2):
        AbstractPhysicsObject(reco::LeafCandidate(leg1.charge()+leg2.charge(),leg1.p4()+leg2.p4())),
        leg1_(leg1),
        leg2_(leg2),
        mT_(UnSet(Double_t)),
        alphaT_(UnSet(Double_t)),
        betaR_(UnSet(Double_t)),
        mR_(UnSet(Double_t)),
        mRP_(UnSet(Double_t)),
        mRT_(UnSet(Double_t)),
        lp_(UnSet(Double_t)){
    }
    DiObject(const DiObject<T,U>& other):
        AbstractPhysicsObject(other),
        leg1_(other.leg1()),
        leg2_(other.leg2()),
        mT_(other.mT_),
        alphaT_(other.alphaT_),
        betaR_(other.betaR_),
        mR_(other.mR_),
        mRP_(other.mRP_),
        mRT_(other.mRT_),
        lp_(other.lp_){
    }

    virtual ~DiObject(){}

    T leg1() const{
        return leg1_;
    }
    U leg2() const{
        return leg2_;
    }

    virtual double mT() const {
      return mT_;
    }

    ///The RA1 variable alpha_T
    Double_t const alphaT() const{
      return alphaT_;
    }
    ///The Razor boost
    Double_t betaR() const{
        return betaR_;
    }
    ///The Razor variable M_R
    Double_t mR() const{
        return mR_;
    }
    ///The Razor variable M_R'
    Double_t mRPrime() const{
        return mRP_;
    }
    ///The Razor variable M_T^R
    Double_t mRT() const{
        return mRT_;
    }
    ///The Razor variable R
    Double_t R() const{
        return mRT()/mR();
    }
    ///The lepton projection (LP) variable
    Double_t lp() const{
        return lp_;
    }

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

    Double_t mT_;
    Double_t alphaT_;
    Double_t betaR_; //the Razor boost
    Double_t mR_; //The Razor MR
    Double_t mRP_; //The Razor MR'
    Double_t mRT_; //The Razor M_T^R
    Double_t lp_; //The lepton projection (LP)

    friend class cmg::DiObjectFactory<T,U>;

};

} // namespace cmg

#endif /*DIOBJECT_H_*/
