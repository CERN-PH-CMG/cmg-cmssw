from PhysicsTools.PatAlgos.patTemplate_cfg import *

##########
runOnMC = False
from CMGTools.Common.Tools.applyJSON_cff import applyJSON
json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Prompt/Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON.txt'
if not runOnMC:
    applyJSON(process, json )


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

##########

# Message logger setup.
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

doSkimHighMET = False
cut_highMET = 'pt()>50'
#process.setName_('SUSY')
process.setName_('MJSkim')

from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    'wreece',
    '/MultiJet/Run2011A-PromptReco-v4/AOD/V2/PAT_CMG_V2_4_0/L1MultiJetSkim',
    #'/TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_3_0',
    'tree_CMG_[0-9]+\\.root'
    ) 
#process.source.fileNames = ['file:tree_CMG.root']
#process.source.fileNames = process.source.fileNames[:10]

ext = 'CMG'

# output to be stored

print 'processing:'
print process.source.fileNames

outFileNameExt = ext

process.load('CMGTools.Susy.susy_cff')

# process.susySchedule.remove( process.runInfoAccountingPath )

process.schedule = cms.Schedule(
#    process.leptonicStopPath,
#    process.leptonicStopTriggerPath,
    process.multijetPath,
    process.multijetTriggerPath,                                
#    process.RA1Path,
#    process.RA2Path,
#    process.razorPath,
#    process.razorEleMuPath,
#    process.razorTriggerPath,
#    process.LPPath,
    process.razorMJPath,
    process.outpath
    )

# pprint.pprint(process.out.outputCommands)
#process.dump = cms.EDAnalyzer("EventContentAnalyzer")

from CMGTools.Susy.susyEventContent_cff import susyEventContent
process.out.fileName = cms.untracked.string('susy_tree_%s.root' %  outFileNameExt)
process.out.outputCommands = cms.untracked.vstring('drop *')
from CMGTools.Common.eventContent.eventCleaning_cff import eventCleaning
process.out.outputCommands.extend( eventCleaning )
process.out.outputCommands += susyEventContent

#process.out.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('leptonicStopPath','leptonicStopTriggerPath','multijetPath','multijetTriggerPath','razorEleMuPath','razorPath','razorTriggerPath','razorMJPath','razorMJTriggerPath') )
process.out.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('multijetPath','razorMJPath') )

#plot the correlations between the selection paths
process.load('CMGTools.Common.histograms.triggerCorrelationHistograms_cfi')
#process.triggerCorrelationHistograms.names = cms.untracked.vstring('leptonicStopPath','leptonicStopTriggerPath','multijetPath','multijetTriggerPath','razorEleMuPath','razorPath','razorTriggerPath','razorMJPath','razorMJTriggerPath')
process.triggerCorrelationHistograms.names = cms.untracked.vstring('multijetPath','razorMJPath')
process.schedule.append( process.triggerCorrelationHistogramsEndPath )

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("susy_histograms_%s.root" %  outFileNameExt)
    )

print process.out.dumpPython()

print 'output file: ', process.out.fileName
# print process.schedule
