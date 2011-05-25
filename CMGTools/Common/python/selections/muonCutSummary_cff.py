import FWCore.ParameterSet.Config as cms

from CMGTools.Common.selections.cutSummaryMuon_cfi import *

muonCutSummarySequence = cms.Sequence(
    cutSummaryMuon
    )
