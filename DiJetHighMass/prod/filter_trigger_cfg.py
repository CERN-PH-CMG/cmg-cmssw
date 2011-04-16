import FWCore.ParameterSet.Config as cms

processName = "FILTER"
process = cms.Process(processName)

## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

runOnData = False
path = 'HLT_Jet50U'

# MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

# Options and Output Report
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# Source file  

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:diJetAnalysis_Data.root')
)

#from CMGTools.DiJetHighMass.skims.HLT_cff import HLT_template

import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
process.HLTSel = copy.deepcopy(hltHighLevel)
process.HLTSel.HLTPaths = cms.vstring(path)

print 'HLT selection: ', process.HLTSel.HLTPaths

outCommand = 'keep *_*_*_' + processName
fileName = 'diJetAnalysis_Filter_' + path + '.root'


process.p = cms.Path(process.HLTSel)



# Output Module Configuration (expects a path 'p')
process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string(fileName),
    # save only events passing the full path
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    outputCommands = cms.untracked.vstring('keep *') 
    )

process.outpath = cms.EndPath(process.out)
