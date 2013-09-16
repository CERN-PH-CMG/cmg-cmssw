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
dataset_name = '/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_5_0/H2TAUTAU_Feb2'
dataset_pattern = 'tauEle_fullsel_.*root'

debugEventContent = False

##########


# Input  & JSON             -------------------------------------------------


process.setName_('AEMVA')

# creating the source
from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    dataset_user,
    dataset_name,
    dataset_pattern) 


#process.source = cms.Source(
#    "PoolSource",
#    fileNames = cms.untracked.vstring(
#    '/store/cmst3/user/cbern/CMG/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_5_0/H2TAUTAU_Feb2/tauEle_fullsel_tree_CMG_1.root'
#    )
#    )

# restricting the number of files to process to a given number
if numberOfFilesToProcess > 0:
    process.source.fileNames = process.source.fileNames[:numberOfFilesToProcess]

###ProductionTaskHook$$$

# Sequence & path definition -------------------------------------------------

process.load('CMGTools.H2TauTau.h2TauTau_cff')

process.cmgTauEleCorSVFitFullSel.cut = 'leg1().tauID("againstElectronMVA")'
process.cmgTauEleCorSVFitFullSel.src = 'cmgTauEleCorSVFitFullSel'

process.tauEleFullSelPath = cms.Path(
    process.cmgTauEleCorSVFitFullSel +
    process.tauEleFullSelCount
    )

process.outpath = cms.EndPath()

process.schedule = cms.Schedule(
    process.tauEleFullSelPath,    
    process.outpath
    )

# OUTPUT definition ----------------------------------------------------------



sep_line = '-'*70

print sep_line
print 'INPUT:'
print sep_line
print process.source.fileNames
print
print sep_line
print 'OUPUT:'
print sep_line

from CMGTools.H2TauTau.tools.setupOutput import *
addTauEleOutput( process, debugEventContent, addPreSel=False)

# Message logger setup.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
