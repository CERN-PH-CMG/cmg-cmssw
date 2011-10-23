from PhysicsTools.PatAlgos.patTemplate_cfg import *

##########


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

numberOfFilesToProcess = -1

dataset_user = 'cbern' 
# dataset_name = '/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_3_0'
dataset_name = '/TauPlusX/Run2011A-PromptReco-v6/AOD/V2/PAT_CMG_V2_3_0/H2TAUTAU'
dataset_pattern = '.*tree.*root'

applyJson = True

##########


# Input  & JSON             -------------------------------------------------


process.setName_('BASELINE')

from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    dataset_user,
    dataset_name,
    dataset_pattern) 



if numberOfFilesToProcess > 0:
    process.source.fileNames = process.source.fileNames[:numberOfFilesToProcess]

if applyJson and dataset_name.find('Run201')>-1:
    from CMGTools.Common.Tools.applyJSON_cff import *
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Prompt/Cert_160404-178078_7TeV_PromptReco_Collisions11_JSON.txt'
    applyJSON(process, json )

print process.source

# Sequence & path definition -------------------------------------------------

process.load('CMGTools.H2TauTau.Colin.h2TauTau_cff')

process.tauMuBaselinePath = cms.Path(
    process.cmgTauMuBaselineSel +
    process.cmgTauMuBaselineCount
    )

# OUTPUT definition ----------------------------------------------------------

# baseline

process.outBaseline = process.out.clone()
process.outBaseline.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('tauMuBaselinePath') )
process.outBaseline.fileName = 'h2TauTau_baseline_tree_CMG.root' 
process.outBaseline.outputCommands = ['keep *']

process.outpath = cms.EndPath( process.outBaseline ) 

# Message logger setup.
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.schedule = cms.Schedule(
    process.tauMuBaselinePath,    
    process.outpath
    )
