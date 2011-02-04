import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgPFJetCount_cfi import cmgPFJetCount
from CMGTools.Common.skims.cmgPFMETCount_cfi import cmgPFMETCount

skimSequence = cms.Sequence(
    cmgPFJetCount +
    cmgPFMETCount
    )

