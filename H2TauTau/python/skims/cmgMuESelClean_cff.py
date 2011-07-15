

from CMGTools.DiTauAnalysis.skims.cmgMuESelClean_cfi import *
from CMGTools.DiTauAnalysis.skims.cmgMuESelCleanHistos_cfi import *

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

