

from CMGTools.H2TauTau.skims.cmgDiTauSelClean_cfi import *
from CMGTools.H2TauTau.skims.cmgDiTauSelCleanHistos_cfi import *

cmgDiTauSelCleanSequence = cms.Sequence(
    cmgDiTauSelMass
    +cmgDiTauSelCharge
    +cmgDiTauSelPt
    +cmgDiTauSelEta
    +cmgDiTauSelAgainstMuon
    +cmgDiTauSelAgainstElectron
    +cmgDiTauSelSumPtIsolation
    
    +cmgDiTauSelClean
    )

cmgDiTauSelCleanHistogramsSequence = cms.Sequence(
    cmgDiTauSelHistograms
    
    +cmgDiTauSelMassHistograms
    +cmgDiTauSelChargeHistograms
    +cmgDiTauSelPtHistograms
    +cmgDiTauSelEtaHistograms
    +cmgDiTauSelAgainstMuonHistograms
    +cmgDiTauSelAgainstElectronHistograms
    +cmgDiTauSelSumPtIsolationHistograms
    
    
    +cmgDiTauSelCleanHistograms
    )

