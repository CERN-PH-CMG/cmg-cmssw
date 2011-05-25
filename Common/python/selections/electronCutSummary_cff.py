import FWCore.ParameterSet.Config as cms

from CMGTools.Common.selections.cutSummaryElectron_cfi import *

electronCutSummarySequence = cms.Sequence(
    cutSummaryElectron
    )
