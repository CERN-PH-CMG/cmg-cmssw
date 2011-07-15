

from CMGTools.DiTauAnalysis.skims.cmgDiTauSelClean_cfi import *
from CMGTools.DiTauAnalysis.skims.cmgDiTauSelCleanHistos_cfi import *

cmgDiTauSelCleanSequence = cms.Sequence(
    cmgDiTauSelCharge
    +cmgDiTauSelMass
     
    +cmgDiTauSelClean
    )

cmgDiTauSelCleanHistogramsSequence = cms.Sequence(
    cmgDiTauSelHistograms

    +cmgDiTauSelChargeHistograms
    +cmgDiTauSelMassHistograms
    
    +cmgDiTauSelCleanHistograms
    )

