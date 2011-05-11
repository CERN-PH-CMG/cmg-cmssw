import FWCore.ParameterSet.Config as cms

from CMGTools.Common.eventContent.particleFlow_cff import *
from CMGTools.Common.eventContent.traditional_cff import *
from CMGTools.Common.eventContent.trigger_cff import *
from CMGTools.Common.eventContent.runInfoAccounting_cff import runInfoAccounting

patObjects = cms.untracked.vstring(
    'keep patMuons_selectedPat*_*_*',
    'keep patElectrons_selectedPat*_*_*',
    'keep cmgPhotons_selectedPat*_*_*'
    )

everything = particleFlow + traditional + patObjects + runInfoAccounting + trigger

MHT = particleFlowMHT + traditionalMHT
