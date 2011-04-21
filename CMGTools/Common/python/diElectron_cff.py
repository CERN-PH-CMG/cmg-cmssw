import FWCore.ParameterSet.Config as cms

from CMGTools.Common.electron_cff import *
from CMGTools.Common.met_cff import *
from CMGTools.Common.factories.cmgDiElectron_cfi import *
from CMGTools.Common.skims.cmgDiElectronSel_cfi import *
# from CMGTools.Common.skims.cmgDiElectronCount_cfi import *
# from CMGTools.Common.histograms.cmgDiElectronHistograms_cfi import *

diElectronSequence = cms.Sequence(
    electronSequence + 
#    pfSimpleMetSequence + 
    cmgDiElectron + 
    cmgDiElectronSel 
#    cmgDiElectronCount +
#    cmgDiElectronHistograms
)
