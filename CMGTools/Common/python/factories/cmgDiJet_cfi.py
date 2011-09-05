import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgDiObject_cfi import diObjectFactory

diJetFactory = diObjectFactory.clone(
       leg1Collection = cms.InputTag("cmgPFJet"),
       leg2Collection = cms.InputTag("cmgPFJet"),
       )

from CMGTools.Common.selections.dijet_cfi import dijetKinematics
cmgDiJet = cms.EDFilter(
    "DiJetPOProducer",
    cfg = diJetFactory.clone(),
    cuts = cms.PSet(
       dijetKinematics = dijetKinematics.clone(),
       ),
    verbose = cms.untracked.bool( False )
)

