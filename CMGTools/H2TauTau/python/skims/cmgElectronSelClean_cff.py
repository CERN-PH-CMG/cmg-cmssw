

from CMGTools.H2TauTau.skims.cmgElectronSelClean_cfi import *
from CMGTools.H2TauTau.skims.cmgElectronSelCleanHistos_cfi import *

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

