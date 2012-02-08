import FWCore.ParameterSet.Config as cms

from CMGTools.HtoZZ2l2nu.MVAStudyConfig_cfi import *

runProcess = cms.PSet(
    input = cms.string("GGtoH600toZZto2L2Nu.root"),
    outdir = cms.string("/tmp/psilva"),
    isMC = cms.bool(True),
    mctruthmode=cms.int32(0),
    xsec = cms.double(1),	
    saveSummaryTree = cms.bool(False),
    runSystematics = cms.bool(False),
    weightsFile = cms.string(""),	 
    evStart = cms.int32(0),
    evEnd = cms.int32(-1),
    dirName = cms.string("evAnalyzer/data"),
    ptResolFileName = cms.string("${CMSSW_RELEASE_BASE}/src/CondFormats/JetMETObjects/data/Spring10_PtResolution_AK5PF.txt"),
    phiResolFileName = cms.string("${CMSSW_RELEASE_BASE}/src/CondFormats/JetMETObjects/data/Spring10_PhiResolution_AK5PF.txt"),
    etaResolFileName = cms.string("${CMSSW_RELEASE_BASE}/src/CondFormats/JetMETObjects/data/Spring10_EtaResolution_AK5PF.txt"),
    jesUncFileName = cms.string("${CMSSW_BASE}/src/CMGTools/HtoZZ2l2nu/data/GR_R_42_V20_AK5PFchs_Uncertainty.txt"),
    mcpileup = cms.string('/afs/cern.ch/user/p/psilva/public/Pileup/Summer11Observed.root'),
    datapileup = cms.string('/afs/cern.ch/user/p/psilva/public/Pileup/Pileup2011AplusB.root'),	
    gammaCategories = cms.vint32(0,55,75,90,125,135),
    useMVA = cms.bool(False),	
    tmvaInput = simpleDiscriminator
    )
