import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgDiObject_cfi import diObjectFactory

tauMuFactory = diObjectFactory.clone(
       leg1Collection = cms.InputTag('cmgTauSel'),
       leg2Collection = cms.InputTag('cmgMuonSel'),
       metCollection = cms.InputTag('cmgPFMET')       
)

cmgTauMu = cms.EDFilter(
    "TauMuPOProducer",
    cfg = tauMuFactory.clone(),
    cuts = cms.PSet(),
    )
