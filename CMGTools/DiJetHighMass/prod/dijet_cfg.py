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
    process.source.fileNames = cms.untracked.vstring('/store/cmst3/user/cbern/RA2SusyJetMET/Data/HotSkim/hotskim_pf.root')
    if runAtFNAL:
        process.source.fileNames = cms.untracked.vstring('file:/uscms/home/mgouzevi/work/TEST_SAMPLES/Data_skim.root')
else:
    process.source.fileNames = cms.untracked.vstring('rfio:/castor/cern.ch/user/m/meschi/CMSSW_3_8_6_p2/Reco/Reco_RSGraviton_500_0.root')   
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
process.load('CMGTools.DiJetHighMass.dijet_met_cff')
process.load("CMGTools.Common.runInfoAccounting_cfi")


from CMGTools.DiJetHighMass.myJetSequence_cff import myJetSequence
myJetSequence(process)

process.analysisSequence = cms.Sequence(
    process.jetSequence +
    process.metSequence +
    process.runInfoAccounting
    )


# Skimming
process.load('CMGTools.DiJetHighMass.skims.selHighMass_cff')


# Total process
process.p = cms.Path(
    process.pat +
    process.analysisSequence +
    process.select
    )


process.out.SelectEvents.SelectEvents.append('p')


process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) ) 
