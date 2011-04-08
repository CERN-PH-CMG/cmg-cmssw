from PhysicsTools.PatAlgos.patTemplate_cfg import *

##########

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(20)
    )

##########

# Message logger setup.
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

doSkimHighMET = False
cut_highMET = 'pt()>50'
process.setName_('ANA')

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring("file:/afs/cern.ch/user/c/cbern/" \
                                      "scratch0/CMG/RelVal/4_1_3/" \
                                      "ttbar_PATandPF2PAT.root")
    )

ext = 'CMG'

# output to be stored

print 'processing:'
print process.source.fileNames

outFileNameExt = ext

process.load('CMGTools.Susy.fullyHadronic_cff')
process.fullyHadronicSchedule.append( process.outpath )

# pprint.pprint(process.out.outputCommands)
#process.dump = cms.EDAnalyzer("EventContentAnalyzer")

from CMGTools.Susy.fullyHadronicEventContent_cff import fullyHadronicEventContent
process.out.fileName = cms.untracked.string('susy_tree_%s.root' %  outFileNameExt)
process.out.outputCommands = cms.untracked.vstring('drop *')
process.out.outputCommands += fullyHadronicEventContent

# process.out.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('RA1Path','RA2Path','razorPath') )
process.out.SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring("RA1Path", 'RA2Path'))

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("susy_histograms_%s.root" %  outFileNameExt)
    )

print process.out.dumpPython()

print 'output file: ', process.out.fileName
# print process.schedule
