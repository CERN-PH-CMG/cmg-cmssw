######################################################################

import FWCore.ParameterSet.Config as cms

from CMGTools.Common.selections.kinematics_cfi import kinematics

multiJetFactory = cms.PSet(
    inputCollection = cms.InputTag("dummy")
    )

cmgMultiJet = cms.EDFilter(
    "MultiObjectPOProducer",
    cfg = multiJetFactory.clone(
      inputCollection = cms.InputTag("cmgPFJetSel")
      ),
    cuts = cms.PSet(kinematics = kinematics.clone())
    )

######################################################################
