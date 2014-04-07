import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgHemi_cfi import *

hemiSequence = cms.Sequence(
    cmgHemi 
)
