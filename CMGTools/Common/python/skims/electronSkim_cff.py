import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgElectronCount_cfi import *
from CMGTools.Common.skims.cmgDiElectronCount_cfi import *

electronSkimSequence = cms.Sequence(
    cmgElectronCount +
    cmgDiElectronCount
    )
