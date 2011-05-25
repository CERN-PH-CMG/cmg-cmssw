import FWCore.ParameterSet.Config as cms

# met histograms 

from CMGTools.Common.histograms.baseMETHistograms_cfi import baseMETHistograms

MHTPFJet30Histograms = baseMETHistograms.clone()
MHTPFJet30Histograms.inputCollection = cms.InputTag('cmgMHTPFJet30')

from CMGTools.Common.histograms.patMETHistograms_cfi import patMETHistograms
patMETHistograms.inputCollection = cms.InputTag('patMETsAK5')

pfMETHistogramSequence = cms.Sequence(
    MHTPFJet30Histograms +
    patMETHistograms 
    )

metHistogramSequence = cms.Sequence(
    pfMETHistogramSequence
    )
