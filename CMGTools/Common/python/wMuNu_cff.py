import FWCore.ParameterSet.Config as cms

from CMGTools.Common.muon_cff import *
from CMGTools.Common.met_cff import *
from CMGTools.Common.factories.cmgWMuNu_cfi import *
from CMGTools.Common.skims.cmgWMuNuSel_cfi import *
from CMGTools.Common.skims.cmgWMuNuCount_cfi import *
from CMGTools.Common.histograms.wLorentzVector_cfi import *

wMuNuSequence = cms.Sequence(
    muonSequence + 
    metSequence + 
    cmgWMuNu + 
    cmgWMuNuSel +
    cmgWMuNuCount +
    wmunuLorentzVector
)
