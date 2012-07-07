import FWCore.ParameterSet.Config as cms
import os

process = cms.Process("FLATNTP")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.maxLuminosityBlocks = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
evReportFreq = 100

#######Define the samples to process
dataset_user = 'benitezj'
#sampleTag = "/PAT_CMG_V5_4_1/H2TAUTAU_TauMu_V541June2"
#sampleTag = "/PAT_CMG_V5_4_0_NewType1MET/H2TAUTAU_TauMu2012_V5_4_0_NewType1MET"
#sampleTag = "/H2TAUTAU_TauMu2012_V5_4_0_NewType1MET"
sampleTag = "/TauMu2012V540"


sampleName = os.environ['SAMPLENAME']
sampleJobIdx = int(os.environ['SAMPLEJOBIDX'])
sampleMergeFactor = int(os.environ['SAMPLEMERGEFACTOR'])

#########################

process.analysis = cms.Path() 


##create the good primary vertices
process.load("CommonTools.ParticleFlow.goodOfflinePrimaryVertices_cfi")
process.analysis += process.goodOfflinePrimaryVertices


######The analysis module
process.load('CMGTools.H2TauTau.tools.joseFlatNtpSample_cfi')
from CMGTools.H2TauTau.tools.joseFlatNtpSample2011Reload_cff import configureFlatNtpSampleTauMu2012
configureFlatNtpSampleTauMu2012(process.flatNtpTauMu,sampleName)
process.flatNtpTauMu.diTauTag = 'cmgTauMuPreSel'
process.flatNtpTauMu.metType = 1
process.flatNtpTauMu.runSVFit = 1



### input files
inputfiles = "tauMu_fullsel_tree_CMG_.*root"
dataset_name = process.flatNtpTauMu.path.value() + sampleTag
firstfile = sampleJobIdx * sampleMergeFactor
lastfile = (sampleJobIdx + 1 ) * sampleMergeFactor
print dataset_user
print dataset_name
print inputfiles
print firstfile
print lastfile


#get input files
from CMGTools.Production.datasetToSource import *
process.source = datasetToSource( dataset_user, dataset_name, inputfiles)
process.source.fileNames = process.source.fileNames[firstfile:lastfile]

#process.source.eventsToProcess = cms.untracked.VEventRange('1:15599','1:20873','1:20916','1:20991','1:21008')
#print process.source.eventsToProcess

#process.source = cms.Source(
#    "PoolSource",
#    fileNames = cms.untracked.vstring(
#    #'/store/cmst3/user/cmgtools/CMG/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_2_0/cmgTuple_0.root'
#    #'/store/cmst3/user/cmgtools/CMG/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1/cmgTuple_0.root'
#    #'file:../../../Common/prod/TEST/cmgTuple_HToTauTau.root'
#    'file:./tauMu_fullsel_tree_CMG.root'
#    )
#    )

#process.source.fileNames = ['file:./H2TAUTAU_TauMu2012_V5_4_0_NewType1MET/tauMu_fullsel_tree_CMG.root']

print process.source.fileNames


##create the vertex weights
if process.flatNtpTauMu.dataType == 0:
    process.load('CMGTools.Common.generator.vertexWeight.vertexWeight_cff')
    process.genSequence = cms.Sequence(
        process.vertexWeightSequence 
        )
    process.analysis += process.genSequence 



##schedule the analyzer
process.analysis += process.flatNtpTauMu
process.schedule = cms.Schedule(process.analysis)
process.TFileService = cms.Service("TFileService", fileName = cms.string("flatNtp.root"))


#####################################################
process.MessageLogger = cms.Service("MessageLogger",
    cerr = cms.untracked.PSet(
    FwkReport = cms.untracked.PSet(
    reportEvery = cms.untracked.int32(evReportFreq),
    optionalPSet = cms.untracked.bool(True),
    limit = cms.untracked.int32(10000000)
    ),
    optionalPSet = cms.untracked.bool(True),
    FwkJob = cms.untracked.PSet(
    optionalPSet = cms.untracked.bool(True),
    limit = cms.untracked.int32(0)
    ),    
    )
)

#process.source.duplicateCheckMode = cms.untracked.string("noDuplicateCheck")
#print process.dumpPython()
