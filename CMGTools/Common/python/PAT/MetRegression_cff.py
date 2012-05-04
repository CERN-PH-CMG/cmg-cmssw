import FWCore.ParameterSet.Config as cms

from CMGTools.External.puJetIDAlgo_cff import *

pfMetForRegression   = cms.EDProducer("MetFlavorProducer",
                                      CorrJetName     = cms.InputTag("patJets"),
                                      # not needed 
                                      JetName         = cms.InputTag("ak5PFJets"),
                                      PFCandidateName = cms.InputTag("particleFlow"),
                                      VertexName      = cms.InputTag("offlinePrimaryVertices"),
                                      RhoName         = cms.InputTag('kt6PFJets','rho'),
                                      JetPtMin        = cms.double(0.),
                                      dZMin           = cms.double(0.1),
                                      runMvas         = cms.bool(True),
                                      MetFlavor       = cms.int32(0),  # 0 PF  1 TK  2 No PU 3 PU  4 PUC   
                                      impactParTkThreshold = cms.untracked.double(1.) ,
                                      # need to use edmFileInPath
                                      tmvaWeights    = cms.string("CMGTools/External/data/mva_JetID_v1.weights.xml"),
                                      tmvaMethod    = cms.string("JetID"),
                                      version       = cms.int32(-1),
                                      cutBased      = cms.bool(False),
                                      tmvaVariables = cms.vstring(
    "nvtx",
    "jetPt",
    "jetEta",
    "jetPhi",
    "dZ",
    "d0",
    "beta",
    "betaStar",
    "nCharged",
    "nNeutrals",
    "dRMean",
    "frac01",
    "frac02",
    "frac03",
    "frac04",
    "frac05",
    ),
                         label          = cms.string("PhilV1"),
                         tmvaSpectators = cms.vstring(),
                          JetIdParams = JetIdParams
                         )

tkMet     =  pfMetForRegression.clone(MetFlavor = cms.int32(1))
nopuMet   =  pfMetForRegression.clone(MetFlavor = cms.int32(2))
puMet     =  pfMetForRegression.clone(MetFlavor = cms.int32(3),
                                      dZMin = 0.2)
pcMet     =  pfMetForRegression.clone(MetFlavor = cms.int32(4))

                          
MetRegressionSequence  = cms.Sequence (
    pfMetForRegression + 
    nopuMet +
    puMet + 
    pcMet +
    tkMet
    )
