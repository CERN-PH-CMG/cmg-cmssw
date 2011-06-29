import FWCore.ParameterSet.Config as cms

# from CMGTools.Common.skims.cmgPFJetIdLooseSel_cfi import *
# from CMGTools.Common.skims.cmgPFJetIdTightSel_cfi import *

cmgPFJetLooseJetIdFailed = cms.EDFilter(
    "CmgPFJetSelector",
    src = cms.InputTag("cmgPFJetSel"),
    cut = cms.string( '!getSelection("cuts_looseJetId")' )
    )

cmgPFJetMediumJetIdFailed = cms.EDFilter(
    "CmgPFJetSelector",
    src = cms.InputTag("cmgPFJetSel"),
    cut = cms.string( '!getSelection("cuts_mediumJetId")' )
    )

cmgPFJetTightJetIdFailed = cms.EDFilter(
    "CmgPFJetSelector",
    src = cms.InputTag("cmgPFJetSel"),
    cut = cms.string( '!getSelection("cuts_tightJetId")' )
    )

cmgPFJetVeryLooseJetId99Failed = cms.EDFilter(
    "CmgPFJetSelector",
    src = cms.InputTag("cmgPFJetSel"),
    cut = cms.string( '!getSelection("cuts_veryLooseJetId99")' )
    )

cmgPFJetVeryLooseJetId95Failed = cms.EDFilter(
    "CmgPFJetSelector",
    src = cms.InputTag("cmgPFJetSel"),
    cut = cms.string( '!getSelection("cuts_veryLooseJetId95")' )
    )


jetIdSequence = cms.Sequence(
    cmgPFJetVeryLooseJetId99Failed +
    cmgPFJetVeryLooseJetId95Failed + 
    cmgPFJetLooseJetIdFailed +
    cmgPFJetMediumJetIdFailed +
    cmgPFJetTightJetIdFailed 
    )
