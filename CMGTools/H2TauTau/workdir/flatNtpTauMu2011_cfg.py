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
sampleTag = "/TauMuSkimJuly23"

sampleName = os.environ['SAMPLENAME']
sampleJobIdx = int(os.environ['SAMPLEJOBIDX'])
sampleMergeFactor = int(os.environ['SAMPLEMERGEFACTOR'])
print sampleName
print sampleJobIdx
print sampleMergeFactor

######The analysis module
process.load('CMGTools.H2TauTau.tools.joseFlatNtpSample_cfi')
process.flatNtp = process.flatNtpTauMu.clone()
from CMGTools.H2TauTau.tools.joseFlatNtpSample_cff import configureFlatNtpSampleTauMu2011
configureFlatNtpSampleTauMu2011(process.flatNtp,sampleName)
process.flatNtp.diTauTag = 'cmgTauMuPreSel'
process.flatNtp.metType = 1
process.flatNtp.runSVFit = 1


### input files
inputfiles = "tauMu_fullsel_tree_CMG_.*root"
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

#process.source.eventsToProcess = cms.untracked.VEventRange('1:6486','1:20430','1:28387','1:88240','1:105383','1:121208','1:165752','1:183902')
#process.source.eventsToProcess = cms.untracked.VEventRange('1:4394', '1:82155', '1:172226', '1:178770', '1:184091')
#process.source.eventsToProcess = cms.untracked.VEventRange('1:15599','1:20873','1:20916','1:20991','1:21008')
#print process.source.eventsToProcess

#process.source.fileNames = ['file:./tauMu_fullsel_tree_CMG.root']

print process.source.fileNames


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
process.cmgTauMuPreSel.cut = cms.string('pt()>0.0' )
process.analysis +=  process.cmgTauMuPreSel 


##jet energy systematics
#process.load("CMGTools.Common.factories.cmgPFJetScaler_cfi")
#process.cmgPFJetSel = process.cmgPFJetUp.clone()
#process.cmgPFJetSel.cuts = cms.PSet()
#process.cmgPFJetSel.cfg.inputCollection = 'cmgPFJetSel'
#process.cmgPFJetSel.cfg.uncertainty = cms.double(0.04)
#process.cmgPFJetSel.cfg.nSigma = -1.
#process.analysis += process.cmgPFJetSel


###run the MVA MET and remake the mu-tau list
#process.load("CMGTools.Common.eventCleaning.goodPVFilter_cfi")
#process.load("CMGTools.Common.miscProducers.mvaMET.mvaMET_cff")
#process.load("CMGTools.Common.factories.cmgBaseMETFromPFMET_cfi")
#process.mvaMETTauMu.recBosonSrc = 'cmgTauMuPreSel'
#process.mvaBaseMETTauMu = process.cmgBaseMETFromPFMET.clone()
#process.mvaBaseMETTauMu.cfg.inputCollection = 'mvaMETTauMu'
#process.load("CMGTools.Common.factories.cmgTauMuCor_cfi")
#process.cmgTauMuMVAPreSel = process.cmgTauMuCor.clone()
#process.cmgTauMuMVAPreSel.cfg.metCollection = 'mvaBaseMETTauMu'
#process.cmgTauMuMVAPreSel.cfg.diObjectCollection = 'cmgTauMuPreSel'
#process.mvaMETSequence = cms.Sequence(
#    process.goodPVFilter + 
#    process.mvaMETTauMu +
#    process.mvaBaseMETTauMu+
#    process.cmgTauMuMVAPreSel
#    )
#process.analysis  += process.mvaMETSequence
#process.flatNtp.diTauTag = 'cmgTauMuMVAPreSel'
#process.flatNtp.metType = 2




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
