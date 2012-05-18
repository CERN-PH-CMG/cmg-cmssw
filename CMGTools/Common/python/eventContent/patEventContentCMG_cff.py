import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning, patTriggerEventContent, patTriggerStandAloneEventContent

from CMGTools.Common.eventContent.everything_cff import everything 

patEventContentCMG = cms.untracked.vstring()
patEventContentCMG.extend( patEventContentNoCleaning )
patEventContentCMG.extend( patTriggerEventContent )
patEventContentCMG.extend( patTriggerStandAloneEventContent )

patEventContentCMG.extend([
    # all the commented lines are needed at CMG-tuple step if we do a simple cloning of the sequences. Need to see if we can gain time / disk space later by organizing the CMG sequences properly
    # 'drop patElectrons_selectedPat*_*_*',
    # 'keep patElectrons_patElectronsWithTrigger_*_*',
    'keep patConversions_patConversions_*_*',
    # 'drop patMuons_selectedPatMuons_*_*',
    # 'keep patMuons_patMuonsWithMVA_*_*',
    'drop patTriggerObjectStandAlonesedmAssociation_*_*_*',
    # FIXME trying to get access to tau lead track... external embedding maybe? 
    # 'keep recoTracks_*_*_PAT',
    'keep *_selectedPatTaus_*_*',
    'keep *_selectedPatTausAK5*_*_*',
    'drop *_selectedPatMuonsAK5NoPUSub_*_*',
    'drop *_selectedPatElectronsAK5NoPUSub_*_*',
    'drop CaloTowers_*_*_*',
    'drop patPFParticles_*_*_*',
    'keep recoVertexs_*_*_*',
    'keep recoPFCandidates_*_*_RECO',
    # drop intermediate PFBRECO collections
    'drop recoPFCandidates_*_*_PAT',
    # the following collection is the collection of PFCandidates used to build the no CHS jets. 
    'keep recoPFCandidates_pfNoPileUp_*_*',
    'keep recoGenParticles_*_*_*',
    'keep recoBeamSpot_*_*_*',
    'keep L1GlobalTriggerObjectMapRecord_*_*_*',
    'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*',
    'keep *_TriggerResults_*_*',
    'keep *_hltTriggerSummaryAOD_*_*',
    # this guys is already present in the AK5 sequence
    'drop recoVertexs_goodOfflinePrimaryVerticesLC_*_*',
    'keep *_PFMETSignificance*_*_*',
    'keep recoPFMETs_pfMetForRegression__*',
    'keep recoPFMETs_nopuMet__*',
    'keep recoPFMETs_puMet__*',
    'keep recoPFMETs_pcMet__*',    
    'keep recoPFMETs_tkMet__*',    
    'keep double_*_rho_*',
    # drop pat genjets embedding collections
    'drop recoGenJets_*Pat*_genJets_PAT',
    'keep recoGenJets_ak5GenJetsNoNu_*_*',
    # keep pruned jets
    'keep recoPFJets_ak5PFJetsCHSpruned_SubJets_*',
    'keep patJets_selectedPatJetsCHSpruned_*_*',
    'keep *_puJetId_*_*',
    'keep *_puJetIdCHS_*_*',
    'keep *_phPFIsoDepositCharged_*_*',
    'keep *_phPFIsoDepositChargedAll_*_*',
    'keep *_phPFIsoDepositNeutral_*_*',
    'keep *_phPFIsoDepositGamma_*_*',
    'keep *_phPFIsoDepositPU_*_*',
    'keep *_pfSelectedPhotons_*_*',
    'keep *_particleFlow_*_PAT'
    ])

patEventContentCMG.extend( everything )
