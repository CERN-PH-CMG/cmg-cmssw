import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgTauE_cfi import *
from CMGTools.Common.skims.cmgTauESel_cfi import *

tauESequence = cms.Sequence(
    cmgTauE
    +cmgTauESel
)
