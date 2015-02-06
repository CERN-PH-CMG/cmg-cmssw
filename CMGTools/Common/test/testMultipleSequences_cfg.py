from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms
from CMGTools.Common.Tools.getGlobalTag import getGlobalTag

sep_line = "-" * 50
print
print sep_line
print "CMGTools : testing multiple analysis sequences"
print sep_line

process.setName_('ANA')

runOnMC = False

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(10000)
        )

process.maxLuminosityBlocks = cms.untracked.PSet( 
    input = cms.untracked.int32(-1)
    )

#process.source.fileNames = cms.untracked.vstring(
#    # 'file:patTuple_PF2PAT.root'
#    'file:/afs/cern.ch/user/c/cbern/scratch0/patTuple_PF2PAT.root'
#    )

process.load("CMGTools.Common.sources.HT.Run2011A_May10ReReco_v1.AOD.PAT_CMG_MAX.source_PAT_cff")

# reading the first 10 files:
nFiles = 10
print 'WARNING: RESTRICTING INPUT TO THE FIRST', nFiles, 'FILES'
process.source.fileNames = process.source.fileNames[:nFiles] 

print process.source.fileNames


# output module for EDM event (ntuple)
process.out.fileName = cms.untracked.string('tree_testMultipleSequences.root')
from CMGTools.Common.eventContent.everything_cff import everything 
process.out.outputCommands = cms.untracked.vstring( 'drop *')
process.out.outputCommands.extend( everything ) 
process.out.dropMetaData = cms.untracked.string('PRIOR')

#output file for histograms etc
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histograms_testMultipleSequences.root"))


# default analysis sequence    
process.load('CMGTools.Common.analysis_cff')

# removing the taus, as PFCandidate embedding does not work, hence an exception in the cmg Tau factory - not needed anymore 
# from CMGTools.Common.Tools.tuneCMGSequences import removeObject
# removeObject( process, 'tau', '') 

if runOnMC:
    process.load("CMGTools.Common.runInfoAccounting_cfi")
    process.outpath += process.runInfoAccounting

from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet
from CMGTools.Common.Tools.visitorUtils import replacePostfix

print 'preparing sequence AK5LCCMG'

cloneProcessingSnippet(process, getattr(process, 'analysisSequence'), 'AK5LCCMG')
replacePostfix(getattr(process,"analysisSequenceAK5LCCMG"),'AK5','AK5LC') 

print 'preparing sequence AK7CMG'

cloneProcessingSnippet(process, getattr(process, 'analysisSequence'), 'AK7CMG')
replacePostfix(getattr(process,"analysisSequenceAK7CMG"),'AK5','AK7') 

from CMGTools.Common.Tools.tuneCMGSequences import * 
tuneCMGSequences(process, postpostfix='CMG')

print 'running'

process.p = cms.Path(
    process.analysisSequence
    + process.analysisSequenceAK5LCCMG
    + process.analysisSequenceAK7CMG
)

process.GlobalTag.globaltag = cms.string(getGlobalTag(runOnMC))

process.schedule = cms.Schedule(
    process.p,
    process.outpath
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) ) 



