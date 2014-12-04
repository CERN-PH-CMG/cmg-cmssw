import FWCore.ParameterSet.Config as cms

totalKinematicsFilterCMG = cms.EDFilter('TotalKinematicsFilterCMG',
  src          = cms.InputTag("genParticles"),
  tolerance    = cms.double(0.5),
  debugOn      = cms.untracked.bool(False),				    
  TaggingMode  = cms.bool(True)
)
