import FWCore.ParameterSet.Config as cms

triggerCorrelationHistograms = cms.EDAnalyzer(
    "TriggerResultHistogramsAnalyzer",
    inputCollection = cms.InputTag("TriggerResults",'','HLT'),
    #names = cms.untracked.vstring('razorPath','LPPath','RA1Path','RA2Path')
    #names = cms.untracked.vstring('HLT_Mu5','HLT_Mu7','HLT_Mu9','HLT_Mu11','HLT_Mu13','HLT_Mu15','HLT_Mu19','HLT_Mu21','HLT_Mu25')
    )

triggerCorrelationHistogramsEndPath = cms.EndPath(triggerCorrelationHistograms)
