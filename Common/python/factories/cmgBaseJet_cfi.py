import FWCore.ParameterSet.Config as cms

from CMGTools.Common.selections.btaggedjet_cfi import trackCountingHighEffBJetTags
from CMGTools.Common.selections.kinematics_cfi import kinematics

baseJetFactory = cms.PSet(
       inputCollection = cms.InputTag("selectedPatJets"),
       btagType = cms.vstring(
                              #see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideBTagPerformance
                              'trackCountingHighEffBJetTags',#0
                              'trackCountingHighPurBJetTags',#1
                              'jetProbabilityBJetTags',#2
                              'jetBProbabilityBJetTags',#3
                              'simpleSecondaryVertexHighEffBJetTags',#4
                              'simpleSecondaryVertexHighPurBJetTags',#5
                              'combinedSecondaryVertexBJetTags'#6
                              ),
       fillJec = cms.bool(True),
       fillJecUncertainty = cms.bool(True),
       jecPath = cms.string("CondFormats/JetMETObjects/data/Spring10_Uncertainty_AK5Calo.txt")
       )

cmgBaseJet = cms.EDFilter(
    "BaseJetPOProducer",
    cfg = baseJetFactory.clone(),
    cuts = cms.PSet(
       btag = trackCountingHighEffBJetTags.clone(),
       jetKinematics = kinematics.clone()
       )
)
