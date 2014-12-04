import FWCore.ParameterSet.Config as cms

# do not really on the default cuts implemented here,
# as they are subject to change. 
# you should override these cuts in your analysis.

cmgMuonCount = cms.EDFilter(
    "CandViewCountFilter",
    src = cms.InputTag("cmgMuonSel"),
    minNumber = cms.uint32(0),
    )


