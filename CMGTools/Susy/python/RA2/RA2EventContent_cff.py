import FWCore.ParameterSet.Config as cms

from CMGTools.Common.eventContent.everything_cff import *

RA2EventContent = cms.untracked.vstring()

RA2EventContent += MHT
RA2EventContent.extend(
    cms.untracked.vstring(
    'keep *_RA2MHTPFJet50Central_*_*',
    'keep *_RA2MHTPFJet30_*_*',
    'keep *_RA2PFJet50Central_*_*',
    'keep *_RA2Jet*_*_*',
    'keep *_RA2dPhi*_*_*',
    )
    )
