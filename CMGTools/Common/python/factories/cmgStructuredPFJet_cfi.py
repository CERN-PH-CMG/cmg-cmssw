import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgPFJet_cfi import pfJetFactory

structuredPFJetFactory = cms.PSet(
       inputCollection = cms.InputTag("selectedPatJetsCHSpruned"),
       subjetCollection = cms.InputTag("ak5PFJetsCHSpruned:SubJets"),
       baseJetFactory = pfJetFactory.baseJetFactory.clone(),
       pfJetFactory = pfJetFactory.clone(),
       )

cmgStructuredPFJet = cms.EDFilter(
    "StructuredPFJetPOProducer",
    cfg = structuredPFJetFactory.clone(),
    cuts = cms.PSet(),
    verbose = cms.untracked.bool( False )
)
