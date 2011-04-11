import FWCore.ParameterSet.Config as cms

# Do not rely on the default cuts implemented here, as they are
# subject to change. You should override these cuts in your analysis.

cmgPhotonSel = cms.EDFilter(
    "CmgPhotonSelector",
    src = cms.InputTag("cmgPhoton"),
    cut = cms.string("pt()>0")
    )


