import FWCore.ParameterSet.Config as cms

baseJetFactory = cms.PSet(
       inputCollection = cms.InputTag("selectedPatJets"),
       btagType = cms.string('trackCountingHighEffBJetTags')
       )
from CMGTools.Common.selections.btaggedjet_cfi import trackCountingHighEffBJetTags
from CMGTools.Common.selections.kinematics_cfi import kinematics
cmgBaseJet = cms.EDFilter(
    "BaseJetPOProducer",
    cfg = baseJetFactory.clone(),
    cuts = cms.PSet(
       btag = trackCountingHighEffBJetTags.clone(),
       jetKinematics = kinematics.clone()
       ),    
)
