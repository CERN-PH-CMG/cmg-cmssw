import FWCore.ParameterSet.Config as cms

from CMGTools.Common.PAT.PATCMG_cff import *
from CMGTools.Common.physicsObjectPrinter_cfi import physicsObjectPrinter
from CMGTools.Common.skims.cmgCandCount_cfi import *
from CMGTools.Common.skims.cmgTriggerObjectSel_cfi import *

#Multijet triggers from the MultiJet PD
#trigger list from here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/SUSYstop
razorMJHadTriggerSel = cmgTriggerObjectSel.clone(
                                            src = 'cmgTriggerObjectSel',
                                            cut = 'getSelectionRegExp("^HLT_DiJet[0-9]+_DiJet[0-9]+_DiJet[0-9]+.*_v[0-9]+$") ||'\
                                                '  getSelectionRegExp("^HLT_QuadJet[0-9]+_DiJet[0-9]+.*_v[0-9]+$") ||'\
                                                '  getSelectionRegExp("^HLT_QuadJet[0-9]+_v[0-9]+$") ||'\
                                                '  getSelectionRegExp("^HLT_QuadJet[0-9]+_L1FastJet_v[0-9]+$") ||'\
                                                '  getSelectionRegExp("^HLT_SixJet[0-9]+.*_v[0-9]+$")')
razorMJHadTriggerCount = cmgCandCount.clone( src = 'razorMJHadTriggerSel', minNumber = 1 )

razorMJHadTriggerInfo = physicsObjectPrinter.clone(
    #inputCollection = cms.untracked.InputTag("razorMJHadTriggerSel"),
    inputCollection = cms.untracked.InputTag("cmgTriggerObjectSel"),
    printSelections = cms.untracked.bool(True)
    )

#Trigger from the MultiJet1Parked PD
razorMJHadBTriggerSel = cmgTriggerObjectSel.clone(
                                            src = 'cmgTriggerObjectSel',
                                            cut = 'getSelectionRegExp("^HLT_QuadJet45_v[0-9]+$") ||'\
                                                '  getSelectionRegExp("^HLT_QuadJet50_v[0-9]+$")')

#muon triggers from the SingleMu PD
razorMJMuTriggerSel = cmgTriggerObjectSel.clone(
                                            src = 'cmgTriggerObjectSel',
                                            cut = 'getSelectionRegExp("^HLT_Mu[0-9]+_eta2p1_v[0-9]+$") ||'\
                                                  'getSelectionRegExp("^HLT_Mu[0-9]+_v[0-9]+$") ||'\
                                                  'getSelectionRegExp("^HLT_IsoMu[0-9]+_v[0-9]+$") ||'\
                                                  'getSelectionRegExp("^HLT_IsoMu[0-9]+_eta2p1_v[0-9]+$")')
razorMJMuTriggerCount = cmgCandCount.clone( src = 'razorMJMuTriggerSel', minNumber = 1 )

#Multijet triggers from the SingleElectron PD
razorMJEleTriggerSel = cmgTriggerObjectSel.clone(
                                            src = 'cmgTriggerObjectSel',
                                            cut = 'getSelectionRegExp("^HLT_Ele[0-9]+_WP80_v[0-9]+$")')
razorMJEleTriggerCount = cmgCandCount.clone( src = 'razorMJEleTriggerSel', minNumber = 1 )

#now OR the triggers
razorMJAllTriggerSel = cmgTriggerObjectSel.clone(
                                            src = 'cmgTriggerObjectSel',
                                            cut = '(%s) || (%s) || (%s) || (%s)' % (razorMJHadTriggerSel.cut.value(), razorMJHadBTriggerSel.cut.value(),\
                                                                                        razorMJMuTriggerSel.cut.value(),razorMJEleTriggerSel.cut.value())
)
razorMJAllTriggerCount = cmgCandCount.clone( src = 'razorMJAllTriggerSel', minNumber = 1 )

razorMJTriggerSequence = cms.Sequence(
    PATCMGTriggerSequence+
#    razorMJHadTriggerInfo+
    razorMJHadTriggerSel+
    razorMJMuTriggerSel+
    razorMJEleTriggerSel+
    razorMJAllTriggerSel
)

razorMJHadTriggerSeq = cms.Sequence(razorMJTriggerSequence+
                                 razorMJHadTriggerCount)

razorMJMuTriggerSeq = cms.Sequence(razorMJTriggerSequence+
                                 razorMJMuTriggerCount)

#razorMJEleTriggerSeq = cms.Sequence(razorMJTriggerSequence+
#                                 razorMJEleTriggerCount)

razorMJAllTriggerSeq = cms.Sequence(razorMJTriggerSequence+
                                 razorMJAllTriggerCount)
