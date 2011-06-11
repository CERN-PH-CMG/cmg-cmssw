import FWCore.ParameterSet.Config as cms

runProcess = cms.PSet(
#    input = cms.string("/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_1_3_patch2/src/CMGTools/HtoZZ2l2nu/data/weighted/DYToMuMu_M-20_Spring11.root"),
#    input = cms.string("/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_1_3_patch2/src/CMGTools/HtoZZ2l2nu/data/weighted/DYToEE_M-20_Spring11.root"),
    input = cms.string("data/GluGluToHToZZTo2L2Nu_M-200_Spring11.root"),
#    input = cms.string("/data/psilva/Higgs/DYToMuMu_M-20_Spring11.root"),
#    input = cms.string("/afs/cern.ch/user/p/psilva/scratch0/CMSSW_4_1_3_patch2/src/CMGTools/HtoZZ2l2nu/data/weighted/TTJets_madgraph_Spring11.root"),
#    input = cms.string("../../datastore/psilva/HtoZZ/weighted/DoubleMuon-v9.root"),
    outdir = cms.string("./"),
    useFitter=cms.bool(False),
    isMC = cms.bool(True),
    evStart = cms.int32(0),
    evEnd = cms.int32(10000),
    dirName = cms.string("evAnalyzer/data"),
    ptResolFileName = cms.string("${CMSSW_RELEASE_BASE}/src/CondFormats/JetMETObjects/data/Spring10_PtResolution_AK5PF.txt"),
    phiResolFileName = cms.string("${CMSSW_RELEASE_BASE}/src/CondFormats/JetMETObjects/data/Spring10_PhiResolution_AK5PF.txt")
    )
