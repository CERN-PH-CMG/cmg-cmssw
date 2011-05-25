import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgDiElectron_cfi import *
from CMGTools.Common.skims.cmgDiElectronSel_cfi import *

diElectronSequence = cms.Sequence(
    cmgDiElectron + 
    cmgDiElectronSel 
)
