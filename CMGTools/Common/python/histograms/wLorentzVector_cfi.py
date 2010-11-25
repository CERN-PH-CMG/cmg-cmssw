import FWCore.ParameterSet.Config as cms

wenuLorentzVector = cms.EDAnalyzer(
                "LorentzHistogramsAnalyzer",
                inputCollection = cms.InputTag("cmgWENu"),
                name = cms.string("WENu")
)

wmunuLorentzVector = cms.EDAnalyzer(
                "LorentzHistogramsAnalyzer",
                inputCollection = cms.InputTag("cmgWMuNu"),
                name = cms.string("WMuNu")
)