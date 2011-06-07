import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgDiObject_cfi import diObjectFactory

tauMuFactory = diObjectFactory.clone(
       leg1Collection = cms.InputTag('cmgTau'),
       leg2Collection = cms.InputTag('cmgMuon')
)

cmgTauMu = cms.EDFilter(
    "TauMuPOProducer",
    cfg = tauMuFactory.clone(),
    cuts = cms.PSet(),
    )
