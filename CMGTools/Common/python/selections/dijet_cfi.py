import FWCore.ParameterSet.Config as cms

dijetKinematics = cms.PSet(
    jetsPhaseSpace = cms.PSet(
    jet1 = cms.untracked.string('leg1().getSelection(\"All Cuts\")'),
    jet2 = cms.untracked.string('leg2().getSelection(\"All Cuts\")')
    ),
    looseMass = cms.string('mass > 200'),
    mediumMass = cms.string('mass > 400'),
    Jet140Uthreshold = cms.string('mass > 510'),
    
    looseDeltaEta = cms.string('abs(leg1.eta()-leg2.eta()) < 1.7'),
    mediumDeltaEta = cms.string('abs(leg1.eta()-leg2.eta()) < 1.5'),
    tightDeltaEta = cms.string('abs(leg1.eta()-leg2.eta()) < 1.3')
    )

