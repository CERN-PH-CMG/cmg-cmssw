import FWCore.ParameterSet.Config as cms

from CMGTools.HtoZZ2l2nu.StandardSelections_cfi import *
cleanEvent = cms.EDProducer("DileptonPlusMETEventProducer",
                            dtag=cms.string('h2zz'),
                            Trigger = BaseTriggerSelection.clone(),
                            Generator = BaseGeneratorSelection.clone(),
                            Vertices = BaseVertexSelection.clone(),
                            Photons = BasePhotonsSelection.clone(),
                            Muons = BaseMuonsSelection.clone(),
                            LooseMuons = BaseLooseMuonsSelection.clone(),
                            Electrons = BaseElectronsSelection.clone(),
                            LooseElectrons = BaseLooseElectronsSelection.clone(),
                            Dileptons = BaseDileptonSelection.clone(),
                            Jets = BaseJetSelection.clone(),
                            MET = BaseMetSelection.clone()
                            )
