'''Add the ElectronMVA from MIT to each pat::Electron in a collection.

Inputs:
- pat::Electrons
- reduced ECAL rechits (they are in the PFAODs, but not in the PAT and CMG-tuples)
Other userFloats could be added there. 
'''
import FWCore.ParameterSet.Config as cms

import os 

basePath = os.environ['CMSSW_BASE'] + '/src/CMGTools/Common/data/ElectronMVAWeights/'

cmgPatElectronProducer = cms.EDProducer(
    "CMGPATElectronProducer",
    src = cms.InputTag( 'selectedPatElectronsAK5' ),
    redEBRecHits = cms.InputTag( 'reducedEcalRecHitsEB' ),
    redEERecHits = cms.InputTag( 'reducedEcalRecHitsEE' ),
    d0LowPt = cms.string( basePath + "Subdet0LowPt_NoIPInfo_BDTG.weights.xml" ),
    d1LowPt = cms.string( basePath + "Subdet1LowPt_NoIPInfo_BDTG.weights.xml" ),
    d2LowPt = cms.string( basePath + "Subdet2LowPt_NoIPInfo_BDTG.weights.xml" ),
    d0HighPt = cms.string( basePath + "Subdet0HighPt_NoIPInfo_BDTG.weights.xml" ),
    d1HighPt = cms.string( basePath + "Subdet1HighPt_NoIPInfo_BDTG.weights.xml" ),
    d2HighPt = cms.string( basePath + "Subdet2HighPt_NoIPInfo_BDTG.weights.xml" )   
)
