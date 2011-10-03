

from CMGTools.H2TauTau.skims.cmgTauMuSelClean_cfi import *
from CMGTools.H2TauTau.skims.cmgTauMuSelCleanHistos_cfi import *

cmgTauMuSelCleanSequence = cms.Sequence(
    cmgTauMuSelMass
    +cmgTauMuSelCharge
    +cmgTauMuSelPt
    +cmgTauMuSelEta
    +cmgTauMuSelAgainstMuon
    +cmgTauMuSelAgainstElectron
    +cmgTauMuSelIsolation
    
    +cmgTauMuSelClean
    )

cmgTauMuSelCleanHistogramsSequence = cms.Sequence(
    cmgTauMuSelHistograms
    
    +cmgTauMuSelMassHistograms
    +cmgTauMuSelChargeHistograms
    +cmgTauMuSelPtHistograms
    +cmgTauMuSelEtaHistograms
    +cmgTauMuSelAgainstMuonHistograms
    +cmgTauMuSelAgainstElectronHistograms
    +cmgTauMuSelIsolationHistograms
       
    +cmgTauMuSelCleanHistograms
    )

