'''Add the ElectronMVA from MIT to each pat::Electron in a collection.

Inputs:
- pat::Electrons
- reduced ECAL rechits (they are in the PFAODs, but not in the PAT and CMG-tuples)
Other userFloats could be added there. 
'''
import FWCore.ParameterSet.Config as cms

import os 

cmgPatElectronProducer = cms.EDProducer(
    "CMGPATElectronProducer",
    src = cms.InputTag( 'selectedPatElectronsAK5' ),
    redEBRecHits = cms.InputTag( 'reducedEcalRecHitsEB' ),
    redEERecHits = cms.InputTag( 'reducedEcalRecHitsEE' ),
    d0LowPt = cms.FileInPath("CMGTools/Common/data/ElectronMVAWeights/Subdet0LowPt_NoIPInfo_BDTG.weights.xml" ),
    d1LowPt = cms.FileInPath("CMGTools/Common/data/ElectronMVAWeights/Subdet1LowPt_NoIPInfo_BDTG.weights.xml" ),
    d2LowPt = cms.FileInPath("CMGTools/Common/data/ElectronMVAWeights/Subdet2LowPt_NoIPInfo_BDTG.weights.xml" ),
    d0HighPt = cms.FileInPath("CMGTools/Common/data/ElectronMVAWeights/Subdet0HighPt_NoIPInfo_BDTG.weights.xml" ),
    d1HighPt = cms.FileInPath("CMGTools/Common/data/ElectronMVAWeights/Subdet1HighPt_NoIPInfo_BDTG.weights.xml" ),
    d2HighPt = cms.FileInPath("CMGTools/Common/data/ElectronMVAWeights/Subdet2HighPt_NoIPInfo_BDTG.weights.xml" )   
)
