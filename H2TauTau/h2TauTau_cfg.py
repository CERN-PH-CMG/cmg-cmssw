from PhysicsTools.PatAlgos.patTemplate_cfg import *

##########


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
    )

numberOfFilesToProcess = 10

dataset_user = 'cmgtools' 
# dataset_name = '/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_3_0'
dataset_name = '/TauPlusX/Run2011A-PromptReco-v6/AOD/V2/PAT_CMG_V2_3_0'

##########


# Input  & JSON             -------------------------------------------------


process.setName_('H2TAUTAU')

from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    dataset_user,
    dataset_name,
    'tree.*root') 


process.source.fileNames = process.source.fileNames[:numberOfFilesToProcess]

if dataset_name.find('Run201')>-1:
    from CMGTools.Common.Tools.applyJSON_cff import *
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Prompt/Cert_160404-178078_7TeV_PromptReco_Collisions11_JSON.txt'
    applyJSON(process, json )

print process.source

# Sequence & path definition -------------------------------------------------

process.load('CMGTools.H2TauTau.Colin.h2TauTau_cff')


process.schedule = cms.Schedule(
    process.tauMuPath,    
    # process.tauEPath,
    process.outpath
    )




# OUTPUT definition ----------------------------------------------------------

outFileNameExt = 'CMG'
process.out.fileName = cms.untracked.string('h2TauTau_tree_%s.root' %  outFileNameExt)
from CMGTools.H2TauTau.eventContent.tauMu_cff import tauMu as tauMuEventContent
process.out.outputCommands.extend( tauMuEventContent ) 
process.out.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('tauMuPath') )

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("h2TauTau_histograms_%s.root" %  outFileNameExt)
    )

print process.out.dumpPython()

print 'output file: ', process.out.fileName


# Message logger setup.
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
