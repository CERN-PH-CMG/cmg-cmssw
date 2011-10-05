import FWCore.ParameterSet.Config as cms

from CMGTools.HtoZZ2l2nu.StandardSelections_cfi import *
evAnalyzer = cms.EDAnalyzer("DileptonPlusMETEventAnalyzer",
                            dtag=cms.string('h2zz'),
                            Trigger = BaseTriggerSelection.clone(),
                            Generator = BaseGeneratorSelection.clone(),
                            Vertices = BaseVertexSelection.clone(),
                            Photons = BasePhotonsSelection.clone(),
                            Muons = BaseMuonsSelection.clone(),
                            Electrons = BaseElectronsSelection.clone(),
                            Dileptons = BaseDileptonSelection.clone(),
                            Jets = BaseJetSelection.clone(),
                            MET = BaseMetSelection.clone()
                            )
