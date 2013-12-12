import FWCore.ParameterSet.Config as cms

patElectronsWithMomenta = cms.EDProducer("PATElectronMomentumEmbedder",
    #electrons = cms.InputTag("patElectronsWithCalibrations"),
    electrons = cms.InputTag("patElectronsWithRegressionVars"),
    allElectrons = cms.PSet(
        aod   = cms.InputTag("patElectronsWithRegressionVars"),
        #regr  = cms.InputTag("patElectronsWithRegression"),
        calib = cms.InputTag("patElectronsWithCalibrations"), # regression + calibration & smearing
    ),
    #defaultMomentum = cms.string("calib"),
    defaultMomentum = cms.string("aod"),
)
