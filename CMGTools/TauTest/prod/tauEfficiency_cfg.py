#from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms
import os

process = cms.Process("EFF")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.maxLuminosityBlocks = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
evReportFreq = 100

##Define the input 
#dataset_user = 'cmgtools'
#dataset_name = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V2/PAT_CMG_V2_5_0"
#dataset_name = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/PAT_CMG_V3_0_0"

dataset_user = 'benitezj'
####No Ch Had Subtraction
#dataset_name = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3/PAT_CMG_NoChHadSub_AllGen"
#dataset_name = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM/V3/PAT_CMG_NoChHadSub_AllGen"
####Ch. Had Subtracted
#dataset_name = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3/PAT_CMG_AllGen"
####check the tau matching cut
dataset_name = "/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3/PAT_CMG_NoChHadSub_NoMatchCut_AllGen"


effOrFake = cms.int32(2)
chamonix  = cms.bool(False)


sampleJobIdx = int(os.environ['SAMPLEJOBIDX'])
firstfile = sampleJobIdx * 10 
lastfile = (sampleJobIdx + 1 )*10


inputfiles = "tree_CMG_.*root"
from CMGTools.Production.datasetToSource import *
process.source = datasetToSource( dataset_user, dataset_name, inputfiles)

#process.source =  cms.Source("PoolSource",noEventSort = cms.untracked.bool(True),duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),fileNames = cms.untracked.vstring())
#process.source.fileNames = cms.untracked.vstring(['/store/cmst3/user/cmgtools/CMG/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V2/PAT_CMG_V2_5_0/tree_CMG_1.root'])
#process.source.fileNames = cms.untracked.vstring(['/store/cmst3/user/benitezj/CMG/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V3/PAT_CMG_NoChHadSub_AllGen/tree_CMG_90.root'])

process.source.fileNames = process.source.fileNames[firstfile:lastfile]
print process.source.fileNames



##configure the Analyzer
process.flatNtp = cms.EDAnalyzer(
    "CMGTauEfficiency",
    effOrFake = effOrFake,
    chamonix = chamonix,
    OOTWeightFall11File = cms.string(os.environ['CMSSW_BASE']+"/src/CMGTools/RootTools/data/Reweight/DYJetsFall11/EfficiencyAnalyzer/EfficiencyAnalyzer.root"),
    OOTWeightChamonixFile = cms.string(os.environ['CMSSW_BASE']+"/src/CMGTools/RootTools/data/Reweight/DYJetsChamonix/EfficiencyAnalyzer/EfficiencyAnalyzer.root"),
    )

###
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

