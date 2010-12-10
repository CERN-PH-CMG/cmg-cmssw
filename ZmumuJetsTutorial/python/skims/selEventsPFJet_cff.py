import FWCore.ParameterSet.Config as cms

jetIDJet = cms.EDFilter(
    "PFJetSelector",
    src = cms.InputTag("cmgPFJet"),
    cut = cms.string( "getSelection(\"cuts_looseJetId\") && getSelection(\"cuts_jetKinematics\")" )
    )


