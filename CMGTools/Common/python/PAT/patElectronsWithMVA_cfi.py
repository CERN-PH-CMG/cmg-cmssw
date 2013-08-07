import FWCore.ParameterSet.Config as cms


patElectronsWithMVA = cms.EDProducer('PATElectronIsoMVAEmbedder',
                                     src    = cms.InputTag("selectedPatElectrons"),
                                     srcRho = cms.InputTag("kt6PFJets","rho"),
                                     srcPF  = cms.InputTag("particleFlow"),
                                     srcSelectedElectrons =cms.InputTag("selectedPatElectrons"),
                                     # the sel muons collection is actually not removed by now
                                     # just, PFcandidates which are muons are not considered
                                     srcSelectedMuons =cms.InputTag("muons"),
                                     srcVertices =cms.InputTag("offlinePrimaryVertices")

)                                 
                                 
# patElectronsWithMVASequence = cms.Sequence( patElectronsWithMVA )

