import FWCore.ParameterSet.Config as cms

pfcandsLorentzVector = cms.EDAnalyzer(
                "LorentzHistogramsAnalyzer",
                inputCollection = cms.InputTag("cmgPFCand"),
                name = cms.string("PFCands")
)