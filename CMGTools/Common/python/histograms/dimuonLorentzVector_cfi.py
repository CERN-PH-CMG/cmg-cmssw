import FWCore.ParameterSet.Config as cms

dimuonLorentzVector = cms.EDAnalyzer(
                "LorentzHistogramsAnalyzer",
                inputCollection = cms.InputTag("cmgDiMuon"),
                name = cms.string("DiMuon")
)
