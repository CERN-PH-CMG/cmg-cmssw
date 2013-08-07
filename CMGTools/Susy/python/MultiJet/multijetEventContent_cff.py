import FWCore.ParameterSet.Config as cms

from CMGTools.Common.eventContent.everything_cff import *

multijetEventContent = cms.untracked.vstring()

multijetEventContent += cms.untracked.vstring(
#                                           'keep *_multi*_*_*',
                                            'keep *_multiPFJetSel40_*_*',
                                            'keep *_multiMuonLoose_*_*',
                                            'keep *_multiPFJetsMuonRequired_*_*',
                                            'keep *_multiPFBJetsMuonRequired*_*_*',
                                           )
