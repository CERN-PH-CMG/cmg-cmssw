## import skeleton process
from PhysicsTools.PatAlgos.patTemplate_cfg import *


# process.load('CMGTools.Common.sources.CMSSW_4_1_3.RelValTTbar_Tauola.GEN_SIM_RECO.START311_V2_PU_E7TeV_AVE_2_BX156_v1.source_cff')
from PhysicsTools.PatAlgos.tools.cmsswVersionTools import pickRelValInputFiles

# (relVal,tag) = ('RelValQCD_FlatPt_15_3000','MC_311_V2')
# (relVal, tag) = ('RelValLM1_sfts','MC_311_V2')
# (relVal, tag) = ('RelValZEE','START311_V2')

# process.source = cms.Source("PoolSource",
#    fileNames = cms.untracked.vstring(
#    pickRelValInputFiles( cmsswVersion  = 'CMSSW_4_1_2'
#                          #, relVal        = 'RelValLM1_sfts'
#                          , relVal = relVal
#                          # , globalTag     = 'START311_V2'
#                          , globalTag = tag
#                          , numberOfFiles = 999
#                          )
#    )
#)

# print process.source
# import sys
# sys.exit(1)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.maxLuminosityBlocks = cms.untracked.PSet( 
    input = cms.untracked.int32(-1)
    )

# process.load("PhysicsTools.PFCandProducer.Sources.source_ZtoMus_DBS_cfi")
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))

# process.load("PhysicsTools.PFCandProducer.Sources.source_ZtoMus_DBS_cfi")
runOnMC = True


process.out.fileName = cms.untracked.string('patTuple_PATandPF2PAT.root' )

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
jetAlgo="AK5"
usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo, jetInputTag='pfJets', runOnMC=runOnMC, postfix=postfix) 
# to run second PF2PAT+PAT with differnt postfix uncomment the following lines
# and add it to path
#postfix2 = "PFlow2"
#jetAlgo2="AK7"
#usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo2, runOnMC=True, postfix=postfix2)

# to use tau-cleaned jet collection uncomment the following:
#useTauCleanedPFJets(process, jetAlgo=jetAlgo, postfix=postfix) 

# to switch default tau to HPS tau uncomment the following:
#adaptPFTaus(process,"hpsPFTau",postfix=postfix)


if runOnMC == False:
    # removing MC matching for standard PAT sequence
    # for the PF2PAT+PAT sequence, it is done in the usePF2PAT function
    removeMCMatchingPF2PAT( process, '' ) 

# Let it run
# process.load('CMGTools.Susy.fullyHadronic_cff')

process.p = cms.Path(
#    process.startupSequence +                      
    getattr(process,"patPF2PATSequence"+postfix) 
#    second PF2PAT
#    + getattr(process,"patPF2PATSequence"+postfix2)
)
if not postfix=="":
    process.p += process.patDefaultSequence


# process.load("CMGTools.Common.runInfoAccounting_cfi")
# process.outpath += process.runInfoAccounting



# Add PF2PAT output to the created file
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
process.out.outputCommands = cms.untracked.vstring('drop *',
                                                   'keep recoPFCandidates_particleFlow_*_*',
                                                   #Counters
                                                   # 'keep *_MEtoEDMConverter_*_*',
                                                   # 'keep GenRunInfoProduct_*_*_*',
                                                   # Vertex info
                                                   'keep recoVertexs_*_*_*',
                                                   'keep recoBeamSpot_*_*_*',
                                                   # Trigger
                                                   'keep L1GlobalTriggerObjectMapRecord_*_*_*',
                                                   'keep *_TriggerResults_*_*',
                                                   'keep *_hltTriggerSummaryAOD_*_*',
                                                   'keep recoPFJets_pfJetsPFlow_*_*',
                                                   # 'keep recoCaloJets_*_*_*',
                                                   *patEventContentNoCleaning ) 

from CMGTools.Common.eventContent.runInfoAccounting_cff import runInfoAccounting
process.out.outputCommands += runInfoAccounting

# top projections in PF2PAT:

process.pfNoPileUpPFlow.enable = cms.bool(True) 
process.pfNoMuonPFlow.enable = cms.bool(True) 
process.pfNoElectronPFlow.enable = cms.bool(True)
process.pfNoTauPFlow.enable = cms.bool(True)
process.pfNoJetPFlow.enable = cms.bool(True)

# verbose flags for the PF2PAT modules

process.pfNoMuon.verbose = cms.bool(False)

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("histograms.root")
    )


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

# to relax the muon isolation, uncomment the following:
process.pfIsolatedMuonsPFlow.combinedIsolationCut = 0.25
process.pfIsolatedElectronsPFlow.combinedIsolationCut = 0.25

# process.patJetCorrFactorsPFlow.levels = []
# process.pfNoPileUpPFlow.enable = False
