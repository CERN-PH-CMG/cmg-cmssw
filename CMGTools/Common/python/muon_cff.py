import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgMuon_cfi import *
from CMGTools.Common.skims.cmgMuonSel_cfi import *
from CMGTools.Common.diMuon_cff import *

muonSequence = cms.Sequence(
    cmgMuon + 
    cmgMuonSel +
    diMuonSequence
)
