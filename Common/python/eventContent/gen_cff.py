import FWCore.ParameterSet.Config as cms

gen = cms.untracked.vstring(
    'keep recoGenParticles_genParticlesStatus3_*_*',
    'keep recoGenParticles_genLeptonsStatus2_*_*',
    'keep recoGenParticles_genLeptonsStatus1_*_*',    
    'keep recoGenJetedmPtrcmgPhysicsObjectWithPtrs_genJetSel_*_*',
    'keep *_addPileupInfo_*_*',
    'keep GenEventInfoProduct_*_*_*',
    'keep LHEEventProduct_*_*_*'
    )

