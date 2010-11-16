#ifndef PHYSICSOBJECT_H_
#define PHYSICSOBJECT_H_

#include "AnalysisDataFormats/CMGTools/interface/AbstractPhysicsObject.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/Ptr.h"

#include <memory>
#include <vector>

namespace cmg{

/// A class for a PhysicsObjects, which have a pointer to a pat Candidate, made from a factory
template <class PATPtr >
class PhysicsObjectWithPtr : public AbstractPhysicsObject {
	
  public:
  
  typedef PATPtr value;
  
  PhysicsObjectWithPtr(const PATPtr& sourcePtr):
    AbstractPhysicsObject::AbstractPhysicsObject( reco::LeafCandidate(0, sourcePtr->polarP4()) ), sourcePtr_(sourcePtr)
    {
    }
  PhysicsObjectWithPtr():
    AbstractPhysicsObject::AbstractPhysicsObject()
    {
    }

  PATPtr const* sourcePtr() const{
    return &sourcePtr_; 
    //return 0;
  }
  
  private:
  PATPtr sourcePtr_;
  
};

}

#endif /*PHYSICSOBJECT_H_*/
