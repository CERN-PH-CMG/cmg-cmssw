

from CMGTools.DiTauAnalysis.skims.cmgMuonSelClean_cfi import *
from CMGTools.DiTauAnalysis.skims.cmgMuonSelCleanHistos_cfi import *

muonSelCleanSequence = cms.Sequence(
    cmgMuonSelPt
    +cmgMuonSelEta
    +cmgMuonSelIsGlobalTracker
    +cmgMuonSelPixelHits
    +cmgMuonSelTrackerHits
    +cmgMuonSelChi2
    +cmgMuonSelVertex
    
    +cmgMuonSelClean
    )

muonSelCleanHistogramsSequence = cms.Sequence(
    cmgMuonSelHistograms

    +cmgMuonSelPtHistograms
    +cmgMuonSelEtaHistograms
    +cmgMuonSelIsGlobalTrackerHistograms
    +cmgMuonSelPixelHitsHistograms
    +cmgMuonSelTrackerHitsHistograms
    +cmgMuonSelChi2Histograms
    +cmgMuonSelVertexHistograms
    
    +cmgMuonSelCleanHistograms
    )

