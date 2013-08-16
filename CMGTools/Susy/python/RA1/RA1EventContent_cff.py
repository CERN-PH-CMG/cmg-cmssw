import FWCore.ParameterSet.Config as cms

from CMGTools.Common.eventContent.everything_cff import *

RA1EventContent = cms.untracked.vstring()

RA1EventContent += cms.untracked.vstring('keep *_RA1*_*_*')
