import FWCore.ParameterSet.Config as cms

from CMGTools.Common.muon_cff import *
from CMGTools.Common.factories.cmgDiMuon_cfi import *
from CMGTools.Common.skims.cmgDiMuonSel_cfi import *
from CMGTools.Common.skims.cmgDiMuonCount_cfi import *
from CMGTools.Common.histograms.cmgDiMuonHistograms_cfi import *

diMuonSequence = cms.Sequence(
    muonSequence + 
    cmgDiMuon + 
    cmgDiMuonSel +
    cmgDiMuonCount +
    cmgDiMuonHistograms
)
