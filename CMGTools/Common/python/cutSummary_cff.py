import FWCore.ParameterSet.Config as cms

from CMGTools.Common.selections.jetCutSummary_cff import *
from CMGTools.Common.selections.muonCutSummary_cff import *
from CMGTools.Common.selections.electronCutSummary_cff import *
from CMGTools.Common.selections.tauCutSummary_cff import *


cutSummarySequence = cms.Sequence(
    jetCutSummarySequence + 
    muonCutSummarySequence + 
    electronCutSummarySequence + 
    tauCutSummarySequence  
    )

