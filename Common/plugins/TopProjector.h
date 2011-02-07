#ifndef CMGTOOLS_COMMON_TOPPROJECTOR_H_
#define CMGTOOLS_COMMON_TOPPROJECTOR_H_

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "PhysicsTools/PFCandProducer/plugins/TopProjector.h"

#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"

namespace cmg{
    typedef TopProjector<cmg::Muon,reco::PFCandidate> TPCMGMuonOnPFCandidate;
    typedef TopProjector<cmg::DiMuon,reco::PFCandidate> TPCMGDiMuonOnPFCandidate;
}

#endif /*CMGTOOLS_COMMON_TOPPROJECTOR_H_*/
