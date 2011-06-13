import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning, patExtraAodEventContent

##
## dilepton filters
##
def configureOutput(process,selPaths=['eePath', 'mumuPath', 'emuPath'],outFile='patTuple.root'):
    process.load('Configuration.EventContent.EventContent_cff')
    process.out.SelectEvents=cms.untracked.PSet( SelectEvents = cms.vstring(selPaths) )
    process.out.fileName = cms.untracked.string(outFile)
    process.out.outputCommands = cms.untracked.vstring('drop *',
                                                       'keep *_*_HBHENoise*_*',
                                                       ##### MC
                                                       'keep *_prunedGen_*_*',
                                                       'keep *_genMetTrue_*_*',
                                                       #### this contain the counter after each filter you want to consider
                                                       #### the genFilter, the goodCollision cleaning, the Skimming
                                                       'keep edmMergeableCounter_*_*_*',
                                                       ##### all the muons and all the electrons
                                                       'keep *_patMuonsWithTrigger_*_*',
                                                       'keep *_patElectronsWithTrigger_*_*',
                                                       ##### the 2 leptons candidate I build
                                                       'keep *_zEECand_*_*',
                                                       'keep *_zMMCand_*_*',
                                                       'keep *_emuCand_*_*',
                                                       ##### MET
                                                       'keep *_tcMet_*_*',
                                                       'keep *_pfMet_*_*',
                                                       ##### Tracking
                                                       'keep *_offlinePrimaryVertices_*_*',
                                                       'keep *_offlineBeamSpot_*_*',
                                                       ###### Trigger
                                                       'keep *_TriggerResults_*_*',
                                                       ###### PileUp
                                                       'keep *_addPileupInfo_*_*',
                                                       ######Taus
                                                       'keep *_selectedPatTaus_*_*',
                                                       'keep *_genEventScale_*_*',
                                                       'keep GenRunInfoProduct_*_*_*',
                                                       'keep *_flavorHistoryFilter_*_*',
                                                       'keep recoGenJets_ak5GenJets_*_*',
                                                       'keep double*_*_rho_'+process.name_(),
                                                       'keep double*_*_sigma_'+process.name_(),
                                                       'keep recoPFCandidates_particleFlow_*_*',
                                                       'keep *_selectedPat*_*_*',
                                                       'keep patMETs_*_*_*'
                                                       )
#process.out.outputCommands.extend( patEventContentNoCleaning )
#process.out.outputCommands.extend( patExtraAodEventContent )
