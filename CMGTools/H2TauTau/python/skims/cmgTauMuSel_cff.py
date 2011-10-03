

from CMGTools.H2TauTau.skims.cmgTauMuSel_cfi import *
from CMGTools.H2TauTau.skims.cmgTauMuSelHistos_cfi import *

cmgTauMuSelSequence = cms.Sequence(
    cmgTauMuSelMass
    +cmgTauMuSelCharge
    +cmgTauMuSelPt
    +cmgTauMuSelEta
    +cmgTauMuSelAgainstMuon
    +cmgTauMuSelAgainstElectron
#    +cmgTauMuSelIsolation
    
    +cmgTauMuSel
    )

cmgTauMuSelHistogramsSequence = cms.Sequence(
    cmgTauMuSelHistograms
    
    +cmgTauMuSelMassHistograms
    +cmgTauMuSelChargeHistograms
    +cmgTauMuSelPtHistograms
    +cmgTauMuSelEtaHistograms
    +cmgTauMuSelAgainstMuonHistograms
    +cmgTauMuSelAgainstElectronHistograms
#    +cmgTauMuSelIsolationHistograms
       
    +cmgTauMuSelHistograms
    )

