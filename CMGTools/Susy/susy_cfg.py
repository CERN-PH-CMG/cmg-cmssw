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

process.load("CMGTools.Common.sources.HT.Run2011A_PromptReco_v1.AOD.PAT_CMG.source_tree_cff")

ext = 'CMG'

# output to be stored

print 'processing:'
print process.source.fileNames

outFileNameExt = ext

process.load('CMGTools.Susy.susy_cff')

# process.susySchedule.remove( process.runInfoAccountingPath )

process.susySchedule.append( process.outpath )

# pprint.pprint(process.out.outputCommands)
#process.dump = cms.EDAnalyzer("EventContentAnalyzer")

from CMGTools.Susy.susyEventContent_cff import susyEventContent
process.out.fileName = cms.untracked.string('susy_tree_%s.root' %  outFileNameExt)
process.out.outputCommands = cms.untracked.vstring('drop *')
process.out.outputCommands += susyEventContent

process.out.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('RA2Path','razorPath', 'RA1Path', 'LPPath') )
#plot the correlations between the selection paths
process.load('CMGTools.Common.histograms.triggerCorrelationHistograms_cfi')
process.triggerCorrelationHistograms.names = cms.untracked.vstring('RA2Path','razorPath', 'RA1Path', 'LPPath')
process.susySchedule.append( process.triggerCorrelationHistogramsEndPath )

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("susy_histograms_%s.root" %  outFileNameExt)
    )

print process.out.dumpPython()

print 'output file: ', process.out.fileName
# print process.schedule
