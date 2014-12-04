import FWCore.ParameterSet.Config as cms

generator = cms.EDFilter("HZHAGeneratorFilter",
  CardsPath = cms.string("CMGTools/PFSim/python/generators/hzha04.cards")
)

