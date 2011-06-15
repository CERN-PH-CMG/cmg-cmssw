import FWCore.ParameterSet.Config as cms

process = cms.Process("SKIM")


##########

# Message logger setup.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

# source
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

process.load("CMGTools.Susy.sources.HT.Run2011A_May10ReReco_v1.AOD.PAT_CMG_NewJEC.RA2.source_cff")


## process.source = cms.Source(
##     "PoolSource",
## 	noEventSort = cms.untracked.bool(True),
## 	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
##         fileNames = cms.untracked.vstring("rfio:/castor/cern.ch/cms/store/cmst3/user/lucieg/CMG/HT/Run2011A-May10ReReco-v1/AOD/PAT_CMG_MAX/RA2/susy_tree_CMG_70.root")
## )


print "Processing:"
print process.source.fileNames

#skimming
process.load("CMGTools.Common.skims.cmgBaseMETSel_cfi")
process.mhtSkim     = process.cmgBaseMETSel.clone(
    src = "RA2MHTPFJet30",
    cut = "et()>100.",
    filter = cms.bool(True)
    )

process.p = cms.Path( process.mhtSkim )

# Setup event content.
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('susy_tree_skimMHTgt100.root'),
                               SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('p')),
                               outputCommands = cms.untracked.vstring('keep *')

                               )


process.outpath = cms.EndPath(process.out)

print process.out.dumpPython()

print "Output file: '%s'" % process.out.fileName

######################################################################


