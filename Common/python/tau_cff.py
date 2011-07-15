import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgTau_cfi import *
from CMGTools.Common.skims.cmgTauSel_cfi import *
from CMGTools.Common.diTau_cff import *

tauSequence = cms.Sequence(
    cmgTau 
    + cmgTauSel
#    + diTauSequence
)
