import FWCore.ParameterSet.Config as cms


from CMGTools.Common.runInfoAccounting_cfi import *
from CMGTools.Common.jet_cff import *
from CMGTools.Common.met_cff import *
from CMGTools.Common.skim_cff import *
from CMGTools.Common.histogram_cff import *

analysis = cms.Sequence(
    runInfoAccounting + 
    jetSequence +
    metSequence +
    skimSequence +
    histogramSequence
    )
