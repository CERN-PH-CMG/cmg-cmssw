import FWCore.ParameterSet.Config as cms

ClusteredPFMetProducer = cms.EDProducer('ClusteredPFMetProducer',
                                        collectionTag = cms.InputTag("particleFlow"),
                                        vertexTag = cms.InputTag("offlinePrimaryVertices"),
                                        produceSummary = cms.bool(False),
                                        minBiasMode = cms.bool(False),
                                        minJetPt = cms.double(2.0),
                                        maxJetEta = cms.double(2.4),
                                        minNeutralPt = cms.double(0),
                                        maxNeutralEta = cms.double(5.0),
                                        # check fast jet algorithm parameters at
                                        # http://www.lpthe.jussieu.fr/~salam/fastjet/repo/doxygen-2.4.3/JetDefinition_8hh.html#aab63b8ab22ff040949752ce9f876f93d
                                        # here we set to anti Kt with 0.5 cone
                                        fastjet = cms.PSet( fj_algo = cms.uint32(2),        # anti-kt 
                                                            distance_par = cms.double(0.7), # cone "size"
                                                            extra_param = cms.double(0),    # not used for anti-kt
                                                            recombination_scheme = cms.int32(0),   # E scheme
                                                            strategy = cms.int32(1)         # Best
                                                            )
                                        )

