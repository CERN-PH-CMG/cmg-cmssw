import FWCore.ParameterSet.Config as cms

triggerProcessName = 'REDIGI37X'
##### Config some trigger matching with the pat::Muons
from PhysicsTools.PatAlgos.triggerLayer1.triggerMatcher_cfi import cleanMuonTriggerMatchHLTMu9
from PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cfi import patTrigger
from PhysicsTools.PatAlgos.triggerLayer1.triggerMatchEmbedder_cfi import cleanPatMuonsTriggerMatch

patMuonTriggerResults = patTrigger.clone(processName = cms.string(triggerProcessName))
patMuonTriggerMatch = cleanMuonTriggerMatchHLTMu9.clone(
                                src = cms.InputTag('selectedPatMuonsPFlow'),
                                matched = cms.InputTag( "patMuonTriggerResults" )
)
triggeredPatMuons = cleanPatMuonsTriggerMatch.clone(src = patMuonTriggerMatch.src,
                                                                 matches = cms.VInputTag('patMuonTriggerMatch'))
patMuonTrigger = cms.Sequence(patMuonTriggerResults*patMuonTriggerMatch*triggeredPatMuons)
#####