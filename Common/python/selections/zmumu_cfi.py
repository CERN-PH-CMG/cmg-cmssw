import FWCore.ParameterSet.Config as cms

zmumu = cms.PSet(
    mass = cms.string('mass() >= 50 && mass() < 120'),             
    leg1_quality = cms.string('leg1().getSelection(\"cuts_vbtfmuon\")'),
    leg2_quality = cms.string('leg2().getSelection(\"cuts_vbtfmuon_isGlobal\")'),
    leg1_kinematics = cms.PSet(
                               pt = cms.string('pt() > 15'),
                               eta = cms.string('abs(eta()) < 2.4'),
                               phi = cms.string('abs(phi()) < 10')
    ),
    leg2_kinematics = cms.PSet(
                               pt = cms.string('pt() > 10'),
                               eta = cms.string('abs(eta()) < 2.4'),
                               phi = cms.string('abs(phi()) < 10')
    )
)
