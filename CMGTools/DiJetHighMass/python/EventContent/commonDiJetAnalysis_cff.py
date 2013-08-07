import FWCore.ParameterSet.Config as cms

#"""Event contents for Colin's MET studies in di-jet events"""

commonDiJetHighMass = cms.untracked.vstring(
    'drop *',
#    'keep *_*_*_*',
    'keep *_ak5GenJets_*_*',	
    'keep *_ak7GenJets_*_*',	
    #------- CaloJet collections ------
    'keep *_ak5CaloJets_*_*',
    'keep *_ak5JetID_*_*',
    'keep *_ak5JetExtender_*_*',
    'keep *_ak7CaloJets_*_*',
    'keep *_ak7JetID_*_*',
    'keep *_ak7JetExtender_*_*',
    #------- PFJet collections --------
    'keep *_kt6PFJets_rho_SKIM',
    'keep *_kt6PFJets_sigma_SKIM',
    'keep *_ak5PFJets_*_SKIM',
    'keep *_ak7PFJets_*_SKIM',
    #------- Trigger collections ------
    'keep edmTriggerResults_TriggerResults_*_*',
    'keep *_hltTriggerSummaryAOD_*_*',
    'keep L1GlobalTriggerObjectMapRecord_*_*_*',
    'keep L1GlobalTriggerReadoutRecord_*_*_*',
    #------- Various collections ------
    'keep *_EventAuxilary_*_*',
    'keep *_offlinePrimaryVertices_*_*',
    'keep *_offlinePrimaryVerticesWithBS_*_*',
    #------- MET collections ----------
    'keep *_met_*_*',
    'keep *_pfMet_*_*',

    #------- CMG ANalysis ------------
    'keep  cmgBaseJets_*_*_*',
    'keep  cmgPFJets_*_*_*',
    'keep  cmgBaseJetcmgBaseJetcmgDiObjects_*_*_*',
    'keep  cmgFatJets_*_*_*',
    'keep  cmgPFJetcmgPFJetcmgDiObjects_*_*_*',
    'keep  cmgFatJetcmgFatJetcmgDiObjects_*_*_*',
    'keep  cmgBaseMETs_cmgPFMET_*_*',
    )
