import FWCore.ParameterSet.Config as cms

from CMGTools.Susy.common.susy_cff import *
from CMGTools.Susy.MultiJet.multijet_cff import *
from CMGTools.Susy.RA1.RA1_cff import *
from CMGTools.Susy.RA2.RA2_cff import *
from CMGTools.Susy.Razor.razor_cff import *
from CMGTools.Susy.LP.LP_cff import *
from CMGTools.Susy.LeptonicStop.leptonicstop_cff import *
from CMGTools.Common.countingSequences_cff import *

multijetPath = cms.Path(
    susySequence +
    multijetSkimSequence
    )

multijetTriggerPath = cms.Path(
    susySequence +
    multijetTriggerSkimSequence
    )

leptonicStopPath = cms.Path(
    susySequence +
    leptonicStopSkimSequence
    )

leptonicStopTriggerPath = cms.Path(
    susySequence +
    leptonicStopTriggerSkimSequence
    )

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

razorEleMuPath = cms.Path(
    susySequence +
    razorEleMuSequence
    )

LPPath = cms.Path(
    susySequence +
    LPSkimSequence
    )


