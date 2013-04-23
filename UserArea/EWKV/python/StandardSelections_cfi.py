import FWCore.ParameterSet.Config as cms

Trigger = cms.PSet( source = cms.InputTag("TriggerResults::HLT"),
                    triggerPaths = cms.vstring(),
                    triggerCats  = cms.vstring()
                    )

Generator= cms.PSet( source = cms.InputTag("genParticles") )

Vertices = cms.PSet( source = cms.InputTag("offlinePrimaryVertices"),
                     beamSpot = cms.InputTag("offlineBeamSpot"),
                     maxZ = cms.double(24),
                     maxRho = cms.double(2.0),
                     minNDOF = cms.int32(4)
                     )

Muons = cms.PSet( source = cms.InputTag("patMuons"), 
                  minPt = cms.double(3),
                  maxEta = cms.double(2.4),
                  id = cms.string("loose"),
                  maxRelIso = cms.double(999999.),
                  )

Photons = cms.PSet( source = cms.InputTag("photons"),
                    conversions=cms.InputTag("allConversions"),
                    gsfElectrons = cms.InputTag("gsfElectrons"),
                    ebrechits = cms.InputTag("reducedEcalRecHitsEB"),
                    eerechits = cms.InputTag("reducedEcalRecHitsEE"),
                    minEt = cms.double(20), 
                    onlyEB = cms.bool(True)
                    )

Electrons = cms.PSet( source = cms.InputTag("selectedPatElectrons"),
                      id=cms.string("veto"),
                      minPt = cms.double(20),
                      maxEta = cms.double(2.5),
                      vetoEBtoEE = cms.bool(True),
                      minDeltaRtoMuons = cms.double(0.1)
                      )

BaseJetSelection = cms.PSet( source = cms.InputTag("selectedPatJets"),
                             rho = cms.InputTag("kt6PFJets:rho"),
                             minPt = cms.double(10),
                             maxEta = cms.double(4.7),
                             minDR2Leptons = cms.double(0.4)
                             )

BaseMetSelection = cms.PSet( source = cms.VInputTag("patMETsPFlow") )

