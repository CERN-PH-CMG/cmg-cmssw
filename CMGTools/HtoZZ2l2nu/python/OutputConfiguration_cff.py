import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning, patTriggerEventContent

##
## dilepton filters
##
def configureOutput(process,selPaths=['llPath', 'photonPath'],outFile='/tmp/patTuple.root'):
    process.out.SelectEvents=cms.untracked.PSet( SelectEvents = cms.vstring(selPaths) )
    process.out.fileName = cms.untracked.string(outFile)
    outputCmds=['drop *']
    outputCmds.extend( patEventContentNoCleaning )
    outputCmds.extend( patTriggerEventContent )
    outputCmds.extend(['keep edmMergeableCounter_*_*_*',                     #counters 
                       'keep *_offlinePrimaryVertices_*_*',                  #vertices and tracks
                       'keep *_offlineBeamSpot_*_*',
                       'keep *_generalTracks_*_*',
                       'keep *_TriggerResults_*_*',
                       'keep *_addPileupInfo_*_*',                            #gen level 
                       'keep *_genEventScale_*_*',     
                       'keep GenRunInfoProduct_*_*_*',
                       'keep GenEventInfoProduct_*_*_*',
                       'keep *_prunedGen_*_*',
                       'keep *_genMetTrue_*_*',
                       'keep *_flavorHistoryFilter_*_*',
                       'keep recoGsfTracks_electronGsfTracks_*_*',             #electrons
                       'keep recoGsfElectronCores_gsfElectronCores_*_*',
                       'keep recoGsfElectrons_gsfElectrons_*_*',
                       'keep recoConversions_allConversions_*_*',
                       'keep recoPhotonCores_*_*_*',                           #photons     
                       'keep recoPhotons_*_*_*',
                       'keep recoCaloClusters_*_*_*',                          #e/g common  
                       'keep recoSuperClusters_*_*_*',
                       'keep *_reducedEcalRecHitsEB_*_*',
                       'keep *_reducedEcalRecHitsEE_*_*',
                       'keep *_generator_weight_EmbeddedRECO',                 # tau replacement
                       'keep *_TriggerResults_*_EmbeddedRECO',
                       'keep double*_*_rho_'+process.name_(),
                       'keep double*_*_sigma_'+process.name_(),
                       'drop CaloTowers_selectedPatJets*_*_*',
                       'drop *_iterativeCone*_*_*',
                       'drop recoGenJets_*_*_*',               
                       #'keep recoGenJets_ak5GenJets_*_*',
                       'keep recoGenJets_selectedPatJets*_*_*',
                       #'drop recoGenJets_selectedPatJets*_*_*',
                       'drop patTaus_*_*_*',
                       'drop *_*PFlowNoPuSub_*_*',
                       'keep recoGenJets_selectedPatJetsPFlowNoPuSub_*_*',
                       'keep patJets_selectedPatJetsPFlowNoPuSub_*_*',
                       'keep recoPFCandidates_selectedPatJetsPFlowNoPuSub_*_*',
                       'keep recoPFCandidates_particleFlow_*_*']               # particle flow
                      )
    process.out.outputCommands = outputCmds


