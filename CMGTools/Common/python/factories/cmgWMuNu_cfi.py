import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgDiObject_cfi import diObjectFactory

wmunuFactory = diObjectFactory.clone(
       leg1Collection = cms.InputTag('cmgMuon'),
       leg2Collection = cms.InputTag('cmgPFMET')
)
# from CMGTools.Common.selections.wmunu_cfi import wmunu
cmgWMuNu = cms.EDFilter(
    "WMuNuPOProducer",
    cfg = wmunuFactory.clone(),
    cuts = cms.PSet(
       # wmunu = wmunu.clone()
      ),
    )
