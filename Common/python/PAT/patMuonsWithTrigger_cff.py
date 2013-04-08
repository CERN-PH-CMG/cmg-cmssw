import FWCore.ParameterSet.Config as cms

muonSource = cms.InputTag('patMuonsWithDirectionalIsolation')

muonTriggerMatchHLT = cms.EDProducer( 'PATTriggerMatcherDRDPtLessByR',
    src     = muonSource,
    matched = cms.InputTag( 'patTrigger' ),
    matchedCuts = cms.string(''),
    maxDPtRel = cms.double( 0.5 ),
    maxDeltaR = cms.double( 0.1 ),
    resolveAmbiguities    = cms.bool( True ),
    resolveByMatchQuality = cms.bool( True )
)

muonMatchHLTL3 = muonTriggerMatchHLT.clone(
    matchedCuts = cms.string('coll("hltL3MuonCandidates")'),
    maxDeltaR = 0.1, maxDPtRel = 10.0
    )  
muonMatchHLTL3T = muonTriggerMatchHLT.clone(
    matchedCuts = cms.string('coll("hltGlbTrkMuonCands")'),
    maxDeltaR = 0.1, maxDPtRel = 10.0
    ) 

## ==== Embed ====
patMuonsWithTrigger = cms.EDProducer( "PATTriggerMatchMuonEmbedder",
    src     = muonSource,
    matches = cms.VInputTag( cms.InputTag('muonMatchHLTL3'),
                             cms.InputTag('muonMatchHLTL3T') )
)


patMuonsWithTriggerSequence = cms.Sequence(
      muonMatchHLTL3 +
      muonMatchHLTL3T +
      patMuonsWithTrigger
)

