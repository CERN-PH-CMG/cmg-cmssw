import FWCore.ParameterSet.Config as cms
import sys
process = cms.Process('GETGBR')

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

process.maxEvents = cms.untracked.PSet(
    input=cms.untracked.int32(1)
)

# Input source
process.source = cms.Source(
    "PoolSource",
    # fileNames = cms.untracked.vstring('file:/eos/cms/store/mc/RunIIFall17MiniAOD/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8/MINIAODSIM/94X_mc2017_realistic_v10-v1/60000/A0D71AEE-13E1-E711-B3C9-FA163E629498.root'),
    # the following is readable from anywhere
    fileNames = cms.untracked.vstring('root://cms-xrd-global.cern.ch//store/mc/RunIIFall17MiniAODv2/VBFHToTauTau_M125_13TeV_powheg_pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/00000/2EE992B1-F942-E811-8F11-0CC47A4C8E8A.root'),
)

from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(
    process.GlobalTag, 'auto:phase1_2017_realistic', '')

process.load('RecoTauTag.Configuration.loadRecoTauTagMVAsFromPrepDB_cfi')
tauIdDiscrMVA_trainings_run2_2017 = { 'tauIdMVAIsoDBoldDMwLT2017' : "tauIdMVAIsoDBoldDMwLT2017", "tauIdMVAIsoDBoldDMdR0p3wLT2017" : "tauIdMVAIsoDBoldDMdR0p3wLT2017"}
tauIdDiscrMVA_2017_version = "v2"

tauIdDiscrMVA_WPs_run2_2017 = {
    'tauIdMVAIsoDBoldDMwLT2017': {
        'Eff95': "DBoldDMwLTEff95",
        'Eff90': "DBoldDMwLTEff90",
        'Eff80': "DBoldDMwLTEff80",
        'Eff70': "DBoldDMwLTEff70",
        'Eff60': "DBoldDMwLTEff60",
        'Eff50': "DBoldDMwLTEff50",
        'Eff40': "DBoldDMwLTEff40"
    }
}

getters = []
for training, gbrForestName in tauIdDiscrMVA_trainings_run2_2017.items():
    process.loadRecoTauTagMVAsFromPrepDB.toGet.append(
        cms.PSet(
            record=cms.string('GBRWrapperRcd'),
            tag=cms.string("RecoTauTag_%s%s" %
                           (gbrForestName, tauIdDiscrMVA_2017_version)),
            label=cms.untracked.string("RecoTauTag_%s%s" % (
                gbrForestName, tauIdDiscrMVA_2017_version))
        )
    )
    getters.append(cms.EDAnalyzer("GBRForestGetterFromDB",
                                  grbForestName=cms.string("RecoTauTag_%s%s" % (
                                      gbrForestName, tauIdDiscrMVA_2017_version)),
                                  outputFileName=cms.untracked.string("GBRForest_%s%s.root" % (
                                      gbrForestName, tauIdDiscrMVA_2017_version)),
                                  ))
    setattr(process, "get%s%s" %
            (gbrForestName, tauIdDiscrMVA_2017_version), getters[-1])

    if training not in tauIdDiscrMVA_WPs_run2_2017: 
        sys.stderr.write("ERROR: Missing WPs for "+training+", will be skipped.\n")
        continue
    for WP in tauIdDiscrMVA_WPs_run2_2017[training]:
        process.loadRecoTauTagMVAsFromPrepDB.toGet.append(
            cms.PSet(
                record=cms.string('PhysicsTGraphPayloadRcd'),
                tag=cms.string("RecoTauTag_%s%s_WP%s" %
                               (gbrForestName, tauIdDiscrMVA_2017_version, WP)),
                label=cms.untracked.string("RecoTauTag_%s%s_WP%s" % (
                    gbrForestName, tauIdDiscrMVA_2017_version, WP))
            )
        )
        getters.append(cms.EDAnalyzer("TGraphGetterFromDB",
                                  tGraphName=cms.string("RecoTauTag_%s%s_WP%s" %
                               (gbrForestName, tauIdDiscrMVA_2017_version, WP)),
                                  outputFileName=cms.untracked.string("RecoTauTag_%s%sWP%s.root" % (
                                      gbrForestName, tauIdDiscrMVA_2017_version, WP)),
                                  ))
        setattr(process, "get%s%sWP%s" %
            (gbrForestName, tauIdDiscrMVA_2017_version, WP), getters[-1])
    mvaoutput_normalization_tag = "RecoTauTag_{}{}_mvaOutput_normalization".format(
        gbrForestName, 
        tauIdDiscrMVA_2017_version
        )
    process.loadRecoTauTagMVAsFromPrepDB.toGet.append(
        cms.PSet(
            record=cms.string('PhysicsTFormulaPayloadRcd'),
            tag=cms.string(mvaoutput_normalization_tag),
            label=cms.untracked.string(mvaoutput_normalization_tag)
            )
        )    
    getter = cms.EDAnalyzer(
        "TFormulaGetterFromDB",
        tFormulaName=cms.string(mvaoutput_normalization_tag),
        outputFileName=cms.untracked.string(mvaoutput_normalization_tag+'.root'),
        )
    setattr(process, 'get'+mvaoutput_normalization_tag, getter)
    getters.append(getter)
    

process.path = cms.Path(sum(getters[1:], getters[0]))
