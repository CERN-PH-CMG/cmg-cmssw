# CFG to add the vertex weight collection to the cmgtuple. 

# WARNING: I THINK THERE MIGHT BE A MEMORY LEAK IN THE CODE RAN BY THIS CFG,
# AS SEVERAL JOBS CRASHED WITH BAD_ALLOC (1/1000 JOBS).
# TIP: GROUP YOUR INPUT FILES BY 20 IN PRODUCTIONTASKS.PY, NOT MORE

import FWCore.ParameterSet.Config as cms

process = cms.Process("VERTEX")


from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    'cmgtools',
    '/W2Jets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B',
    'cmgTuple.*root') 


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5000) )
process.source.fileNames = process.source.fileNames[:12]
print process.source.fileNames

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))

process.load('CMGTools.RootTools.utils.vertexWeight.vertexWeight_cff')
process.p = cms.Path(
    process.vertexWeightSequence
    )

process.out = cms.OutputModule(
    "PoolOutputModule",
    outputCommands =  cms.untracked.vstring(
    'keep *',
    ),
    fileName = cms.untracked.string('cmgTuple.root'),
    )

process.endpath = cms.EndPath(
    process.out
    )


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 5000
