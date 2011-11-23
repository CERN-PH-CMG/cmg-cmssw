import FWCore.ParameterSet.Config as cms

pfJetOnPFJet = cms.EDProducer(
    "TPPFJetOnPFJet",
    enable = cms.bool(True),
    #find the leptons that fail the ID cuts
    topCollection = cms.InputTag('cmgPFJetSel'),
    bottomCollection = cms.InputTag('cmgPFJetSel'),
    #
    name = cms.untracked.string('PFJetOnPFJet'),
    verbose = cms.untracked.bool(False)
)

