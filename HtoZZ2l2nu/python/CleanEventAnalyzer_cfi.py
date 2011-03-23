import FWCore.ParameterSet.Config as cms

analysis = cms.EDAnalyzer("DileptonPlusMETEventAnalyzer",
                          source = cms.InputTag("cleanEvent")
                          )
