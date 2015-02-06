import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgMuEle_cfi import *
from CMGTools.Common.skims.cmgMuEleSel_cfi import *

muEleSequence = cms.Sequence(
    cmgMuEle 
    +cmgMuEleSel 
)
