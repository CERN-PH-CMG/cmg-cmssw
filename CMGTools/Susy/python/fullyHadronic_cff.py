import FWCore.ParameterSet.Config as cms

from CMGTools.Susy.fullyHadronicCommon_cff import *
from CMGTools.Susy.RA1.RA1_cff import *
from CMGTools.Susy.RA2.RA2_cff import *
from CMGTools.Susy.Razor.razor_cff import *

RA1Path = cms.Path(
    fullyHadronicCommonSequence *
    RA1SkimSequence
    )

RA2Path = cms.Path(
    fullyHadronicCommonSequence +
    RA2SkimSequence
    )

razorPath = cms.Path(
    fullyHadronicCommonSequence +
    razorSkimSequence
    )

fullyHadronicSchedule = cms.Schedule(
    RA1Path,
    RA2Path,
    razorPath
    )
