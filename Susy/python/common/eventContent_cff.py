import FWCore.ParameterSet.Config as cms

leptons = [
    'keep *_susyMuon_*_*',
    'keep *_susyElectron_*_*'
    ]

trigger = [
    'keep *_TriggerResults_*_*',
    ]

jetId = [
    'keep cmgPFJets_cmgPFJet*Failed_*_*'
    ]

eventContent = leptons + trigger + jetId 
