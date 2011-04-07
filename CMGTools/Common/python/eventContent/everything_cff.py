import FWCore.ParameterSet.Config as cms

from CMGTools.Common.eventContent.particleFlow_cff import *
from CMGTools.Common.eventContent.traditional_cff import *
from CMGTools.Common.eventContent.runInfoAccounting_cff import runInfoAccounting

patObjects = cms.untracked.vstring(
    'keep patMuons_*_*_*',
    'keep patElectrons_*_*_*',
    'keep patPhotons_*_*_*'
    )

everything = particleFlow + traditional + patObjects + runInfoAccounting 

MHT = particleFlowMHT + traditionalMHT
