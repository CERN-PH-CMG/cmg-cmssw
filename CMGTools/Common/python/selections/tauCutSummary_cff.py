import FWCore.ParameterSet.Config as cms

from CMGTools.Common.selections.cutSummaryTau_cfi import *

tauCutSummarySequence = cms.Sequence(
    cutSummaryTau
    )
