import FWCore.ParameterSet.Config as cms

from CMGTools.Common.eventContent.particleFlow_cff import *
from CMGTools.Common.eventContent.traditional_cff import *
from CMGTools.Common.eventContent.runInfoAccounting_cff import runInfoAccounting

hemispheres = cms.untracked.vstring(
    'keep *_cmgHemi_*_*',
    'keep *_cmgDiHemi_*_*',
    )

everything = particleFlow + traditional + hemispheres + runInfoAccounting

MHT = particleFlowMHT + traditionalMHT
