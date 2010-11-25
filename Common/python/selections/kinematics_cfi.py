import FWCore.ParameterSet.Config as cms

additionalCut = cms.PSet(
    pt = cms.string('pt()>50')
    )

kinematics = cms.PSet(
    pt = cms.string('pt() > 10'),
    eta = cms.string('abs(eta()) < 2.4'),
    phi = cms.string('abs(phi()) < 10'),
    addedCut = additionalCut.clone()
    )

                    

