import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgTriggerObject_cfi import *

triggerSequence = cms.Sequence(
    cmgTriggerObjectSequence
)