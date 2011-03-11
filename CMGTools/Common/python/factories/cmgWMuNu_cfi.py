import FWCore.ParameterSet.Config as cms

wmunuFactory = cms.PSet(
       leg1Collection = cms.InputTag('cmgMuon'),
       leg2Collection = cms.InputTag('cmgMETPFCandidates')
)
from CMGTools.Common.selections.wmunu_cfi import wmunu
cmgWMuNu = cms.EDFilter(
    "WMuNuPOProducer",
    cfg = wmunuFactory.clone(),
    cuts = cms.PSet(
       wmunu = wmunu.clone()
      ),
    )