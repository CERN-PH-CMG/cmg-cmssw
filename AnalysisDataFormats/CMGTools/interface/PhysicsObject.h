#ifndef PHYSICSOBJECT_H_
#define PHYSICSOBJECT_H_

#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Common/interface/Ptr.h"

#include "FWCore/Utilities/interface/Exception.h"

#include <memory>
#include <vector>

namespace cmg {

template <class PATPtr> class PhysicsObjectWithPtr;
template <class PATPtr> class PhysicsObjectFactory;
template <class T> class Factory;

/// A class for a PhysicsObjects, which have a pointer to a PAT
/// Candidate, made from a factory.
template <class PATPtr>
class PhysicsObjectWithPtr : public AbstractPhysicsObject {

public:

  typedef PATPtr value;

  PhysicsObjectWithPtr(const PATPtr& sourcePtr):
    AbstractPhysicsObject::AbstractPhysicsObject(*sourcePtr),
    sourcePtr_(sourcePtr)
    {
    }
  PhysicsObjectWithPtr():
    AbstractPhysicsObject::AbstractPhysicsObject()
    {
    }

  PATPtr const* sourcePtr() const{
    return &sourcePtr_;
  }

  /// Overides the methods reco::Candidate so that it can be used with
  /// the TopProjector.
  virtual reco::Candidate::size_type numberOfSourceCandidatePtrs() const{
    reco::Candidate::size_type result = 0;
    if ( (sourcePtr_.isNonnull() && sourcePtr_.isAvailable()) || (sourceCandidatePtr_.isNonnull() && sourceCandidatePtr_.isAvailable()) ){
         result = 1;
    }
    return result;
  }

  /// Overides the methods reco::Candidate so that it can be used with
  /// the TopProjector.
  virtual reco::CandidatePtr sourceCandidatePtr(reco::Candidate::size_type i) const{
    if(i >= numberOfSourceCandidatePtrs()){
        throw cms::Exception("PhysicsObjectWithPtr") << " sourceCandidatePtr index too large: " << i << std::endl; 
    }
    if( sourceCandidatePtr_.isNull() ) {
      return reco::CandidatePtr(sourcePtr_.id(), sourcePtr_.get(), sourcePtr_.key());
    }
    else 
      return sourceCandidatePtr_;
  }

  virtual void setSourceCandidatePtr( const reco::CandidatePtr& ptr ) {
    sourceCandidatePtr_ = ptr;
  };


private:
  PATPtr       sourcePtr_;
  reco::CandidatePtr sourceCandidatePtr_; 

  friend class cmg::PhysicsObjectFactory<PATPtr>;
  friend class cmg::Factory<cmg::PhysicsObjectWithPtr<PATPtr> >;

};

}

#endif /*PHYSICSOBJECT_H_*/
