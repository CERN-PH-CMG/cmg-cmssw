import FWCore.ParameterSet.Config as cms

eleTriggerMatchHLT = cms.EDProducer("PATTriggerMatcherDRDPtLessByR",
   src     = cms.InputTag( "selectedPatElectrons" ),
   matched = cms.InputTag( "patTrigger" ),
   andOr   = cms.bool( False ),
   matchedCuts = cms.string('dummy'),
   maxDPtRel = cms.double( 0.5 ),
   maxDeltaR = cms.double( 0.5 ),
   resolveAmbiguities    = cms.bool( True ),
   resolveByMatchQuality = cms.bool( True )
)

eleTriggerMatchHLT1= eleTriggerMatchHLT.clone(matchedCuts = cms.string('coll("hltL1IsoRecoEcalCandidate")||coll("hltL1NonIsoRecoEcalCandidate")'))  
eleTriggerMatchHLT2= eleTriggerMatchHLT.clone(matchedCuts = cms.string('coll("hltPixelMatchElectronsL1Iso")||coll("hltPixelMatchElectronsL1NonIso")'))  
eleTriggerMatchHLT3= eleTriggerMatchHLT.clone(matchedCuts = cms.string('coll("hltPixelMatch3HitElectronsL1Iso")||coll("hltPixelMatch3HitElectronsL1NonIso")'))  
eleTriggerMatchHLT4= eleTriggerMatchHLT.clone(matchedCuts = cms.string('coll("hltPixelMatchElectronsActivity")'))  
eleTriggerMatchHLT5= eleTriggerMatchHLT.clone(matchedCuts = cms.string('coll("hltPixelMatch3HitElectronsActivity")'))  
eleTriggerMatchHLT6= eleTriggerMatchHLT.clone(matchedCuts = cms.string('coll("hltRecoEcalSuperClusterActivityCandidate")'))  
eleTriggerMatchHLT7= eleTriggerMatchHLT.clone(matchedCuts = cms.string('coll("hltL1SeededRecoEcalCandidate")'))  
eleTriggerMatchHLT8= eleTriggerMatchHLT.clone(matchedCuts = cms.string('coll("hltPixelMatch3HitElectronsL1Seeded")'))  
eleTriggerMatchHLT9= eleTriggerMatchHLT.clone(matchedCuts = cms.string('coll("hltPixelMatchElectronsL1Seeded")'))  
eleTriggerMatchHLT10= eleTriggerMatchHLT.clone(matchedCuts = cms.string('coll("hltEle27WP80PixelMatchElectronsL1Seeded")'))  



## ==== Embed ====
patElectronsWithTrigger = cms.EDProducer( "PATTriggerMatchElectronEmbedder",
    src     = cms.InputTag( "selectedPatElectrons" ),
    matches = cms.VInputTag(  cms.InputTag('eleTriggerMatchHLT1'),
                              cms.InputTag('eleTriggerMatchHLT2'),
                              cms.InputTag('eleTriggerMatchHLT3'),
                              cms.InputTag('eleTriggerMatchHLT4'),
                              cms.InputTag('eleTriggerMatchHLT5'),
                              cms.InputTag('eleTriggerMatchHLT6'),
                              cms.InputTag('eleTriggerMatchHLT7'),
                              cms.InputTag('eleTriggerMatchHLT8'),
                              cms.InputTag('eleTriggerMatchHLT9'),
                              cms.InputTag('eleTriggerMatchHLT10') )
)

patElectronsWithTriggerSequence = cms.Sequence(
   eleTriggerMatchHLT1+
   eleTriggerMatchHLT2+
   eleTriggerMatchHLT3+
   eleTriggerMatchHLT4+
   eleTriggerMatchHLT5+
   eleTriggerMatchHLT6+
   eleTriggerMatchHLT7+
   eleTriggerMatchHLT8+
   eleTriggerMatchHLT9+
   eleTriggerMatchHLT10+
   patElectronsWithTrigger

)

