import FWCore.ParameterSet.Config as cms
import copy



common = [
    'keep *',
    'drop *_*AK7*_*_*',
    'drop *_*AK5LC*_*_*',
    'drop *_*DiTau*_*_*',
    'drop *_*TauMu*_*_*',
    'drop *_*TauE*_*_*',
    'drop *_*MuE*_*_*',
    'drop pat*_*_*_*',
    'drop cmgPFBaseJet*_*_*_*',
    'drop *_cmgPFJetForRecoil_*_*',
    'drop *_genWorZ_*_*',
    'keep *_pfMetSignificance*_*_*',
    'keep double_*_*_*',
    'keep int_*_*_*',
    ]


commonDebug = copy.deepcopy( common ) 
commonDebug.extend( [
    'keep *_cmgPFJetForRecoil_*_*',
    'keep *_genWorZ_*_*'    
    ])

