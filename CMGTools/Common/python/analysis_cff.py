import FWCore.ParameterSet.Config as cms


from CMGTools.Common.jet_cff import *
from CMGTools.Common.met_cff import *
from CMGTools.Common.muon_cff import *
from CMGTools.Common.electron_cff import *
from CMGTools.Common.tau_cff import *

from CMGTools.Common.skim_cff import *
from CMGTools.Common.cutSummary_cff import *
from CMGTools.Common.histogram_cff import *
from CMGTools.Common.trigger_cff import *

from CMGTools.Common.miscProducers.misc_cff import *

cmgObjectSequence = cms.Sequence(
    jetSequence +
    metSequence +
    muonSequence +
    electronSequence +
    tauSequence + 
    triggerSequence
    )


analysisSequence = cms.Sequence(
    # build the CMG objects and select them
    cmgObjectSequence +
    # build miscellaneous objects not inheriting from cmg::PhysicsObject
    miscSequence + 
    # skimming (counting the selected CMG objects)
    skimSequence +
    # summary of cuts for all objects, after skimming
    cutSummarySequence 
    # histograms for all objects, after skimming
    # histogramSequence 
    )
