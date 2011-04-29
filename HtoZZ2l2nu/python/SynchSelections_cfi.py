import FWCore.ParameterSet.Config as cms

# base values for the vertex selection ------------------------------------------
BaseGeneratorSelection = cms.PSet( source = cms.InputTag("prunedGen"),
                                   filterId = cms.int32(25)
                                   )


# base values for the vertex selection ------------------------------------------
BaseVertexSelection = cms.PSet( source = cms.InputTag("offlinePrimaryVertices"),
                                maxZ = cms.double(24),
                                maxRho = cms.double(2.0),
                                minNDOF = cms.int32(5)
                                )

# base values for muon selection ----------------------------------------------
BaseMuonsSelection = cms.PSet( source = cms.InputTag("selectedPatMuons"),
                               minPt = cms.double(15),
                               maxEta = cms.double(2.4),
                               maxTrackChi2 = cms.double(10),
                               minValidTrackerHits = cms.int32(10),
                               minValidMuonHits=cms.int32(1),
                               id = cms.string("TMLastStationLoose"),
                               maxRelIso = cms.double(0.15)
                               )

# base values for electron selection ----------------------------------------------
BaseElectronsSelection = cms.PSet( source = cms.InputTag("selectedPatElectrons"),
                                   minPt = cms.double(15),
                                   minSuperClusterEt = cms.double(0),
                                   maxEta = cms.double(2.5),
                                   vetoTransitionElectrons = cms.bool(True),
                                   applyConversionVeto = cms.bool(True),
                                   maxTrackLostHits = cms.int32(1),
                                   id = cms.string("simpleEleId80relIso"),
                                   maxRelIso = cms.double(0.15),
                                   minDeltaRtoMuons = cms.double(0.1)
                                   )

#my base values for jet selection -------------------------------------------------
BaseJetSelection = cms.PSet( source = cms.InputTag("selectedPatJets"),
                             jetId = cms.PSet( version = cms.string("FIRSTDATA"), quality = cms.string("LOOSE") ),
                             minPt = cms.double(25),
                             maxEta = cms.double(2.5),
                             minDeltaRtoLepton = cms.double(0.4)
                             )

# base values for the dilepton selection ------------------------------------------
BaseDileptonSelection = cms.PSet( minDileptonMass = cms.double(0),
                                  maxDileptonMass = cms.double(7000),
                                  constrainByVertex = cms.bool(False),
                                  maxDxy = cms.double(0.02),
                                  maxDz = cms.double(0.1)
                                  )

# base values for met selection -----------------------------------------------------
BaseMetSelection = cms.PSet( source = cms.InputTag("patMETs") )

