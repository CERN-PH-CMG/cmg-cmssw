import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgDiObject_cfi import diObjectFactory

wenuFactory = diObjectFactory.clone(
       leg1Collection = cms.InputTag('cmgElectron'),
       leg2Collection = cms.InputTag('cmgPFMET')
)
cmgWENu = cms.EDFilter(
    "WENuPOProducer",
    cfg = wenuFactory.clone(),
    cuts = cms.PSet(
      ),
    )