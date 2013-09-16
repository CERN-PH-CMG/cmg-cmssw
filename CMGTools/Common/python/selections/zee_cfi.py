import FWCore.ParameterSet.Config as cms

zee = cms.PSet(
    mass = cms.string('mass() >= 50 && mass() < 120'),             
    leg1_quality = cms.string(''),
    leg2_quality = cms.string(''),
    leg1_kinematics = cms.PSet(
                               pt = cms.string('leg1().pt() > 15'),
                               eta = cms.string('abs(leg1().eta()) < 3'),
                               phi = cms.string('abs(leg1().phi()) < 10')
    ),
    leg2_kinematics = cms.PSet(
                               pt = cms.string('leg2().pt() > 10'),
                               eta = cms.string('abs(leg2().eta()) < 3'),
                               phi = cms.string('abs(leg2().phi()) < 10')
    )
)
