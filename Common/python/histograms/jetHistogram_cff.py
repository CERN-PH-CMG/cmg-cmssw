import FWCore.ParameterSet.Config as cms

from CMGTools.Common.histograms.pfJetLorentzVector_cfi import pfJetLorentzVector
from CMGTools.Common.histograms.pfJetHistograms_cfi import pfJetHistograms

# jet histograms 

pfJetHistogramSequence = cms.Sequence(
    pfJetLorentzVector +
    pfJetHistograms 
    )

jetHistogramSequence = cms.Sequence(
    pfJetHistogramSequence
    )
