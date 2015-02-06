import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgPFJetCount_cfi import *

jetSkimSequence = cms.Sequence(
    cmgPFJetCount
    )
