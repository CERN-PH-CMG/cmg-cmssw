from PhysicsTools.PatAlgos.patTemplate_cfg import *

##########
runOnMC = True
from CMGTools.Common.Tools.applyJSON_cff import applyJSON
json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Prompt/Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON.txt'
if not runOnMC:
    applyJSON(process, json )
##########
skimEvents = True

##########
from CMGTools.Common.Tools.getGlobalTag import getGlobalTag
process.GlobalTag.globaltag = cms.string(getGlobalTag(runOnMC))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )
##########

# Message logger setup.
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

process.setName_('MJSkim')

from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    'cmgtools',
    #'/MultiJet/Run2011A-PromptReco-v4/AOD/V2/PAT_CMG_V2_4_0',
    '/SMS-T2tt_Mstop-225to1200_mLSP-50to1025_7TeV-Pythia6Z/Summer11-PU_START42_V11_FastSim-v1/AODSIM/V2/PAT_CMG_V2_4_0',
    'patTuple_PF2PAT_[0-9]+\\.root'
    ) 
#process.source.fileNames = process.source.fileNames[:10]

ext = 'CMG'

# output to be stored

print 'processing:'
print process.source.fileNames

outFileNameExt = ext

#rerun the cmg stuff
process.p = cms.Path()
process.load('CMGTools.Common.analysis_cff')
from CMGTools.Common.Tools.tuneCMGSequences import * 
tuneCMGSequences(process, postpostfix='CMG')
process.p += process.analysisSequence

process.load('CMGTools.Susy.susy_cff')
process.load('CMGTools.Susy.common.susy_cff')
process.schedule = cms.Schedule(
    process.p,
    process.multijetPath,
    process.multijetPathNoTrigger,
    process.multijetPathMultijetTrigger,
    process.multijetPathRazorTrigger,
    process.multijetPathHTTrigger,
    process.multijetPathL1Passthrough,
    process.multijetPathL1Seed,
    process.outpath
    )
if runOnMC:
    process.p += process.susyGenSequence
else:
    process.p += process.susyDataSequence

from CMGTools.Susy.susyEventContent_cff import susyEventContent
process.out.fileName = cms.untracked.string('susy_tree_%s.root' %  outFileNameExt)
process.out.outputCommands = cms.untracked.vstring('drop *')
from CMGTools.Common.eventContent.eventCleaning_cff import eventCleaning
process.out.outputCommands.extend( eventCleaning )
process.out.outputCommands += susyEventContent

SelectEvents = cms.vstring('multijetPath','multijetPathNoTrigger','multijetPathMultijetTrigger',\
                               'multijetPathRazorTrigger','multijetPathL1Passthrough','multijetPathL1Seed','multijetPathHTTrigger')
if not skimEvents:
    SelectEvents.append('p')
process.out.SelectEvents = cms.untracked.PSet( SelectEvents = SelectEvents )


#plot the correlations between the selection paths
process.load('CMGTools.Common.histograms.triggerCorrelationHistograms_cfi')
process.triggerCorrelationHistograms.names = cms.untracked.vstring( SelectEvents )
process.schedule.append( process.triggerCorrelationHistogramsEndPath )

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("susy_histograms_%s.root" %  outFileNameExt)
    )

print 'output file: ', process.out.fileName
