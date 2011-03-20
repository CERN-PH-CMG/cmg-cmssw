import FWCore.ParameterSet.Config as cms

# base values for muon selection ---------------------------------------
BaseMuonsSelection = cms.PSet( source = cms.InputTag("selectedPatMuons"),
                               minPt = cms.double(20),
                               maxEta = cms.double(2.4),
                               id = cms.string('TMLastStationLoose'),
                               maxD0 = cms.double(200),
                               maxRelIso = cms.double(0.5),
                               maxTrackChi2 = cms.double(10),
                               minTrackValidHits = cms.int32(10),
                               minMuonHuits=cms.int32(10)
                               )

# base values for electron selection ----------------------------------------------
BaseElectronsSelection = cms.PSet( source = cms.InputTag("selectedPatElectrons"),
                                   minEt = cms.double(20),
                                   minSCEt = cms.double(17),
                                   maxEta = cms.double(2.5),
                                   id = cms.string('simpleEleId90relIso'),
                                   maxD0 = cms.double(400),
                                   maxRelIso = cms.double(0.5),
                                   maxTrackLostHits = cms.int32(1),
                                   minDeltaRtoMuons = cms.double(0.1)
                                   )

#my base values for jet selection -----------------------------------------------
BaseJetSelection = cms.PSet( source = cms.InputTag("selectedPatJets"),
                             jetId = cms.PSet( version = cms.string("FIRSTDATA"), quality = cms.string("LOOSE") ),
                             minPt = cms.double(30),
                             maxEta = cms.double(2.5),
                             minDeltaRtoLepton = cms.double(0.4)
                             )

#my base values for met selection ------------------------------------------------
BaseMetSelection = cms.PSet( sources = cms.VInputTag("patMET") )

