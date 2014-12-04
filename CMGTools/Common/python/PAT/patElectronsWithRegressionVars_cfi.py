import FWCore.ParameterSet.Config as cms


patElectronsWithRegressionVars = cms.EDProducer('PATElectronRegressionEmbedder',
                                     electrons    = cms.InputTag("patElectronsWithDirectionalIsolation"),
)                                 

