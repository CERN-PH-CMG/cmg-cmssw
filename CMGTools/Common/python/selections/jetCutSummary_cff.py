import FWCore.ParameterSet.Config as cms

from CMGTools.Common.selections.cutSummaryPFJet_cfi import *

jetCutSummarySequence = cms.Sequence(
    cutSummaryPFJet
    )
