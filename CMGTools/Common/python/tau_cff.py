import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgTau_cfi import *
from CMGTools.Common.skims.cmgTauSel_cfi import *
# from CMGTools.Common.skims.cmgTauCount_cfi import *
# from CMGTools.Common.histograms.cmgTauHistograms_cfi import *

tauSequence = cms.Sequence(
    cmgTau 
    + cmgTauSel 
#    + cmgTauCount 
#    + cmgTauHistograms
)
