from PhysicsTools.PatAlgos.patTemplate_cfg import *

##########


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

##########

# Message logger setup.
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )


process.setName_('H2TAUTAU')

from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    'cmgtools',
    # '/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_3_0',
    # '/VBF_HToTauTau_M-115_7TeV-powheg-pythia6-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_3_0',
    '/TauPlusX/Run2011A-PromptReco-v6/AOD/V2/PAT_CMG_V2_3_0/HTAUTAU',
    'tree.*root') 

process.source.fileNames = process.source.fileNames[:20]

print 'processing:'
print process.source.fileNames

ext = 'CMG'

outFileNameExt = ext

process.load('CMGTools.H2TauTau.Colin.h2TauTau_cff')

process.cmgTauMuSel.cut = ''

process.schedule = cms.Schedule(
    process.tauMuPath,    
    # process.tauEPath,
    process.outpath
    )


process.out.fileName = cms.untracked.string('h2TauTau_tree_%s.root' %  outFileNameExt)
from CMGTools.H2TauTau.eventContent.tauMu_cff import tauMu as tauMuEventContent
process.out.outputCommands.extend( tauMuEventContent ) 
# process.out.outputCommands = ['drop *']
# process.out.dropMetaData = cms.untracked.string('ALL')

process.out.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('tauMuPath') )

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("h2TauTau_histograms_%s.root" %  outFileNameExt)
    )

print process.out.dumpPython()

print 'output file: ', process.out.fileName
