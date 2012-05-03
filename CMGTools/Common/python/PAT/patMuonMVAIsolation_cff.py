import FWCore.ParameterSet.Config as cms


muonsWithMVAIso = cms.EDProducer('PATMuonIsoMVAEmbedder',
                                 src    = cms.InputTag("patMuons"),
                                 srcRho = cms.InputTag("kt6PFJets","rho"),
                                 srcPF  = cms.InputTag("particleFlow"),
                                 srcSelectedElectrons =cms.InputTag("selectedPatElectrons"),
                                 srcSelectedMuons =cms.InputTag("selectedPatMuons"),
                                 srcVertices =cms.InputTag("offlinePrimaryVertices")

)                                 
                                 
muonMVAIsolation = cms.Sequence(muonsWithMVAIso)
