import FWCore.ParameterSet.Config as cms

genJetFactory = cms.PSet(
       inputCollection = cms.InputTag("selectedPatJetsAK5","genJets")
       )
from CMGTools.Common.selections.kinematics_cfi import kinematics
genJet = cms.EDFilter(
    "GenJetPOProducer",
    cfg = genJetFactory.clone(),
    cuts = cms.PSet(),
    verbose = cms.untracked.bool( False )
)
