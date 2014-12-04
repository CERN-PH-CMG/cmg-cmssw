import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgElectron_cfi import *
from CMGTools.Common.skims.cmgElectronSel_cfi import *
from CMGTools.Common.diElectron_cff import *

electronSequence = cms.Sequence(
    cmgElectron + 
    cmgElectronSel +
    diElectronSequence 
    # cmgElectronCount +
    # cmgElectronHistograms
)
