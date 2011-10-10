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
process.setName_('SUSY')

# process.load("CMGTools.Common.sources.HT.Run2011A_PromptReco_v1.AOD.PAT_CMG.source_tree_cff")
process.load("CMGTools.Common.sources.HT.Run2011A_May10ReReco_v1.AOD.PAT_CMG.source_cff")
#process.load("CMGTools.Common.sources.LM6_SUSY_sftsht_7TeV_pythia6.Summer11_PU_S4_START42_V11_v1.AODSIM.V2.PAT_CMG_V2_2_0.source_cff")
process.load("CMGTools.Common.sources.RelValTTbar.CMSSW_4_2_3_START42_V12_v2.GEN_SIM_RECO.PAT_CMG_V2_3_0.source_cff")
process.source.fileNames = ['file:tree_CMG.root']

ext = 'CMG'

# output to be stored

print 'processing:'
print process.source.fileNames

outFileNameExt = ext

process.load('CMGTools.Susy.susy_cff')

# process.susySchedule.remove( process.runInfoAccountingPath )

process.schedule = cms.Schedule(
    process.multijetPath,
    process.multijetTriggerPath,                                
    process.RA1Path,
    process.RA2Path,
    process.razorPath,
    process.razorEleMuPath,
    process.razorTriggerPath,
    process.LPPath,
    process.outpath
    )

# pprint.pprint(process.out.outputCommands)
#process.dump = cms.EDAnalyzer("EventContentAnalyzer")

from CMGTools.Susy.susyEventContent_cff import susyEventContent
process.out.fileName = cms.untracked.string('susy_tree_%s.root' %  outFileNameExt)
process.out.outputCommands = cms.untracked.vstring('drop *')
process.out.outputCommands += susyEventContent
from CMGTools.Common.eventContent.eventCleaning_cff import eventCleaning
process.out.outputCommands.extend( eventCleaning )

process.out.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('multijetPath','multijetTriggerPath','RA2Path','razorEleMuPath','razorPath','razorTriggerPath', 'RA1Path', 'LPPath') )
#plot the correlations between the selection paths
process.load('CMGTools.Common.histograms.triggerCorrelationHistograms_cfi')
process.triggerCorrelationHistograms.names = cms.untracked.vstring('multijetPath','multijetTriggerPath','RA2Path','razorEleMuPath','razorPath','razorTriggerPath', 'RA1Path', 'LPPath')
process.schedule.append( process.triggerCorrelationHistogramsEndPath )

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("susy_histograms_%s.root" %  outFileNameExt)
    )

print process.out.dumpPython()

print 'output file: ', process.out.fileName
# print process.schedule
