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

evAnalyzer.Trigger.gammaTriggers = cms.vstring('HLT_Photon20_CaloIdVL_IsoL_v',
                                               'HLT_Photon30_CaloIdVL_IsoL_v',
                                               'HLT_Photon50_CaloIdVL_IsoL_v',
                                               'HLT_Photon75_CaloIdVL_IsoL_v',
                                               'HLT_Photon90_CaloIdVL_IsoL_v',
                                               'HLT_Photon125_v'
                                               )

