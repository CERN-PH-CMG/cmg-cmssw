#ifndef DIOBJECTPAIR_H_
#define DIOBJECTPAIR_H_

#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"
#include "AnalysisDataFormats/CMGTools/interface/DiObject.h"

#include "AnalysisDataFormats/CMGTools/interface/UnSet.h"

#include <vector>

namespace cmg {

  template <typename T, typename U,typename V, typename W> class DiObjectPair;
  template <typename T,typename U,  typename V,typename W > class DiObjectPairFactory;
  template< typename T, typename U ,typename V,typename W>
  class DiObjectPair : public AbstractPhysicsObject{
  public:



    /// default constructor
    DiObjectPair():
      AbstractPhysicsObject() {
    }

    /// constructs from leg1 and leg2  
      DiObjectPair(const DiObject<T,U>& leg1, const DiObject<V,W>& leg2):
      AbstractPhysicsObject(reco::LeafCandidate(leg1.charge()+leg2.charge(),leg1.p4()+leg2.p4())),
      first_(leg1),
      second_(leg2)
	{
    }
    
    /// copy constructor
      DiObjectPair(const DiObjectPair<T,U,V,W>& other):
	AbstractPhysicsObject(other),
	first_(other.first()),
	second_(other.second()) 
	  {

	
	  }

    virtual ~DiObjectPair(){}

    const DiObject<T,U>& first() const{ return first_; }
    const DiObject<V,W>& second() const{ return second_; }

    
    virtual void accept(AbstractPhysicsObjectVisitor* v) const{
      v->visit(this);
      v->visit(&first_);
      v->visit(&second_);
    }

   ///Overides the methods reco::Candidate so that it can be used with the TopProjector
   virtual reco::Candidate::size_type numberOfSourceCandidatePtrs() const{
    return first().numberOfSourceCandidatePtrs() + second().numberOfSourceCandidatePtrs();
   }
   ///Overides the methods reco::Candidate so that it can be used with the TopProjector
   virtual reco::CandidatePtr sourceCandidatePtr( reco::Candidate::size_type i ) const{
     reco::Candidate::size_type leg1Size = first().numberOfSourceCandidatePtrs();
    reco::Candidate::size_type leg2Size = second().numberOfSourceCandidatePtrs();
    if(i < leg1Size){
     return first().sourceCandidatePtr(i);
    }else if(i < (leg1Size + leg2Size) ){
     return second().sourceCandidatePtr(i-leg1Size);
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
            result = &first_;
            break;
         case 1:
            result = &second_;
            break;
         default:
            result = 0;
        };
        return result;
   }

  private:

   DiObject<T,U>  first_;
   DiObject<V,W> second_;



   
   friend class cmg::DiObjectPairFactory<T,U,V,W >;
};

} // namespace cmg

#endif /*DIOBJECTPAIR_H_*/
