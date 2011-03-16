import FWCore.ParameterSet.Config as cms

wenuFactory = cms.PSet(
       leg1Collection = cms.InputTag('cmgElectron'),
       leg2Collection = cms.InputTag('cmgPFMET')
)

cmgWENu = cms.EDFilter(
    "WENuPOProducer",
    cfg = wenuFactory.clone(),
    cuts = cms.PSet(
      ),
    )