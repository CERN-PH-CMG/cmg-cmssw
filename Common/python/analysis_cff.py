import FWCore.ParameterSet.Config as cms


from CMGTools.Common.runInfoAccounting_cfi import *
from CMGTools.Common.muon_cff import *
from CMGTools.Common.electron_cff import *
from CMGTools.Common.jet_cff import *
from CMGTools.Common.met_cff import *
from CMGTools.Common.skim_cff import *
from CMGTools.Common.histogram_cff import *

from CMGTools.Common.cutsummary_cff import *

analysisSequence = cms.Sequence(
    runInfoAccounting +
    muonSequence +
    electronSequence + 
    jetSequence +
    metSequence +
    skimSequence +
    cutsummary + 
    histogramSequence 
    )
