from PhysicsTools.PatAlgos.patTemplate_cfg import *

import pprint

#warning
process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(1000)
        )

# Message logger setup.
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

selectEvents = False
doSkimHighMET = False
cut_highMET = 'pt()>50'

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring( 'file:GENandPF2PAT.root' ) 
    )

ext = 'GENCMG'


# output to be stored

print 'processing:'

process.setName_('ANA')

print process.source.fileNames

outFileNameExt = ext

# reinitializing stuff defined in patTemplate_cfg
process.out.outputCommands = cms.untracked.vstring('drop *')
process.out.outputCommands.append('keep *_cmgHemi*_*_*')
process.out.outputCommands.append('keep *_cmgDiHemi*_*_*')
process.out.SelectEvents.SelectEvents = cms.vstring()

process.load("CMGTools.Common.countingSequences_cff")


from CMGTools.Common.eventContent.particleFlow_cff import particleFlow
process.out.outputCommands += particleFlow   
 
process.load('CMGTools.SusyJetMET.RA2CMG_cff')
process.RA2CMGSequence.remove( process.caloJetSequence)
process.RA2CMGSequence.remove( process.caloMetSequence)
process.RA2CMGSequence.remove( process.electronSequence)
process.RA2CMGSequence.remove( process.muonSequence)
process.RA2CMGSequence.remove( process.cutsummarymuon)

from CMGTools.Common.factories.cmgHemi_cfi import cmgHemi
from CMGTools.Common.factories.cmgDiHemi_cfi import cmgDiHemi

process.cmgHemi = cmgHemi.clone()
process.cmgDiHemi = cmgDiHemi.clone()

process.hemiSequence = cms.Sequence(process.cmgHemi * process.cmgDiHemi)
process.RA2CMGSequence.replace(process.skimSequence,
                               process.skimSequence + process.hemiSequence)

process.p = cms.Path(
    process.RA2CMGSequence
    )


# pprint.pprint(process.out.outputCommands)

#process.dump = cms.EDAnalyzer("EventContentAnalyzer")
# process.p += process.dump

if selectEvents:
    process.out.SelectEvents.SelectEvents.append('p')
process.out.fileName = cms.untracked.string('susy_tree_%s.root' %  outFileNameExt)

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("susy_histograms_%s.root" %  outFileNameExt )
    )

print process.out.dumpPython()

print 'output file: ', process.out.fileName
# print process.schedule


