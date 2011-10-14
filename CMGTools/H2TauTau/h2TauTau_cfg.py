from PhysicsTools.PatAlgos.patTemplate_cfg import *

##########


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

##########

# Message logger setup.
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )


process.setName_('H2TAUTAU')

process.source.fileNames = ['file:tree_CMG_4.root']

ext = 'CMG'

# output to be stored

print 'processing:'
print process.source.fileNames

outFileNameExt = ext

process.load('CMGTools.H2TauTau.Colin.h2TauTau_cff')


process.schedule = cms.Schedule(
    process.tauMuPath,    
    # process.tauEPath,
    process.outpath
    )


process.out.fileName = cms.untracked.string('h2TauTau_tree_%s.root' %  outFileNameExt)
from CMGTools.H2TauTau.Colin.eventContent.h2TauTau_cff import h2TauTau as h2TauTauEventContent
process.out.outputCommands.extend( h2TauTauEventContent ) 
process.out.outputCommands = cms.untracked.vstring('keep *')


process.out.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('tauMuPath') )

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("h2TauTau_histograms_%s.root" %  outFileNameExt)
    )

print process.out.dumpPython()

print 'output file: ', process.out.fileName
