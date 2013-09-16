import FWCore.ParameterSet.Config as cms
import os

process = cms.Process("FLATNTP")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.maxLuminosityBlocks = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
evReportFreq = 100

#######Define the samples to process
#dataset_user = 'benitezj'
#sampleTag = "/PAT_CMG_V5_4_1/H2TAUTAU_TauMu_V541June2"
#sampleTag = "/PAT_CMG_V5_4_0_NewType1MET/H2TAUTAU_TauMu2012_V5_4_0_NewType1MET"

#for data and MC samples
#sampleTag = "/H2TAUTAU_TauMu2012_V5_4_0_NewType1MET"

#for embedded samples
#sampleTag = "/TauMu2012V540"

#dataset_user = 'cmgtools'
sampleTag = ""


dataset_user  = os.environ['SAMPLEOWNER']
sampleName = os.environ['SAMPLENAME']
sampleJobIdx = int(os.environ['SAMPLEJOBIDX'])
sampleMergeFactor = int(os.environ['SAMPLEMERGEFACTOR'])


#dataset_user  = 'cbern'
#sampleName = 'HiggsVBF125'
#sampleJobIdx = 1
#sampleMergeFactor = 1


#########################

process.analysis = cms.Path() 


######The analysis module
process.load('CMGTools.H2TauTau.tools.joseFlatNtpSample_cfi')
process.flatNtp = process.flatNtpTauMu.clone()
from CMGTools.H2TauTau.tools.joseFlatNtpSample_cff import configureFlatNtpSampleTauMu2012
configureFlatNtpSampleTauMu2012(process.flatNtp,sampleName)
process.flatNtp.diTauTag = 'cmgTauMuPreSel'
process.flatNtp.metType = 2
process.flatNtp.runSVFit = 2


### input files
#inputfiles = "tauMu_fullsel_tree_CMG_.*root"
inputfiles = "cmgTuple.*root"
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

#process.source.eventsToProcess = cms.untracked.VEventRange('1:15599','1:20873','1:20916','1:20991','1:21008')
#process.source.eventsToProcess = cms.untracked.VEventRange('1:20401','1:20509','1:20536','1:20611','1:20625')
#process.source.eventsToProcess = cms.untracked.VEventRange('1:20536','1:20611')
#print process.source.eventsToProcess

#*        0 *         1 *     20401 * 61.421115 *
#*        1 *         1 *     20509 * 59.559642 *
#*        2 *         1 *     20536 * 54.727180 *
#*        3 *         1 *     20611 * 148.97924 *
#*        4 *         1 *     20625 * 51.737155 *


#process.source = cms.Source(
#    "PoolSource",
#    fileNames = cms.untracked.vstring(
#    #'/store/cmst3/user/cmgtools/CMG/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_2_0/cmgTuple_0.root'
#    #'/store/cmst3/user/cmgtools/CMG/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1/cmgTuple_0.root'
#    #'file:../../../Common/prod/TEST/cmgTuple_HToTauTau.root'
#    'file:./tauMu_fullsel_tree_CMG.root'
#    )
#    )

#process.source.fileNames = ['file:./tauMu_fullsel_tree_CMG.root']

print process.source.fileNames


# set up JSON ---------------------------------------------------------------
if process.flatNtp.dataType != 0 :
   #from CMGTools.H2TauTau.tools.setupJSON import setupJSON
   #json = setupJSON(process)
   json = process.flatNtp.jsonfile.value()
   print 'json:', json
   from CMGTools.Common.Tools.applyJSON_cff import applyJSON
   applyJSON(process, json )
   print process.PoolSource.lumisToProcess

## run the vertex weights
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

process.load('CMGTools.Common.factories.cmgTauMu_cfi')
process.cmgTauMu.cfg.leg1Collection = 'cmgTauScaler'
process.cmgTauMu.cfg.metCollection = 'cmgPFMETRaw'
process.analysis +=  process.cmgTauMu

process.load('CMGTools.Common.skims.cmgTauMuSel_cfi')
process.cmgTauMuPreSel = process.cmgTauMuSel.clone()
#process.cmgTauMuPreSel.cut = cms.string('pt()>0' )
process.cmgTauMuPreSel.cut = cms.string('leg1().eta()!=leg2().eta() && leg1().pt()>20.0 && abs(leg1().eta())<2.3 && leg1().tauID("decayModeFinding")>0.5 && leg2().pt()>20.0 && abs(leg2().eta())<2.1' )
process.analysis +=  process.cmgTauMuPreSel 

# event filter --------------------------------
process.load('CMGTools.Common.skims.cmgTauMuCount_cfi')
process.cmgTauMuCount.src = 'cmgTauMuPreSel'
process.cmgTauMuCount.minNumber = 1

##run the MVA MET and remake the mu-tau list
if process.flatNtp.metType ==2 :
   process.load("CMGTools.Common.eventCleaning.goodPVFilter_cfi")
   process.load("CMGTools.Common.miscProducers.mvaMET.mvaMETTauMu_cfi")
   process.mvaMETTauMu.recBosonSrc = 'cmgTauMuPreSel'
   process.load("CMGTools.Common.factories.cmgBaseMETFromPFMET_cfi")
   process.mvaBaseMETTauMu = process.cmgBaseMETFromPFMET.clone()
   process.mvaBaseMETTauMu.cfg.inputCollection = 'mvaMETTauMu'
   process.load("CMGTools.Common.factories.cmgTauMuCor_cfi")
   process.cmgTauMuCorPreSel = process.cmgTauMuCor.clone()
   process.cmgTauMuCorPreSel.cfg.metCollection = 'mvaBaseMETTauMu'
   process.cmgTauMuCorPreSel.cfg.diObjectCollection = 'cmgTauMuPreSel'
   process.mvaMETSequence = cms.Sequence(
      process.goodPVFilter + 
      process.mvaMETTauMu +
      process.mvaBaseMETTauMu +
      process.cmgTauMuCorPreSel
      )
   process.analysis  += process.mvaMETSequence
   process.flatNtp.diTauTag = 'cmgTauMuCorPreSel'
   #process.mvaMETTauMu.verbose = True


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
