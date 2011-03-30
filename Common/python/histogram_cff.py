import FWCore.ParameterSet.Config as cms

from CMGTools.Common.histograms.pfJetLorentzVector_cfi import pfJetLorentzVector
from CMGTools.Common.histograms.pfJetHistograms_cfi import pfJetHistograms

pfJetHistogramSequence = cms.Sequence(
    pfJetLorentzVector +
    pfJetHistograms 
    )

from CMGTools.Common.histograms.baseMETHistograms_cfi import baseMETHistograms


mhtPFJets30Histograms = baseMETHistograms.clone()
mhtPFJets30Histograms.inputCollection = 'cmgMHTPFJets30'

from CMGTools.Common.histograms.patMETHistograms_cfi import patMETHistograms


# add other histograms here

pfMETHistogramSequence = cms.Sequence(
    mhtPFJets30Histograms +
    patMETHistograms 
    )

histogramSequence = cms.Sequence(
    pfJetHistogramSequence +
    pfMETHistogramSequence
    )
