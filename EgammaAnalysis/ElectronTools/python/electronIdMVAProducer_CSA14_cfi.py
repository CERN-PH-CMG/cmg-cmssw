import FWCore.ParameterSet.Config as cms

mvaTrigV0CSA14 = cms.EDFilter("ElectronIdMVAProducerCSA14",
                         verbose = cms.untracked.bool(False),
                         vertexTag = cms.InputTag('offlinePrimaryVertices'),
                         electronTag = cms.InputTag('gedGsfElectrons'),
                         reducedEBRecHitCollection = cms.InputTag('reducedEcalRecHitsEB'),
                         reducedEERecHitCollection = cms.InputTag('reducedEcalRecHitsEE'),
                         method = cms.string("BDT"),
                         mvaWeightFile = cms.vstring(
                                                     "EgammaAnalysis/ElectronTools/data/CSA14/TrigIDMVA_EB_BDT.weights.xml",
                                                     "EgammaAnalysis/ElectronTools/data/CSA14/TrigIDMVA_EE_BDT.weights.xml",
    ),
                         Trig = cms.bool(True),
)



#mvaNonTrigV0CSA14 = cms.EDFilter("ElectronIdMVAProducerCSA14",
#                            verbose = cms.untracked.bool(False),
#                            vertexTag = cms.InputTag('offlinePrimaryVertices'),
#                            electronTag = cms.InputTag('gedGsfElectrons'),
#                            reducedEBRecHitCollection = cms.InputTag('reducedEcalRecHitsEB'),
#                            reducedEERecHitCollection = cms.InputTag('reducedEcalRecHitsEE'),
#                            method = cms.string("BDT"),
#                            mvaWeightFile = cms.vstring(
#                                                        "EgammaAnalysis/ElectronTools/data/CSA14/EIDmva_EB_5_old_BDT.weights.xml",
#                                                        "EgammaAnalysis/ElectronTools/data/CSA14/EIDmva_EE_5_old_BDT.weights.xml",
#                                                        "EgammaAnalysis/ElectronTools/data/CSA14/EIDmva_EB_10_old_BDT.weights.xml",
#                                                        "EgammaAnalysis/ElectronTools/data/CSA14/EIDmva_EE_10_old_BDT.weights.xml",
#                                  ),
#                            Trig = cms.bool(False),
#)
