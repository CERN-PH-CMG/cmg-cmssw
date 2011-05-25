import FWCore.ParameterSet.Config as cms

# electron histograms

from CMGTools.Common.histograms.cmgElectronHistograms_cfi import *
from CMGTools.Common.histograms.cmgDiElectronHistograms_cfi import *

electronHistogramSequence = cms.Sequence(
    cmgElectronHistograms +
    cmgDiElectronHistograms
    )
