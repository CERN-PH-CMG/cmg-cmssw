import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgDiObject_cfi import diObjectFactory

tauEFactory = diObjectFactory.clone(
       leg1Collection = cms.InputTag('cmgTauSel'),
       leg2Collection = cms.InputTag('cmgElectronSel')
)

cmgTauE = cms.EDFilter(
    "TauEPOProducer",
    cfg = tauEFactory.clone(),
    cuts = cms.PSet(),
    )
