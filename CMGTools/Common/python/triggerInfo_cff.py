import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgTriggerObject_cfi import *
from CMGTools.Common.skims.cmgTriggerObjectSel_cfi import *
from CMGTools.Common.skims.cmgTriggerObjectCount_cfi import *

triggerSequence = cms.Sequence(
    cmgTriggerObjectSequence*
    cmgTriggerObjectSel*
    cmgTriggerObjectCount
)