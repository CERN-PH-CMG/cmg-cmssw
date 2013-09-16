
# from L1Trigger.Configuration.L1RawToDigi_cff import *
from PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff import *
patTrigger.processName = cms.string('*')
from CMGTools.Common.trigger_cff import *

PATTriggerSequence = cms.Sequence(
    # l1GtRecord + 
    patTriggerDefaultSequence
    )
