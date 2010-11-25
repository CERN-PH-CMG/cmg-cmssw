import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgWENu_cfi import cmgWENu
from CMGTools.Common.factories.cmgWMuNu_cfi import cmgWMuNu
from CMGTools.Common.histograms.wLorentzVector_cfi import wenuLorentzVector, wmunuLorentzVector

wSequence = cms.Sequence(
    cmgWENu +
    cmgWMuNu + 
    wenuLorentzVector + 
    wmunuLorentzVector
    )