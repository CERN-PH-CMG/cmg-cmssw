import FWCore.ParameterSet.Config as cms

# base values for trigger event
BaseTriggerSelection = cms.PSet( source = cms.InputTag("TriggerResults::HLT"),
                                 triggerPaths = cms.vstring(),
                                 triggerObjects = cms.VInputTag("eleTriggerMatchPFlow","muTriggerMatchPFlow")
                                 )
from CMGTools.HtoZZ2l2nu.TriggerSequences_cff import getTriggerPaths
doubleEle, doubleMu, muEG, singleEle, singleMu, mcTrigs = getTriggerPaths()
BaseTriggerSelection.triggerPaths.extend(doubleEle)
BaseTriggerSelection.triggerPaths.extend(doubleMu)
BaseTriggerSelection.triggerPaths.extend(muEG)
#BaseTriggerSelection.triggerPaths.extend(singleEle)
BaseTriggerSelection.triggerPaths.extend(singleMu)
BaseTriggerSelection.triggerPaths.extend(mcTrigs)







# base values for the vertex selection ------------------------------------------
BaseGeneratorSelection = cms.PSet( source = cms.InputTag("prunedGen"),
                                   filterId = cms.int32(25),
                                   genJets=cms.InputTag("ak5GenJets"),
                                   puReweight=cms.InputTag("puWeights:puWeight"),
                                   normPuReweight=cms.InputTag("puWeights:renPuWeight"),
                                   higgsPtWeights=cms.VInputTag("hKfactorStd",
                                                                "hKfactorRup","hKfactorRdown",
                                                                "hKfactorFup","hKfactorFdown")
                                   )


# base values for the vertex selection ------------------------------------------
BaseVertexSelection = cms.PSet( source = cms.InputTag("offlinePrimaryVertices"),
                                beamSpot = cms.InputTag("offlineBeamSpot"),
                                maxZ = cms.double(24),
                                maxRho = cms.double(2.0),
                                minNDOF = cms.int32(7)
                                )

# base values for muon selection ----------------------------------------------
BaseMuonsSelection = cms.PSet( source = cms.InputTag("selectedPatMuonsPFlow"),
                               minPt = cms.double(5),
                               maxEta = cms.double(2.4),
                               maxTrackChi2 = cms.double(10),
                               minValidTrackerHits = cms.int32(11),
                               minValidMuonHits=cms.int32(1),
                               maxDistToBeamSpot=cms.double(0.02),
                               id = cms.string(""),
                               maxRelIso = cms.double(1.0)
                               )

# base values for photon selection ----------------------------------------------
BasePhotonsSelection = cms.PSet( source = cms.InputTag("patPhotons"),
                                 ebrechits = cms.InputTag("reducedEcalRecHitsEB"),
                                 eerechits = cms.InputTag("reducedEcalRecHitsEE"),
                                 minEt = cms.double(0), 
                                 maxEta = cms.double(2.5),
                                 maxSihihEB = cms.double(0.013),
                                 maxSihihEE = cms.double(0.03),
                                 maxHoE = cms.double(0.05),
                                 minSipipEB = cms.double(0.0)
                                 )

# base values for electron selection ----------------------------------------------
BaseElectronsSelection = cms.PSet( source = cms.InputTag("selectedPatElectronsPFlow"),
                                   minPt = cms.double(5),
                                   minSuperClusterEt = cms.double(5),
                                   maxEta = cms.double(2.5),
                                   vetoTransitionElectrons = cms.bool(True),
                                   applyConversionVeto = cms.bool(True),
                                   maxDistToBeamSpot=cms.double(0.04),
                                   maxTrackLostHits = cms.int32(1),
                                   id = cms.string("eidTightMC"),#eidTight"),
                                   maxRelIso = cms.double(1.0),
                                   minDeltaRtoMuons = cms.double(0.1)
                                   )

#my base values for jet selection -------------------------------------------------
BaseJetSelection = cms.PSet( source = cms.InputTag("selectedPatJetsPFlow"),
                             rho = cms.InputTag("kt6PFJetsPFlow:rho"),
                             jetId = cms.PSet( version = cms.string("FIRSTDATA"), quality = cms.string("LOOSE") ),
                             minPt = cms.double(15),
                             maxEta = cms.double(5.0),
                             minDeltaRtoLepton = cms.double(0.4)
                             )

# base values for the dilepton selection ------------------------------------------
BaseDileptonSelection = cms.PSet( minDileptonMass = cms.double(0),
                                  maxDileptonMass = cms.double(7000),
                                  minPt = cms.double(20),
                                  maxEleRelIso = cms.double(0.15),
                                  maxMuRelIso = cms.double(0.15),
                                  maxDz = cms.double(1.0)
                                  )

# base values for met selection -----------------------------------------------------
BaseMetSelection = cms.PSet( source = cms.InputTag("patMETsPFlow"),
                             trksource = cms.InputTag("hzzPFMetProducer:hzzTkMet"),
                             pfnopusource = cms.InputTag("hzzPFMetProducer:hzzPfMetNoPileup")
#                             hzzmetSources = cms.VInputTag("hzzPFMetProducer:hzzPfMet",
#                                                           "hzzPFMetProducer:hzzTkMet",
#                                                           "hzzPFMetProducer:hzzPfMetNoPileup",
#                                                           "hzzPFMetProducer:hzzPfMetNoPileupJetNeutralVeto",
#                                                           "hzzPFMetProducer:hzzPfMetNoPileupClusteredNeutrals")
                             )

