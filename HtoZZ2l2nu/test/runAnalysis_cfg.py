import FWCore.ParameterSet.Config as cms

from CMGTools.HtoZZ2l2nu.MVAStudyConfig_cfi import *

runProcess = cms.PSet(
    input = cms.string("/data/psilva/Higgs/ntuples/EventSummaries.root"),
    outdir = cms.string("./"),
    isMC = cms.bool(True),
    mctruthmode=cms.int32(0),
    runSystematics = cms.bool(@runSystematics),	
    xsec = cms.double(1),
    saveSummaryTree = cms.bool(False),
    evStart = cms.int32(0),
    evEnd = cms.int32(-1),
    dirName = cms.string("evAnalyzer/data"),
    ptResolFileName = cms.string("${CMSSW_RELEASE_BASE}/src/CondFormats/JetMETObjects/data/Spring10_PtResolution_AK5PF.txt"),
    phiResolFileName = cms.string("${CMSSW_RELEASE_BASE}/src/CondFormats/JetMETObjects/data/Spring10_PhiResolution_AK5PF.txt"),
    etaResolFileName = cms.string("${CMSSW_RELEASE_BASE}/src/CondFormats/JetMETObjects/data/Spring10_EtaResolution_AK5PF.txt"),
    jesUncFileName = cms.string("${CMSSW_BASE}/src/CMGTools/HtoZZ2l2nu/data/GR_R_42_V20_AK5PFchs_Uncertainty.txt"),
    mcpileup = cms.string('/afs/cern.ch/user/p/psilva/public/Pileup/Summer11Truth.root'),
    datapileup = cms.string('/afs/cern.ch/user/p/psilva/public/Pileup/PileupTruth2011A.root'),
    puWeightFile = cms.string('/afs/cern.ch/user/p/psilva/public/Pileup/Weight3D_2011A.root'),
    gammaCategories = cms.vint32(0,20,30,50,75,125),
    useMVA = cms.bool(True),
    tmvaInput = simpleDiscriminator
    )

runProcess.tmvaInput.input = runProcess.input

