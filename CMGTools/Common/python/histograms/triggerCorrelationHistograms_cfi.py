import FWCore.ParameterSet.Config as cms

triggerCorrelationHistograms = cms.EDAnalyzer(
    "TriggerResultHistogramsAnalyzer",
    inputCollection = cms.InputTag("TriggerResults"),
    )

triggerCorrelationHistogramsEndPath = cms.EndPath(triggerCorrelationHistograms)
