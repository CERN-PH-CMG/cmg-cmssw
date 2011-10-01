import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgDiObject_cfi import diObjectFactory

muEFactory = diObjectFactory.clone(
    leg1Collection = cms.InputTag('cmgMuonSel'),
    leg2Collection = cms.InputTag('cmgElectronSel')
)

cmgMuE = cms.EDFilter(
    "MuEPOProducer",
    cfg = muEFactory.clone(),
    cuts = cms.PSet(),
    )
