import FWCore.ParameterSet.Config as cms

import os 
rootfile_dir = os.environ['CMSSW_BASE'] + '/src/CMGTools/Utilities/data/metRecoilCorrection/'
httdata_dir = os.environ['CMSSW_BASE'] + '/src/CMGTools/H2TauTau/data/'

TauMuCfg = cms.PSet(
    path = cms.untracked.string(""),
    printSelectionPass = cms.int32(0),
    dataPeriodFlag = cms.int32(2011),
    firstRun =  cms.int32( 0 ),
    lastRun =  cms.int32( 0 ),
    dataType = cms.int32(-1),
    pupWeightName = cms.InputTag(""),
    pupWeightNames1 = cms.InputTag(""),
    pupWeightNames2 = cms.InputTag(""),
    pupWeightNames3 = cms.InputTag(""),
    pupWeightNames4 = cms.InputTag(""),
    pupWeightNames5 = cms.InputTag(""),
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
    trigPathTest1 = cms.InputTag(""),
    trigPathTest2 = cms.InputTag(""),
    trigPathTest3 = cms.InputTag(""),
    trigPathTest4 = cms.InputTag(""),
    trigPathTest5 = cms.InputTag(""),
    trigPathTest6 = cms.InputTag(""),
    trigPathTest7 = cms.InputTag(""),
    trigPathTest8 = cms.InputTag(""),
    trigPathTest9 = cms.InputTag(""),
    trigPathTest10 = cms.InputTag(""),
    diTauTag =  cms.InputTag("cmgTauMu"),
    unscaledTauTag =  cms.InputTag("cmgTauSel"),
    verticesListTag = cms.InputTag("goodOfflinePrimaryVertices"), #note that these need to be created in the FLATNTP step
    genParticlesTag =  cms.InputTag("genParticlesPruned"),
    trigPathsListTag = cms.InputTag("cmgTriggerObjectSel"),
    trigObjsListTag = cms.InputTag("cmgTriggerObjectListSel"),
    pfJetListTag =  cms.InputTag("cmgPFJetSel"),
    pujetidname = cms.string("full53x"),
    muonVetoListTag =  cms.InputTag("cmgMuonSel"),
    electronVetoListTag =  cms.InputTag("cmgElectronSel"),
    tauVetoListTag =  cms.InputTag("cmgTauSel"),
    sampleGenEventType =   cms.int32( 0 ),
    sampleGenMassMin =   cms.double( -1. ),
    sampleGenMassMax =   cms.double( 1000000. ),
    sampleTruthEventType =  cms.int32( 0 ),
    randsigma = cms.double(0.),
    recoilCorrection = cms.int32(0), # 0 =no correction , 1=Z or Higgs , 2=W
    recoiliScale = cms.double(0.),
    fileCorrectTo = cms.string(rootfile_dir + 'recoilfit_ztt53X_20pv_njet.root'), #need to configure this for each sample
    fileZmmData = cms.string(rootfile_dir + 'recoilfit_datamm53X_20pv_njet.root'),
    fileZmmMC = cms.string(rootfile_dir + 'recoilfit_zmm53X_20pv_njet.root'),
    metType = cms.int32(1), # 1=PFMET, 2=MVAMET
    metscale = cms.double(-1.),
    mvaMETTag = cms.InputTag("mvaBaseMETTauMu"),
    mvaMETSigTag = cms.InputTag("mvaMETTauMu"),
    runSVFit = cms.int32(2), # 0 do not run, 1 run old version, 2 run new version
    smearVisMass0pi0 = cms.double(-1.),
    smearSVFitMass0pi0 = cms.double(-1.),
    smearVisMass1pi0 = cms.double(-1.),
    smearSVFitMass1pi0 = cms.double(-1.),
    mvaWeights = cms.string(httdata_dir + 'VBFMVA_BDTG.weights.44X.xml'),
    mvaWeights2012 = cms.string(httdata_dir + 'VBFMVA_BDTG_HCP_42X.weights.xml'),
    muPtCut = cms.double(17.),
    tauPtCut = cms.double(20.),
    muEtaCut = cms.double(2.1),
    tauEtaCut = cms.double(2.3),
    tauAntiMuDisc = cms.string(''),
    tauAntiEDisc = cms.string(''),
    signalWeightDir =  cms.string(httdata_dir),
    signalWeightMass =  cms.string('0'),
    jsonfile = cms.string(''),
    saveLHENUP = cms.int32(0),
    correctTauES = cms.int32(0),
    embeddedWeightType = cms.int32(1),
    embeddedKinWeightFile = cms.string(''),
    )


flatNtpTauMu = cms.EDAnalyzer(
    "TauMuFlatNtp",
    TauMuCfg
    )


TauEleCfg = TauMuCfg.clone()
TauEleCfg.diTauTag = "cmgTauEle"
TauEleCfg.mvaMETTag = "mvaBaseMETTauEle"
TauEleCfg.mvaMETSigTag = "mvaMETTauEle"
    
flatNtpTauEle = cms.EDAnalyzer(
    "TauEleFlatNtp",
    TauEleCfg
    )
