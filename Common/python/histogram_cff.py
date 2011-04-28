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


MHTPFJet30Histograms = baseMETHistograms.clone()
MHTPFJet30Histograms.inputCollection = 'cmgMHTPFJet30'

from CMGTools.Common.histograms.patMETHistograms_cfi import patMETHistograms

pfMETHistogramSequence = cms.Sequence(
    MHTPFJet30Histograms +
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

# taus

from CMGTools.Common.histograms.cmgTauHistograms_cfi import *
from CMGTools.Common.histograms.tauLorentzVector_cfi import *

tauHistogramSequence = cms.Sequence(
    cmgTauHistograms +
    tauLorentzVector
    )

# di-electrons and di-muons

from CMGTools.Common.histograms.cmgDiMuonHistograms_cfi import *
from CMGTools.Common.histograms.cmgDiTauHistograms_cfi import *
from CMGTools.Common.histograms.cmgDiElectronHistograms_cfi import *


histogramSequence = cms.Sequence(
    pfJetHistogramSequence +
    pfMETHistogramSequence +
    electronHistogramSequence +
    muonHistogramSequence +
    tauHistogramSequence +
    cmgDiMuonHistograms +
    cmgDiTauHistograms +
    cmgDiElectronHistograms 
    )

