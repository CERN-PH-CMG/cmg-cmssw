## import skeleton process
from PhysicsTools.PatAlgos.patTemplate_cfg import *

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring( 'file:gen.root' ) 
    )

# process.load("PhysicsTools.PFCandProducer.Sources.source_ZtoMus_DBS_cfi")
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.out.fileName = cms.untracked.string('patTuple_PF2PAT.root')

# load the PAT config
process.load("PhysicsTools.PatAlgos.patSequences_cff")


# Configure PAT to use PF2PAT instead of AOD sources
# this function will modify the PAT sequences. It is currently 
# not possible to run PF2PAT+PAT and standart PAT at the same time
from PhysicsTools.PatAlgos.tools.pfTools import *

postfix = "PFlow"
usePF2PAT(process,runPF2PAT=True, jetAlgo='AK5', runOnMC=True, postfix=postfix) 

process.pfPileUpPFlow.Enable = False

process.PF2PATPFlow.remove( process.pfMuonsFromVertexPFlow )
process.pfSelectedMuonsPFlow.src = 'pfAllMuonsPFlow'
process.PF2PATPFlow.remove( process.pfElectronsFromVertexPFlow )
process.pfSelectedElectronsPFlow.src = 'pfAllElectronsPFlow'
process.PF2PATPFlow.remove( process.pfTauSequencePFlow )
process.PF2PATPFlow.remove( process.pfNoTauPFlow )

process.pfNoMuonPFlow.enable = False
process.pfNoElectronPFlow.enable = False

# before PAT: matching the PFCandidate to the gen particles, not
# the object reconstructed in the traditional way. 
process.electronMatchPFlow.src = 'pfIsolatedElectronsPFlow'
process.muonMatchPFlow.src = 'pfIsolatedMuonsPFlow'

process.patDefaultSequencePFlow.remove( process.makePatTausPFlow )
process.patDefaultSequencePFlow.remove( process.selectedPatTausPFlow )
process.patDefaultSequencePFlow.remove( process.countPatTausPFlow )

process.patDefaultSequencePFlow.remove( process.makePatPhotonsPFlow )
process.patDefaultSequencePFlow.remove( process.selectedPatPhotonsPFlow )
process.patDefaultSequencePFlow.remove( process.countPatPhotonsPFlow )

process.patDefaultSequencePFlow.remove( process.patJetCorrFactorsPFlow )

# swithToPFJets( process, cms.InputTag('pfJetsPFlow'), 'AK5', 'PFlow', None ) 

process.ak5GenJetsNoNu.src = 'particleFlow:GEN'
# process.patJetGenJetMatchPFlow.matched = 'ak5GenJetsNoNu2'
process.patJetsPFlow.addAssociatedTracks = False
process.patJetsPFlow.addBTagInfo = False
process.patJetsPFlow.addDiscriminators = False
process.patJetsPFlow.addEfficiencies = False
process.patJetsPFlow.addJetCharge = False
process.patJetsPFlow.addJetCorrFactors = False
process.patJetsPFlow.addJetID = False
process.patJetsPFlow.addTagInfos = False

process.patDefaultSequencePFlow.remove( process.jetTracksAssociatorAtVertexPFlow )
process.patDefaultSequencePFlow.remove( process.btaggingAODPFlow )
process.patDefaultSequencePFlow.remove( process.patJetChargePFlow )


# turn to false when running on data
getattr(process, "patElectrons"+postfix).embedGenMatch = True
getattr(process, "patMuons"+postfix).embedGenMatch = True

process.dump = cms.EDAnalyzer("EventContentAnalyzer")

# Let it run
process.p = cms.Path(
#    process.patDefaultSequence  +
    # getattr(process,"patPF2PATSequence"+postfix)
    process.PF2PATPFlow +
    process.genForPF2PATSequence +
    process.makePatJetsPFlow +
    process.selectedPatJetsPFlow +
    process.makePatMETsPFlow
    # + process.dump
)

# Add PF2PAT output to the created file
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
#process.load("PhysicsTools.PFCandProducer.PF2PAT_EventContent_cff")
#process.out.outputCommands =  cms.untracked.vstring('drop *')
process.out.outputCommands = cms.untracked.vstring('drop *',
                                                   'keep *_ak5GenJets*_*_*',
                                                   *patEventContentNoCleaning ) 


## ------------------------------------------------------
#  In addition you usually want to change the following
#  parameters:
## ------------------------------------------------------
#
#   process.GlobalTag.globaltag =  ...    ##  (according to https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions)
#                                         ##
#   process.source.fileNames = [          ##
#    '/store/relval/CMSSW_3_5_0_pre1/RelValTTbar/GEN-SIM-RECO/STARTUP3X_V14-v1/0006/14920B0A-0DE8-DE11-B138-002618943926.root'
#   ]                                     ##  (e.g. 'file:AOD.root')
#                                         ##
#   process.maxEvents.input = ...         ##  (e.g. -1 to run on all events)
#                                         ##
#   process.out.outputCommands = [ ... ]  ##  (e.g. taken from PhysicsTools/PatAlgos/python/patEventContent_cff.py)
#                                         ##
#   process.out.fileName = ...            ##  (e.g. 'myTuple.root')
#                                         ##
#   process.options.wantSummary = True    ##  (to suppress the long output at the end of the job)    

process.MessageLogger.cerr.FwkReport.reportEvery = 10
