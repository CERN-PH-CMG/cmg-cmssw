#ifndef GENERICFACTORIES_H_
#define GENERICFACTORIES_H_

#include "AnalysisDataFormats/CMGTools/interface/GenericTypes.h"
#include "CMGTools/Common/interface/PhysicsObjectFactory.h"

namespace cmg{
    
    typedef PhysicsObjectFactory< PFCandPtr > PFCandFactory;
    typedef PhysicsObjectFactory< pat::TauPtr > TauFactory;
    
    typedef PhysicsObjectFactory< GenParticlePtr > GenParticleFactory;
    typedef PhysicsObjectFactory< GenJetPtr > GenJetFactory;
    
}

#endif /*GENERICFACTORIES_H_*/
