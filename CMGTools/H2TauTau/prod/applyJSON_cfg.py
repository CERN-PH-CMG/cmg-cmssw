import FWCore.ParameterSet.Config as cms

sep_line = "-" * 50
print
print sep_line
print "CMGTools JSON"
print "For a speedy analysis, better to apply that at the very end on your final sample!"
print sep_line

process = cms.Process("JSON")


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

process.maxLuminosityBlocks = cms.untracked.PSet( 
    input = cms.untracked.int32(-1)
    )

from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    'cmgtools',
    '/TauPlusX/Run2011A-PromptReco-v6/AOD/V2/PAT_CMG_V2_3_0',
    'tree.*root') 


from CMGTools.Common.Tools.applyJSON_cff import *

# always check that your JSON is compatible with your source...

json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Prompt/Cert_160404-178078_7TeV_PromptReco_Collisions11_JSON.txt'
applyJSON(process, json )

print process.source


process.out = cms.OutputModule(
    "PoolOutputModule",
    outputCommands =  cms.untracked.vstring( 'keep *'),
    fileName = cms.untracked.string('json.root'),
    )

process.endpath = cms.EndPath(
    process.out
    )

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) ) 
