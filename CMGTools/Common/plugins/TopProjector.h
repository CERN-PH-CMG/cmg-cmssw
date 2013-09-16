#ifndef CMGTOOLS_COMMON_TOPPROJECTOR_H_
#define CMGTOOLS_COMMON_TOPPROJECTOR_H_

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "CommonTools/ParticleFlow/plugins/TopProjector.h"

#include "AnalysisDataFormats/CMGTools/interface/Muon.h"
#include "AnalysisDataFormats/CMGTools/interface/Tau.h"
#include "AnalysisDataFormats/CMGTools/interface/PFJet.h"
#include "AnalysisDataFormats/CMGTools/interface/CompoundTypes.h"

typedef TopProjector<cmg::Muon,reco::PFCandidate> TPCMGMuonOnPFCandidate;
typedef TopProjector<cmg::DiMuon,reco::PFCandidate> TPCMGDiMuonOnPFCandidate;
typedef TopProjector<cmg::Tau,reco::PFCandidate> TPCMGTauOnPFCandidate;
typedef TopProjector<cmg::DiTau,reco::PFCandidate> TPCMGDiTauOnPFCandidate;
typedef TopProjector<cmg::PFJet,cmg::PFJet> TPCMGPFJetOnPFJet;


#endif /*CMGTOOLS_COMMON_TOPPROJECTOR_H_*/
