import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgDiTau_cfi import *
from CMGTools.Common.skims.cmgDiTauSel_cfi import *

diTauSequence = cms.Sequence(
    cmgDiTau + 
    cmgDiTauSel 
)
