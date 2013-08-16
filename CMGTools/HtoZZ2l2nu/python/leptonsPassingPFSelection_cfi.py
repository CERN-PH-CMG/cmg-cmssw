import FWCore.ParameterSet.Config as cms

leptonsPassingPF = cms.EDProducer("LeptonsPassingPFSelection",
                                muons = cms.InputTag("probeMuons"),
                                pf    = cms.InputTag("patMuonsPFlow"),
                                pfCut = cms.string(""),
                                )


