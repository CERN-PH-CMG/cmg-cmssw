import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgPFJetCount_cfi import cmgPFJetCount
from CMGTools.Common.skims.cmgPFMETCount_cfi import cmgPFMETCount
from CMGTools.Common.skims.cmgMuonCount_cfi import *
from CMGTools.Common.skims.cmgDiMuonCount_cfi import *
from CMGTools.Common.skims.cmgElectronCount_cfi import *
from CMGTools.Common.skims.cmgDiElectronCount_cfi import *

skimSequence = cms.Sequence(
    cmgPFJetCount +
    cmgPFMETCount +
    cmgMuonCount +
    cmgElectronCount +
    cmgDiMuonCount +
    cmgDiElectronCount
    )

