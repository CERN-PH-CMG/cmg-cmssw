from PhysicsTools.PatAlgos.patTemplate_cfg import *
import os.path, tempfile

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))

runOnMC = True
triggerProcessName = 'HLT'
if runOnMC:
    triggerProcessName = 'REDIGI37X'
    process.GlobalTag.globaltag = cms.string('GR_R_38X_V15::All')
    process.source.fileNames = cms.untracked.vstring('rfio:///castor/cern.ch/user/w/wreece/SDMUONFILTER/ZmumuSummer10/1E16ABCD-0986-DF11-B57C-90E6BA442F1E.root')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.out.fileName = cms.untracked.string('patTuple_PATandPF2PAT.root')

# load the PAT config
process.load("PhysicsTools.PatAlgos.patSequences_cff")

#run PF2PAT
from PhysicsTools.PatAlgos.tools.pfTools import usePF2PAT
postfix = "PFlow"
usePF2PAT(process,runPF2PAT=True, jetAlgo="AK5", runOnMC=runOnMC, postfix=postfix) 

# a loose offline muon selection
process.patMuonSelector = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag('patMuonsPFlow'),
    cut =cms.string("pt >= 10 && abs(eta) < 2.4")
    )
# we require 2 muons
process.patMuonFilter = cms.EDFilter("CandViewCountFilter",
  src = cms.InputTag('patMuonSelector'),
  minNumber = cms.uint32(2)
)
process.dimuonSkim = cms.Sequence(process.patMuonSelector+process.patMuonFilter)

#this is used for bookeeping 
process.load("CMGTools.Common.countingSequences_cff")
process.p = cms.Path(
    process.startupSequence* #Store the number of events we start with                    
    getattr(process,"patPF2PATSequence"+postfix)* #Run PF2PAT
    process.dimuonSkim* #Skim for events with two muons
    process.finalSequence #Store the number of events we end with
)
process.e = cms.EndPath(
    process.saveHistosInRunInfo* #SAVE THE counters
    process.out
)
process.schedule = cms.Schedule(process.p,
                                process.e)

# Add PF2PAT output to the created file
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
process.out.outputCommands = cms.untracked.vstring('drop *',
                                                   'keep recoPFCandidates_particleFlow_*_*',
                                                   # Gen information
                                                   'keep *_genParticles_*_*',
                                                   'keep *_generator_*_*',
                                                   'keep *_genMetTrue_*_*',
                                                   'keep recoGenJets_ak5GenJets_*_*',
                                                   #Counters
                                                   'keep *_MEtoEDMConverter_*_*',
                                                   'keep GenRunInfoProduct_*_*_*',
                                                   # Vertex info
                                                   'keep recoVertexs_*_*_*',
                                                   #'keep *_generalTracks_*_*',
                                                   'keep recoBeamSpot_*_*_*',
                                                   # Trigger
                                                   'keep L1GlobalTriggerObjectMapRecord_*_*_*',
                                                   'keep *_TriggerResults_*_*',
                                                   'keep *_hltTriggerSummaryAOD_*_*',
                                                   *patEventContentNoCleaning ) 

process.MessageLogger.cerr.FwkReport.reportEvery = 500
