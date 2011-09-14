import FWCore.ParameterSet.Config as cms

from CMGTools.HtoZZ2l2nu.MVAStudyConfig_cfi import *

runProcess = cms.PSet(
    input = cms.string("/data/psilva/Higgs/ntuples/EventSummaries.root"),
    outdir = cms.string("./"),
    isMC = cms.bool(True),
    mctruthmode=cms.int32(0),
    xsec = cms.double(1),
    saveSummaryTree = cms.bool(False),
    useFitter=cms.bool(False),
    evStart = cms.int32(0),
    evEnd = cms.int32(-1),
    dirName = cms.string("evAnalyzer/data"),
    ptResolFileName = cms.string("${CMSSW_RELEASE_BASE}/src/CondFormats/JetMETObjects/data/Spring10_PtResolution_AK5PF.txt"),
    phiResolFileName = cms.string("${CMSSW_RELEASE_BASE}/src/CondFormats/JetMETObjects/data/Spring10_PhiResolution_AK5PF.txt"),
    etaResolFileName = cms.string("${CMSSW_RELEASE_BASE}/src/CondFormats/JetMETObjects/data/Spring10_EtaResolution_AK5PF.txt"),
    jesUncFileName = cms.string("${CMSSW_RELEASE_BASE}/src/CondFormats/JetMETObjects/data/Spring10_Uncertainty_AK5PF.txt"),
    useMVA = cms.bool(True),
    tmvaInput = simpleDiscriminator
    )

runProcess.tmvaInput.input = runProcess.input

