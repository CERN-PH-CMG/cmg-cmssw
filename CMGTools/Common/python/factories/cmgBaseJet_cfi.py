import FWCore.ParameterSet.Config as cms

baseJetFactory = cms.PSet(
       inputCollection = cms.InputTag("selectedPatJets"),
       btagType = cms.string('combinedSecondaryVertexBJetTags')
       )
from CMGTools.Common.selections.kinematics_cfi import kinematics
cmgBaseJet = cms.EDFilter(
    "BaseJetPOProducer",
    cfg = baseJetFactory.clone(),
    cuts = cms.PSet(
       jetKinematics = kinematics.clone()
       ),    
)
