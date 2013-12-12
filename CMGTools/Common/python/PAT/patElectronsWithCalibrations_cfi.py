import FWCore.ParameterSet.Config as cms

patElectronsWithCalibrations = cms.EDProducer("CalibratedPatElectronProducer",
    inputPatElectronsTag = cms.InputTag("patElectronsWithRegression"),
    isMC = cms.bool(True),
    verbose = cms.bool(False),
    synchronization = cms.bool(False),
    updateEnergyError = cms.bool(True),
    applyLinearityCorrection = cms.bool(True),
    correctionsType = cms.int32(2),
    combinationType = cms.int32(3),
    lumiRatio = cms.double(1.0),
    inputDataset = cms.string("Summer12_LegacyPaper"),
    combinationRegressionInputPath = cms.string("EgammaAnalysis/ElectronTools/data/eleEnergyRegWeights_WithSubClusters_VApr15.root"),
    scaleCorrectionsInputPath = cms.string("EgammaAnalysis/ElectronTools/data/scalesNewReg-May2013.csv"),
    linearityCorrectionsInputPath = cms.string("EgammaAnalysis/ElectronTools/data/linearityNewReg-May2013.csv")
)
