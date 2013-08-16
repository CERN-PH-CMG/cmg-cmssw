import FWCore.ParameterSet.Config as cms

from CMGTools.Common.histograms.jetHistogram_cff import *
from CMGTools.Common.histograms.metHistogram_cff import *
from CMGTools.Common.histograms.electronHistogram_cff import *
from CMGTools.Common.histograms.muonHistogram_cff import *
from CMGTools.Common.histograms.tauHistogram_cff import *

histogramSequence = cms.Sequence(
    jetHistogramSequence +
    metHistogramSequence +
    electronHistogramSequence +
    muonHistogramSequence +
    tauHistogramSequence
    )

