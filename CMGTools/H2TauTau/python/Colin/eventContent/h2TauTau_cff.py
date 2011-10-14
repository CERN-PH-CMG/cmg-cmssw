import FWCore.ParameterSet.Config as cms

h2TauTau = cms.untracked.vstring(
    # keeping only the selected version of these di-objects
    'drop *_cmgDiTau_*_*',
    'drop *_cmgTauMu_*_*',
    'drop *_cmgTauE_*_*',
    'drop *_cmgMuE_*_*'   
    )
