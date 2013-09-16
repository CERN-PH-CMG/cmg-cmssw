import FWCore.ParameterSet.Config as cms
import copy

from CMGTools.H2TauTau.eventContent.common_cff import * 

diTauSpecific = [
    'drop cmgTaucmgTaucmgDiObjects_*_*_*',
    'keep *_cmgDiTauCorSVFitFullSel_*_*',
    ]

diTauSpecificDebug = copy.deepcopy( diTauSpecific)
diTauSpecificDebug.extend( [
    'keep *_cmgDiTau_*_*',
    'keep *_cmgDiTauPreSel_*_*',
    'keep *_cmgDiTauFullSel_*_*',
    'keep *_recoilCorMETDiTau_*_*',
    'keep *_cmgDiTauCorPreSel_*_*',    
    ] )

diTau = copy.deepcopy( common )
diTau.extend( diTauSpecific )

diTauDebug = copy.deepcopy( commonDebug )
diTauDebug.extend( diTauSpecificDebug )

##### In case one has to run on a PAT tuple instead of CMG tuple make the event content explicit
diTau=["drop *"] 
diTau+=["keep *_source_*_*"] 
diTau+=["keep *_generator_*_*"] 
diTau+=["keep *_TriggerResults__*"] 
diTau+=["keep *_addPileupInfo__HLT"] 
diTau+=["keep *_genJetSel__PAT"] 
diTau+=["keep *_tauGenJetsSelectorAllHadrons__PAT"] 
diTau+=["keep *_genParticlesPruned__PAT"] 
diTau+=["keep *_vertexWeight*__*"] 
diTau+=["keep *_ak5CaloJets_rho_RECO"] 
diTau+=["keep *_ak5PFJets_rho_RECO"] 
diTau+=["keep *_ak5TrackJets_rho_RECO"] 
diTau+=["keep *_ak7BasicJets_rho_RECO"] 
diTau+=["keep *_ak7CaloJets_rho_RECO"] 
diTau+=["keep *_ak7PFJets_rho_RECO"] 
diTau+=["keep *_kt4CaloJets_rho_RECO"] 
diTau+=["keep *_kt4PFJets_rho_RECO"] 
diTau+=["keep *_kt6CaloJets_rho_RECO"] 
diTau+=["keep *_kt6CaloJetsCentral_rho_RECO"] 
diTau+=["keep *_kt6PFJets_rho_RECO"] 
diTau+=["keep *_kt6PFJetsCentralChargedPileUp_rho_RECO"]
diTau+=["keep *_kt6PFJetsCentralNeutral_rho_RECO"] 
diTau+=["keep *_kt6PFJetsCentralNeutralTight_rho_RECO"]
diTau+=["keep *_TriggerResults__RECO"] 
diTau+=["keep *_offlinePrimaryVertices__RECO"] 
diTau+=["keep *_pfMetSignificance__PAT"] 
diTau+=["keep *_ak5PFJetsCHS_rho_PAT"] 
diTau+=["keep *_ak5PFJetsCHSpruned_rho_PAT"] 
diTau+=["keep *_kt6PFJetsCHSForIso_rho_PAT"] 
diTau+=["keep *_kt6PFJetsForIso_rho_PAT"] 
diTau+=["keep *_kt6PFJetsForRhoComputationVoronoi_rho_PAT"] 
diTau+=["keep *_TriggerResults__PAT"] 
diTau+=["keep *_nJetsPtGt1__PAT"] 
diTau+=["keep *_cmgPFBaseJetLead__PAT"] 
diTau+=["keep *_cmgPFBaseJetLeadCHS__PAT"] 
diTau+=["keep *_cmgPFMET__PAT"] 
diTau+=["keep *_cmgPFMETRaw__PAT"] 
diTau+=["keep *_cmgDiElectronSel__PAT"]
diTau+=["keep *_cmgDiMuonSel__PAT"]
diTau+=["keep *_cmgElectronSel__PAT"] 
diTau+=["keep *_cmgMuonSel__PAT"] 
diTau+=["keep *_cmgPFJetLooseJetIdFailed__PAT"]
diTau+=["keep *_cmgPFJetMediumJetIdFailed__PAT"] 
diTau+=["keep *_cmgPFJetSel__PAT"] 
diTau+=["keep *_cmgPFJetSelCHS__PAT"] 
diTau+=["keep *_cmgPFJetTightJetIdFailed__PAT"]
diTau+=["keep *_cmgPFJetVeryLooseJetId95Failed__PAT"]
diTau+=["keep *_cmgPFJetVeryLooseJetId95gammaFailed__PAT"]
diTau+=["keep *_cmgPFJetVeryLooseJetId95h0Failed__PAT"]
diTau+=["keep *_cmgPFJetVeryLooseJetId99Failed__PAT"]
diTau+=["keep *_cmgPhotonSel__PAT"] 
diTau+=["keep *_cmgStructuredPFJetSel__PAT"] 
diTau+=["keep *_cmgTriggerObjectListSel__PAT"] 
diTau+=["keep *_cmgTriggerObjectSel__PAT"] 
diTau+=["keep *_patElectronsWithTrigger__PAT"] 
diTau+=["keep *_patMuonsWithTrigger__PAT"] 
diTau+=["keep *_nopuMet__PAT"] 
diTau+=["keep *_pcMet__PAT"] 
diTau+=["keep *_pfMetForRegression__PAT"] 
diTau+=["keep *_puMet__PAT"] 
diTau+=["keep *_tkMet__PAT"] 
diTau+=["keep *_TriggerResults__H2TAUTAU"] 
diTau+=["keep *_cmgDiTauCorSVFitFullSel__H2TAUTAU"] 
diTau+=["keep *_mvaMETdiTau__H2TAUTAU"] 
diTau+=["keep *_mvaMETdiTau__H2TAUTAU"] 
diTau+=["keep *_goodPVFilter__H2TAUTAU"] 
diTau+=['keep *_genParticles_*_*']
#print diTau
