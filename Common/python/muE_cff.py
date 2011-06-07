import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgMuE_cfi import *
from CMGTools.Common.skims.cmgMuESel_cfi import *

muESequence = cms.Sequence(
    cmgMuE 
    +cmgMuESel 
)
