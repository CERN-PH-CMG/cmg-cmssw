import FWCore.ParameterSet.Config as cms

##single trigger object (container) with all HLT paths
from CMGTools.Common.factories.cmgTriggerObject_cfi import *
from CMGTools.Common.skims.cmgTriggerObjectSel_cfi import *
from CMGTools.Common.skims.cmgTriggerObjectCount_cfi import *

##single L1 trigger object (container) with all L1 seeds
from CMGTools.Common.factories.cmgL1TriggerObject_cfi import *
from CMGTools.Common.skims.cmgL1TriggerObjectSel_cfi import *
from CMGTools.Common.skims.cmgL1TriggerObjectCount_cfi import *

##actual trigger objects and their paths
from CMGTools.Common.factories.cmgTriggerObjectList_cfi import *
from CMGTools.Common.skims.cmgTriggerObjectListSel_cfi import *
from CMGTools.Common.skims.cmgTriggerObjectListCount_cfi import *

triggerSequence = cms.Sequence(
    cmgL1TriggerObject*
    cmgL1TriggerObjectSel*
    cmgL1TriggerObjectCount*
    cmgTriggerObject*
    cmgTriggerObjectSel*
    cmgTriggerObjectCount*
    cmgTriggerObjectList*
    cmgTriggerObjectListSel*
    cmgTriggerObjectListCount
)
