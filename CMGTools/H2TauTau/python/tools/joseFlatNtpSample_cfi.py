import FWCore.ParameterSet.Config as cms

import os 
rootfile_dir = os.environ['CMSSW_BASE'] + '/src/CMGTools/Common/data/metRecoilCorrection/'
httdata_dir = os.environ['CMSSW_BASE'] + '/src/CMGTools/H2TauTau/data/'


flatNtpTauMu = cms.EDAnalyzer(
    "TauMuFlatNtp",
    path = cms.untracked.string(""),
    firstRun =  cms.int32( 0 ),
    lastRun =  cms.int32( 0 ),
    dataType = cms.int32(-1),
    pupWeightName = cms.InputTag(""),
    trigPath1 = cms.InputTag(""),
    trigPath2 = cms.InputTag(""),
    trigPath3 = cms.InputTag(""),
    trigPath4 = cms.InputTag(""),
    trigPath5 = cms.InputTag(""),
    trigPath6 = cms.InputTag(""),
    trigPath7 = cms.InputTag(""),
    trigPath8 = cms.InputTag(""),
    trigPath9 = cms.InputTag(""),
    trigPath10 = cms.InputTag(""),
    diTauTag =  cms.InputTag(""),
    verticesListTag = cms.InputTag("goodOfflinePrimaryVertices"), #note that these need to be created in the FLATNTP step
    genParticlesTag =  cms.InputTag("genParticlesPruned"),
    trigPathsListTag = cms.InputTag("cmgTriggerObjectSel"),
    trigObjsListTag = cms.InputTag("cmgTriggerObjectListSel"),
    pfJetListTag =  cms.InputTag("cmgPFJetSel"),
    diMuonVetoListTag =  cms.InputTag("cmgMuonSel"),
    sampleGenEventType =   cms.int32( 0 ),
    sampleTruthEventType =  cms.int32( 0 ),
    randsigma = cms.double(0.),
    recoilCorrection = cms.int32(0), # 0 =no correction , 1=Z or Higgs , 2=W
    fileCorrectTo = cms.string(rootfile_dir + 'recoilfit_wjets_njet.root'), #need to configure this for each sample
    fileZmmData = cms.string(rootfile_dir + 'recoilfit_datamm_njet.root'),
    fileZmmMC = cms.string(rootfile_dir + 'recoilfit_zmm42X_njet.root'),
    metType = cms.int32(1), # 1=PFMET, 2=MVAMET
    runSVFit = cms.int32(2), # 0 do not run, 1 run old version, 2 run new version
    mvaWeights = cms.string(httdata_dir + 'VBFMVA_BDTG.weights.44X.xml'),
    )


flatNtpTauEle = cms.EDAnalyzer(
    "TauEleFlatNtp",
    path = cms.untracked.string(""),
    firstRun =  cms.int32( 0 ),
    lastRun =  cms.int32( 0 ),
    dataType = cms.string(""),
    pupWeightName = cms.InputTag(""),
    trigPath1 = cms.InputTag(""),
    trigPath2 = cms.InputTag(""),
    trigPath3 = cms.InputTag(""),
    trigPath4 = cms.InputTag(""),
    trigPath5 = cms.InputTag(""),
    trigPath6 = cms.InputTag(""),
    trigPath7 = cms.InputTag(""),
    trigPath8 = cms.InputTag(""),
    trigPath9 = cms.InputTag(""),
    trigPath10 = cms.InputTag(""),
    diTauTag =  cms.InputTag(""),
    verticesListTag = cms.InputTag("offlinePrimaryVertices"),
    genParticlesTag =  cms.InputTag("genParticlesPruned"),
    trigPathsListTag = cms.InputTag("cmgTriggerObjectSel"),
    trigObjsListTag = cms.InputTag("cmgTriggerObjectListSel"),
    pfJetListTag =  cms.InputTag("cmgPFJetSel"),
    diMuonVetoListTag =  cms.InputTag("cmgElectronSel"),
    sampleGenEventType =   cms.int32( 0 ),
    sampleTruthEventType =  cms.int32( 0 ),
    randsigma = cms.double(0.),
    recoilCorrection = cms.int32(0), # 0 =no correction , 1=Z or Higgs , 2=W
    fileCorrectTo = cms.string(rootfile_dir + 'recoilfit_wjets_njet.root'), #need to configure this for each sample
    fileZmmData = cms.string(rootfile_dir + 'recoilfit_datamm_njet.root'),
    fileZmmMC = cms.string(rootfile_dir + 'recoilfit_zmm42X_njet.root'),
    runSVFit = cms.int32(1), # 0 do not run, 1 run old version, 2 run new version
    mvaWeights = cms.string(httdata_dir + 'VBFMVA_BDTG.weights.44X.xml'),
    )
