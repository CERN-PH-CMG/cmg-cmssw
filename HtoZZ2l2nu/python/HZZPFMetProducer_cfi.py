import FWCore.ParameterSet.Config as cms

hzzPFMetProducer = cms.EDProducer('HZZPFMetProducer',
                                  collectionTag = cms.InputTag("particleFlow"),
                                  vertexTag = cms.InputTag("offlinePrimaryVertices"),
                                  jetTag =cms.InputTag("selectedPatJetsPFlow"),
                                  jetId = cms.PSet( version = cms.string("FIRSTDATA"), quality = cms.string("LOOSE") ),
                                  minJetPt = cms.double(10),
                                  maxJetEta = cms.double(2.5),
                                  dzCut = cms.double(0.1),
                                  minNeutralPt = cms.double(4.0),
                                  maxNeutralEta = cms.double(3.0),
                                  )

