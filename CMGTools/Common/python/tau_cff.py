import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgTau_cfi import *
from CMGTools.Common.skims.cmgTauSel_cfi import *

tauSequence = cms.Sequence(
    cmgTau 
    +cmgTauSel
)
