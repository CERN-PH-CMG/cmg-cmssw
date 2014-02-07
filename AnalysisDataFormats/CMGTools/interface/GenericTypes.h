#ifndef GENERICTYPES_H_
#define GENERICTYPES_H_

#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"

#include "AnalysisDataFormats/CMGTools/interface/PatTypes.h"
#include "AnalysisDataFormats/CMGTools/interface/PhysicsObject.h"

namespace cmg{
    
    typedef edm::Ptr< reco::PFCandidate > PFCandidatePtr;
    typedef PhysicsObjectWithPtr< PFCandidatePtr > PFCandidate;
    
    typedef edm::Ptr< reco::GenParticle > GenParticlePtr;
    typedef PhysicsObjectWithPtr< GenParticlePtr > GenParticle;
    
    typedef edm::Ptr< reco::GenJet > GenJetPtr;
    typedef PhysicsObjectWithPtr< GenJetPtr > GenJet;
    
}

#endif /*GENERICTYPES_H_*/
