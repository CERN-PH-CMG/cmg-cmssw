######################################################################

from PhysicsTools.PatAlgos.patTemplate_cfg import *

##########

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(1)
    )

##########

# Message logger setup.
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.setName_('ANA')

process.load("CMGTools.Common.sources.HT.Run2011A_PromptReco_v1.AOD.PAT_CMG.source_tree_cff")

ext = 'CMG'

print "Processing:"
print process.source.fileNames

outFileNameExt = ext

process.load('CMGTools.Susy.susy_cff')
# Run only RA1.
process.schedule = cms.Schedule(process.RA1Path)
process.schedule.append(process.outpath)
process.out.SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('RA1Path'))

# Setup event content.
from CMGTools.Susy.susyEventContent_cff import susyEventContent
process.out.fileName = cms.untracked.string('susy_tree_%s.root' %  outFileNameExt)
process.out.outputCommands = cms.untracked.vstring('drop *')
process.out.outputCommands += susyEventContent

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("susy_histograms_%s.root" %  outFileNameExt)
    )

print process.out.dumpPython()

print "Output file: '%s'" % process.out.fileName

######################################################################
