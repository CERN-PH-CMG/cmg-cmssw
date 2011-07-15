

from CMGTools.DiTauAnalysis.skims.cmgDiMuSelClean_cfi import *
from CMGTools.DiTauAnalysis.skims.cmgDiMuSelCleanHistos_cfi import *

cmgDiMuSelCleanSequence = cms.Sequence(
    cmgDiMuSelCharge
    +cmgDiMuSelMass
     
    +cmgDiMuSelClean
    )

cmgDiMuSelCleanHistogramsSequence = cms.Sequence(
    cmgDiMuSelHistograms

    +cmgDiMuSelChargeHistograms
    +cmgDiMuSelMassHistograms
    
    +cmgDiMuSelCleanHistograms
    )

