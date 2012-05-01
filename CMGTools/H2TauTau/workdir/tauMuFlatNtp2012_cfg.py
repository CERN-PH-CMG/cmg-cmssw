#from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms
import os

process = cms.Process("HISTOGRAMER")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.maxLuminosityBlocks = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
evReportFreq = 100

#####################Define the sample to process
sampleName = os.environ['SAMPLENAME']
sampleJobIdx = int(os.environ['SAMPLEJOBIDX'])
print "sampleName: "
print sampleName
print "sampleJobIdx: "
print sampleJobIdx

process.load('CMGTools.H2TauTau.tools.joseFlatNtpSample_cfi')
from CMGTools.H2TauTau.tools.joseFlatNtpSample_cff import *
configureFlatNtpSample(process.flatNtp,sampleName,year='2012')


dataset_user = 'benitezj'
#sampleTag = "/PAT_CMG_V2_4_1/H2TAUTAU_Oct26"
#sampleTag = "/PAT_CMG_V2_4_1/H2TAUTAU_Jan26TauIso"
#inputfiles = 'h2TauTau_fullsel_tree_CMG_.*root'

#sampleTag = "/PAT_CMG_V2_5_0/H2TAUTAU_Feb11"
#sampleTag = "/PAT_CMG_V2_5_0/H2TAUTAU_Feb11TauIso"

#######QCD samples(consistent with H2TAUTAU_Feb11TauIso) ##These samples had Tau Iso applied
#sampleTag = "/PAT_CMG_V2_5_0/H2TAUTAU_Mar21" ##following sample had the macthingCone cut removed
#sampleTag = "/PAT_CMG_443_Mar21/H2TAUTAU_QCD_Mar23" 

#sampleTag = "/PAT_CMG_V2_5_0/H2TAUTAU_Mar27"

#################2012 samples
sampleTag = "/PATCMG_TEST52/H2TAUTAU_TEST52"

##which channel
inputfiles = "tauMu_fullsel_tree_CMG_.*root"



#########################
dataset_name = process.flatNtp.path.value() + sampleTag
firstfile = sampleJobIdx * 10 
lastfile = (sampleJobIdx + 1 )*10

print dataset_user
print dataset_name
print inputfiles
print firstfile
print lastfile

#get input files
from CMGTools.Production.datasetToSource import *
process.source = datasetToSource( dataset_user, dataset_name, inputfiles)
process.source.fileNames = process.source.fileNames[firstfile:lastfile]


#process.source = cms.Source(
#    "PoolSource",
#    fileNames = cms.untracked.vstring(
#    #'/store/cmst3/user/cmgtools/CMG/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_5_0/tree_CMG_1.root'
#    #    '/store/cmst3/user/cmgtools/CMG/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_5_0/tree_CMG_1.root'
#    'file:../../prod/TEST52/tauMu_fullsel_tree_CMG.root'
#    )
#    )


print process.source.fileNames

##
process.analysis = cms.Path(process.flatNtp) 
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
