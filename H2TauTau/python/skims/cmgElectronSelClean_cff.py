

from CMGTools.DiTauAnalysis.skims.cmgElectronSelClean_cfi import *
from CMGTools.DiTauAnalysis.skims.cmgElectronSelCleanHistos_cfi import *

electronSelCleanSequence = cms.Sequence(
    cmgElectronSelPt
    +cmgElectronSelEta

    +cmgElectronSelClean
    )

electronSelCleanHistogramsSequence = cms.Sequence(
    cmgElectronSelHistograms

    +cmgElectronSelPtHistograms
    +cmgElectronSelEtaHistograms
    
    +cmgElectronSelCleanHistograms
    )

