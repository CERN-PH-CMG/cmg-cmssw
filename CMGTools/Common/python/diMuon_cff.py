import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgDiMuon_cfi import *
from CMGTools.Common.skims.cmgDiMuonSel_cfi import *

diMuonSequence = cms.Sequence(
    cmgDiMuon + 
    cmgDiMuonSel 
)
