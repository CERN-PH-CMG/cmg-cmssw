

from CMGTools.DiTauAnalysis.skims.cmgTauMuSelClean_cfi import *
from CMGTools.DiTauAnalysis.skims.cmgTauMuSelCleanHistos_cfi import *

cmgTauMuSelCleanSequence = cms.Sequence(
    cmgTauMuSelCharge
    +cmgTauMuSelMass
     
    +cmgTauMuSelClean
    )

cmgTauMuSelCleanHistogramsSequence = cms.Sequence(
    cmgTauMuSelHistograms

    +cmgTauMuSelChargeHistograms
    +cmgTauMuSelMassHistograms
    
    +cmgTauMuSelCleanHistograms
    )

