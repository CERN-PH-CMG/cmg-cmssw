from PhysicsTools.PatAlgos.patTemplate_cfg import *

##########


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.maxLuminosityBlocks = cms.untracked.PSet( 
    input = cms.untracked.int32(-1)
    )


##########

# Message logger setup.
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

doSkimHighMET = False
cut_highMET = 'pt()>50'
process.setName_('ANA')

#COLIN: tell Will to remove the rather printouts (or rather to add an enable flag allowing to get these printouts). What are these printouts? 

from CMGTools.Susy.LP.sourcesW import inputFiles
#COLIN put relval source files in a standard place. 
process.source = cms.Source(
    "PoolSource",
    
    noEventSort = cms.untracked.bool(True),
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    fileNames = cms.untracked.vstring()
    )
process.source.fileNames.extend( inputFiles )

ext = 'CMG'

# output to be stored

print 'processing:'
print process.source.fileNames

outFileNameExt = ext

process.load('CMGTools.Susy.susyCommon_cff')
process.LPPath = cms.Path(process.analysisSequence * process.LPSkimSequence)
# pprint.pprint(process.out.outputCommands)
#process.dump = cms.EDAnalyzer("EventContentAnalyzer")

from CMGTools.Susy.susyEventContent_cff import susyEventContent
process.out.fileName = cms.untracked.string('susy_tree_%s.root' %  outFileNameExt)
process.out.outputCommands = cms.untracked.vstring('drop *')
process.out.outputCommands += susyEventContent

process.out.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('LPPath') )
#plot the correlations between the selection paths

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("susy_histograms_%s.root" %  outFileNameExt)
    )

print process.out.dumpPython()

print 'output file: ', process.out.fileName
# print process.schedule
