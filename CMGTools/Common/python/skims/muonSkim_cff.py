import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgMuonCount_cfi import *
from CMGTools.Common.skims.cmgDiMuonCount_cfi import *

muonSkimSequence = cms.Sequence(
    cmgMuonCount +
    cmgDiMuonCount
    )
