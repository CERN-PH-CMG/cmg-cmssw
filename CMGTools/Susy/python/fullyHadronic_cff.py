import FWCore.ParameterSet.Config as cms

from CMGTools.Susy.fullyHadronicCommon_cff import *
from CMGTools.Susy.RA1.RA1_cff import *
from CMGTools.Susy.RA2.RA2_cff import *
from CMGTools.Susy.Razor.razor_cff import * 

fullyHadronicSequence = cms.Sequence(
    fullyHadronicCommonSequence +
    RA1Sequence +
    RA2Sequence +
    razorSequence 
    )
