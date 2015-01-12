import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgTauEle_cfi import *
from CMGTools.Common.skims.cmgTauEleSel_cfi import *

tauESequence = cms.Sequence(
    cmgTauEle
    +cmgTauEleSel
)
