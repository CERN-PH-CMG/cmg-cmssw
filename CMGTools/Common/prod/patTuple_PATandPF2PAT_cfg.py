## import skeleton process
from PhysicsTools.PatAlgos.patTemplate_cfg import *

# the source is already defined in patTemplate_cfg.
# overriding source and various other things
#process.load("PhysicsTools.PFCandProducer.Sources.source_ZtoEles_DBS_312_cfi")
#process.source = cms.Source("PoolSource", 
#     fileNames = cms.untracked.vstring('file:myAOD.root')
#)

# process.load("PhysicsTools.PFCandProducer.Sources.source_ZtoMus_DBS_cfi")
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(500) )
process.out.fileName = cms.untracked.string('patTuple_PF2PAT.root')

# load the PAT config
process.load("PhysicsTools.PatAlgos.patSequences_cff")

# Configure PAT to use PF2PAT instead of AOD sources
# this function will modify the PAT sequences. It is currently 
# not possible to run PF2PAT+PAT and standart PAT at the same time
from PhysicsTools.PatAlgos.tools.pfTools import *

# An empty postfix means that only PF2PAT is run,
# otherwise both standard PAT and PF2PAT are run. In the latter case PF2PAT
# collections have standard names + postfix (e.g. patElectronPFlow)  
postfix = "PFlow"
usePF2PAT(process,runPF2PAT=True, jetAlgo='AK5', runOnMC=True, postfix=postfix) 

# turn to false when running on data
getattr(process, "patElectrons"+postfix).embedGenMatch = True
getattr(process, "patMuons"+postfix).embedGenMatch = True

# Let it run

process.load("CMGTools.Common.countingSequences_cff")

# colin : to test the counters
process.selectedPatJetsPFlow.cut = "pt()>30"
process.countPatJetsPFlow.minNumber = 3

process.p = cms.Path(
    process.startupSequence + 
    getattr(process,"patPF2PATSequence"+postfix) + 
    process.finalSequence 
)


if not postfix=="":
    process.p += process.patDefaultSequence

# Add PF2PAT output to the created file
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
#process.load("PhysicsTools.PFCandProducer.PF2PAT_EventContent_cff")
#process.out.outputCommands =  cms.untracked.vstring('drop *')
process.out.outputCommands = cms.untracked.vstring('drop *',
                                                   #Counters
                                                   'keep *_MEtoEDMConverter_*_*',
                                                   'keep GenRunInfoProduct_*_*_*',
                                                   # Vertex info
                                                   'keep recoVertexs_*_*_*',
                                                   'keep recoBeamSpot_*_*_*',
                                                   # Trigger
                                                   'keep L1GlobalTriggerObjectMapRecord_*_*_*',
                                                   'keep *_TriggerResults_*_*',
                                                   'keep *_hltTriggerSummaryAOD_*_*',
                                                   *patEventContentNoCleaning ) 


process.outpath += process.saveHistosInRunInfo


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
process.options.wantSummary = True    ##  (to suppress the long output at the end of the job)    

process.MessageLogger.cerr.FwkReport.reportEvery = 10
