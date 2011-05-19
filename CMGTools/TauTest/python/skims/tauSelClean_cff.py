

from CMGTools.TauTest.skims.cmgTauSelClean_cfi import *
from CMGTools.TauTest.skims.cmgTauSelCleanHistos_cfi import *

tauSelCleanSequence = cms.Sequence(
    cmgTauSelPt
    +cmgTauSelEta

    +cmgTauSelClean
    )

tauSelCleanHistogramsSequence = cms.Sequence(
    cmgTauSelHistograms

    +cmgTauSelPtHistograms
    +cmgTauSelEtaHistograms
    
    +cmgTauSelCleanHistograms
    )

