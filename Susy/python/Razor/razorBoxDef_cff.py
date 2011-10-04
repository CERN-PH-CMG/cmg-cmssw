import FWCore.ParameterSet.Config as cms

razorBoxDef = cms.EDProducer(
    "RazorBoxDef",
    tightElectrons = cms.InputTag('susyElectron'),
    looseElectrons = cms.InputTag('susyElectron'),
    tightMuons = cms.InputTag('susyMuon'),
    looseMuons = cms.InputTag('susyMuon'),
    jets = cms.InputTag('cmgPFJetSel'),
    bjets = cms.InputTag('cmgPFJetSel')
)