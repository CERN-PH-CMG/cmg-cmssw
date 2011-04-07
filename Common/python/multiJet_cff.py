import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgMultiJet_cfi import *
from CMGTools.Common.histograms.cmgMultiJetHistograms_cfi import *

multiJetSequence = cms.Sequence(
    cmgMultiJet +
    cmgMultiJetHistograms
)
