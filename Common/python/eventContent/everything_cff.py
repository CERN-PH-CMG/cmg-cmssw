import FWCore.ParameterSet.Config as cms

from CMGTools.Common.eventContent.particleFlow_cff import *
from CMGTools.Common.eventContent.traditional_cff import *
from CMGTools.Common.eventContent.trigger_cff import *
from CMGTools.Common.eventContent.gen_cff import *
from CMGTools.Common.eventContent.eventCleaning_cff import *
from CMGTools.Common.eventContent.runInfoAccounting_cff import *

patObjects = cms.untracked.vstring(
    'drop patMuons_selectedPat*_*_*',
    'drop patTaus_selectedPat*_*_*',
    'keep patElectrons_selectedPat*_*_*',
    #COLIN : the following should be in traditional_cff
    'keep cmgPhotons_selectedPat*_*_*',
    'keep recoVertexs_offlinePrimaryVertices_*_*'
    )

everything = particleFlow + traditional + patObjects + runInfoAccounting + trigger + gen + eventCleaning

MHT = particleFlowMHT + traditionalMHT
