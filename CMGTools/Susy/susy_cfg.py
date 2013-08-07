from PhysicsTools.PatAlgos.patTemplate_cfg import *

from CMGTools.Production.datasetToSource import *
#datasetInfo = ('cmgtools', '/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0','patTuple_[0-9]+\\.root')
datasetInfo = ('cmgtools', '/SMS-T2tt_FineBin_Mstop-225to1200_mLSP-0to1000_8TeV-Pythia6Z/Summer12-START52_V9_FSIM-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B','patTuple_[0-9]+\\.root')
process.source = datasetToSource(
    *datasetInfo
    )
process.source.fileNames = process.source.fileNames[:1]
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(200) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

###ProductionTaskHook$$$
runOnMC = 'START5' in datasetInfo[1]
### Set the global tag from the dataset name
from CMGTools.Common.Tools.getGlobalTag import getGlobalTagByDataset
process.GlobalTag.globaltag = getGlobalTagByDataset( runOnMC, datasetInfo[1])
print 'Global tag       : ', process.GlobalTag.globaltag
###

##########
from CMGTools.Common.Tools.applyJSON_cff import applyJSON
json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-208686_8TeV_PromptReco_Collisions12_JSON.txt'
if not runOnMC:
    applyJSON(process, json )

##########
skimEvents = True
runPAT = False
# Message logger setup.
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

process.setName_('MJSkim')

ext = 'CMG'

# output to be stored

print 'processing:'
print process.source.fileNames

outFileNameExt = ext

#rerun the cmg stuff
process.p = cms.Path()

if runPAT:
    process.load('CMGTools.Common.PAT.PATCMG_cff')

    if not runOnMC:
        # removing MC stuff
        print 'removing MC stuff, as we are running on Data'
        process.PATCMGSequence.remove(process.PATCMGGenSequence)

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
    process.trkVetoLeptonSequencePath,
    process.outpath
    )
if runOnMC:
    process.p += process.susyGenSequence
else:
    process.p += process.susyDataSequence

#don't know where this comes from, but it screws things up and we don't use it
del process.eIdSequence

from CMGTools.Susy.susyEventContent_cff import susyEventContent
process.out.fileName = cms.untracked.string('susy_tree_%s.root' %  outFileNameExt)
process.out.outputCommands = cms.untracked.vstring('drop *')
if runPAT:
    process.out.outputCommands.extend(cms.untracked.vstring('drop cmg*_*_*_PAT'))
from CMGTools.Common.eventContent.eventCleaning_cff import eventCleaning
process.out.outputCommands.extend( eventCleaning )
process.out.outputCommands += susyEventContent

SelectEvents = cms.vstring('razorMJSkimSequenceHadPath','razorMJSkimSequenceElePath','razorMJSkimSequenceMuPath')
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

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
