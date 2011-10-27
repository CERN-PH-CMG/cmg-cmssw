import FWCore.ParameterSet.Config as cms

cmgPFJetCorrector = cms.EDProducer(
    "PFJetCorrector",
    # make sure your jet and rho collections are compatible
    src = cms.InputTag( 'cmgPFJetSel' ),
    rho = cms.InputTag( 'kt6PFJetsAK5:rho' ),
    verbose = cms.untracked.bool( False )
)
