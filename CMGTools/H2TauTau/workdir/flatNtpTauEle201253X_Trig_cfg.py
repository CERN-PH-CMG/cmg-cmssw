import FWCore.ParameterSet.Config as cms
import os

process = cms.Process("FLATNTP")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.maxLuminosityBlocks = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
evReportFreq = 100

dataset_user  = os.environ['SAMPLEOWNER']
sampleName = os.environ['SAMPLENAME']
sampleJobIdx = int(os.environ['SAMPLEJOBIDX'])
sampleMergeFactor = int(os.environ['SAMPLEMERGEFACTOR'])


#########################
process.analysis = cms.Path() 


######The analysis module
process.load('CMGTools.H2TauTau.tools.joseFlatNtpSample_cfi')
process.flatNtp = process.flatNtpTauEle.clone()
from CMGTools.H2TauTau.tools.joseFlatNtpSample53X_cff import configureFlatNtpSampleTauEle2012Trig
configureFlatNtpSampleTauEle2012Trig(process.flatNtp,sampleName)
process.flatNtp.diTauTag = 'cmgTauElePreSel'
process.flatNtp.metType = 2
process.flatNtp.runSVFit = 1


### input files
inputfiles = "cmgTuple_.*root"
dataset_name = process.flatNtp.path.value() 
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

# set up JSON ---------------------------------------------------------------
if process.flatNtp.dataType != 0 :
   json = process.flatNtp.jsonfile.value()
   print 'json:', json
   from CMGTools.Common.Tools.applyJSON_cff import applyJSON
   applyJSON(process, json )
   print process.PoolSource.lumisToProcess


# run the vertex weights
if process.flatNtp.dataType == 0:
   process.load('CMGTools.RootTools.utils.vertexWeight.vertexWeight_cff')
   process.genSequence = cms.Sequence(
      process.vertexWeightSequence 
      )
   process.analysis += process.genSequence 
   

##create the good primary vertices
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
process.cmgTauElePreSel.cut = cms.string('leg1().eta()!=leg2().eta() && leg1().pt()>10.0 && abs(leg1().eta())<2.3 && leg1().tauID("decayModeFinding")>0.5 && leg1().tauID("byRawIsoMVA")>0.0 && leg2().pt()>27.0 && abs(leg2().eta())<2.1' )
process.analysis +=  process.cmgTauElePreSel 


# event filter --------------------------------
process.load('CMGTools.Common.skims.cmgTauEleCount_cfi')
process.cmgTauEleCount.src = 'cmgTauElePreSel'
process.cmgTauEleCount.minNumber = 1
process.analysis +=  process.cmgTauEleCount


##run the MVA MET and remake the mu-tau list
if process.flatNtp.metType == 2:
   process.load("CMGTools.Common.eventCleaning.goodPVFilter_cfi")
   process.load("CMGTools.Common.miscProducers.mvaMET.mvaMETTauEle_cfi")
   process.mvaMETTauEle.recBosonSrc = 'cmgTauElePreSel'
   process.load("CMGTools.Common.factories.cmgBaseMETFromPFMET_cfi")
   process.mvaBaseMETTauEle = process.cmgBaseMETFromPFMET.clone()
   process.mvaBaseMETTauEle.cfg.inputCollection = 'mvaMETTauEle'
   process.load("CMGTools.Common.factories.cmgTauEleCor_cfi")
   process.cmgTauEleCorPreSel = process.cmgTauEleCor.clone()
   process.cmgTauEleCorPreSel.cfg.metCollection = 'mvaBaseMETTauEle'
   process.cmgTauEleCorPreSel.cfg.diObjectCollection = 'cmgTauElePreSel'
   process.mvaMETSequence = cms.Sequence(
      process.goodPVFilter + 
      process.mvaMETTauEle +
      process.mvaBaseMETTauEle +
      process.cmgTauEleCorPreSel
      )
   process.analysis  += process.mvaMETSequence
   process.flatNtp.diTauTag = 'cmgTauEleCorPreSel'
   


# schedule the analyzer
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
