import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning, patTriggerEventContent

##
## dilepton filters
##
def configureOutput(process,selPaths=['eePath', 'mumuPath', 'emuPath', 'photonPath'],outFile='/tmp/patTuple.root'):
    process.load('Configuration.EventContent.EventContent_cff')
    process.out.SelectEvents=cms.untracked.PSet( SelectEvents = cms.vstring(selPaths) )
    process.out.fileName = cms.untracked.string(outFile)
    outputCmds=['drop *',
                'keep *_prunedGen_*_*',
                'keep *_genMetTrue_*_*',
                'keep edmMergeableCounter_*_*_*',
                'keep *_eeCandidates_*_*',
                'keep *_mumuCandidates_*_*',
                'keep *_emuCandidates_*_*',
                'keep *_offlinePrimaryVertices_*_*',
                'keep *_offlineBeamSpot_*_*',
                'keep *_pfNoPileUp*_*_*',
                'keep *_pfPileUp*_*_*',
                'keep *_pfAllChargedHadrons*_*_*',
                'keep *_pfAllNeutralHadrons*_*_*',
                'keep *_pfAllPhotons*_*_*',
                'keep *_TriggerResults_*_*',
                'keep *_addPileupInfo_*_*',
                'keep *_genEventScale_*_*',
                'keep GenRunInfoProduct_*_*_*',
                'keep GenEventInfoProduct_*_*_*',
                'keep *_flavorHistoryFilter_*_*',
                'keep recoGenJets_ak5GenJets_*_*',
                'keep *_chargedMetProducer_*_*',
                'keep *_trackMetProducer_*_*',
                'keep *_hzzPFMetProducer_*_*',
                'keep *_generalTracks_*_*',
                'keep patPhotons_*_*_*',
                'keep *_reducedEcalRecHitsEB_*_*',
                'keep *_reducedEcalRecHitsEE_*_*',
                'keep double*_*_rho_'+process.name_(),
                'keep double*_*_sigma_'+process.name_(),
                'keep recoPFCandidates_particleFlow_*_*']
    outputCmds.extend( patEventContentNoCleaning )
    outputCmds.extend( patTriggerEventContent )
    process.out.outputCommands = outputCmds


