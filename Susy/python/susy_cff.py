import FWCore.ParameterSet.Config as cms

from CMGTools.Susy.common.susy_cff import *
from CMGTools.Susy.RA1.RA1_cff import *
from CMGTools.Susy.RA2.RA2_cff import *
from CMGTools.Susy.Razor.razor_cff import *
from CMGTools.Susy.LP.LP_cff import *
from CMGTools.Common.countingSequences_cff import *

RA1Path = cms.Path(
    susySequence *
    RA1SkimSequence
    )

RA2Path = cms.Path(
    susySequence +
    RA2SkimSequence
    )

razorPath = cms.Path(
    susySequence +
    razorSkimSequence
    )

razorTriggerPath = cms.Path(
    susySequence +
    razorTriggerSkimSequence
    )

LPPath = cms.Path(
    susySequence +
    LPSkimSequence
    )


