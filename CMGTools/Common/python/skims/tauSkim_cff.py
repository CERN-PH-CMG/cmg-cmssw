import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgTauCount_cfi import *
from CMGTools.Common.skims.cmgDiTauCount_cfi import *

tauSkimSequence = cms.Sequence(
    cmgTauCount # +
    # cmgDiTauCount
    )
