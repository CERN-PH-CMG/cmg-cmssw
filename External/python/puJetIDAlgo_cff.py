import FWCore.ParameterSet.Config as cms

dRdRProfMultBetaFull = cms.PSet( 
 impactParTkThreshold = cms.untracked.double(1.) ,
 tmvaWeights = cms.untracked.string("CMGTools/External/data/TMVAClassification_BDT_dRdRProfMultBetaFull.weights_postproc.xml"),
 tmvaMethod  = cms.untracked.string("BDT_dRdRProfMultBetaFull"),
 version = cms.untracked.int32(-1),
 tmvaVariables = cms.untracked.vstring(
    "dRMean",
    "frac01",
    "frac02",
    "frac03",
    "frac04",
    "frac05",
    "nParticles",
    "beta",
    "betaStar",
    ),
 tmvaSpectators = cms.untracked.vstring(
    "nvtx",
    "jetPt",
    "jetEta",
    )
)

PhilV0 = cms.PSet( 
 impactParTkThreshold = cms.untracked.double(1.) ,
 tmvaWeights = cms.untracked.string("CMGTools/External/data/mva_JetID.weights.xml"),
 tmvaMethod  = cms.untracked.string("JetID"),
 version = cms.untracked.int32(0),
)

