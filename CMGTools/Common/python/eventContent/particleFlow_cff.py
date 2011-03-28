import FWCore.ParameterSet.Config as cms


particleFlowJets = cms.untracked.vstring(
    'keep *_cmgPFJetSel_*_*',
    'keep *_cmgPFBaseJetSel_*_*',
    'keep cmgAbstractPhysicsObjects_*PF*Jet*_*_*'
    # here add all PF jet objects, even the base ones. 
    )

particleFlowMET = cms.untracked.vstring(
    'keep  *_cmgPFMET_*_*',                                    
    # 'keep  *_cmgMETPFCandidates*_*_*',
    # here add the other PF-based MET objects, even the base ones. 
    )

# to be added to the output module if needed
particleFlowMHT = cms.untracked.vstring(
    'keep  *_cmgMHTPFJets_*_*'
    )

particleFlowMuons = cms.untracked.vstring(
    # does not look like PF?     
    'keep *_cmgMuonSel_*_*',
    'keep *_cmgDiMuonSel_*_*'
    )

particleFlowElectrons = cms.untracked.vstring(
    # does not look like PF? 
    'keep *_cmgElectronSel_*_*',
    'keep *_cmgDiElectronSel_*_*'
    )

particleFlowW = cms.untracked.vstring(
    # does not look like PF? 
    'keep *_cmgWENuSel_*_*',
    'keep *_cmgWMuNuSel_*_*',
    'keep *_cmgWTauNu_*_*'                                  
    )

particleFlowVBF = cms.untracked.vstring(
    'keep *_vbfEventJetJetPF_*_*'                                 
    )

particleFlow = particleFlowJets + particleFlowMET + particleFlowMuons + particleFlowElectrons + particleFlowW + particleFlowVBF


particleFlowBase = cms.untracked.vstring(
    'keep *_cmgPFBaseJetSel_*_*',
    # here add the other generic PF objects
    )
