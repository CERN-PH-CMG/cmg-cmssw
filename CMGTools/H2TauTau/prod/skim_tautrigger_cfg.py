# Select PFAOD or AOD MC on the HLT trigger bit.
# to be used on WJets, in particular.

import FWCore.ParameterSet.Config as cms
import sys 
import os 

process = cms.Process("PFAOD")

from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    # 'cmgtools_group',
    # '/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V2', # 2.5% 
    'cmgtools_group',
    '/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V2', # 33% -> not worth skimming.
    '.*root')

#process.source = cms.Source(
#    'PoolSource',
#    fileNames = cms.untracked.vstring('/store/cmst3/group/cmgtools/CMG/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V2/PFAOD_10000_1_Zb0.root')
#    )


process.source.fileNames = process.source.fileNames[1:2]

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))
#WARNING!
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )


process.hltHighLevel = cms.EDFilter("HLTHighLevel",
    TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
    HLTPaths = cms.vstring(# 'HLT_Ele15_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_v2',
                           'HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v1',
                           'HLT_IsoMu15_LooseIsoPFTau15_v9',
                           'HLT_IsoMu15_eta2p1_MediumIsoPFTau20_v1'),           # provide list of HLT paths (or patterns) you want
    eventSetupPathsKey = cms.string(''), # not empty => use read paths from AlCaRecoTriggerBitsRcd via this key
    andOr = cms.bool(True),             # how to deal with multiple triggers: True (OR) accept if ANY is true, False (AND) accept if ALL are true
    throw = cms.bool(True)    # throw exception on unknown path names
)

process.p = cms.Path(
    process.hltHighLevel
    )

process.load("Configuration.EventContent.EventContent_cff")
process.out = cms.OutputModule(
    "PoolOutputModule",
    process.AODSIMEventContent,
    fileName = cms.untracked.string( 'PFAOD.root' ),
    SelectEvents   = cms.untracked.PSet(
      SelectEvents = cms.vstring('p')
    ),
    )

from CMGTools.Production.PFAOD.PFAOD_EventContent_cff import V3
process.out.outputCommands.extend( V3 )

process.endpath = cms.EndPath(
    process.out
    )


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100


