import FWCore.ParameterSet.Config as cms

# from CMGTools.Common.skims.cmgPFJetIdLooseSel_cfi import *
# from CMGTools.Common.skims.cmgPFJetIdTightSel_cfi import *

cmgPFJetLooseJetIdFailed = cms.EDFilter(
    "CmgPFJetSelector",
    src = cms.InputTag("cmgPFJetSel"),
    cut = cms.string( '!jetID("POG_PFID_Loose")' )
    )

cmgPFJetMediumJetIdFailed = cms.EDFilter(
    "CmgPFJetSelector",
    src = cms.InputTag("cmgPFJetSel"),
    cut = cms.string( '!jetID("POG_PFID_Medium")' )
    )

cmgPFJetTightJetIdFailed = cms.EDFilter(
    "CmgPFJetSelector",
    src = cms.InputTag("cmgPFJetSel"),
    cut = cms.string( '!jetID("POG_PFID_Tight")' )
    )


jetIdSequence = cms.Sequence(
    cmgPFJetLooseJetIdFailed +
    cmgPFJetMediumJetIdFailed +
    cmgPFJetTightJetIdFailed 
    )
