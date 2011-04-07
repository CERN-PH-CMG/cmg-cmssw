import FWCore.ParameterSet.Config as cms

from CMGTools.Common.selections.cutSummaryMuon_cfi import *
from CMGTools.Common.selections.cutSummaryElectron_cfi import *
from CMGTools.Common.selections.cutSummaryPFJet_cfi import *

cutSummarySequence = cms.Sequence(
    cutSummaryMuon +
    cutSummaryElectron +
    cutSummaryPFJet
    )

