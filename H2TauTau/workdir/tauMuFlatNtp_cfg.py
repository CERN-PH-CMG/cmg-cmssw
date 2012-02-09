#from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms
import os

process = cms.Process("HISTOGRAMER")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.maxLuminosityBlocks = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
evReportFreq = 100


###Define all samples
from CMGTools.H2TauTau.tools.joseFlatNtpSample_cff import *
loadFlatNtpSamples(process)

##define sample
#sampleAlias = 'TauPlusXMay'
sampleAlias = os.environ['SAMPLENAME']
print sampleAlias

tauMuFlatNtp = getattr(process,"tauMuFlatNtp"+sampleAlias)

dataset_user = 'benitezj'
#sampleTag = "/PAT_CMG_V2_4_1/H2TAUTAU_Oct26"
sampleTag = "/PAT_CMG_V2_4_1/H2TAUTAU_Jan26TauIso"
sampleName = tauMuFlatNtp.path.value()
dataset_name = sampleName + sampleTag
print dataset_user
print dataset_name


#get input files
from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
 dataset_user,
 dataset_name,
 'h2TauTau_fullsel_tree_CMG_.*root')

#process.source.fileNames = cms.untracked.vstring(['/store/cmst3/user/benitezj/CMG/TauPlusX/Run2011A-May10ReReco-v1/AOD/V2/PAT_CMG_V2_4_1/H2TAUTAU_Oct26/h2TauTau_fullsel_tree_CMG_9.root'])

#process.source.duplicateCheckMode = cms.untracked.string("noDuplicateCheck")
print process.source.fileNames


##define path
# process.tauMuFlatNtpTauPlusXMay
process.analysis = cms.Path(tauMuFlatNtp) #getattr(process,"tauMuFlatNtp"+sampleAlias)) 
process.schedule = cms.Schedule(process.analysis)

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("tauMuFlatNtp.root")
    )


########
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

print process.dumpPython()
