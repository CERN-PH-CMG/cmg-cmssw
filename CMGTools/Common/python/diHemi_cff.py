import FWCore.ParameterSet.Config as cms

from CMGTools.Common.hemi_cff import *
from CMGTools.Common.factories.cmgDiHemi_cfi import *
# from CMGTools.Common.skims.cmgDiHemiSel_cfi import *
# from CMGTools.Common.skims.cmgDiHemiCount_cfi import *
from CMGTools.Common.histograms.cmgDiHemiHistograms_cfi import *

diHemiSequence = cms.Sequence(
    hemiSequence +
    cmgDiHemi +
#     cmgDiHemiSel +
#     cmgDiHemiCount +
    cmgDiHemiHistograms
)
