import FWCore.ParameterSet.Config as cms

process = cms.Process('GETGBR')

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/eos/cms/store/mc/RunIIFall17MiniAOD/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8/MINIAODSIM/94X_mc2017_realistic_v10-v1/60000/A0D71AEE-13E1-E711-B3C9-FA163E629498.root'),
)

from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_realistic', '')

process.load('RecoTauTag.Configuration.loadRecoTauTagMVAsFromPrepDB_cfi')
tauIdDiscrMVA_trainings_run2_2017 = { 'tauIdMVAIsoDBoldDMwLT2017' : "tauIdMVAIsoDBoldDMwLT2017", }
tauIdDiscrMVA_2017_version = "v2"
getters = []
for training, gbrForestName in tauIdDiscrMVA_trainings_run2_2017.items():
    process.loadRecoTauTagMVAsFromPrepDB.toGet.append(
        cms.PSet(
           record = cms.string('GBRWrapperRcd'),
           tag = cms.string("RecoTauTag_%s%s" % (gbrForestName, tauIdDiscrMVA_2017_version)),
           label = cms.untracked.string("RecoTauTag_%s%s" % (gbrForestName, tauIdDiscrMVA_2017_version))
        )
    )
    getters.append( cms.EDAnalyzer("GBRForestGetterFromDB",
        grbForestName = cms.string("RecoTauTag_%s%s" % (gbrForestName, tauIdDiscrMVA_2017_version)),
        outputFileName = cms.untracked.string("GBRForest_%s%s.root" % (gbrForestName, tauIdDiscrMVA_2017_version)),
    ) )
    setattr(process, "get%s%s" % (gbrForestName, tauIdDiscrMVA_2017_version), getters[-1])
    
process.path = cms.Path( sum(getters[1:], getters[0]) )
