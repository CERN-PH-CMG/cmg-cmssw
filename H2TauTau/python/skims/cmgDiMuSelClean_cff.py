

from CMGTools.H2TauTau.skims.cmgDiMuSelClean_cfi import *
from CMGTools.H2TauTau.skims.cmgDiMuSelCleanHistos_cfi import *

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

