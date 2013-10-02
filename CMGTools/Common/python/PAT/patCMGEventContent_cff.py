import FWCore.ParameterSet.Config as cms

patCMGEventContent = cms.untracked.vstring(
#    'keep recoPFCandidates_particleFlow_*_*',
    # Gen information
#    'keep *_genParticles_*_*',
#    'keep *_generator_*_*',
#    'keep *_genMetTrue_*_*',
#    'keep recoGenJets_ak5GenJets_*_*',
    #Counters
    'keep *_MEtoEDMConverter_*_*',
    'keep GenRunInfoProduct_*_*_*',
    # Vertex info
    'keep recoVertexs_*_*_*',
    #'keep *_generalTracks_*_*',
    'keep recoBeamSpot_*_*_*',
    # Trigger
    'keep L1GlobalTriggerObjectMapRecord_*_*_*',
    'keep *_TriggerResults_*_*',
    'keep *_hltTriggerSummaryAOD_*_*',
    # PU JetID
    "keep *_puJetId_*_*", # input variables
    "keep *_puJetMva_*_*" # final MVAs and working point flags
    )
