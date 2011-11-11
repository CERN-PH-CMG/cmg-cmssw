import FWCore.ParameterSet.Config as cms

from CMGTools.Common.eventContent.everything_cff import *

leptonicStopEventContent = cms.untracked.vstring()

leptonicStopEventContent += cms.untracked.vstring(
#                                           'keep *_leptonicStop*_*_*',
#                                           #the objects, W's, Z's, and jets
                                           'keep *_leptonicStopWMuNuSel_*_*',
                                           'keep *_leptonicStopWENuSel_*_*',
                                           #W's with modified met
                                           'keep *_leptonicStopWMuNuNoEle_*_*',                                           
                                           'keep *_leptonicStopWMuNuNoMu_*_*',
                                           'keep *_leptonicStopWEleNuNoEle_*_*',
                                           'keep *_leptonicStopWEleNuNoMu_*_*',
                                           #Z
                                           'keep *_leptonicStopDiMuon_*_*',
                                           'keep *_leptonicStopDiElectron_*_*',
                                           #lepton cleaned jets
                                           'keep *_leptonicStopPFJetsLeptonVeto_*_*',
#                                           leptons
                                           'keep *_leptonicStopElectronTight_*_*',
                                           'keep *_leptonicStopElectronLoose_*_*',
                                           'keep *_leptonicStopMuonTight_*_*',
                                           'keep *_leptonicStopMuonLoose_*_*',                                           
#                                           the number of jets                                           
                                           'keep *_leptonicStopPFJetSelSize_*_*',
                                           'keep *_leptonicStopPFBJetSelSize_*_*',
                                           'keep *_leptonicStopPFBJetsLeptonVetoSize_*_*',
                                           #the trigger objects to check
                                           'keep *_leptonicStopTriggerSel_*_*',
                                           #the modified MET objects
                                           'keep *_leptonicStopTriggerSel_*_*',
                                           )
