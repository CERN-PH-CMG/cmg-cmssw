from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

# changing process name, as PAT is already taken 
processName = 'ANA'
runOnData = True
postfix="PFlow"
ext = 'Data'

import os
path = os.getcwd()
runAtFNAL = False

if path.find('uscms') > -1:
    runAtFNAL=True
    print "Run at FNAL"
else:
    print "Run probably at CERN"
    
process.setName_(processName)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10))

if (not runOnData):
    process.source.duplicateCheckMode = cms.untracked.string("noDuplicateCheck")


if runOnData:
    process.source.fileNames = cms.untracked.vstring('/store/cmst3/user/lucieg/CMG/MuHad/Run2011A-PromptReco-v4/AOD/PFAOD_95_1_sDI.root')
    if runAtFNAL:
        process.source.fileNames = cms.untracked.vstring('file:/uscms/home/mgouzevi/work/TEST_SAMPLES/Data_skim.root')
else:
    process.source.fileNames = cms.untracked.vstring('/store/cmst3/user/lucieg/CMG/MuHad/Run2011A-PromptReco-v4/AOD/PFAOD_95_1_sDI.root')
    if runAtFNAL:
        process.source.fileNames = cms.untracked.vstring('file:/uscms/home/mgouzevi/work/TEST_SAMPLES/Reco_RSGraviton_500_skim.root')



print 'processing: '
print process.source.fileNames

process.out.fileName = cms.untracked.string('diJetAnalysis_%s.root' % ext )

#output file for histograms etc
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("diJetAnalysis_Histograms_%s.root" % ext ) )


# Event Content
from CMGTools.DiJetHighMass.EventContent.diJetAnalysis_cff import diJetHighMass

process.out.outputCommands = diJetHighMass

# Pat Sequence definition
from CMGTools.DiJetHighMass.myPatSequence_cff import myPatSequence
myPatSequence(process, runOnData)

process.pat = cms.Sequence(
    process.patDefaultSequence
    )


# Analysis Sequence definition
process.load('CMGTools.DiJetHighMass.baseJets_cff')
process.load('CMGTools.DiJetHighMass.pfJets_cff')
process.load("CMGTools.Common.runInfoAccounting_cfi")

from CMGTools.DiJetHighMass.myJetSequence_cff import myJetSequence
myJetSequence(process)


process.analysisSequence = cms.Sequence(
    process.jetSequence
    )


# Skimming
process.load('CMGTools.DiJetHighMass.skims.selHighMass_cff')

process.dump = cms.EDAnalyzer("EventContentAnalyzer")

# Total process
process.p = cms.Path(
    process.pat +
    process.analysisSequence +
    process.select
    )


if not runOnData:
   process.load("CMGTools.Common.runInfoAccounting/runInfoAccounting_cfi")
   process.outpath += process.runInfoAccounting
else:
   process.load("CMGTools.Common.runInfoAccounting/runInfoAccountingData_cfi")
   process.outpath += process.runInfoAccountingData

process.out.SelectEvents.SelectEvents.append('p')


process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) ) 
