import FWCore.ParameterSet.Config as cms
import copy

from CMGTools.H2TauTau.eventContent.common_cff import * 

diTauSpecific = [
    'drop *_cmgDiTau_*_*',
    'drop *_cmgDiTauPreSel_*_*',
    'drop *_cmgDiTauFullSel_*_*',
    'drop *_recoilCorMETDiTau_*_*',
    'drop *_cmgDiTauCorPreSel_*_*',
    'keep *_cmgDiTauCorSVFitPreSel_*_*',
    'keep *_cmgDiTauCorSVFitFullSel_*_*',
    ]

diTauSpecificDebug = copy.deepcopy( diTauSpecific)
diTauSpecificDebug.extend( [
    'keep *_cmgDiTau_*_*',
    'keep *_cmgDiTauPreSel_*_*',
    'keep *_cmgDiTauFullSel_*_*',
    'keep *_recoilCorMETDiTau_*_*',
    'keep *_cmgDiTauCorPreSel_*_*',    
    ] )

diTau = copy.deepcopy( common )
diTau.extend( diTauSpecific )

diTauDebug = copy.deepcopy( commonDebug )
diTauDebug.extend( diTauSpecificDebug )

