######################################################################

import FWCore.ParameterSet.Config as cms

from CMGTools.Common.selections.kinematics_cfi import kinematics

photonFactory = cms.PSet(
    inputCollection = cms.InputTag("dummy"),
    )

cmgPhoton = cms.EDFilter("PhotonPOProducer",
                         cfg = photonFactory.clone(
    inputCollection = cms.InputTag("selectedPatPhotonsAK5")
    ),
                         cuts = cms.PSet(kinematics = kinematics.clone())
                         )

######################################################################
