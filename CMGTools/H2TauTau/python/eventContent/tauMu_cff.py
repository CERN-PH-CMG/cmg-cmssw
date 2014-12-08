import FWCore.ParameterSet.Config as cms
import copy

from CMGTools.H2TauTau.eventContent.common_cff import * 

tauMuSpecific = [
    'keep double_fixedGridRho*_*_*',
    'keep edmTriggerResults_TriggerResults_*_*',
    'keep patPackedTriggerPrescales_*_*_*',
    'keep patElectrons_*_*_*',
    'keep patJets_slimmedJets_*_*',
    'keep patMETs_*_*_*',
    'keep patMuons_*_*_*',
    'keep patPacked*_*_*_*',
    'keep patTaus_*_*_*',
    'keep patTrigger*_*_*_*',
    'keep recoVertexs_*_*_*',
    'keep cmgMETSignificances_*_*_*',
    'keep patCompositeCandidates_*_*_*',
    'keep patJets_patJetsAK4PF_*_*',
    'keep PileupSummaryInfos_*_*_*',
    'keep recoGenParticles_prunedGenParticles_*_*',
    'keep recoGsfElectronCores_*_*_*',
    'keep recoSuperClusters_*_*_*',
    'keep recoGenJets_slimmedGenJets_*_*'
    ]

tauMuSpecificDebug = copy.deepcopy( tauMuSpecific)
tauMuSpecificDebug.extend( [
    # 'drop *_cmgTauMu_*_*',
 
    ] )

tauMu = copy.deepcopy( common )
tauMu.extend( tauMuSpecific )

tauMuDebug = copy.deepcopy( commonDebug )
tauMuDebug.extend( tauMuSpecificDebug )

