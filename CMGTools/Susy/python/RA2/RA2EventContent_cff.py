import FWCore.ParameterSet.Config as cms

from CMGTools.Common.eventContent.everything_cff import *

RA2EventContent = cms.untracked.vstring()

RA2EventContent += MHT
RA2EventContent += cms.untracked.vstring('keep *_RA2cmgMHTPFJets50_*_*')
