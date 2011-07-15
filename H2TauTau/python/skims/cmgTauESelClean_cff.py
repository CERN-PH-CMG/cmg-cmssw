

from CMGTools.H2TauTau.skims.cmgTauESelClean_cfi import *
from CMGTools.H2TauTau.skims.cmgTauESelCleanHistos_cfi import *

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

