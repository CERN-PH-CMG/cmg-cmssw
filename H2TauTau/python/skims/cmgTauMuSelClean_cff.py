
from CMGTools.H2TauTau.skims.cmgTauMuSelClean_cfi import *
from CMGTools.H2TauTau.skims.cmgTauMuSelCleanHistos_cfi import *

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

