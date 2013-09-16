import FWCore.ParameterSet.Config as cms


patMuonsWithMVA = cms.EDProducer('PATMuonIsoMVAEmbedder',
                                 src    = cms.InputTag("patMuonsWithPF"),
                                 srcRho = cms.InputTag("kt6PFJets","rho"),
                                 srcPF  = cms.InputTag("particleFlow"),
                                 srcSelectedElectrons =cms.InputTag("gsfElectrons"),
                                 srcSelectedMuons =cms.InputTag("muons"),
                                 srcVertices =cms.InputTag("offlinePrimaryVertices")

)                                 
                                 
# patMuonsWithMVASequence = cms.Sequence( patMuonsWithMVA )
