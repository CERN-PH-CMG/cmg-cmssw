import FWCore.ParameterSet.Config as cms
import os

process = cms.Process("FLATNTP")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.maxLuminosityBlocks = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
evReportFreq = 100

#######Define the samples to process
dataset_user = 'benitezj'
#sampleTag = "/H2TAUTAU_TauEle_V541June29"
sampleTag = "/TauEleSkimJuly23"


sampleName = os.environ['SAMPLENAME']
sampleJobIdx = int(os.environ['SAMPLEJOBIDX'])
sampleMergeFactor = int(os.environ['SAMPLEMERGEFACTOR'])
print sampleName
print sampleJobIdx
print sampleMergeFactor


######The analysis module
process.load('CMGTools.H2TauTau.tools.joseFlatNtpSample_cfi')
process.flatNtp = process.flatNtpTauEle.clone()
from CMGTools.H2TauTau.tools.joseFlatNtpSample_cff import configureFlatNtpSampleTauEle2011
configureFlatNtpSampleTauEle2011(process.flatNtp,sampleName)
process.flatNtp.diTauTag = 'cmgTauElePreSel'
process.flatNtp.metType = 1
process.flatNtp.runSVFit = 2


######Define the input files
inputfiles = "tauEle_fullsel_tree_CMG_.*root"
dataset_name = process.flatNtp.path.value() + sampleTag
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
print process.source.fileNames

#process.source.eventsToProcess = cms.untracked.VEventRange(
#'1:6068',
#'1:6099',
#'1:6580',
#'1:15132',
#'1:15150',
#'1:15362',
#'1:20565',
#'1:21111',
#'1:21127',
#'1:28242',
#'1:28490',
#'1:28653',
#'1:28762',
#'1:39658',
#'1:39959',
#'1:40145',
#'1:47160',
#'1:58271',
#'1:62162',
#'1:62337',
#'1:81878',
#'1:82084',
#'1:88471',
#'1:88704',
#'1:102863',
#'1:102946',
#'1:103290',
#'1:105123',
#'1:121657',
#'1:163950',
#'1:172248',
#'1:172411',
#'1:172498',
#'1:172509',
#'1:178461',
#'1:184088',
#'1:187591',
#'1:187788',
#'1:190397')

#process.source.fileNames = ['file:./tauEle_fullsel_tree_CMG.root']


#########################
process.analysis = cms.Path() 

# set up JSON ---------------------------------------------------------------
if process.flatNtp.dataType != 0 :
   from CMGTools.H2TauTau.tools.setupJSON import setupJSON
   json = setupJSON(process)
   print 'json:', json
   print process.PoolSource.lumisToProcess

# run the vertex weights
if process.flatNtp.dataType == 0:
   process.load('CMGTools.RootTools.utils.vertexWeight.vertexWeight_cff')
   process.genSequence = cms.Sequence(
      process.vertexWeightSequence 
      )
   process.analysis += process.genSequence 
   

##create the cleaned primary vertices
process.load("CommonTools.ParticleFlow.goodOfflinePrimaryVertices_cfi")
process.analysis += process.goodOfflinePrimaryVertices

##create mu-tau candidates
process.load('CMGTools.Common.factories.cmgTauScaler_cfi')
process.analysis +=  process.cmgTauScaler 
#process.cmgTauScaler.cfg.uncertainty = 0.03
#process.cmgTauScaler.cfg.nSigma = 1.0

process.load('CMGTools.Common.factories.cmgTauEle_cfi')
process.cmgTauEle.cfg.leg1Collection = 'cmgTauScaler'
process.cmgTauEle.cfg.metCollection = 'cmgPFMETRaw'
process.analysis +=  process.cmgTauEle

process.load('CMGTools.Common.skims.cmgTauEleSel_cfi')
process.cmgTauElePreSel = process.cmgTauEleSel.clone()
process.cmgTauElePreSel.cut = cms.string('pt()>0.0' )
process.analysis +=  process.cmgTauElePreSel 


##schedule the analyzer
process.analysis += process.flatNtp
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
