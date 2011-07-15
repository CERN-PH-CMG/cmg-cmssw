

from CMGTools.H2TauTau.skims.cmgMuESelClean_cfi import *
from CMGTools.H2TauTau.skims.cmgMuESelCleanHistos_cfi import *

cmgMuESelCleanSequence = cms.Sequence(
    cmgMuESelCharge
    +cmgMuESelMass
     
    +cmgMuESelClean
    )

cmgMuESelCleanHistogramsSequence = cms.Sequence(
    cmgMuESelHistograms

    +cmgMuESelChargeHistograms
    +cmgMuESelMassHistograms
    
    +cmgMuESelCleanHistograms
    )

