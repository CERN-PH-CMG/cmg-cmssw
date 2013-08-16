import FWCore.ParameterSet.Config as cms
import os, sys

## using VarParsing taken from here:
## https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideAboutPythonConfigFile#Passing_Command_Line_Arguments_T
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("TOP2")

# setup 'analysis'  options
options = VarParsing.VarParsing ('analysis')

# setup any defaults you want
options.maxEvents = 1000 # -1 
options.outputFile = './'
options.inputFiles= '/store/cmst3/user/jueugste/Ntuples/TTJets_MassiveBinDECAY_treeCMG_leptonJetsSkim_9.root', 

# add additional inputs
options.register ('sampleLocation',
                  '/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0', # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.string,          # string, int, or float
                  "Location of the sample")

options.register ('sampleName',
                  'ttbar', # default value
                  VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                  VarParsing.VarParsing.varType.string,          # string, int, or float
                  "Name of the sample")


# get and parse the command line arguments
options.parseArguments()

## check if data
#if 'Run201' in options.sampleName or ['Run201' in f for f in options.inputFiles]:
if all(('Run201' in f) for f in options.inputFiles):
    runOnData = cms.untracked.bool(True)
    print 'detected it is data'
else:
    runOnData = cms.untracked.bool(False)
    print 'detected it is MC'

    #runOnData = False

#sample = 'ttbar'
sample = options.sampleName

# Message logger setup.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 500
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

# source
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

process.source = cms.Source("PoolSource",
    ##fileNames = cms.untracked.vstring('/store/cmst3/user/jueugste/Ntuples/TTJets_MassiveBinDECAY_treeCMG_leptonJetsSkim_9.root'),
    fileNames = cms.untracked.vstring(options.inputFiles),
    #fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/p/palencia/work/cmg/V5_10_0/CMGTools/CMSSW_5_3_3_patch3/src/UserCode/TopMass/test/ttbar_treeCMG_lJetsSel.root'),
    #fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/p/palencia/work/cmg/V5_10_0/CMGTools/CMSSW_5_3_3_patch3/src/UserCode/TopMass/test/muDataRun2012B_treeCMG_lJetsSel.root'),
)
 

print "\n Processing files:"
print process.source.fileNames

print "\n Processing events:"
print process.maxEvents



process.ana = cms.EDAnalyzer('TtbarLeptonJets',
                             isData = runOnData
    )

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(sample+'Histos.root')
    )

process.p = cms.Path( process.ana )
   

######################################################################



