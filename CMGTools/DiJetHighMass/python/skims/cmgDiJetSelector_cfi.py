import FWCore.ParameterSet.Config as cms

# Do not rely on the default cuts implemented here, as they are
# subject to change. You should override these cuts in your analysis.

cmgDiFatJetSel = cms.EDFilter(
    "CmgDiFatJetSelector",
    src = cms.InputTag("cmgDiFatJet"),
    #cut = cms.string("")
    cut = cms.string("getSelection(\"cuts_fatDijetKinematics_tightMass\") && getSelection(\"cuts_fatDijetKinematics_tightDeltaEta\") && getSelection(\"cuts_fatDijetKinematics_jetsPhaseSpace\")")
    )

cmgDiPFJetSel = cms.EDFilter(
    "CmgDiPFJetSelector",
    src = cms.InputTag("cmgDiPFJet"),
    #cut = cms.string("")
    cut = cms.string("getSelection(\"cuts_dijetKinematics_Jet250Uthreshold\") && getSelection(\"cuts_dijetKinematics_tightDeltaEta\") && getSelection(\"cuts_dijetKinematics_jetsPhaseSpace\")")
    )


cmgDiBaseJetSel = cms.EDFilter(
    "CmgDiBaseJetSelector",
    src = cms.InputTag("cmgDiBaseJet"),
    cut = cms.string("getSelection(\"cuts_dijetKinematics_Jet250Uthreshold\") && getSelection(\"cuts_dijetKinematics_tightDeltaEta\") && getSelection(\"cuts_dijetKinematics_jetsPhaseSpace\")")
    )
