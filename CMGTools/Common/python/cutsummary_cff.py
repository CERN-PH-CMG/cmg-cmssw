import FWCore.ParameterSet.Config as cms

from CMGTools.Common.selections.cutsummarymuon_cfi import *

cutsummary = cms.Sequence(
    cutsummarymuon
    )

