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
BaseTriggerSelection.gammaTriggers = cms.vstring('HLT_Photon20_CaloIdVL_IsoL',
                                                 'HLT_Photon30_CaloIdVL_IsoL',
                                                 'HLT_Photon50_CaloIdVL_IsoL',
                                                 'HLT_Photon75_CaloIdVL_IsoL',
                                                 'HLT_Photon90_CaloIdVL_IsoL',
                                                 'HLT_Photon125',
                                                 'HLT_Photon135',
                                                 'HLT_Photon200'
                                                 )

# base values for the vertex selection ------------------------------------------
BaseGeneratorSelection = cms.PSet( source = cms.InputTag("prunedGen"),
                                   filterId = cms.int32(25),
                                   genJets=cms.InputTag("ak5GenJets"),
                                   puReweight=cms.InputTag("puWeights:puWeight")
                                   )


# base values for the vertex selection ------------------------------------------
BaseVertexSelection = cms.PSet( source = cms.InputTag("offlinePrimaryVertices"),
                                beamSpot = cms.InputTag("offlineBeamSpot"),
                                maxZ = cms.double(24),
                                maxRho = cms.double(2.0),
                                minNDOF = cms.int32(4)
                                )

# base values for muon selection ----------------------------------------------
BaseMuonsSelection = cms.PSet( source = cms.InputTag("selectedPatMuonsPFlow"),
                               minPt = cms.double(20),
                               maxEta = cms.double(2.4),
                               requireGlobal = cms.bool(True),
                               requireTracker = cms.bool(True),
                               maxTrackChi2 = cms.double(10),
                               minValidTrackerHits = cms.int32(11),
                               minValidMuonHits=cms.int32(1),
                               maxDistToBeamSpot=cms.double(0.02),
                               id = cms.string(""),#TMLastStationAngTight"),
                               maxRelIso = cms.double(0.15),
                               usePFIso = cms.bool(False),
                               doDeltaBetaCorrection = cms.bool(False)
                               )

# base values for loose muon selection ----------------------------------------------
BaseLooseMuonsSelection = BaseMuonsSelection.clone( minPt = cms.double(5),
                                                    maxTrackChi2 = cms.double(9999.),
                                                    id = cms.string("TrackerMuonArbitrated"),                               
                                                    maxRelIso = cms.double(0.5),
                                                    minValidMuonHits=cms.int32(0)
                                                    )

# base values for photon selection ----------------------------------------------
BasePhotonsSelection = cms.PSet( source = cms.InputTag("selectedPatPhotons"),
                                 conversionSource = cms.InputTag("allConversions"),
                                 ebrechits = cms.InputTag("reducedEcalRecHitsEB"),
                                 eerechits = cms.InputTag("reducedEcalRecHitsEE"),
                                 minEt = cms.double(0), 
                                 maxEta = cms.double(2.5),
                                 maxHoE = cms.double(0.05),
                                 minSipipEB = cms.double(0.001),
                                 minSihihEB = cms.double(0.001),
                                 maxSihihEB = cms.double(0.011),
                                 maxSihihEE = cms.double(0.03),
                                 minR9 = cms.double(-1),
                                 trkIsoCoeffsEB = cms.vdouble(2.0,  0.001,  0.0167),
                                 trkIsoCoeffsEE = cms.vdouble(2.0,  0.001,  0.0032),
                                 ecalIsoCoeffsEB = cms.vdouble(4.2, 0.006,  0.183),
                                 ecalIsoCoeffsEE = cms.vdouble(4.2, 0.006,  0.090),
                                 hcalIsoCoeffsEB = cms.vdouble(2.2, 0.0025, 0.062),
                                 hcalIsoCoeffsEE = cms.vdouble(2.2, 0.025,  0.180),
                                 requireNoElectron = cms.bool(True)
                                 )

# base values for electron selection ----------------------------------------------
BaseElectronsSelection = cms.PSet( source = cms.InputTag("selectedPatElectronsPFlow"),
                                   minPt = cms.double(20),
                                   minSuperClusterEt = cms.double(17),
                                   maxEta = cms.double(2.5),
                                   vetoTransitionElectrons = cms.bool(True),
                                   applyConversionVeto = cms.bool(True),
                                   maxDistToBeamSpot=cms.double(0.04),
                                   maxTrackLostHits = cms.int32(1),
                                   id = cms.string("eidVBTF85"),
                                   maxRelIso = cms.double(0.15),
                                   minDeltaRtoMuons = cms.double(0.1),
                                   usePFIso = cms.bool(False),
                                   doDeltaBetaCorrection = cms.bool(False)
                                   )

# base values for electron selection ----------------------------------------------
BaseLooseElectronsSelection = BaseElectronsSelection.clone( minPt = cms.double(5),
                                                            minSuperClusterEt = cms.double(5),
                                                            vetoTransitionElectrons = cms.bool(False),
                                                            maxTrackLostHits = cms.int32(2),
                                                            id = cms.string("eidVBTF95"),
                                                            maxRelIso = cms.double(0.5),
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



AssocJetSelection = cms.PSet( source = cms.InputTag("ClusteredPFMetProducer", "JET"),
                             rho = cms.InputTag("kt6PFJetsPFlow:rho"),
                             jetId = cms.PSet( version = cms.string("FIRSTDATA"), quality = cms.string("LOOSE") ),
                             minPt = cms.double(15),
                             maxEta = cms.double(5.0),
                             minDeltaRtoLepton = cms.double(0.4)
                             )



# base values for the dilepton selection ------------------------------------------
BaseDileptonSelection = cms.PSet( minDileptonMass = cms.double(0),
                                  maxDileptonMass = cms.double(7000),
                                  maxDz = cms.double(1.0)
                                  )

# base values for met selection -----------------------------------------------------
BaseMetSelection = cms.PSet( source = cms.InputTag("patMETsPFlow"),
                             trksource = cms.InputTag("trackMetProducer"),
                             hzzmetSources = cms.VInputTag("ClusteredPFMetProducer:assoc",                #1
                                                           "ClusteredPFMetProducer:standard",             #2  
                                                           "ClusteredPFMetProducer:central",              #3
                                                           "ClusteredPFMetProducer:cleaned",              #4 
                                                           "ClusteredPFMetProducer:assocCharged",         #5 
                                                           "ClusteredPFMetProducer:assocWithFwd",         #6
                                                           "ClusteredPFMetProducer:assoc",             #7  //to be replaced by something else
                                                           "ClusteredPFMetProducer:assocWithFwd",      #8  //to be replaced by something else
                                                           "ClusteredPFMetProducer:assoc",             #9  //to be replaced by something else
                                                           "ClusteredPFMetProducer:assocWithFwd",      #10 //to be replaced by something else
                                                           "ClusteredPFMetProducer:assocBeta",            #11
                                                           "ClusteredPFMetProducer:assocWithFwdBeta"),    #12
                             pfCands = cms.InputTag("particleFlow"),
                             pvAssocCandidatesSource = cms.InputTag("ClusteredPFMetProducer:pvAssocCandidates"),
                             sumEtSources = cms.InputTag("ClusteredPFMetProducer:globalPfMetSums")
                             )

