import FWCore.ParameterSet.Config as cms

from CMGTools.Common.eventContent.everything_cff import *

razorMJjetEventContent = cms.untracked.vstring()

razorMJjetEventContent += cms.untracked.vstring(
                                           'keep *_razorMJ*_*_*',
                                           'drop cmgPFJets_razorMJPFJetSelID_*_*',
                                           'drop cmgPFJets_razorMJPFJetSel80_*_*',
                                           'drop cmgTriggerObjects_razorMJ*TriggerSel_*_*',
                                           'drop cmgHemispheres_razorMJHemiHadBox*_*_*'
                                           )
