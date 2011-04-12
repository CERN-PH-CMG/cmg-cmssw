import FWCore.ParameterSet.Config as cms

from CMGTools.Common.eventContent.everything_cff import *

razorEventContent = cms.untracked.vstring()

razorEventContent += cms.untracked.vstring('keep *_razor*_*_*')
