#$Revision: 1.8 $
import FWCore.ParameterSet.Config as cms

process = cms.Process("TheNtupleMaker")

process.load("FWCore.MessageService.MessageLogger_cfi")
# See TheNtupleMaker twiki for a brief explanation
#process.MessageLogger.destinations = cms.untracked.vstring("cerr")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.default.limit = 5

# This is required in order to configure HLTConfigProducer
process.load("L1TriggerConfig.L1GtConfigProducers.L1GtConfig_cff")


# Get Will's SUSY MJ objects/sequences:
process.load("Configuration.StandardSequences.GeometryDB_cff")
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

process.load("CMGTools.Susy.RazorMultiJet.razorMultijet_cff")
process.load("CMGTools.Susy.common.susy_cff")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

# Run on MC or data

runOnMC = True

# Input file

dataset_user = 'cmgtools' 
#dataset_name = '/SingleMu/Run2012A-13Jul2012-v1/AOD/PAT_CMG_V5_6_0_B'
dataset_name = '/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
#dataset_name = '/HT/Run2012A-PromptReco-v1/RECO/PAT_CMG_V5_4_0_runrange_190605-194076'
#dataset_name = '/JetHT/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_194480-195016'
#dataset_name = '/QCD_Pt-15to3000_TuneZ2star_Flat_8TeV_pythia6/Summer12-PU_S7_START52_V9-v5/AODSIM/V5/PAT_CMG_V5_4_0'
dataset_files = 'patTuple.*root'

from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    dataset_user,
    dataset_name,
    dataset_files,
    )

print 'input:', process.source.fileNames

# set up JSON ---------------------------------------------------------------

if runOnMC==False:
    from CMGTools.Common.Tools.applyJSON_cff import *
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-204567_8TeV_PromptReco_Collisions12_JSON.txt'
    print 'json:', json
    applyJSON(process, json )

print 'runOnMC:', runOnMC

process.load("Ntuples.TNMc1.ntuple_cfi")

from CMGTools.External.pujetidsequence_cff import *

process.selectedPatJetspuJetId = pileupJetIdProducer.clone(
    produceJetIds = cms.bool(True),
    jetids = cms.InputTag(""),
    runMvas = cms.bool(False),
    jets = cms.InputTag("selectedPatJets"),
    vertexes = cms.InputTag("offlinePrimaryVertices"),
    algos = cms.VPSet(cutbased)
    )

process.selectedPatJetsCHSpuJetId = pileupJetIdProducer.clone(
    produceJetIds = cms.bool(True),
    jetids = cms.InputTag(""),
    runMvas = cms.bool(False),
    jets = cms.InputTag("selectedPatJetsCHS"),
    vertexes = cms.InputTag("offlinePrimaryVertices"),
    algos = cms.VPSet(cutbased)
    )

process.ak5PFJetsCHSprunedSubJetspuJetId = pileupJetIdProducer.clone(
    produceJetIds = cms.bool(True),
    jetids = cms.InputTag(""),
    runMvas = cms.bool(False),
    jets = cms.InputTag("ak5PFJetsCHSpruned:SubJets"),
    vertexes = cms.InputTag("offlinePrimaryVertices"),
    algos = cms.VPSet(cutbased)
    )

#process.p = cms.Path(process.selectedPatJetspuJetId * process.selectedPatJetsCHSpuJetId * process.ak5PFJetsCHSprunedSubJetspuJetId * process.demo)

process.razorMJObjectSequence.remove(process.razorMJHemiSequence)
# This is the UCSB tau veto.  Need to remove for SMSs:
process.razorMJTauSequence.remove(process.razorMJTauVeto)

if runOnMC==True:
    process.p = cms.Path(process.razorMJObjectSequence+process.susyGenSequence+process.demo)
else:
    process.p = cms.Path(process.razorMJObjectSequence+process.demo)

#process.p = cms.Path(process.razorMJObjectSequence+process.susyGenSequence+process.demo)

#process.p = cms.Path(process.demo)
