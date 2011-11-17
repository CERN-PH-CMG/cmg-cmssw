import FWCore.ParameterSet.Config as cms
import copy

from CMGTools.H2TauTau.eventContent.common_cff import * 

tauMuSpecific = [
    'drop *_cmgTauMu_*_*',
    'drop *_cmgTauMuPreSel_*_*',
    'drop *_cmgTauMuFullSel_*_*',
    'drop *_recoilCorMETTauMu_*_*',
    'drop *_cmgTauMuCorPreSel_*_*',
    'keep *_cmgTauMuCorSVFitPreSel_*_*',
    'keep *_cmgTauMuCorSVFitFullSel_*_*',
    ]

tauMuSpecificDebug = copy.deepcopy( tauMuSpecific)
tauMuSpecificDebug.extend( [
    'keep *_cmgTauMu_*_*',
    'keep *_cmgTauMuPreSel_*_*',
    'keep *_cmgTauMuFullSel_*_*',
    'keep *_recoilCorMETTauMu_*_*',
    'keep *_cmgTauMuCorPreSel_*_*',    
    ] )

tauMu = copy.deepcopy( common )
tauMu.extend( tauMuSpecific )

tauMuDebug = copy.deepcopy( commonDebug )
tauMuDebug.extend( tauMuSpecificDebug )

