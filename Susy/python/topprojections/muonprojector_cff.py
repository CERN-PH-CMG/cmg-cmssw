import FWCore.ParameterSet.Config as cms

muonOnMuon = cms.EDProducer(
    "TPMuonOnMuon",
    enable = cms.bool(True),
    #find the leptons that fail the ID cuts
    topCollection = cms.InputTag('susyMuon'),
    bottomCollection = cms.InputTag('cmgMuonSel'),
    #
    name = cms.untracked.string('muonOnMuon'),
    verbose = cms.untracked.bool(False)
)

