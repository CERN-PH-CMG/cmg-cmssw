from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms
from CMGTools.Common.Tools.getGlobalTag import getGlobalTag

sep_line = "-" * 50
print
print sep_line
print "CMGTools main test"
print sep_line



process.setName_('ANA')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(5000)
    )

process.maxLuminosityBlocks = cms.untracked.PSet( 
    input = cms.untracked.int32(-1)
    )



# process.load("CMGTools.Common.sources.RelValZMM.CMSSW_4_2_5_START42_V12_v1.GEN_SIM_RECO.source_cff")
# process.load("CMGTools.Common.sources.SingleMu.Run2011A_May10ReReco_v1.AOD.PAT_CMG_V2.source_PAT_cff")
process.load("CMGTools.Common.sources.DoubleMu.Run2011A_ZMu_PromptSkim_v6.RAW_RECO.PAT_CMG_V2_3_0_JecPileUpDecoupled.source_cff")

# process.source.fileNames = cms.untracked.vstring(
#     'file:../prod/patTuple_PF2PAT.root'
#     )

# reading the first 10 files:
nFiles = 10
print 'WARNING: RESTRICTING INPUT TO THE FIRST', nFiles, 'FILES'
process.source.fileNames = process.source.fileNames[:nFiles-1] 

print process.source.fileNames

# output module for EDM event (ntuple)
process.out.fileName = cms.untracked.string('tree_testCMGTools.root')
from CMGTools.Common.eventContent.everything_cff import everything 
process.out.outputCommands = cms.untracked.vstring( 'drop *',
                                                    'keep *_*IdTight*_*_*')
process.out.outputCommands.extend( everything ) 
process.out.dropMetaData = cms.untracked.string('PRIOR')

#output file for histograms etc
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histograms_testCMGTools.root"))

# default analysis sequence    
process.load('CMGTools.Common.analysis_cff')

# now, we're going to tune the default analysis sequence to our needs
# by modifying the parameters of the modules present in this sequence. 

# removing the taus, as PFCandidate embedding does not work, hence an exception in the cmg Tau factory - not needed anymore
# from CMGTools.Common.Tools.tuneCMGSequences import removeObject
# removeObject( process, 'tau', '') 

# Select events with 2 jet ...  
# process.cmgPFJetCount.minNumber = 2
# with pT > 50.
# process.cmgPFJetSel.cut = "pt()>50"
# and MET larger than 50
# process.cmgPFMETSel.cut = "pt()>50"

# note: we're reading ttbar events

runOnMC = False

if runOnMC:
    process.load("CMGTools.Common.runInfoAccounting_cfi")
    process.outpath += process.runInfoAccounting

import os 
rootfile_dir = os.environ['CMSSW_BASE'] + 'src/CMGTools/Common/data'
process.vertexWeight = cms.EDFilter(
    "VertexWeightProducer",
    src = cms.InputTag(''),
    inputHistMC = cms.string( rootfile_dir + '/Pileup_Summer11MC.root'),
    inputHistData = cms.string( rootfile_dir + '/Pileup_2011_EPS_8_jul.root'),
    )

process.p = cms.Path(
    process.vertexWeight + 
    process.analysisSequence
)
process.GlobalTag.globaltag = cms.string(getGlobalTag(runOnMC))

process.schedule = cms.Schedule(
    process.p,
    process.outpath
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) ) 
