from PhysicsTools.PatAlgos.patTemplate_cfg import *

from PhysicsTools.Configuration.SUSY_pattuple_cff import getSUSY_pattuple_outputCommands



RA2Official = getSUSY_pattuple_outputCommands( process )


RA2Official += [
  'keep edmMergeableCounter_*Counter_*_*',
  'keep recoPFRecHits_particleFlow*_Cleaned_*',
  'keep recoPFCandidates_particleFlow_*_*',
  'keep recoMuons_muons_*_*',
  'keep recoGsfTracks_electronGsfTracks_*_*',
  'keep recoGsfElectrons_gsfElectrons_*_*',
  'drop recoElectronSeeds_*_*_*',
  'keep *_gsfElectronCores_*_*',
  'keep *_photonCore_*_*',
  'keep recoConversions_conversions_*_*',
  # added so that we keep the muon tracks, for the greedy muon filter to work
  'keep recoTracks_*_*_*',
  'drop recoTracks_*onversions_*_*',
  'keep *_reducedEcalRecHits*_*_*',
  'drop *_badEcalRecHits_*_*',
  'drop *_badHcalRecHits_*_*',
  'keep *_badTowers_*_*',
  'keep *_badMET_*_*',
  'drop *_kt*_*_*',
  'drop *_ak7*_*_*',
  'drop *_iterativeCone5*_*_*',
  'drop recoTrackJets_ak5TrackJets__*',
  'drop recoRecoChargedRefCandidates_trackRefsForJets__*',
  'drop patMHTs_*_*_*',
  'keep HcalNoiseSummary_*_*_*',
  'drop edmTriggerResults_TriggerResults__*',
  'keep edmTriggerResults_TriggerResults__HLT',
  'keep edmTriggerResults_TriggerResults__REDIGI',
  'keep edmTriggerResults_TriggerResults__REDIGI36',
  'keep *_patPhotons_*_*',
  'keep *_patElectrons_*_*',
  'keep *_patMuons_*_*',
  'drop *_patTaus_*_*',
  'keep recoPFMETs_pfMet__*',
  'keep *_patMETs*_*_*',
  'keep *_patJetsAK5*_*_*',
  'keep *_patJetsIC5*_*_*',
  'drop recoBaseTagInfosOwned_*_tagInfos_*',
  'drop CaloTowers_*_caloTowers_*',
  'drop recoPFCandidates_*_pfCandidates_*',
  'drop recoGenJets_*_genJets_*',
  'drop recoVertexs_offlinePrimaryVerticesWithBS_*_*',
  'keep recoVertexs_goodVertices_*_*',
]


from PhysicsTools.PatAlgos.patEventContent_cff import *
RA2Official += patTriggerStandAloneEventContent
