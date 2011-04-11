from PhysicsTools.PatAlgos.patTemplate_cfg import *

import pprint

#warning
process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )

# Message logger setup.
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

selectEvents = False
doSkimHighMET = False
cut_highMET = 'pt()>50'

process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
      'file:GENandPF2PAT.root',
      # 'file:GENandPF2PAT_2.root'
      ),
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
    )

ext = 'GENCMG'


# output to be stored

print 'processing:'

process.setName_('ANA')

print process.source.fileNames

outFileNameExt = ext

# reinitializing stuff defined in patTemplate_cfg
#process.out.outputCommands = cms.untracked.vstring('drop *')
#process.out.outputCommands.append('keep *_cmgHemi*_*_*')
#process.out.outputCommands.append('keep *_cmgDiHemi*_*_*')
# process.out.SelectEvents.SelectEvents = cms.vstring()

process.load("CMGTools.Common.countingSequences_cff")


from CMGTools.Common.eventContent.particleFlow_cff import particleFlow
process.out.outputCommands += particleFlow   
 
#process.load('CMGTools.SusyJetMET.RA2CMG_cff')

#from CMGTools.Common.factories.cmgHemi_cfi import cmgHemi
#from CMGTools.Common.factories.cmgDiHemi_cfi import cmgDiHemi

#process.cmgHemi = cmgHemi.clone()
#process.cmgDiHemi = cmgDiHemi.clone()

#process.hemiSequence = cms.Sequence(process.cmgHemi * process.cmgDiHemi)
#process.RA2CMGSequence.replace(process.skimSequence,
#                               process.skimSequence + process.hemiSequence)


process.load('CMGTools.Susy.fullyHadronic_cff')
process.fullyHadronicCommonSequence.remove( process.caloJetSequence)
process.fullyHadronicCommonSequence.remove( process.caloMetSequence)
process.fullyHadronicCommonSequence.remove( process.electronSequence)
process.fullyHadronicCommonSequence.remove( process.muonSequence)
process.fullyHadronicCommonSequence.remove( process.cutsummarymuon)

process.fullyHadronicSchedule.append( process.outpath )




process.out.SelectEvents.SelectEvents = cms.vstring('RA1Path','RA2Path')
process.out.fileName = cms.untracked.string('susy_tree_%s.root' %  outFileNameExt)

## RA2 output

process.RA2out = process.out.clone()
process.RA2out.fileName = 'susy_tree_%s_%s.root' % ('RA2', outFileNameExt)
process.RA2out.SelectEvents.SelectEvents = cms.vstring('RA2Path')
process.outpath += process.RA2out

print 'Common output ----'
print process.out.dumpPython()
print 
print 'RA2 output    ----'
print process.RA2out.dumpPython()


process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("susy_histograms_%s.root" %  outFileNameExt )
    )


print 'output file: ', process.out.fileName
# print process.schedule


