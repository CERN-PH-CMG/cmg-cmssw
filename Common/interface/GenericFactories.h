#ifndef GENERICFACTORIES_H_
#define GENERICFACTORIES_H_

#include "AnalysisDataFormats/CMGTools/interface/GenericTypes.h"
#include "CMGTools/Common/interface/PhysicsObjectFactory.h"

namespace cmg{
    
    typedef PhysicsObjectFactory< PFCandidatePtr > PFCandidateFactory;
    typedef PhysicsObjectFactory< GenParticlePtr > GenParticleFactory;
    typedef PhysicsObjectFactory< GenJetPtr > GenJetFactory;
    
}

#endif /*GENERICFACTORIES_H_*/
