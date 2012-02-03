import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgDiObject_cfi import diObjectFactory

tauEFactory = diObjectFactory.clone(
       leg1Collection = cms.InputTag('cmgTauSel'),
       leg2Collection = cms.InputTag('cmgElectronSel'),
       metCollection = cms.InputTag('cmgPFMET')       

)

cmgTauEle = cms.EDFilter(
    "TauElePOProducer",
    cfg = tauEFactory.clone(),
    cuts = cms.PSet(),
    )
