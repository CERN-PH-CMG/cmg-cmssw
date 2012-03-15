import FWCore.ParameterSet.Config as cms
from HiggsAnalysis.HiggsTo2photons.hggPhotonIDCuts_cfi import *

PhoIDProd = cms.EDProducer('PhoIDProd',
                                   hggPhotonIDConfiguration = cms.PSet(hggPhotonIDCuts),
                                   vtxCollection = cms.InputTag("offlinePrimaryVerticesWithBS"),
                                   eleCollection = cms.InputTag("gsfElectrons"),
                                   tkCollection = cms.InputTag("generalTracks"),
                                   pfCollection = cms.InputTag("particleFlow"),
                                   rhoCollection = cms.InputTag("kt6PFJets","rho"),
)

