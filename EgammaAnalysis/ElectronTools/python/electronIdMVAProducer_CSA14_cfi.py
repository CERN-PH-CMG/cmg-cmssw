import FWCore.ParameterSet.Config as cms


mvaTrigV050nsCSA14 = cms.EDFilter("ElectronIdMVAProducerCSA14",
                         verbose = cms.untracked.bool(False),
                         vertexTag = cms.InputTag('offlinePrimaryVertices'),
                         electronTag = cms.InputTag('gedGsfElectrons'),
                         reducedEBRecHitCollection = cms.InputTag('reducedEcalRecHitsEB'),
                         reducedEERecHitCollection = cms.InputTag('reducedEcalRecHitsEE'),
                         method = cms.string("BDT"),
                         mvaWeightFile = cms.vstring(
                                                     "EgammaAnalysis/ElectronTools/data/CSA14/TrigIDMVA_50ns_EB_BDT.weights.xml",
                                                     "EgammaAnalysis/ElectronTools/data/CSA14/TrigIDMVA_50ns_EE_BDT.weights.xml",
    ),
                         Trig = cms.bool(True),
)


mvaTrigV025nsCSA14 = cms.EDFilter("ElectronIdMVAProducerCSA14",
                                   verbose = cms.untracked.bool(False),
                                   vertexTag = cms.InputTag('offlinePrimaryVertices'),
                                   electronTag = cms.InputTag('gedGsfElectrons'),
                                   reducedEBRecHitCollection = cms.InputTag('reducedEcalRecHitsEB'),
                                   reducedEERecHitCollection = cms.InputTag('reducedEcalRecHitsEE'),
                                   method = cms.string("BDT"),
                                   mvaWeightFile = cms.vstring(
                                                               "EgammaAnalysis/ElectronTools/data/CSA14/TrigIDMVA_25ns_EB_BDT.weights.xml",
                                                               "EgammaAnalysis/ElectronTools/data/CSA14/TrigIDMVA_25ns_EE_BDT.weights.xml",
                                                               ),
                                   Trig = cms.bool(True),
                                   )


mvaNonTrigV050nsCSA14 = cms.EDFilter("ElectronIdMVAProducerCSA14",
                            verbose = cms.untracked.bool(False),
                            vertexTag = cms.InputTag('offlinePrimaryVertices'),
                            electronTag = cms.InputTag('gedGsfElectrons'),
                            reducedEBRecHitCollection = cms.InputTag('reducedEcalRecHitsEB'),
                            reducedEERecHitCollection = cms.InputTag('reducedEcalRecHitsEE'),
                            method = cms.string("BDT"),
                            mvaWeightFile = cms.vstring(
                                                        "EgammaAnalysis/ElectronTools/data/CSA14/EIDmva_EB_5_50ns_BDT.weights.xml",
                                                        "EgammaAnalysis/ElectronTools/data/CSA14/EIDmva_EE_5_50ns_BDT.weights.xml",
                                                        "EgammaAnalysis/ElectronTools/data/CSA14/EIDmva_EB_10_50ns_BDT.weights.xml",
                                                        "EgammaAnalysis/ElectronTools/data/CSA14/EIDmva_EE_10_50ns_BDT.weights.xml",
                                  ),
                            Trig = cms.bool(False),
)

mvaNonTrigV025nsCSA14 = cms.EDFilter("ElectronIdMVAProducerCSA14",
                                     verbose = cms.untracked.bool(False),
                                     vertexTag = cms.InputTag('offlinePrimaryVertices'),
                                     electronTag = cms.InputTag('gedGsfElectrons'),
                                     reducedEBRecHitCollection = cms.InputTag('reducedEcalRecHitsEB'),
                                     reducedEERecHitCollection = cms.InputTag('reducedEcalRecHitsEE'),
                                     method = cms.string("BDT"),
                                     mvaWeightFile = cms.vstring(
                                                                 "EgammaAnalysis/ElectronTools/data/CSA14/EIDmva_EB_5_25ns_BDT.weights.xml",
                                                                 "EgammaAnalysis/ElectronTools/data/CSA14/EIDmva_EE_5_25ns_BDT.weights.xml",
                                                                 "EgammaAnalysis/ElectronTools/data/CSA14/EIDmva_EB_10_25ns_BDT.weights.xml",
                                                                 "EgammaAnalysis/ElectronTools/data/CSA14/EIDmva_EE_10_25ns_BDT.weights.xml",
                                                                 ),
                                     Trig = cms.bool(False),
                                     )

mvaNonTrigV025nsPHYS14 = cms.EDFilter("ElectronIdMVAProducerCSA14",
                                     verbose = cms.untracked.bool(False),
                                     vertexTag = cms.InputTag('offlinePrimaryVertices'),
                                     electronTag = cms.InputTag('gedGsfElectrons'),
                                     reducedEBRecHitCollection = cms.InputTag('reducedEcalRecHitsEB'),
                                     reducedEERecHitCollection = cms.InputTag('reducedEcalRecHitsEE'),
                                     method = cms.string("BDTSimpleCat"),
                                     mvaWeightFile = cms.vstring(
                                                                 "EgammaAnalysis/ElectronTools/data/PHYS14/EIDmva_EB1_5_oldscenario2phys14_BDT.weights.xml",
                                                                 "EgammaAnalysis/ElectronTools/data/PHYS14/EIDmva_EB2_5_oldscenario2phys14_BDT.weights.xml",
                                                                 "EgammaAnalysis/ElectronTools/data/PHYS14/EIDmva_EE_5_oldscenario2phys14_BDT.weights.xml",
                                                                 "EgammaAnalysis/ElectronTools/data/PHYS14/EIDmva_EB1_10_oldscenario2phys14_BDT.weights.xml",
                                                                 "EgammaAnalysis/ElectronTools/data/PHYS14/EIDmva_EB2_10_oldscenario2phys14_BDT.weights.xml",
                                                                 "EgammaAnalysis/ElectronTools/data/PHYS14/EIDmva_EE_10_oldscenario2phys14_BDT.weights.xml",
                                                                 ),
                                     Trig = cms.bool(False),
                                     )
