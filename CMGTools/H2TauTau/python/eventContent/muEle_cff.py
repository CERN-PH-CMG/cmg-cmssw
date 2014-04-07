import FWCore.ParameterSet.Config as cms
import copy

from CMGTools.H2TauTau.eventContent.common_cff import * 

muEleSpecific = [
    'drop cmgMuoncmgElectroncmgDiObjects_*_*_*',
    'keep *_cmgMuEleCorSVFitFullSel_*_*',
    ]

muEleSpecificDebug = copy.deepcopy( muEleSpecific)
muEleSpecificDebug.extend( [
    'keep *_cmgMuEle_*_*',
    'keep *_cmgMuElePreSel_*_*',
    'keep *_cmgMuEleFullSel_*_*',
    'keep *_recoilCorMETMuEle_*_*',
    'keep *_cmgMuEleCorPreSel_*_*',    
    ] )

muEle = copy.deepcopy( common )
muEle.extend( muEleSpecific )

muEleDebug = copy.deepcopy( commonDebug )
muEleDebug.extend( muEleSpecificDebug )

