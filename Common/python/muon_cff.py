import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgMuon_cfi import *
from CMGTools.Common.skims.cmgMuonSel_cfi import *
# from CMGTools.Common.skims.cmgMuonCount_cfi import *
# from CMGTools.Common.histograms.cmgMuonHistograms_cfi import *
# from CMGTools.Common.histograms.muonLorentzVector_cfi import *

muonSequence = cms.Sequence(
    cmgMuon + 
    cmgMuonSel 
    # cmgMuonCount +
    # cmgMuonHistograms +
    #COLIN: the name of this module does not suggest it's making histograms. Same remark for all POs. 
    #muonLorentzVector 
)
