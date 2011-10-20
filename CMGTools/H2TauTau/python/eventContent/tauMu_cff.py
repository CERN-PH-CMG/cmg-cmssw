import FWCore.ParameterSet.Config as cms

tauMu = cms.untracked.vstring(
      'keep *',
      'drop *_cmgTauMu_*_*',
      'drop *_*AK7*_*_*', 
      'drop *_*AK5LC*_*_*',  #?
      'keep *_PFMETSignificance*_*_*',
      'drop *_*DiTau*_*_*',
      'drop *_*TauE*_*_*',
      'drop *_*MuE*_*_*',
      'drop pat*_*_*_*',
      'drop *_cmgPFBaseJet*_*_*'
    )
