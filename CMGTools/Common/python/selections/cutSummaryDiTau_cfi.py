import FWCore.ParameterSet.Config as cms

from CMGTools.Common.selections.cutSummaryTau_cfi import *


cutSummaryDiTau = cms.EDAnalyzer(
    "CutSummaryAnalyzer",
    inputCollection = cms.InputTag("cmgDiTauSel")
)


cutSummaryDiTauSeq = cms.Sequence(
    cutSummaryTau
    + cutSummaryDiTau
    )
