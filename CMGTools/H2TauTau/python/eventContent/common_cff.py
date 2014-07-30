import FWCore.ParameterSet.Config as cms
import copy



common = [
    'keep *',
    'keep patElectrons_*_*_*',
    'keep patMuons_*_*_*',
    'keep recoPFMETs_*_*_*',
    'keep cmgMETSignificance*_*_*_*',
    'keep double_*_*_*',
    'keep int_*_*_*',
    ]


commonDebug = copy.deepcopy( common ) 
commonDebug.extend( [
    'keep *_cmgPFJetForRecoil_*_*',
    'keep *_genWorZ_*_*'    
    ])

