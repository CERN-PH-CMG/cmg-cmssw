import FWCore.ParameterSet.Config as cms

from CMGTools.Common.histograms.pfJetLorentzVector_cfi import pfJetLorentzVector
from CMGTools.Common.histograms.pfJetHistograms_cfi import pfJetHistograms

pfJetHistogramSequence = cms.Sequence(
    pfJetLorentzVector +
    pfJetHistograms 
    )

from CMGTools.Common.histograms.baseMETHistograms_cfi import baseMETHistograms

mhtPFJetsHistograms = baseMETHistograms.clone()
mhtPFJetsHistograms.inputCollection = 'cmgMHTPFJets'

mhtPFJets30Histograms = baseMETHistograms.clone()
mhtPFJets30Histograms.inputCollection = 'cmgMHTPFJets30'

from CMGTools.Common.histograms.patMETHistograms_cfi import patMETHistograms

# mhtPFJets50Histograms = baseMETHistograms.clone()
# mhtPFJets50Histograms.inputCollection = 'cmgMHTPFJets50'

# add other histograms here

# pfMETHistograms = baseMETHistograms.clone()
# pfMETHistograms.inputCollection = 'patMETsPFlow'

pfMETHistogramSequence = cms.Sequence(
    mhtPFJets30Histograms +
    # mhtPFJets50Histograms 
    patMETHistograms 
    )

histogramSequence = cms.Sequence(
    pfJetHistogramSequence +
    pfMETHistogramSequence
    )
