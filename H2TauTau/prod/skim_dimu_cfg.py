import FWCore.ParameterSet.Config as cms

from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,cmsswIs52X

sep_line = '-'*70

########## CONTROL CARDS

process = cms.Process("DIMU")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

# -1 : process all files
numberOfFilesToProcess = 100




# Input  & JSON             -------------------------------------------------


# process.setName_('H2TAUTAU')


dataset_user = 'cmgtools'
dataset_name = '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_16_0'
dataset_files = 'cmgTuple.*root'

# creating the source
from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    dataset_user,
    dataset_name,
    dataset_files,
    )

# restricting the number of files to process to a given number
if numberOfFilesToProcess>0:
    process.source.fileNames = process.source.fileNames[:numberOfFilesToProcess]


###ProductionTaskHook$$$
    
process.load('CMGTools.Common.skims.cmgDiMuonSel_cfi')
process.load('CMGTools.Common.skims.cmgDiMuonCount_cfi')

process.cmgDiMuonSel.src = 'cmgDiMuonSel'
process.cmgDiMuonSel.cut = ('mass()>50 && leg1().pt()>15 && charge()==0')
process.cmgDiMuonCount.minNumber = 1

process.p = cms.Path( process.cmgDiMuonSel +
                      process.cmgDiMuonCount )

process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('cmgTuple.root'),
    # save only events passing the full path
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    # save PAT Layer 1 output; you need a '*' to
    # unpack the list of commands 'patEventContent'
    outputCommands = cms.untracked.vstring('keep *')
    )

process.endpath = cms.EndPath(process.out)

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
