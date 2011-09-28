import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgCandSel_cfi import *
from CMGTools.Common.skims.cmgCandCount_cfi import *
from CMGTools.Common.skims.cmgCandMerge_cfi import *
from CMGTools.Common.physicsObjectPrinter_cfi import physicsObjectPrinter

# count high pt jets
from CMGTools.Common.skims.cmgPFJetSel_cfi import *

multiPFJetSel50 = cmgPFJetSel.clone( src = 'cmgPFJetSel', cut = 'pt()>50 && abs(eta)<5.0' )
multiPFJetSel50Count = cmgCandCount.clone( src = 'multiPFJetSel50', minNumber = 4 )

#make a skim on the HLT - should match all multi triggers
from CMGTools.Common.skims.cmgTriggerObjectSel_cfi import *
multiTriggerSel = cmgTriggerObjectSel.clone(
                                            src = 'cmgTriggerObjectSel',
                                            cut = 'getSelectionRegExp("^HLT_QuadJet[0-9]+.*_v[0-9]+$") ||'\
                                                ' getSelectionRegExp("^HLT_EightJet[0-9]+.*_v[0-9]+$")'
                                            )
multiTriggerCount = cmgCandCount.clone( src = 'multiTriggerSel', minNumber = 1 )

multiTriggerInfo = physicsObjectPrinter.clone(
    inputCollection = cms.untracked.InputTag("multiTriggerSel"),
    printSelections = cms.untracked.bool(True)
    )

multiTriggerSequence = cms.Sequence(
    multiTriggerSel
    )


multiJetSequence = cms.Sequence(
    multiPFJetSel50
)

multiObjectSequence = cms.Sequence(
    multiJetSequence + 
    multiTriggerSequence                              
    )


multiSequence = cms.Sequence(
    multiObjectSequence
    )

#offline based selection
multijetSkimSequence = cms.Sequence(
    multiObjectSequence + 
    multiPFJetSel50Count
    )

#trigger based selection - we take all multi triggered events
multijetTriggerSkimSequence = cms.Sequence(
    multiObjectSequence+
    multiTriggerCount                                
    )
