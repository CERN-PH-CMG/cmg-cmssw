import FWCore.ParameterSet.Config as cms

from CMGTools.HtoZZ2l2nu.StandardSelections_cfi import *
evAnalyzer = cms.EDAnalyzer("GammaPlusJetsEventAnalyzer",
                            dtag=cms.string('h2zz'),
                            Trigger = BaseTriggerSelection.clone(),
                            Vertices = BaseVertexSelection.clone(),
                            Photons = BasePhotonsSelection.clone(),
                            Jets = BaseJetSelection.clone(),
                            MET = BaseMetSelection.clone()
                            )



