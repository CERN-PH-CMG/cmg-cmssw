######################################################################
## Main cmsRun configuration file for the CMGTools tutorial.
## See also:
##   https://twiki.cern.ch/twiki/bin/view/CMS/CMGToolsTutorial
######################################################################

import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.patTemplate_cfg import *
from CMGTools.Common.eventContent.everything_cff import everything
from CMGTools.Common.Tools.getGlobalTag import getGlobalTag
from CMGTools.Common.physicsObjectPrinter_cfi import physicsObjectPrinter
from CMGTools.Production.datasetToSource import *

##########

sep_line = "-" * 50
print
print sep_line
print "CMGTools 'ZmumuJets' tutorial"
print sep_line

##########

process.setName_("CMG")

# Pick up the data files (H->ZZ skim).
process.source = datasetToSource(
    "cmgtools",
    "/DoubleMu/Run2011A-HZZ-PromptSkim-v6/AOD/PAT_CMG_V2_4_0",
    "patTuple_PF2PAT_.*root"
    )

# One can limit the number of files to avoid some of the CASTOR
# overhead (i.e., faster start-up).
nFilesMax = 10
print "WARNING: Restricting input to the first %d files (of the %d found)" % \
      (nFilesMax, len(process.source.fileNames))
if len(process.source.fileNames) > nFilesMax:
    process.source.fileNames = process.source.fileNames[:nFilesMax - 1]

print "Will process the following files:"
for fileName in process.source.fileNames:
    print "  %s" % fileName

# One can limit the number of events to process.
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(5000)
    )
tmp = process.maxEvents.input.value()
if tmp > -1:
    print "Will process at most %d events" % tmp

# One can limit the number of lumi-sections to process.
process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )
tmp = process.maxLuminosityBlocks.input.value()
if tmp > -1:
    print "Will process at most %d lumi-sections" % tmp

# Output module for EDM events (ntuple).
process.out.fileName = cms.untracked.string("tree_tutorial.root")
process.out.outputCommands = cms.untracked.vstring("drop *")
process.out.outputCommands.extend(everything)
process.out.dropMetaData = cms.untracked.string("PRIOR")

# Output file for histograms etc.
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histograms_tutorial.root"))

# Load the default analysis sequence.
process.load("CMGTools.Common.analysis_cff")

##########

# Now tune the default analysis sequence to our needs by modifying the
# parameters of the modules present in this sequence.

# Switch on production of trigger objects.
process.cmgTriggerObject.cfg.useTriggerObjects = cms.untracked.bool(True)

# Select events with 2 jets ...
# process.cmgPFJetCount.minNumber = 2
# with pT > 50.
# process.cmgPFJetSel.cut = "pt()>50"
# and MET larger than 50
# process.cmgPFMETSel.cut = "pt()>50"

##########

runOnMC = False

if runOnMC:
    process.load("CMGTools.Common.runInfoAccounting_cfi")
    process.outpath += process.runInfoAccounting

process.p = cms.Path(
    process.analysisSequence
    )

process.GlobalTag.globaltag = cms.string(getGlobalTag(runOnMC))

process.schedule = cms.Schedule(
    process.p,
    process.outpath
    )

##########

# How much feedback do we want from the framework?
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

######################################################################
