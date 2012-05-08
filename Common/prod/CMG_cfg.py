import FWCore.ParameterSet.Config as cms

sep_line = '-'*67
print sep_line
print 'CMG-tuplizer, contact Colin before any modification'
print sep_line

process = cms.Process("CMG")


print 'querying database for source files'


runOnMC = True


from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
   'cmgtools_group',
   '/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5',
   # '/TauPlusX/Run2011A-PromptReco-v4/AOD/V5'
   # 'CMS',
   # '/TauPlusX/Run2011A-03Oct2011-v1/AOD'
   )

process.source.fileNames = ['file:patTuple.root']

print sep_line
print process.source.fileNames
print sep_line 

## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )

print 'loading the main CMG sequence'

process.load('CMGTools.Common.CMG.CMG_cff')

if runOnMC is False:
    # removing MC stuff
    print 'removing MC stuff, as we are running on Data'
    process.PATCMGSequence.remove(genSequence)
    
print 'cloning the jet sequence to build PU chs jets'

from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet
process.jetCHSSequence = cloneProcessingSnippet(process, process.jetSequence, 'CHS')
from CMGTools.Common.Tools.visitorUtils import replaceSrc
replaceSrc( process.jetCHSSequence, 'selectedPatJets', 'selectedPatJetsCHS')
replaceSrc( process.jetCHSSequence, 'puJetId', 'puJetIdCHS')


########################################################
## Path definition
########################################################

process.dump = cms.EDAnalyzer('EventContentAnalyzer')

process.p = cms.Path(
    process.CMGSequence + 
    process.jetCHSSequence 
    )

########################################################
## CMG output definition
########################################################

from CMGTools.Common.eventContent.patEventContentCMG_cff import everything
process.outcmg = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('cmgTuple_reload.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    outputCommands = everything,
    dropMetaData = cms.untracked.string('PRIOR')
    )

process.outpath = cms.EndPath(process.outcmg)

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10

## Options and Output Report
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

print sep_line

print 'starting CMSSW'
