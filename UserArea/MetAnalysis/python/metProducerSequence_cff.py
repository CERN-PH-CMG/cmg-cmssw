import FWCore.ParameterSet.Config as cms
from CMGTools.External.JetIdParams_cfi import *
from CMGTools.External.puJetIDAlgo_cff import PhilV1

# track met from charged candidates only
ourTrackMet = cms.EDProducer("TrackMetProducer",
                             collectionTag = cms.InputTag("particleFlow"),
                             vtxcollectionTag = cms.InputTag("offlinePrimaryVertices"),
                             iDZCut = cms.double(0.1)   #### chiara!
                             )

# met from jets not coming from PU only
ourNoPUMet = cms.EDProducer("NoPUMetProducer",
                            collectionTag = cms.InputTag("particleFlow"),
                            srcVertices = cms.InputTag("offlinePrimaryVertices"),
                            iDZCut = cms.double(0.1),     #### chiara!
                            iPhi1  = cms.double(100.),    #### chiara! 
                            iPhi2  = cms.double(100.),    #### chiara! 
                            iEta1  = cms.double(100.),    #### chiara! 
                            iEta2  = cms.double(100.)    #### chiara! 
                            )

# met from jets from PU only
ourPUMet = cms.EDProducer("PUMetProducer",
                          collectionTag = cms.InputTag("particleFlow"),
                          srcVertices = cms.InputTag("offlinePrimaryVertices"),
                          iDZCut = cms.double(0.1),     #### chiara!
                          iPhi1  = cms.double(100.),    #### chiara! 
                          iPhi2  = cms.double(100.),    #### chiara! 
                          iEta1  = cms.double(100.),    #### chiara! 
                          iEta2  = cms.double(100.)    #### chiara! 
                          )

mvaMet   = cms.EDProducer("MVAMetProducerFromScratch",
                          CorrJetName     = cms.InputTag("ak5PFJetsL2L3"),
                          JetName         = cms.InputTag("ak5PFJets"),
                          PFCandidateName = cms.InputTag("particleFlow"),
                          VertexName      = cms.InputTag("offlinePrimaryVertices"),
                          JetPtMin        = cms.double(1.),
                          puJetIDAlgo     = PhilV1,
                          JetIdParams     = JetIdParams
                          )

                          

#metSequence = cms.Sequence ( ourTrackMet * ourNoPUMet * ourPUMet )
metSequence  = cms.Sequence ( mvaMet )
