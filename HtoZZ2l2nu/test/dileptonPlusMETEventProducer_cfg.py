import FWCore.ParameterSet.Config as cms

process = cms.Process("HtoZZto2l2nu")
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring('file:patTuple.root')
                            )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

from CMGTools.HtoZZ2l2nu.StandardSelections_cfi import *
process.cleanEvent = cms.EDProducer("DileptonPlusMETEventProducer",
                                    Vertices = BaseVertexSelection.clone(),
                                    Muons = BaseMuonsSelection.clone(),
                                    Electrons = BaseElectronsSelection.clone(),
                                    Dileptons = BaseDileptonSelection.clone(),
                                    Jets = BaseJetSelection.clone(),
                                    MET = BaseMetSelection.clone()
                                    )

process.out = cms.OutputModule("PoolOutputModule",
                               outputCommands = cms.untracked.vstring('drop *', 
                                                                      'keep *_*_*_HtoZZto2l2nu'),
                               fileName = cms.untracked.string('eh.root')
                               )
process.p = cms.Path(process.cleanEvent)
process.e = cms.EndPath(process.out)


