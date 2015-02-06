
import FWCore.ParameterSet.Config as cms

sep_line = "-" * 50
print
print sep_line
print "CMGTools main test"
print sep_line

process = cms.Process('ANA')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(2000)
    )

process.maxLuminosityBlocks = cms.untracked.PSet( 
    input = cms.untracked.int32(-1)
    )


runOnMC = False



from CMGTools.Production.datasetToSource import *
# process.source = datasetToSource(
#    'cmgtools_group',
#    '/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3/TestMVAs',
#    'patTuple_PF2PAT.*root'
#    ) 
process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring( 'file:../prod/patTuple_data.root' )
    ) 

# reading the first 10 files:
nFiles = 10
print 'WARNING: RESTRICTING INPUT TO THE FIRST', nFiles, 'FILES'
process.source.fileNames = process.source.fileNames[:nFiles] 


print process.source.fileNames

# output module for EDM event (ntuple)
process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('tree_testCMGTools.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    outputCommands = cms.untracked.vstring( 'drop *')
    )
from CMGTools.Common.eventContent.everything_cff import everything 
process.out.outputCommands.extend( everything )
process.out.outputCommands.append( 'drop cmg*_*_*_PAT' )
process.out.outputCommands.append( 'keep cmgMETSignificance_*_*_PAT' )
process.out.outputCommands.append( 'keep cmgBaseMETs_cmgPFMETType1Corrected*_*_PAT' )

process.out.dropMetaData = cms.untracked.string('PRIOR')

#output file for histograms etc
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histograms_testCMGTools.root"))

# default analysis sequence    
process.load('CMGTools.Common.analysis_cff')

# adding no PU Sub sequence
from CMGTools.Common.Tools.visitorUtils import replacePostfix
from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet
cloneProcessingSnippet(process, getattr(process, 'analysisSequence'), 'AK5NoPUSubCMG')
replacePostfix(getattr(process,"analysisSequenceAK5NoPUSubCMG"),'AK5','AK5NoPUSub') 

# but we need only the jets and taus:
from CMGTools.Common.Tools.tuneCMGSequences import * 
tuneCMGSequences(process, postpostfix='CMG')

# adding the standard leptons: 
from CMGTools.Common.PAT.addStdLeptons import addCmgMuons, addCmgElectrons
process.cmgStdLeptonSequence = cms.Sequence(
    addCmgMuons( process, 'StdLep', 'selectedPatMuons'  ) +
    addCmgElectrons( process, 'StdLep', 'selectedPatElectrons'  ) 
    )
process.cmgObjectSequence += process.cmgStdLeptonSequence


process.p = cms.Path(
    process.analysisSequence +
    process.analysisSequenceAK5NoPUSubCMG
    )

process.outpath = cms.EndPath(process.out)


from Configuration.AlCa.autoCond import autoCond
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from CMGTools.Common.Tools.getGlobalTag import getGlobalTag
process.GlobalTag.globaltag = cms.string(getGlobalTag(runOnMC))

process.schedule = cms.Schedule(
    process.p,
    process.outpath
    )

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) ) 

