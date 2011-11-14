import FWCore.ParameterSet.Config as cms

tauMu = cms.untracked.vstring(
      'keep *',
      'drop *_cmgTauMu_*_*',
      'drop *_cmgTauMuSel_*_*',
      'drop *_cmgTauMuFullSel_*_*',
      'drop *_cmgTauMuCor_*_*',
      'drop *_cmgTauMuCorPreSel_*_*',
      'drop *_cmgTauMuFullSel_*_*',
      'drop *_*AK7*_*_*', 
      'drop *_*AK5LC*_*_*',  #?
      'keep *_PFMETSignificance*_*_*',
      'drop *_*DiTau*_*_*',
      'drop *_*TauE*_*_*',
      'drop *_*MuE*_*_*',
      'drop pat*_*_*_*',
      'drop *_cmgPFBaseJet*_*_*',
      # dropping intermediate collections for recoil corrections
      'drop *_recoilCorMETTauMu_*_*',
      'drop *_cmgPFJetForRecoil_*_*',
      'drop *_genWorZ_*_*',
    )
