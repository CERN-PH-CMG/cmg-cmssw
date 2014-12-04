import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgPFMETCount_cfi import *

metSkimSequence = cms.Sequence(
    cmgPFMETCount
    )
