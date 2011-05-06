import FWCore.ParameterSet.Config as cms


from CMGTools.Common.runInfoAccounting_cfi import *
from CMGTools.Common.muon_cff import *
from CMGTools.Common.electron_cff import *
from CMGTools.Common.tau_cff import *
from CMGTools.Common.jet_cff import *
from CMGTools.Common.met_cff import *
from CMGTools.Common.diMuon_cff import *
from CMGTools.Common.diTau_cff import *
from CMGTools.Common.diElectron_cff import *
from CMGTools.Common.skim_cff import *
from CMGTools.Common.cutSummary_cff import *
from CMGTools.Common.histogram_cff import *
from CMGTools.Common.triggerInfo_cff import *


cmgObjectSequence = cms.Sequence(
    muonSequence +
    electronSequence +
    tauSequence + 
    jetSequence +
    metSequence +
    diMuonSequence +
    diTauSequence +
    diElectronSequence +
    triggerSequence
    )

analysisSequence = cms.Sequence(
    # build the CMG objects and select them
    cmgObjectSequence +
    # skimming (counting the selected CMG objects)
    skimSequence +
    # summary of cuts for all objects, after skimming
    cutSummarySequence +
    # histograms for all objects, after skimming
    histogramSequence 
    )
