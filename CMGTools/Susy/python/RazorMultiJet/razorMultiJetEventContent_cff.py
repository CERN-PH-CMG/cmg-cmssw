import FWCore.ParameterSet.Config as cms

from CMGTools.Common.eventContent.everything_cff import *

razorMJjetEventContent = cms.untracked.vstring()

razorMJjetEventContent += cms.untracked.vstring(
                                           'keep *_razorMJ*_*_*',
                                           )
