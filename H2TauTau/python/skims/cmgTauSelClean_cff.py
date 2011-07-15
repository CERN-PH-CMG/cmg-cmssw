

from CMGTools.H2TauTau.skims.cmgTauSelClean_cfi import *
from CMGTools.H2TauTau.skims.cmgTauSelCleanHistos_cfi import *

tauSelCleanSequence = cms.Sequence(
    cmgTauSelPt
    +cmgTauSelEta
    +cmgTauSelDecayModeFinding
    +cmgTauSelAgainstMuonTight
    +cmgTauSelAgainstElectronLoose
    +cmgTauSelVertex
    +cmgTauSelSumPtIsolation    
    +cmgTauSelClean

    +cmgTauSelHistograms
    +cmgTauSelPtHistograms
    +cmgTauSelEtaHistograms
    +cmgTauSelDecayModeFindingHistograms
    +cmgTauSelAgainstMuonTightHistograms
    +cmgTauSelAgainstElectronLooseHistograms
    +cmgTauSelVertexHistograms
    +cmgTauSelSumPtIsolationHistograms    
    +cmgTauSelCleanHistograms
    )

#tauSelCleanHistogramsSequence = cms.Sequence(
#    )

