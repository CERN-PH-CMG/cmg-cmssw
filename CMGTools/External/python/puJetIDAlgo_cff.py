import FWCore.ParameterSet.Config as cms
from CMGTools.External.JetIdParams_cfi import *

PuJetIdOptBDT = cms.PSet( 
 impactParTkThreshold = cms.untracked.double(1.) ,
 tmvaWeights = cms.untracked.string("CMGTools/External/data/TMVAClassification_PuJetIdOptBDT.weights.xml"),
 tmvaMethod  = cms.untracked.string("PuJetIdOptBDT"),
 version = cms.untracked.int32(-1),
 tmvaVariables = cms.untracked.vstring(
    "frac01",
    "frac02",
    "frac03",
    "frac04",
    "frac05",
    "nvtx",
    "nNeutrals",
    "beta",
    "betaStar",
    "dZ",
    "nCharged",
    ),
 tmvaSpectators = cms.untracked.vstring(
    "jetPt",
    "jetEta",
    ),
 JetIdParams = PuJetIdOptBDT_wp
 )

PuJetIdMinBDT = cms.PSet( 
 impactParTkThreshold = cms.untracked.double(1.) ,
 tmvaWeights = cms.untracked.string("CMGTools/External/data/TMVAClassification_PuJetIdMinBDT.weights.xml"),
 tmvaMethod  = cms.untracked.string("PuJetIdMinBDT"),
 version = cms.untracked.int32(-1),
 tmvaVariables = cms.untracked.vstring(
    "frac01",
    "frac02",
    "frac03",
    "frac04",
    "frac05",
    "nNeutrals",
    "beta",
    "betaStar",
    "dZ",
    "nCharged",
    ),
 tmvaSpectators = cms.untracked.vstring(
    "nvtx",
    "jetPt",
    "jetEta",
    ),
 JetIdParams = PuJetIdMinBDT_wp
 )


PhilV0 = cms.PSet( 
 impactParTkThreshold = cms.untracked.double(1.) ,
 tmvaWeights = cms.untracked.string("CMGTools/External/data/mva_JetID.weights.xml"),
 tmvaMethod  = cms.untracked.string("JetID"),
 version = cms.untracked.int32(0),
 JetIdParams = EmptyJetIdParams
)

PhilV1 = cms.PSet(
 impactParTkThreshold = cms.untracked.double(1.) ,
 tmvaWeights = cms.untracked.string("CMGTools/External/data/mva_JetID_v1.weights.xml"),
 tmvaMethod  = cms.untracked.string("JetID"),
 version = cms.untracked.int32(-1),
 tmvaVariables = cms.untracked.vstring(
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
   JetIdParams = JetIdParams
 )

