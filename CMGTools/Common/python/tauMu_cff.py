import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgTauMu_cfi import *
from CMGTools.Common.skims.cmgTauMuSel_cfi import *

tauMuSequence = cms.Sequence(
    cmgTauMu 
    +cmgTauMuSel 
)
