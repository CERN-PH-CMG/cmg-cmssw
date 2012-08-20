import FWCore.ParameterSet.Config as cms
from CMGTools.HtoZZ2l2nu.StandardSelections_cfi import *
from CMGTools.External.puJetIDAlgo_cff import *

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
                                                            ),

                                        #stuff for MVA met
                                        Vertices = BaseVertexSelection.clone(),
                                        Muons = BaseMuonsSelection.clone(id=cms.string("medium")),
                                        Electrons = BaseElectronsSelection.clone(id=cms.string("medium")),
                                        Photons = BasePhotonsSelection.clone(),
                                        mvaMet_CorrJetName     = cms.InputTag("ak5PFJetsL1L2L3ForMVAMET"),
                                        mvaMet_JetName         = cms.InputTag("ak5PFJets"),
                                        mvaMet_RhoName         = cms.InputTag('kt6PFJets','rho'),
                                        mvaMet_dZMin           = cms.double(0.1),
                                        JetIdParams = PhilV1
                                        )
