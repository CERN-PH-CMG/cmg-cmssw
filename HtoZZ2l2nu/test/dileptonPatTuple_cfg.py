from PhysicsTools.PatAlgos.patTemplate_cfg import *

runOnMC=False
postfix = "PFlow"

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    SkipEvent = cms.untracked.vstring('ProductNotFound')
    )

#
# input
#
process.source.fileNames = cms.untracked.vstring(
    '/store/relval/CMSSW_3_9_5/RelValTTbar/GEN-SIM-RECO/START39_V6-v1/0008/0AEEDFA4-88FA-DF11-B6FF-001A92811718.root',
    '/store/relval/CMSSW_3_9_5/RelValTTbar/GEN-SIM-RECO/START39_V6-v1/0008/143F2BA5-87FA-DF11-9774-0018F3D0967E.root',
    '/store/relval/CMSSW_3_9_5/RelValTTbar/GEN-SIM-RECO/START39_V6-v1/0008/5A9FB196-8CFA-DF11-A9BE-003048678ADA.root',
    '/store/relval/CMSSW_3_9_5/RelValTTbar/GEN-SIM-RECO/START39_V6-v1/0008/6625D0A3-8DFA-DF11-ACB8-001BFCDBD182.root',
    '/store/relval/CMSSW_3_9_5/RelValTTbar/GEN-SIM-RECO/START39_V6-v1/0008/745D5219-8CFA-DF11-B780-00261894395B.root',
    '/store/relval/CMSSW_3_9_5/RelValTTbar/GEN-SIM-RECO/START39_V6-v1/0008/921277A7-88FA-DF11-BA67-0018F3D096C6.root',
    '/store/relval/CMSSW_3_9_5/RelValTTbar/GEN-SIM-RECO/START39_V6-v1/0008/D6BCEF61-8FFA-DF11-84DA-002618943833.root',
    '/store/relval/CMSSW_3_9_5/RelValTTbar/GEN-SIM-RECO/START39_V6-v1/0008/EE928FA3-8DFA-DF11-B263-0018F3D096FE.root'
    )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
   
#
# include counting sequences
#
process.load("CMGTools.Common.countingSequences_cff")

#
# include pre-filter sequences
#
process.noscraping = cms.EDFilter("FilterOutScraping",
                                  applyfilter = cms.untracked.bool(True),
                                  debugOn = cms.untracked.bool(False),
                                  numtrack = cms.untracked.uint32(10),
                                  thresh = cms.untracked.double(0.25) )
process.primaryVertexFilter = cms.EDFilter("VertexSelector",
                                           src = cms.InputTag("offlinePrimaryVertices"),
                                           cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2"),
                                           filter = cms.bool(True)
                                           )
process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')
process.preFilter = cms.Sequence( process.noscraping*process.primaryVertexFilter*process.HBHENoiseFilter)

#
# prepare the trigger filter
#
# from HLTrigger.HLTfilters.hltHighLevel_cfi import *
# process.trigFilter = hltHighLevel.clone(TriggerResultsTag = "TriggerResults::HLT",
#                                          HLTPaths = ["HLT_Mu12_v1"])

#
# prepare FastJet sequence for pileup corrections
#
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
process.load('RecoJets.Configuration.RecoPFJets_cff')
process.kt6PFJets.doRhoFastjet = True
process.kt6PFJets.Ghost_EtaMax = cms.double(5.0) 
process.kt6PFJets.Rho_EtaMax = cms.double(5.0)
process.ak5PFJets.doAreaFastjet = True
process.ak5PFJets.Ghost_EtaMax = cms.double(5.0)  
process.ak5PFJets.Rho_EtaMax = cms.double(5.0)
process.fjSequence = cms.Sequence(process.kt6PFJets+process.ak5PFJets)

#
# configure PAT + PF2PAT
#
process.load("ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff")
process.patElectronId = process.simpleEleIdSequence
process.patElectrons.electronIDSources.simpleEleId95relIso=cms.InputTag("simpleEleId95relIso")
process.patElectrons.electronIDSources.simpleEleId90relIso=cms.InputTag("simpleEleId90relIso")
process.patElectrons.electronIDSources.simpleEleId85relIso=cms.InputTag("simpleEleId85relIso")
process.patElectrons.electronIDSources.simpleEleId80relIso=cms.InputTag("simpleEleId80relIso")
process.patElectrons.electronIDSources.simpleEleId70relIso=cms.InputTag("simpleEleId70relIso")
process.patElectrons.electronIDSources.simpleEleId60relIso=cms.InputTag("simpleEleId60relIso")

process.selectedPatJets.cut = cms.string("pt >= 15 && abs(eta) < 2.5")
from PhysicsTools.PatAlgos.tools.jetTools import *
fjJECLevels=['L1FastJet', 'L2Relative', 'L3Absolute']
osJECLevels=['L1Offset', 'L2Relative', 'L3Absolute']
if( not runOnMC ):
    fjJECLevels.extend(['L2L3Residual'])
    osJECLevels.extend(['L2L3Residual'])
    
process.patJetCorrFactors.levels = fjJECLevels
process.patJetCorrFactors.rho = cms.InputTag('kt6PFJets','rho')

from PhysicsTools.PatAlgos.tools.pfTools import usePF2PAT
usePF2PAT(process,runPF2PAT=True, jetAlgo="AK5", runOnMC=runOnMC, postfix=postfix)

if(not runOnMC ):
    from PhysicsTools.PatAlgos.tools.coreTools import *
    removeMCMatching(process,['All'])
    getattr(process, "patElectronsPFlow").embedGenMatch = False
    getattr(process, "patMuonsPFlow").embedGenMatch = False

from PhysicsTools.PatAlgos.tools.jetTools import *
switchJetCollection(process,cms.InputTag('ak5PFJets'),
                    doJTA        = True,
                    doBTagging   = True,
                    jetCorrLabel = ('AK5PF', cms.vstring(fjJECLevels)),
                    doType1MET   = False,
                    genJetCollection=cms.InputTag("ak5GenJets"),
                    doJetID      = True
                    )
process.patJets.embedCaloTowers = cms.bool(False)
process.patJets.embedPFCandidates = cms.bool(True)
addJetCollection(process,cms.InputTag('ak5PFJets'),
                 'AK5','PFOffset',
                 doJTA = True,
                 doBTagging = True,
                 jetCorrLabel = ('AK5PF', cms.vstring(osJECLevels)),
                 doType1MET = False,
                 doL1Cleaning = False,
                 doL1Counters = True,
                 genJetCollection=cms.InputTag("ak5GenJets"),
                 doJetID = True,
                 jetIdLabel = "ak5"
                 )

from PhysicsTools.PatAlgos.tools.metTools import *
addTcMET(process, 'TC')
addPfMET(process, 'PF')


#
# configure muon pre-selection
#
from PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi import *
process.selectedPatMuonsPFlow.cut=cms.string("pt >= 15 && abs(eta) < 2.4")
process.patMuonFilter = countPatMuons.clone(src = 'selectedPatMuons'+postfix, minNumber = 1)
process.patDiMuonFilter = countPatMuons.clone(src = 'selectedPatMuons'+postfix, minNumber = 2)

#
# configure electron pre-selection
#
from PhysicsTools.PatAlgos.selectionLayer1.electronCountFilter_cfi import *
process.selectedPatElectronsPFlow.cut =cms.string("pt >= 15 && abs(eta) < 2.5")
process.patElectronFilter = countPatElectrons.clone(src = 'selectedPatElectrons'+postfix, minNumber = 1)
process.patDiElectronFilter = countPatElectrons.clone(src = 'selectedPatElectrons'+postfix, minNumber = 2)

#
# define the paths
#
process.p = cms.Path( process.startupSequence*
                      process.preFilter*
                      process.patElectronId*
                      process.fjSequence*
                      getattr(process,"patPF2PATSequence"+postfix)*
                      process.patDefaultSequence
                      )
process.eePath = cms.Path( process.patDiElectronFilter)
process.mumuPath = cms.Path( process.patDiMuonFilter)
process.emuPath = cms.Path( process.patElectronFilter*process.patMuonFilter)
process.e = cms.EndPath( process.finalSequence*process.saveHistosInRunInfo*process.out )
process.schedule = cms.Schedule( process.p, process.eePath, process.mumuPath, process.emuPath,process.e )

#
# configure the output
#
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning, patExtraAodEventContent
process.out.SelectEvents=cms.untracked.PSet( SelectEvents = cms.vstring('eePath', 'mumuPath', 'emuPath') )
process.out.outputCommands = cms.untracked.vstring('drop *',
                                                   'keep *_MEtoEDMConverter_*_*',
                                                   'keep *_genEventScale_*_*',
                                                   'keep GenRunInfoProduct_*_*_*',
                                                   'keep *_flavorHistoryFilter_*_*',
                                                   'keep recoGenJets_ak5GenJets_*_*',
                                                   'keep double*_*_rho_'+process.name_(),
                                                   'keep double*_*_sigma_'+process.name_(),
                                                   'keep recoPFCandidates_particleFlow_*_*'
                                                   )
process.out.outputCommands.extend( patEventContentNoCleaning )
process.out.outputCommands.extend( patExtraAodEventContent )
process.outpath = cms.EndPath(process.out)

#
# message logger
#
process.MessageLogger.cerr.FwkReport.reportEvery = 500

print "[dileptonPatTuple] will run the following process"
print process.p
print "Events will be selected for the following paths:"
print process.out.SelectEvents.SelectEvents
