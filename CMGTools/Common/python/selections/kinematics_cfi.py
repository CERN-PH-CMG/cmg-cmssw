import FWCore.ParameterSet.Config as cms

kinematics = cms.PSet(
    pt = cms.string('pt() > 10'),
    eta = cms.string('abs(eta()) < 2.4'),
    phi = cms.string('abs(phi()) < 3.2')
    )

                    

dijetKinematics = cms.PSet(
    looseMass = cms.string('mass() > 180'),
    tightMass = cms.string('mass() > 220'),
    deltaEta = cms.string('abs(leg1().eta()-leg2.eta()) < 1.3'),
    looseDeltaEta = cms.string('abs(leg1().eta()-leg2.eta()) < 2.5'),
    tightDeltaEta = cms.string('abs(leg1().eta()-leg2.eta()) < 1.3')
    )
