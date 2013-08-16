from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms
# from CMGTools.Common.Tools.getGlobalTag import getGlobalTag

sep_line = "-" * 50
print
print sep_line
print "CMGTools adding vertex weight"
print sep_line



process.setName_('VERTEX')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

process.maxLuminosityBlocks = cms.untracked.PSet( 
    input = cms.untracked.int32(-1)
    )



from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    'cbern',
#    '/TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_3_0/H2TAUTAU_Nov14',
#    '/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_3_0/H2TAUTAU_Nov14',
    '/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_3_0/H2TAUTAU_Nov14',
    '.*fullsel.*root' ) 


# reading the first 10 files:
# nFiles = 5
# print 'WARNING: RESTRICTING INPUT TO THE FIRST', nFiles, 'FILES'
# process.source.fileNames = process.source.fileNames[:nFiles] 

print process.source.fileNames

# output module for EDM event (ntuple)
process.out.fileName = cms.untracked.string('h2tautau_fullsel.root')
process.out.outputCommands = cms.untracked.vstring(
    'keep *',
    )


process.load('CMGTools.Common.generator.vertexWeight.vertexWeight_cff')

process.p = cms.Path(
    process.vertexWeightSequence
)


process.schedule = cms.Schedule(
    process.p,
    process.outpath
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) ) 
