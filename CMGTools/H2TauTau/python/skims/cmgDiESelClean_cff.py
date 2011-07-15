

from CMGTools.DiTauAnalysis.skims.cmgDiESelClean_cfi import *
from CMGTools.DiTauAnalysis.skims.cmgDiESelCleanHistos_cfi import *

cmgDiESelCleanSequence = cms.Sequence(
    cmgDiESelCharge
    +cmgDiESelMass
     
    +cmgDiESelClean
    )

cmgDiESelCleanHistogramsSequence = cms.Sequence(
    cmgDiESelHistograms

    +cmgDiESelChargeHistograms
    +cmgDiESelMassHistograms
    
    +cmgDiESelCleanHistograms
    )

