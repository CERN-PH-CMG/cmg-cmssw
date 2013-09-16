import FWCore.ParameterSet.Config as cms

sep_line = '-'*67
print sep_line
print 'CMG-tuplizer, contact Colin before any modification'
print sep_line

process = cms.Process("CMG")


print 'querying database for source files'


runOnMC = True

datasetInfo = ('cmgtools_group', '/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B','.*root')

process.source = cms.Source(
    'PoolSource',
    fileNames = cms.untracked.vstring('file:patTuple.root')
    )


###ProductionTaskHook$$$


# drop all CMG objects from the input, to recreate them.
# This is important if there was a change in AnalysisDataFormats after the patTuple was create.
# It would result in a segmentation violation otherwiese.
process.source.inputCommands=cms.untracked.vstring('keep *',
    #'drop cmgTriggerObjects_*_*_*',
    'drop cmgMuons_*_*_*',
    'drop cmgElectrons_*_*_*',
    'drop cmgPhotons_*_*_*',
    # 'drop cmgBaseJets_*_*_*',
    'drop cmgPFJets_*_*_*',
    'drop cmgTaus_*_*_*',
    # 'drop cmgBaseMETs_*_*_*',
    'drop cmgPFMETs_*_*_*',
    )

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
    process.CMGSequence.remove(process.genSequence)
    
print 'cloning the jet sequence to build PU chs jets'

from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet
process.jetCHSSequence = cloneProcessingSnippet(process, process.jetSequence, 'CHS')
from CMGTools.Common.Tools.visitorUtils import replaceSrc
replaceSrc( process.jetCHSSequence, 'patJetsWithVar', 'patJetsWithVarCHS')
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


########################################################
## Conditions 
########################################################

from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

from CMGTools.Common.Tools.getGlobalTag import getGlobalTag
from CMGTools.Common.Tools.getGlobalTag import getGlobalTagByDataset
process.GlobalTag.globaltag = getGlobalTagByDataset( runOnMC, datasetInfo[1])
print 'Global tag       : ', process.GlobalTag.globaltag

