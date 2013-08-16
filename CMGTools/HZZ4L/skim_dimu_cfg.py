from PhysicsTools.PatAlgos.patTemplate_cfg import *

sep_line = '-'*70
########## CONTROL CARDS


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

# -1 : process all files
numberOfFilesToProcess = 5

dataset_user = 'cmgtools' 
dataset_name = '/DoubleMu/Run2011A-HZZ-PromptSkim-v6/AOD/PAT_CMG_V2_4_0'
# dataset_name = '/TTJets_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_4_0'
dataset_pattern = 'tree.*root'

##########


# Input  & JSON             -------------------------------------------------

process.setName_('DIMU')

# creating the source
from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    dataset_user,
    dataset_name,
    dataset_pattern) 

# restricting the number of files to process to a given number
if numberOfFilesToProcess > 0:
    process.source.fileNames = process.source.fileNames[:numberOfFilesToProcess]

###ProductionTaskHook$$$

runOnMC = process.source.fileNames[0].find('Run201')==-1 and process.source.fileNames[0].find('embedded')==-1


# Sequence & path definition -------------------------------------------------

process.load('CMGTools.Common.skims.cmgDiMuonSel_cfi')
process.load('CMGTools.Common.skims.cmgDiMuonCount_cfi')

process.cmgDiMuonSel.src = 'cmgDiMuonSel'
process.cmgDiMuonSel.cut = ('mass()>50 && leg1().pt()>15 && leg2().pt()>15 && charge()==0 && leg1.relIso(0.5)<0.5 && leg2.relIso(0.5)<0.5')
process.cmgDiMuonCount.minNumber = 1

process.p = cms.Path( process.cmgDiMuonSel +
                      process.cmgDiMuonCount )

# set up JSON ---------------------------------------------------------------
#if runOnMC==False:
#    from CMGTools.H2TauTau.tools.setupJSON import setupJSON
#    json = setupJSON(process)

# setting up vertex weighting -----------------------------------------------

if runOnMC:
    #SIMULATION
    process.load('CMGTools.Common.generator.vertexWeight.vertexWeight_cff')
    process.p += process.vertexWeightSequence


# OUTPUT definition ----------------------------------------------------------

outFileNameExt = 'CMG'
basicName = 'tree_dimu_%s.root' %  outFileNameExt
process.out.fileName = cms.untracked.string( basicName )
process.out.outputCommands.append( 'keep *' )
process.out.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('p') )


# Message logger setup.
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# histograms                 ------

print sep_line
print 'INPUT:'
print sep_line
print process.source.fileNames
print
#if runOnMC==False:
#    print 'json:', json
print
print sep_line
print 'PROCESSING'
print sep_line
print 'runOnMC:', runOnMC
print 
print sep_line
print 'OUPUT:'
print sep_line
justn = 30 
print 'selection EDM output'.ljust(justn), basicName

