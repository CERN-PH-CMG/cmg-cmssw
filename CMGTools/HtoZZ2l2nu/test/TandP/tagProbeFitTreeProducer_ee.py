import FWCore.ParameterSet.Config as cms

process = cms.Process("TagProbe")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100

from CMGTools.HtoZZ2l2nu.localPatTuples_cff import configureFromCommandLine
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring()
                            )
castorDir, outputFile, process.source.fileNames = configureFromCommandLine()
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5000) )    

#selection
muontype = "isGlobalMuon && isTrackerMuon"
kinacc = muontype + " && pt > 20 && abs(eta) < 2.4"
goodtrk = kinacc + " && globalTrack.normalizedChi2<10 && globalTrack.hitPattern.numberOfValidTrackerHits>11 && globalTrack.hitPattern.numberOfValidMuonHits>1"  
iso = goodtrk + " && (neutralHadronIso+chargedHadronIso+photonIso)/pt < 0.2"

## Tags. 
process.tagMuons = cms.EDFilter("PATMuonRefSelector",
                                src = cms.InputTag("selectedPatMuonsPFlow"),
                                cut = cms.string(kinacc)
                                )
## Probes.
process.probeMuons = cms.EDFilter("PATMuonRefSelector",
                                  src = cms.InputTag("selectedPatMuonsPFlow"),
                                  cut = cms.string("isTrackerMuon && pt > 15"), 
                                  )

## Combine Tags and Probes into Z candidates, applying a mass cut
process.tpPairs = cms.EDProducer("CandViewShallowCloneCombiner",
                                 decay = cms.string("tagMuons@+ probeMuons@-"), # charge coniugate states are implied
                                 cut   = cms.string("60 < mass < 120"),
                                 )

## Make the tree
process.muonEffs = cms.EDAnalyzer("TagProbeFitTreeProducer",
                                  # pairs
                                  tagProbePairs = cms.InputTag("tpPairs"),
                                  arbitration   = cms.string("OneProbe"),
                                  # variables to use
                                  variables = cms.PSet( eta = cms.string("eta"),
                                                        pt  = cms.string("pt"),
                                                        nsegm = cms.string("numberOfMatches") 
                                                        ),
                                  flags = cms.PSet( passingMuon = cms.string(muontype),
                                                    passingKin = cms.string(kinacc),
                                                    passingTrk = cms.string(goodtrk),
                                                    passingIso = cms.string(iso)
                                                    ),
                                  # mc-truth info
                                  isMC = cms.bool(False),
                                  motherPdgId = cms.vint32(22,23),
                                  makeMCUnbiasTree = cms.bool(True),
                                  checkMotherInUnbiasEff = cms.bool(True),
                                  tagMatches = cms.InputTag("muMcMatch"),
                                  probeMatches  = cms.InputTag("muMcMatch"),
                                  allProbes     = cms.InputTag("probeMuons"),
                                  )

process.tagAndProbe = cms.Path( (process.tagMuons + process.probeMuons) *   # 'A*B' means 'B needs output of A'; 
                                #( process.probesPassingCal +  process.drToNearestJet   +
                                process.tpPairs 
                                #    + process.muMcMatch )
                                * process.muonEffs
                                )

process.TFileService = cms.Service("TFileService", fileName = cms.string("TagAndProbe_ZMuMu.root"))




