import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgElectron_cfi import cmgElectron
from CMGTools.Common.histograms.electronLorentzVector_cfi import electronLorentzVector

electronSequence = cms.Sequence(
    cmgElectron + 
    electronLorentzVector
)
