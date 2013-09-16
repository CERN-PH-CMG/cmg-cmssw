import FWCore.ParameterSet.Config as cms


patMuonsWithPF = cms.EDProducer('PATPFMuonEmbedder',
                                 src    = cms.InputTag("selectedPatMuons"),
                                 srcPF  = cms.InputTag("particleFlow"),
)                                 
                                 
