import FWCore.ParameterSet.Config as cms

from CMGTools.Common.eventContent.everything_cff import *

razorEventContent = cms.untracked.vstring()

razorEventContent += cms.untracked.vstring(
                                           #'keep *_razor*_*_*'
                                           'keep cmgBaseMETs_razor*Met_*_*',
                                           'keep cmgHemispheres_razorHemi*Box_*_*',
                                           'keep cmgHemispherecmgHemispherecmgDiObjects_razorDiHemi*Box_*_*',
                                           'keep cmgPFJets_razorPFJetsMuonVeto_*_*',
                                           'keep cmgPFJets_razorPFBJetsMuonVeto_*_*',
                                           'keep cmgPFJets_razorPFBJetSel_*_*',
                                           'keep cmgTriggerObjects_razorTriggerSel_*_*',
                                           'keep razorRazorBoxs_razorBoxDef_*_*'
                                           )
