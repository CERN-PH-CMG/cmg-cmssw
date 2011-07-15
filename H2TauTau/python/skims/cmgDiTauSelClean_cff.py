

from CMGTools.H2TauTau.skims.cmgDiTauSelClean_cfi import *
from CMGTools.H2TauTau.skims.cmgDiTauSelCleanHistos_cfi import *

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

