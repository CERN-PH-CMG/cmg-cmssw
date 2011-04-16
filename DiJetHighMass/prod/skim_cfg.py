import FWCore.ParameterSet.Config as cms

processName = "SAVE"
process = cms.Process(processName)

## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

runOnData = False
path = 'HLT_Jet50U'

# MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

# Options and Output Report
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# Source file  
process.source = cms.Source("PoolSource",
#    fileNames = cms.untracked.vstring('rfio:/castor/cern.ch/user/m/meschi/CMSSW_3_8_6_p2/Reco/Reco_RSGraviton_500_0.root')
     fileNames = cms.untracked.vstring('/store/cmst3/user/cbern/RA2SusyJetMET/Data/HotSkim/hotskim_pf.root')
)

#from CMGTools.DiJetHighMass.skims.HLT_cff import HLT_template

import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
process.HLTSel = copy.deepcopy(hltHighLevel)
process.HLTSel.HLTPaths = cms.vstring(path)

print 'HLT selection: ', process.HLTSel.HLTPaths

outCommand = 'keep *_*_*_' + processName
fileName = 'Data_skim.root'


process.p = cms.Path()



# Output Module Configuration (expects a path 'p')
process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string(fileName),
    # save only events passing the full path
    outputCommands = cms.untracked.vstring('keep *_*_*_RECO', 'keep *_*_*_HLT') 
    )

process.outpath = cms.EndPath(process.out)
