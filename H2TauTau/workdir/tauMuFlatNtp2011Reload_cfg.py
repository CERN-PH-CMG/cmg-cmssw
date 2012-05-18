import FWCore.ParameterSet.Config as cms
import os

process = cms.Process("FLATNTP")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.maxLuminosityBlocks = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
evReportFreq = 100

#######Define the samples to process
dataset_user = 'benitezj'
#sampleTag = "/PAT_CMG_V5_1_0/H2TAUTAU_JoseMay8"
#sampleTag = "/PAT_CMG_V5_1_0/H2TAUTAU_JoseMay9"
#sampleTag = "/PAT_CMG_V5_2_0/H2TAUTAU_JoseMay14"
#sampleTag = "/PAT_CMG_V5_2_0/H2TAUTAU_JoseMay15"
sampleTag = "/PAT_CMG_V5_2_0/H2TAUTAU_TauMu_JoseMay16"
#dataset_user = 'cbern'
#sampleTag = "/PAT_CMG_V5_2_0/H2TAUTAU_ColinMay12"
#sampleTag = "/PAT_CMG_V5_2_0/H2TAUTAU_TauMu_MetRaw_ColinMay12"
#sampleTag = "/PAT_CMG_V5_2_0/H2TAUTAU_ColinMay15"


sampleName = os.environ['SAMPLENAME']
sampleJobIdx = int(os.environ['SAMPLEJOBIDX'])

#########################
process.load('CMGTools.H2TauTau.tools.joseFlatNtpSample_cfi')
process.flatNtp.diTauTag = 'cmgTauMuCorSVFitFullSel'
process.flatNtp.pfJetListTag =  cms.InputTag("cmgPFJetSel")
from CMGTools.H2TauTau.tools.joseFlatNtpSample2011Reload_cff import configureFlatNtpSample
configureFlatNtpSample(process.flatNtp,sampleName)

inputfiles = "tauMu_fullsel_tree_CMG_.*root"
dataset_name = process.flatNtp.path.value() + sampleTag
firstfile = sampleJobIdx * 5
lastfile = (sampleJobIdx + 1 ) * 5
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

runOnMC = process.source.fileNames[0].find('Run201')==-1 and process.source.fileNames[0].find('embedded')==-1

process.analysis = cms.Path() 

## Jet recalibration -------------------
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from CMGTools.Common.miscProducers.cmgPFJetCorrector_cfi import cmgPFJetCorrector
process.cmgPFJetSel = cmgPFJetCorrector.clone()
#process.cmgPFJetSel.src = cms.InputTag( 'cmgPFJetSel','','PAT' )
#process.cmgPFJetSel.payload = cms.string('AK5PF')
process.cmgPFJetSel.src = cms.InputTag( 'cmgPFJetSelCHS','','PAT' )
process.cmgPFJetSel.payload = cms.string('AK5PFchs')
process.cmgPFJetSel.verbose = False
if runOnMC:
    process.GlobalTag.globaltag = 'START44_V13::All'
    process.cmgPFJetSel.levels = ['L1FastJet','L2Relative','L3Absolute']
else:
    process.GlobalTag.globaltag  = 'GR_R_44_V15::All'
    #process.GlobalTag.globaltag  = 'GR_R_44_V7::All' #used in the prod
    process.cmgPFJetSel.levels = ['L1FastJet','L2Relative','L3Absolute','L2L3Residual'] #, were not applied in the V5_2_0 prod
print 'GLOBAL TAG', process.GlobalTag.globaltag 
process.analysis += process.cmgPFJetSel


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
