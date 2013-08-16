from PhysicsTools.PatAlgos.patTemplate_cfg import *

########## CONTROL CARDS


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

# -1 : process all files
numberOfFilesToProcess = 10

dataset_user = 'cbern' 
# dataset_name = '/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_3_0'
dataset_name = '/TauPlusX/Run2011A-05Aug2011-v1/AOD/V2/PAT_CMG_V2_4_0/H2TAUTAU_Nov21'
dataset_pattern = 'h2TauTau_presel_tree.*root'


##########


# Input  & JSON             -------------------------------------------------


process.setName_('BASELINE')

# creating the source
from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    dataset_user,
    dataset_name,
    dataset_pattern) 

# restricting the number of files to process to a given number
if numberOfFilesToProcess > 0:
    process.source.fileNames = process.source.fileNames[:numberOfFilesToProcess]

# attach JSON mask to the source if reading a data file
json = None
if dataset_name.find('Run201')>-1:
    from CMGTools.Common.Tools.applyJSON_cff import *
    from CMGTools.RootTools.tools.jsonPick import *
    json = jsonPick( dataset_name )
    applyJSON(process, json )


# Sequence & path definition -------------------------------------------------

process.load('CMGTools.H2TauTau.h2TauTau_cff')

# running only the tauMu selection and counting
process.tauMuBaselinePath = cms.Path(
    process.cmgTauMuFullSel +
    process.tauMuFullSelCount
    )

# OUTPUT definition ----------------------------------------------------------


# full baseline selection

process.outBaseline = process.out.clone()
process.outBaseline.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('tauMuBaselinePath') )
outFileNameExt = 'CMG'
baselineName = 'h2TauTau_baseline_tree_%s.root' % outFileNameExt
process.outBaseline.fileName = baselineName 
process.outBaseline.outputCommands = ['keep *']

process.outpath = cms.EndPath( process.outBaseline ) 

# Message logger setup.
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.schedule = cms.Schedule(
    process.tauMuBaselinePath,    
    process.outpath
    )

sep_line = '-'*70

print sep_line
print 'INPUT:'
print sep_line
print process.source.fileNames
if json != None:
    print 'JSON:', json
print
print sep_line
print 'OUPUT:'
print sep_line
print 'baseline selection EDM output:  ', baselineName

