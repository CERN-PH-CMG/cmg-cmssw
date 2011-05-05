import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgPFJetSel_cfi import cmgPFJetSel

jetIDJet = cmgPFJetSel.clone(
    cut = cms.string( "getSelection(\"cuts_looseJetId\") && pt() > 30" )
    )


