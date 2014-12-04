import FWCore.ParameterSet.Config as cms

from RecoParticleFlow.PostProcessing.selectGoodPFEvents_cff import *
from Configuration.EventContent.EventContent_cff import RECOSIMEventContent
from RecoParticleFlow.PostProcessing.pfPostProcessingEventContent_cff import pfPostProcessingEventContent


inconsistentMuonsPath = cms.Path(
    ~inconsistentMuons 
    )


outInconsistentMuons = cms.OutputModule(
    "PoolOutputModule",
    RECOSIMEventContent,
    #outputCommands = cms.untracked.vstring('keep *'),
    #process.RECOSIMEventContent,
    fileName = cms.untracked.string('inconsistentMuons.root'),
    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('inconsistentMuonsPath'))
    )
outInconsistentMuons.outputCommands += pfPostProcessingEventContent

from RecoParticleFlow.PostProcessing.pfPostProcessingEventContent_cff import pfPostProcessingEventContent
outInconsistentMuons.outputCommands += pfPostProcessingEventContent

greedyMuonsPath = cms.Path(
    ~greedyMuons
    )

outGreedyMuons = outInconsistentMuons.clone()
outGreedyMuons.fileName = cms.untracked.string('greedyMuons.root')
outGreedyMuons.SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('greedyMuonsPath'))

#pfEventFilterPath = cms.Path(
#    ~pfEventFilter
#    )

#outPFEventFilter = outInconsistentMuons.clone()
#outPFEventFilter.fileName = cms.untracked.string('pfEventFilter.root')
#outPFEventFilter.SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('pfEventFilterPath'))
