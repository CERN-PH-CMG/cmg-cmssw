import FWCore.ParameterSet.Config as cms

process = cms.Process("PHOTONTAG")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:MyPickedEvents.root'
    )
)

process.PhoIDProd = cms.EDProducer('PhoIDProd'
)

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('MyPickedEventsWithProduct.root')
)

  
process.p = cms.Path(process.PhoIDProd)

process.e = cms.EndPath(process.out)
