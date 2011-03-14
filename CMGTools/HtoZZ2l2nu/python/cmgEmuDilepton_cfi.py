import FWCore.ParameterSet.Config as cms

#
emuDileptonFactory = cms.PSet( leg1Collection = cms.InputTag("cmgElectron"),
                               leg2Collection = cms.InputTag("cmgMuon")
                               )

#
from CMGTools.Common.selections.kinematics_cfi import kinematics
emukinematics = cms.PSet( mass = cms.string('mass() >= 0'),
                          pt = cms.string('pt() >= 0'),
                          leg1_quality = cms.string(''),
                          leg1_kinematics = cms.PSet(  pt = cms.string('leg1().pt() > 15'),
                                                       eta = cms.string('abs(leg1().eta()) < 2.5'),
                                                       phi = cms.string('abs(leg1().phi()) < 10')
                                                       ),
                          leg2_quality = cms.string('leg2().getSelection(\"cuts_vbtfmuon\")'),
                          leg2_kinematics = cms.PSet( pt = cms.string('leg2().pt() > 15'),
                                                      eta = cms.string('abs(leg2().eta()) < 2.4'),
                                                      phi = cms.string('abs(leg2().phi()) < 10')
                                                      )
                          )

#
cmgEmuDilepton = cms.EDFilter( "EmuDileptonPOProducer",
                               cfg = emuDileptonFactory.clone(),
                               cuts = cms.PSet( kinematics = kinematics.clone(),
                                                emukinematics = emukinematics.clone()
                                                )
                               )


