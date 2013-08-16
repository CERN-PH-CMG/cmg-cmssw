import FWCore.ParameterSet.Config as cms

electronOnElectron = cms.EDProducer(
    "TPElectronOnElectron",
    enable = cms.bool(True),
    #find the leptons that fail the ID cuts
    topCollection = cms.InputTag('susyElectron'),
    bottomCollection = cms.InputTag('cmgElectronSel'),
    #
    name = cms.untracked.string('electronOnElectron'),
    verbose = cms.untracked.bool(False)
)

