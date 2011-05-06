import FWCore.ParameterSet.Config as cms

##### Config some trigger matching with the pat::Muons
from PhysicsTools.PatAlgos.triggerLayer1.triggerMatcher_cfi import cleanMuonTriggerMatchHLTMu20
from PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cfi import patTrigger
from PhysicsTools.PatAlgos.triggerLayer1.triggerMatchEmbedder_cfi import cleanPatMuonsTriggerMatch

patMuonTriggerResults = patTrigger.clone(processName = cms.string('*'))
patMuonTriggerMatch = cleanMuonTriggerMatchHLTMu20.clone(
                                src = cms.InputTag('selectedPatMuonsPFlow'),
                                matched = cms.InputTag( "patMuonTriggerResults" ),
                                matchedCuts = cms.string( 'path( "HLT_Mu9" ) ||'+\
                                                          'path( "HLT_Mu11" ) ||'+\
                                                          'path( "HLT_Mu13_v*" ) ||'+\
                                                          'path( "HLT_Mu15_v*" ) ||'+\
                                                          'path( "HLT_Mu17_v*" ) ||'+\
                                                          'path( "HLT_Mu19_v*" ) ||'+\
                                                          'path( "HLT_Mu20_v*" ) ||'+\
                                                          'path( "HLT_Mu21_v*" )'
                                                          )
)
triggeredPatMuons = cleanPatMuonsTriggerMatch.clone(src = patMuonTriggerMatch.src,
                                                                 matches = cms.VInputTag('patMuonTriggerMatch'))
patMuonTrigger = cms.Sequence(patMuonTriggerResults*patMuonTriggerMatch*triggeredPatMuons)
#####

