import FWCore.ParameterSet.Config as cms

from CMGTools.Common.analysis_cff import *
from CMGTools.Susy.common.susyLepton_cff import *
from CMGTools.Susy.common.boolToInt_cff import *
from CMGTools.Susy.common.jetId_cff import *
from CMGTools.Susy.common.dumpPDFWeights_cff import *
from CMGTools.Susy.RA1.RA1_cff import *
from CMGTools.Susy.RA2.RA2_cff import *
from CMGTools.Susy.Razor.razor_cff import *
from CMGTools.Susy.LP.LP_cff import *


susySequence  = cms.Sequence(
    boolToIntSequence+
    dumpPdfWeights
    )
