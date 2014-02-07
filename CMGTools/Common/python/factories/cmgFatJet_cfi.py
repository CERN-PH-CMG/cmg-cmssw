######################################################################

import FWCore.ParameterSet.Config as cms

cmgFatJetFactory = cms.PSet(
    inputCollection = cms.InputTag("dummy"),
    Rmax = cms.double(1.1)
    )

cmgFatJet = cms.EDFilter(
    "FatJetPOProducer",
    cfg = cmgFatJetFactory.clone(),
    cuts = cms.PSet(
       leadingJetKinematics = cms.untracked.string('leadPtr().getSelection(\"All Cuts\")')
       ),
    verbose = cms.untracked.bool( False )
    )

######################################################################
