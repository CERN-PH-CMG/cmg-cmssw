import FWCore.ParameterSet.Config as cms

cmgPFJetCorrector = cms.EDProducer(
    "PFJetCorrector",
    # make sure your jet and rho collections are compatible
    src = cms.InputTag( 'cmgPFJetSel' ),
    rho = cms.InputTag( 'kt6PFJetsAK5:rho' ),
    payload = cms.string('AK5PFchs'),
    levels = cms.vstring('L1FastJet','L2Relative','L3Absolute','L2L3Residual'),
    sort = cms.bool(True),
    verbose = cms.untracked.bool( False )
)
