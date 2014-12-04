import FWCore.ParameterSet.Config as cms

# do not really on the default cuts implemented here,
# as they are subject to change. 
# you should override these cuts in your analysis.

cmgPFJetCount = cms.EDFilter(
    "CandViewCountFilter",
    src = cms.InputTag("cmgPFJetSel"),
    minNumber = cms.uint32(0),
    )


