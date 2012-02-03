import FWCore.ParameterSet.Config as cms
import copy

from CMGTools.H2TauTau.eventContent.common_cff import * 

tauEleSpecific = [
    'drop *_cmgTauEle_*_*',
    'drop *_cmgTauElePreSel_*_*',
    'drop *_cmgTauEleFullSel_*_*',
    'drop *_recoilCorMETTauEle_*_*',
    'drop *_cmgTauEleCorPreSel_*_*',
    'keep *_cmgTauEleCorSVFitPreSel_*_*',
    'keep *_cmgTauEleCorSVFitFullSel_*_*',
    ]

tauEleSpecificDebug = copy.deepcopy( tauEleSpecific)
tauEleSpecificDebug.extend( [
    'keep *_cmgTauEle_*_*',
    'keep *_cmgTauElePreSel_*_*',
    'keep *_cmgTauEleFullSel_*_*',
    'keep *_recoilCorMETTauEle_*_*',
    'keep *_cmgTauEleCorPreSel_*_*',    
    ] )

tauEle = copy.deepcopy( common )
tauEle.extend( tauEleSpecific )

tauEleDebug = copy.deepcopy( commonDebug )
tauEleDebug.extend( tauEleSpecificDebug )

