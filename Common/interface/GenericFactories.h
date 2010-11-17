#ifndef GENERICFACTORIES_H_
#define GENERICFACTORIES_H_

#include "AnalysisDataFormats/CMGTools/interface/GenericTypes.h"
#include "CMGTools/Common/interface/PhysicsObjectFactory.h"

namespace cmg{
    
    typedef PhysicsObjectFactory< PFCandPtr > PFCandFactory;
    
}

#endif /*GENERICFACTORIES_H_*/
