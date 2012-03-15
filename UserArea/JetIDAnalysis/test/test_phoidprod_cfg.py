import FWCore.ParameterSet.Config as cms

from HiggsAnalysis.HiggsTo2photons.hggPhotonIDCuts_cfi import *

process = cms.Process("PHOTONTAG")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:MyPickedEvents.root'
    )
)

process.PhoIDProd = cms.EDProducer('PhoIDProd',
                                   hggPhotonIDConfiguration = cms.PSet(hggPhotonIDCuts),
                                   vtxCollection = cms.InputTag("offlinePrimaryVerticesWithBS"),
                                   eleCollection = cms.InputTag("gsfElectrons"),
                                   tkCollection = cms.InputTag("generalTracks"),
                                   pfCollection = cms.InputTag("particleFlow"),
                                   rhoCollection = cms.InputTag("kt6PFJets","rho"),
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('MyPickedEventsWithProduct.root')
)

  
process.p = cms.Path(process.PhoIDProd)

process.e = cms.EndPath(process.out)
