import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgDiObject_cfi import diObjectFactory

diFatJetFactory = diObjectFactory.clone(
       leg1Collection = cms.InputTag("cmgFatJet"),
       leg2Collection = cms.InputTag("cmgFatJet"),
       )

from CMGTools.Common.selections.dijet_cfi import dijetKinematics

cmgDiFatJet = cms.EDFilter(
    "DiFatJetPOProducer",
    cfg = diFatJetFactory.clone(),
    cuts = cms.PSet(
    fatDijetKinematics = dijetKinematics.clone(),
    ),
    verbose = cms.untracked.bool( False )
)

