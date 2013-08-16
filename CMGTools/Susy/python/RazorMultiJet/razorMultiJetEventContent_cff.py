import FWCore.ParameterSet.Config as cms

from CMGTools.Common.eventContent.everything_cff import *

razorMJjetEventContent = cms.untracked.vstring()

razorMJjetEventContent += cms.untracked.vstring(
                                           'drop *',
                                           'keep PileupSummaryInfos_addPileupInfo_*_HLT',
                                           'keep recoVertexs_goodOfflinePrimaryVertices_*_PAT',
                                           'keep cmgBaseMETs_cmgPFMET__PAT',
                                           'keep cmgBaseMETs_cmgPFMETRaw__PAT',
                                           'keep *_razorMJ*_*_*',
                                           'drop cmgElectrons_razorMJElectronSAKCMG_*_*',
                                           'drop cmgElectrons_razorMJIsolatedElectrons_*_*',
                                           'drop cmgMuons_razorMJMuonSAKCMG_*_*',
                                           'drop cmgPFJets_cmgPFJetTightJetIdFailed_*_*',
                                           'drop cmgPFJets_razorMJPFJetSelID_*_*',
                                           'drop cmgPFJets_razorMJPFJetSel70_*_*',
                                           'drop cmgPFJets_razorMJTightBtaggedJets_*_*',
                                           'drop cmgPFJets_razorMJTightBtaggedVetoJets_*_*',
                                           'drop cmgPFJets_razorMJPFJetSelIDLepton_*_*',
                                           'drop cmgTriggerObjects_razorMJ*TriggerSel_*_*',
                                           'keep cmgTriggerObjects_razorMJAllTriggerSel_*_*',
                                           'drop cmgTaus_razorMJTauCleaned_*_*',
                                           'drop patElectrons_razorMJElectronSAK_*_*',
                                           'drop patMuons_razorMJMuonSAK_*_*',
                                           'drop cmgHemispheres_razorMJHemiHadBox*_*_*',
                                           'drop cmgHemispheres_razorMJHemiLepBox*_*_*',
                                           'drop recoCandidatesOwned_razorMJDiHemi*_*_*',
                                           'keep double_vertexWeight*_*_MJSkim',
                                           'keep *_razorMJTrackIsolationMaker_*_*',
                                           'keep *_isrWeight_*_*',
                                           'keep *_genParticlesStatus3_*_*'
                                           )
