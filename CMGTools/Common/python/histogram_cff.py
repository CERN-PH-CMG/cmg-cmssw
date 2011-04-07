import FWCore.ParameterSet.Config as cms

from CMGTools.Common.histograms.pfJetLorentzVector_cfi import pfJetLorentzVector
from CMGTools.Common.histograms.pfJetHistograms_cfi import pfJetHistograms

# jet histograms 

pfJetHistogramSequence = cms.Sequence(
    pfJetLorentzVector +
    pfJetHistograms 
    )

# met histograms 

from CMGTools.Common.histograms.baseMETHistograms_cfi import baseMETHistograms


mhtPFJets30Histograms = baseMETHistograms.clone()
mhtPFJets30Histograms.inputCollection = 'cmgMHTPFJets30'

from CMGTools.Common.histograms.patMETHistograms_cfi import patMETHistograms

pfMETHistogramSequence = cms.Sequence(
    mhtPFJets30Histograms +
    patMETHistograms 
    )

# electron histograms

from CMGTools.Common.histograms.cmgElectronHistograms_cfi import *

electronHistogramSequence = cms.Sequence(
    cmgElectronHistograms
    )

# muon histograms

from CMGTools.Common.histograms.cmgMuonHistograms_cfi import *
from CMGTools.Common.histograms.muonLorentzVector_cfi import *

muonHistogramSequence = cms.Sequence(
    cmgMuonHistograms +
    muonLorentzVector
    )

# add other histograms here


histogramSequence = cms.Sequence(
    pfJetHistogramSequence +
    pfMETHistogramSequence +
    electronHistogramSequence +
    muonHistogramSequence
    )
