import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgDiObject_cfi import diObjectFactory

cmgDiPFCandidateFactory = diObjectFactory.clone(
       leg1Collection = cms.InputTag("particleFlow"),
       leg2Collection = cms.InputTag("particleFlow"),
)
cmgDiPFCandidate = cms.EDFilter(
    "DiPFCandidatePOProducer",
    cfg = cmgDiPFCandidateFactory.clone(),
    cuts = cms.PSet(
      ),
    )

