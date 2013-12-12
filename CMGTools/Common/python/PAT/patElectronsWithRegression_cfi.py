import FWCore.ParameterSet.Config as cms

patElectronsWithRegression = cms.EDProducer("RegressionEnergyPatElectronProducer",
       debug = cms.untracked.bool(False),
       inputElectronsTag = cms.InputTag('patElectronsWithRegressionVars'),
       inputCollectionType = cms.uint32(1),
       useRecHitCollections = cms.bool(False),
       produceValueMaps = cms.bool(False),
       regressionInputFile = cms.string("EgammaAnalysis/ElectronTools/data/eleEnergyRegWeights_WithSubClusters_VApr15.root"),
       energyRegressionType = cms.uint32(2),
       rhoCollection = cms.InputTag('kt6PFJets:rho:RECO'),
       vertexCollection = cms.InputTag('goodOfflinePrimaryVertices'),
       nameEnergyReg = cms.string("eneRegForGsfEle"),
       nameEnergyErrorReg = cms.string("eneErrorRegForGsfEle"),
       recHitCollectionEB = cms.InputTag('reducedEcalRecHitsEB'),
       recHitCollectionEE = cms.InputTag('reducedEcalRecHitsEE')
)
