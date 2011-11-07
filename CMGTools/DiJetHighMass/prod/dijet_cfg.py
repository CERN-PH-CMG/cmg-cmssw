from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

####### ------------ Parameters ---------- ######

processName = 'DIJET'
runOnData = True
ext = 'CMG'



####### ----------- Set input and output parameters ---------- ########

print 'processing:'
print process.source.fileNames

process.load("CMGTools.Common.sources.TTJets_TuneZ2_7TeV_madgraph_tauola.Summer11_PU_S4_START42_V11_v1.AODSIM.V2.PAT_CMG_V2_3_0.source_cff")

process.source.fileNames = cms.untracked.vstring('file:patTuple_PF2PAT.root')


outFileNameExt = ext

process.setName_(processName)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )



process.out.fileName = cms.untracked.string('diJetAnalysis_%s.root' % ext )

# Event Content
from CMGTools.DiJetHighMass.EventContent.diJetAnalysis_cff import diJetHighMass
process.out.outputCommands = diJetHighMass


#output file for histograms etc
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("diJetAnalysis_Histograms_%s.root" % ext ) )


print 'output file: ', process.out.fileName

####### ----------- The analysis sequence --------------- ##########



# Analysis Sequence definition
process.load('CMGTools.DiJetHighMass.baseJets_cff')
process.load('CMGTools.DiJetHighMass.pfJets_cff')

from CMGTools.DiJetHighMass.myJetSequence_cff import myJetSequence
myJetSequence(process)


process.analysisSequence = cms.Sequence(
    process.jetSequence
    )


# Skimming
process.load('CMGTools.DiJetHighMass.skims.selHighMass_cff')

process.dump = cms.EDAnalyzer("EventContentAnalyzer")

# Total process
process.p = cms.Path(
    process.analysisSequence +
    process.select
    )


process.out.SelectEvents.SelectEvents.append('p')


#### -------------- Few settigs

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) ) 

#### Final
print process.out.dumpPython()
