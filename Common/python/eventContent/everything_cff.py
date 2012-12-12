import FWCore.ParameterSet.Config as cms

from CMGTools.Common.eventContent.particleFlow_cff import *
from CMGTools.Common.eventContent.traditional_cff import *
from CMGTools.Common.eventContent.trigger_cff import *
from CMGTools.Common.eventContent.gen_cff import *
from CMGTools.Common.eventContent.eventCleaning_cff import *
from CMGTools.Common.eventContent.runInfoAccounting_cff import *

patObjects = cms.untracked.vstring(
    'drop patTaus_selectedPat*_*_*',
    'drop patElectrons_*_*_*',
    'keep patElectrons_patElectronsWithTrigger_*_*',
    'drop patMuons_*_*_*',
    'keep patMuons_patMuonsWithTrigger_*_*',
    'drop patElectrons_*AK5NoPUSub_*_*',
    'drop patMuons_*AK5NoPUSub_*_*',    
    #COLIN : the following should be in traditional_cff
    'keep edmMergeableCounter_*_*_*',
    'keep cmgPhotons_selectedPat*_*_*',
    'keep recoVertexs_offlinePrimaryVertices_*_*',
    'keep recoPFMETs_pfMetForRegression__*',
    'keep double_*_rho_*',
    'keep *_nJetsPtGt1_*_*',
    'keep recoPFMETs_nopuMet__*',
    'keep recoPFMETs_puMet__*',
    'keep recoPFMETs_pcMet__*',    
    'keep recoPFMETs_tkMet__*',
    'keep recoCaloMETs_*_*_*',
    'keep *_ak5SoftTrackJetsForVbfHbb__*'
    )

everything = particleFlow + traditional + patObjects + runInfoAccounting + trigger + gen + eventCleaning

MHT = particleFlowMHT + traditionalMHT
