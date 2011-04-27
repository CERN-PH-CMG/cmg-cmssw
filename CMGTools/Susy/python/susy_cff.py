import FWCore.ParameterSet.Config as cms

from CMGTools.Susy.susyCommon_cff import *
from CMGTools.Susy.RA1.RA1_cff import *
from CMGTools.Susy.RA2.RA2_cff import *
from CMGTools.Susy.Razor.razor_cff import * 
from CMGTools.Susy.LP.LP_cff import *
from CMGTools.Common.countingSequences_cff import * 



RA1Path = cms.Path(
    susyCommonSequence *
    RA1SkimSequence
    )

RA2Path = cms.Path(
    susyCommonSequence +
    RA2SkimSequence 
    )

razorPath = cms.Path(
    susyCommonSequence +
    razorSkimSequence
    )

LPPath = cms.Path(
    susyCommonSequence +
    LPSkimSequence 
    )

# runInfoAccountingPath = cms.Path(
#    runInfoAccounting
#    )

susySchedule = cms.Schedule(
    RA1Path,
    RA2Path,
    razorPath,
    LPPath
    # , runInfoAccountingPath
    )
