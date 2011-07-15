

from CMGTools.DiTauAnalysis.skims.cmgTauESelClean_cfi import *
from CMGTools.DiTauAnalysis.skims.cmgTauESelCleanHistos_cfi import *

cmgTauESelCleanSequence = cms.Sequence(
    cmgTauESelCharge
    +cmgTauESelMass
     
    +cmgTauESelClean
    )

cmgTauESelCleanHistogramsSequence = cms.Sequence(
    cmgTauESelHistograms

    +cmgTauESelChargeHistograms
    +cmgTauESelMassHistograms
    
    +cmgTauESelCleanHistograms
    )

