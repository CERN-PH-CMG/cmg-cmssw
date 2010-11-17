#ifndef GENERICTYPES_H_
#define GENERICTYPES_H_

#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "AnalysisDataFormats/CMGTools/interface/PatTypes.h"
#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"

namespace cmg{
    
    typedef edm::Ptr< reco::PFCandidate > PFCandPtr;
    typedef PhysicsObjectWithPtr< PFCandPtr > PFCand;
    
    typedef PhysicsObjectWithPtr< pat::TauPtr > Tau;
    
}

#endif /*GENERICTYPES_H_*/
