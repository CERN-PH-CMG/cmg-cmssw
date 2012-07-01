#$Revision: 1.4 $
import FWCore.ParameterSet.Config as cms

process = cms.Process("TheNtupleMaker")

process.load("FWCore.MessageService.MessageLogger_cfi")
# See TheNtupleMaker twiki for a brief explanation
#process.MessageLogger.destinations = cms.untracked.vstring("cerr")
#process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.MessageLogger.cerr.default.limit = 5

# This is required in order to configure HLTConfigProducer
process.load("L1TriggerConfig.L1GtConfigProducers.L1GtConfig_cff")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# Run on MC or data

runOnMC = True

# Input file

dataset_user = 'cmgtools' 
#dataset_name = '/HT/Run2012A-PromptReco-v1/RECO/PAT_CMG_V5_4_0_runrange_190605-194076'
dataset_name = '/JetHT/Run2012B-PromptReco-v1/AOD/PAT_CMG_V5_4_0_runrange_194480-195016'
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
    json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-196531_8TeV_PromptReco_Collisions12_JSON.txt'
    print 'json:', json
    applyJSON(process, json )

print 'runOnMC:', runOnMC

process.load("Ntuples.TNMc1.ntuple_cfi")

process.p = cms.Path(process.demo)
