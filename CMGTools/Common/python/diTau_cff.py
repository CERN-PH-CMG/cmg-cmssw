import FWCore.ParameterSet.Config as cms

from CMGTools.Common.tau_cff import *
from CMGTools.Common.factories.cmgDiTau_cfi import *
from CMGTools.Common.skims.cmgDiTauSel_cfi import *
from CMGTools.Common.skims.cmgDiTauCount_cfi import *
from CMGTools.Common.histograms.cmgDiTauHistograms_cfi import *

diTauSequence = cms.Sequence(
    tauSequence + 
    cmgDiTau + 
    cmgDiTauSel +
    cmgDiTauCount +
    cmgDiTauHistograms
)
