import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgDiObject_cfi import diObjectFactory

diPFJetFactory = diObjectFactory.clone(
       leg1Collection = cms.InputTag("cmgPFJet"),
       leg2Collection = cms.InputTag("cmgPFJet"),
       )

from CMGTools.Common.selections.dijet_cfi import dijetKinematics
cmgDiPFJet = cms.EDFilter(
    "DiPFJetPOProducer",
    cfg = diPFJetFactory.clone(),
    cuts = cms.PSet(
       dijetKinematics = dijetKinematics.clone()
       ),
    verbose = cms.untracked.bool( False )
)

