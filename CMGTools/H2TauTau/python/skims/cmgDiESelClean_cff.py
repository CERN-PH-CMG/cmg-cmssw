

from CMGTools.H2TauTau.skims.cmgDiESelClean_cfi import *
from CMGTools.H2TauTau.skims.cmgDiESelCleanHistos_cfi import *

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

