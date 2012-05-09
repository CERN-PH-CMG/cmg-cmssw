import FWCore.ParameterSet.Config as cms
import copy

from CMGTools.H2TauTau.eventContent.common_cff import * 

diTauSpecific = [
    'drop cmgTaucmgTaucmgDiObjects_*_*_*',
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

