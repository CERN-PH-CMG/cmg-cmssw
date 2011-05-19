import FWCore.ParameterSet.Config as cms

runProcess = cms.PSet(
#    input = cms.string("/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_1_3_patch2/src/CMGTools/HtoZZ2l2nu/data/unweighted/DYToMuMu_M-20_Spring11.root"),
#    input = cms.string("/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_1_3_patch2/src/CMGTools/HtoZZ2l2nu/data/unweighted/DYToEE_M-20_Spring11.root"),
    input = cms.string("/data/psilva/Higgs/DoubleMuon-v7.root"),
    outdir = cms.string("./"),
    isMC = cms.bool(True),
    evStart = cms.int32(0),
    evEnd = cms.int32(-1),
    dirName = cms.string("evAnalyzer/data"),
    ptResolFileName = cms.string("${CMSSW_RELEASE_BASE}/src/CondFormats/JetMETObjects/data/Spring10_PtResolution_AK5PF.txt"),
    phiResolFileName = cms.string("${CMSSW_RELEASE_BASE}/src/CondFormats/JetMETObjects/data/Spring10_PhiResolution_AK5PF.txt")
    )
