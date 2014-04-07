import FWCore.ParameterSet.Config as cms
import copy



common = [
    'keep *',
    'drop *_*AK7*_*_*',
    'drop *_*AK5LC*_*_*',
    'drop *_*DiTau*_*_*',
    'drop *_*TauMu*_*_*',
    'drop *_*TauE*_*_*',
    'drop *_*MuE*_*_*',
    'drop pat*_*_*_*',
    'keep patElectrons_*_*_*',
    'keep patMuons_*_*_*',
    'drop cmgPFBaseJet*_*_*_*',
    'drop cmgTaus_*_*_*',
    'drop cmgPFJets_cmgPFJetSelCHS_*_*', # no need for CHS
    'drop cmgMuoncmgMuoncmgDiObjects_cmgDiMuonSel_*_*', # no need for dimuons
    'drop cmgElectroncmgElectroncmgDiObjects_cmgDiElectronSel_*_*', # no need for dielectrons
    'drop cmgPFJets_*JetId*_*_*',
    'drop recoCaloMETs_*_*_*', # Can keep for MVA MET debugging
    'drop *_cmgPFJetForRecoil_*_*',
    'drop *_cmgPFJetForRecoilPresel_*_*',
    'drop cmgTriggerObjects_cmgL1TriggerObjectSel__PAT',
    'drop cmgBaseJets_cmgPFBaseJetLeadCHS__PAT',
    'drop recoTrackJets_ak5SoftTrackJetsForVbfHbb__PAT',
    'drop l1extra*_*_*_*',
    'drop *_pcMet_*_*',
    'drop *_tkMet_*_*',
    'drop *_genWorZ_*_*',
    'keep recoPFMETs_*_*_*',
    'keep cmgMETSignificance*_*_*_*',
    'keep double_*_*_*',
    'keep int_*_*_*',
    ]


commonDebug = copy.deepcopy( common ) 
commonDebug.extend( [
    'keep *_cmgPFJetForRecoil_*_*',
    'keep *_genWorZ_*_*'    
    ])

