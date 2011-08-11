import FWCore.ParameterSet.Config as cms

from CMGTools.HtoZZ2l2nu.MVAStudyConfig_cfi import *

runProcess = cms.PSet(
    input = cms.string("data/GluGluToHToZZTo2L2Nu_M-200_Spring11.root"),
    outdir = cms.string("./"),
    xsec = cms.double(1),
    isMC = cms.bool(True),
    saveSummaryTree = cms.bool(False),
    useFitter=cms.bool(False),
    evStart = cms.int32(0),
    evEnd = cms.int32(10000),
    dirName = cms.string("evAnalyzer/data"),
    ptResolFileName = cms.string("${CMSSW_RELEASE_BASE}/src/CondFormats/JetMETObjects/data/Spring10_PtResolution_AK5PF.txt"),
    phiResolFileName = cms.string("${CMSSW_RELEASE_BASE}/src/CondFormats/JetMETObjects/data/Spring10_PhiResolution_AK5PF.txt"),
    etaResolFileName = cms.string("${CMSSW_RELEASE_BASE}/src/CondFormats/JetMETObjects/data/Spring10_EtaResolution_AK5PF.txt"),
    jesUncFileName = cms.string("${CMSSW_RELEASE_BASE}/src/CondFormats/JetMETObjects/data/Spring10_Uncertainty_AK5PF.txt"),
    useMVA = cms.bool(True),
    tmvaInput = redMETstudy
    )

