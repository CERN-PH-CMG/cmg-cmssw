import FWCore.ParameterSet.Config as cms
import copy

from CMGTools.H2TauTau.eventContent.common_cff import * 

tauMuSpecific = [
    # 'drop cmgTaucmgMuoncmgDiObjects_*_*_*',
    ]

tauMuSpecificDebug = copy.deepcopy( tauMuSpecific)
tauMuSpecificDebug.extend( [
    # 'drop *_cmgTauMu_*_*',
 
    ] )

tauMu = copy.deepcopy( common )
tauMu.extend( tauMuSpecific )

tauMuDebug = copy.deepcopy( commonDebug )
tauMuDebug.extend( tauMuSpecificDebug )

