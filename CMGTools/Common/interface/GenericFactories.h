#ifndef GENERICFACTORIES_H_
#define GENERICFACTORIES_H_

#include "AnalysisDataFormats/CMGTools/interface/GenericTypes.h"
#include "CMGTools/Common/interface/PhysicsObjectFactory.h"

namespace cmg{

  //these factories will produce PhysicsObjectWithPtr<T> 
  //where T is for example a PFCandidatePtr. 
  //in which case the concrete type of the produced objects is:
  // PhysicsObjectWithPtr<PFCandidatePtr>
  typedef PhysicsObjectFactory< PFCandidatePtr > PFCandidateFactory;
  typedef PhysicsObjectFactory< GenParticlePtr > GenParticleFactory;
  typedef PhysicsObjectFactory< GenJetPtr > GenJetFactory;
    
}

#endif /*GENERICFACTORIES_H_*/
