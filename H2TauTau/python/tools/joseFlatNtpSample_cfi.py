import FWCore.ParameterSet.Config as cms

import os 
rootfile_dir = os.environ['CMSSW_BASE'] + '/src/CMGTools/Common/data/metRecoilCorrection/'


flatNtp = cms.EDAnalyzer(
    "TauMuFlatNtp",
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
    genParticlesTag =  cms.InputTag("genParticlesPruned"),
    pfJetListTag =  cms.InputTag("cmgPFJetSel"),
    sampleGenEventType =   cms.int32( 0 ),
    sampleTruthEventType =  cms.int32( 0 ),
    randsigma = cms.double(0.),
    recoilCorrection = cms.int32(0), # 0 =no correction , 1=Z or Higgs , 2=W
    fileCorrectTo = cms.string(rootfile_dir + 'recoilfit_wjets_njet.root'), #need to configure this for each sample
    fileZmmData = cms.string(rootfile_dir + 'recoilfit_datamm_njet.root'),
    fileZmmMC = cms.string(rootfile_dir + 'recoilfit_zmm42X_njet.root'),    
    )
