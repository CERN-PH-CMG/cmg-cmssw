import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgHemi_cfi import *
# from CMGTools.Common.skims.cmgHemiSel_cfi import *
# from CMGTools.Common.skims.cmgHemiCount_cfi import *
from CMGTools.Common.histograms.cmgHemiHistograms_cfi import *

hemiSequence = cms.Sequence(
    cmgHemi +
#     cmgHemiSel +
#     cmgHemiCount +
    cmgHemiHistograms
)
