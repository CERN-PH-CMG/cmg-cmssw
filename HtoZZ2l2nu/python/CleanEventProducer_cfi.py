import FWCore.ParameterSet.Config as cms

from CMGTools.HtoZZ2l2nu.StandardSelections_cfi import *
cleanEvent = cms.EDProducer("DileptonPlusMETEventProducer",
                            dtag=cms.string('h2zz'),
                            Generator = BaseGeneratorSelection.clone(),
                            Vertices = BaseVertexSelection.clone(),
                            Muons = BaseMuonsSelection.clone(),
                            Electrons = BaseElectronsSelection.clone(),
                            Dileptons = BaseDileptonSelection.clone(),
                            Jets = BaseJetSelection.clone(),
                            MET = BaseMetSelection.clone()
                            )
