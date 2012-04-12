import FWCore.ParameterSet.Config as cms
from CMGTools.External.JetIdParams_cfi import *

basePtVtxCat = cms.PSet( 
 impactParTkThreshold = cms.untracked.double(1.) ,
 tmvaWeights = cms.untracked.string("CMGTools/External/data/TMVAClassification_BDT_basePtVtxCat.weights.xml"),
 tmvaMethod  = cms.untracked.string("BDT_basePtVtxCat"),
 version = cms.untracked.int32(-1),
 tmvaVariables = cms.untracked.vstring(
    "dRMean",
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
    )
 )

baseNoVtxCat = cms.PSet( 
 impactParTkThreshold = cms.untracked.double(1.) ,
 tmvaWeights = cms.untracked.string("CMGTools/External/data/TMVAClassification_BDT_baseNoVtxCat.weights.xml"),
 tmvaMethod  = cms.untracked.string("BDT_baseNoVtxCat"),
 version = cms.untracked.int32(-1),
 tmvaVariables = cms.untracked.vstring(
    "dRMean",
    "frac01",
    "frac02",
    "frac03",
    "frac04",
    "frac05",
    "nNeutrals",
    "nvtx",
    "beta",
    "betaStar",
    "dZ",
    "nCharged",
    ),
 tmvaSpectators = cms.untracked.vstring(
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

