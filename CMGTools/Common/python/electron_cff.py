import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgElectron_cfi import *
from CMGTools.Common.skims.cmgElectronSel_cfi import *
from CMGTools.Common.skims.cmgElectronCount_cfi import *
from CMGTools.Common.histograms.cmgElectronHistograms_cfi import *

electronSequence = cms.Sequence(
    cmgElectron + 
    cmgElectronSel 
    # cmgElectronCount +
    # cmgElectronHistograms
)
