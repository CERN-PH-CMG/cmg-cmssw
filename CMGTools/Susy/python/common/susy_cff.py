import FWCore.ParameterSet.Config as cms

from CMGTools.Susy.common.susyLepton_cff import *
from CMGTools.Susy.common.boolToInt_cff import *
from CMGTools.Susy.common.jetId_cff import *
from CMGTools.Susy.common.dumpPDFWeights_cff import *
from CMGTools.Susy.common.simpleGenInfo_cff import *
from CMGTools.Susy.common.emulate2011Trigger_cff import *

susyGenSequence  = cms.Sequence(
    dumpPdfWeights+
    simpleGenInfo+
    emulate2011Trigger
    )

susyDataSequence  = cms.Sequence(
    boolToIntSequence
    )
