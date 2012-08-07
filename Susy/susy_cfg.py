from PhysicsTools.PatAlgos.patTemplate_cfg import *

##########
runOnMC = True
from CMGTools.Common.Tools.applyJSON_cff import applyJSON
json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Prompt/Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON.txt'
if not runOnMC:
    applyJSON(process, json )
##########
skimEvents = False
runPAT = True

##########
from CMGTools.Common.Tools.getGlobalTag import getGlobalTag
process.GlobalTag.globaltag = cms.string(getGlobalTag(runOnMC))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(200000) )

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
    '/TTJets_TuneZ2star_8TeV-madgraph-tauola/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_V5_4_0_NewType1MET',
    'patTuple_[0-9]+\\.root'
    #'cmgTuple_[0-9]+\\.root'
    ) 
#process.source.fileNames = process.source.fileNames[:10]

ext = 'CMG'

# output to be stored

print 'processing:'
print process.source.fileNames

outFileNameExt = ext

#rerun the cmg stuff
process.p = cms.Path()

if runPAT:
    process.load('CMGTools.Common.CMG.CMG_cff')

    if not runOnMC:
        # removing MC stuff
        print 'removing MC stuff, as we are running on Data'
        process.PATCMGSequence.remove(genSequence)

    print 'cloning the jet sequence to build PU chs jets'

    from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet
    process.jetCHSSequence = cloneProcessingSnippet(process, process.jetSequence, 'CHS')
    from CMGTools.Common.Tools.visitorUtils import replaceSrc
    replaceSrc( process.jetCHSSequence, 'selectedPatJets', 'selectedPatJetsCHS')
    replaceSrc( process.jetCHSSequence, 'puJetId', 'puJetIdCHS')

    process.p += process.CMGSequence
    process.p += process.jetCHSSequence

process.load('CMGTools.Susy.susy_cff')
process.load('CMGTools.Susy.common.susy_cff')
process.schedule = cms.Schedule(
    process.p,
    process.razorMJSkimSequenceHadPath,
    process.razorMJSkimSequenceElePath,
    process.razorMJSkimSequenceMuPath,
    process.razorMJSkimSequenceTauPath,
    process.outpath
    )
if runOnMC:
    process.p += process.susyGenSequence
else:
    process.p += process.susyDataSequence

from CMGTools.Susy.susyEventContent_cff import susyEventContent
process.out.fileName = cms.untracked.string('susy_tree_%s.root' %  outFileNameExt)
process.out.outputCommands = cms.untracked.vstring('drop *')
if runPAT:
    process.out.outputCommands.extend(cms.untracked.vstring('drop cmg*_*_*_PAT'))
from CMGTools.Common.eventContent.eventCleaning_cff import eventCleaning
process.out.outputCommands.extend( eventCleaning )
process.out.outputCommands += susyEventContent

SelectEvents = cms.vstring('razorMJSkimSequenceHadPath','razorMJSkimSequenceElePath','razorMJSkimSequenceMuPath','razorMJSkimSequenceTauPath')
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
