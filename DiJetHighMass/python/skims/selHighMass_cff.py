import FWCore.ParameterSet.Config as cms

highMass = cms.EDFilter(
    "DiJetSelector",    
    src = cms.InputTag("cmgBaseDiJetAK5PF"),
    cut = cms.string("getSelection(\"cuts_dijetKinematics_looseMass\") && getSelection(\"cuts_dijetKinematics_looseDeltaEta\")"),
)


filterHighMass = cms.EDFilter(
    "CandViewCountFilter",
    src = cms.InputTag("highMass"),
    minNumber = cms.uint32(1),
    )


select = cms.Sequence(
    highMass +
    filterHighMass
    )


