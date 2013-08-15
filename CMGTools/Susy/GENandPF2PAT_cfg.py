import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.patTemplate_cfg import *

import sys

#-------------------------
# General settings.
#-------------------------

# Pick the SuSy benchmark point.
working_point = "LM1"
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(False))

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("FastSimulation.Configuration.RandomServiceInitialization_cff")

#-------------------------
# Source settings.
#-------------------------

susy_points = ["LM0", "LM1", "LM2", "LM2mhfeq360", "LM3", "LM4",
               "LM5", "LM6", "LM7", "LM8", "LM9", "LM9p",
               "LM9t175", "LM11", "LM12", "LM13"]
if not working_point in susy_points:
    print >> sys.stderr, \
          "ERROR Working point '%s' is not defined!" % working_point
    sys.exit(1)
process.load("CMGTools.Susy.GeneratorSources." \
             "PYTHIA6_SUSY_%s_sftsht_7TeV_cff" % working_point)
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(30000))

#-------------------------
# GEN sequences: convert gen particles to PFCandidates.
#-------------------------

process.load("CMGTools.Susy.GEN_cff")

#-------------------------
# PF2PAT + PAT, working at GENlevel.
#-------------------------

process.load("PhysicsTools.PatAlgos.patSequences_cff")

from PhysicsTools.PatAlgos.tools.pfTools import *
postfix = "PFlow"
usePF2PAT(process, runPF2PAT=True, jetAlgo='AK5', jetInputTag = 'pfJets', runOnMC=True, postfix=postfix)
from CMGTools.Susy.PF2PATonGEN_cff import *
PF2PATonGEN(process)

#-------------------------
# CMG RA2 analysis
#-------------------------

process.dump = cms.EDAnalyzer("EventContentAnalyzer")

process.load("CMGTools.Common.countingSequences_cff")

process.p = cms.Path(
    process.ProductionFilterSequence +
    # process.startupSequence +
    process.GENSequence +
    process.PF2PATonGENSequence 
    # process.finalSequence 
    )


#-------------------------
# Output settings.
#-------------------------

process.load("CMGTools.Susy.outputGEN_cfi")

# from CMGTools.Common.eventContent.runInfoAccounting_cff import runInfoAccounting
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
#process.load("PhysicsTools.PFCandProducer.PF2PAT_EventContent_cff")
#process.out.outputCommands =  cms.untracked.vstring('drop *')
process.out.fileName = 'GENandPF2PAT.root'
process.out.outputCommands = cms.untracked.vstring(*patEventContentNoCleaning)
process.out.outputCommands.append( 'keep *_genMetTrue*_*_*' )
process.out.outputCommands.append( 'keep *_genMetTrue*_*_*' )
process.out.outputCommands.append( 'keep recoPFCandidates_particleFlow_*_*' )

# for run info accounting
process.out.outputCommands.append( 'keep GenRunInfoProduct_*_*_*' )
process.out.outputCommands.append( 'keep GenFilterInfo_*_*_*' )


# process.out.outputCommands.append( 'keep cmgHemispheres_*_*_*' )
# process.out.outputCommands += runInfoAccounting

#-------------------------

process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.genFilterEfficiencyProducer.filterPath = 'p'
process.outpath = cms.EndPath(
    process.out +
    process.genFilterSummary
    # process.saveHistosInRunInfo
    # + process.outputGEN
    )

#-------------------------
# Schedule all paths.
#-------------------------
process.schedule = cms.Schedule(
    process.p,
    process.outpath
)
