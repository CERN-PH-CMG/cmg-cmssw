import FWCore.ParameterSet.Config as cms

# from PhysicsTools.PatAlgos.patTemplate_cfg import *

sep_line = '-'*70
########## CONTROL CARDS

process = cms.Process("H2TAUTAU")


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

# -1 : process all files
numberOfFilesToProcess = 5

debugEventContent = False

#Jose: we'll need a flag to deal with 2011 data/MC because of name changes in 5_2_X
#     this is probably not yet implemented right, currently only testing in 5_2
year = '2012'

##########



# Input  & JSON             -------------------------------------------------


# process.setName_('H2TAUTAU')

dataset_user = 'benitezj' 
dataset_name = '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V5-v2/AODSIM/PATCMG_TEST52'

dataset_files = 'cmgTuple.*root'
if year=='2011':
    dataset_files = 'tree.*root'


# creating the source
from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    dataset_user,
    dataset_name,
    dataset_files,
    )


#process.source = cms.Source(
#    "PoolSource",
#    fileNames = cms.untracked.vstring(
#    #'/store/cmst3/user/cmgtools/CMG/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_5_0/tree_CMG_1.root'
#    #'/store/cmst3/user/cmgtools/CMG/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_5_0/tree_CMG_1.root'
#    'file:../../../Common/prod/TEST/cmgTuple_HToTauTau.root'
#    )
#    )


# process.source.fileNames = ['file:ttjets.root']

# restricting the number of files to process to a given number
if numberOfFilesToProcess>0:
    process.source.fileNames = process.source.fileNames[:numberOfFilesToProcess]


###ProductionTaskHook$$$
    
runOnMC = process.source.fileNames[0].find('Run201')==-1 and process.source.fileNames[0].find('embedded')==-1


# Sequence & path definition -------------------------------------------------

# Message logger setup.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )


# set up JSON ---------------------------------------------------------------
if runOnMC==False:
    from CMGTools.H2TauTau.tools.setupJSON import setupJSON
    json = setupJSON(process)


# generator ----------------------------------------------
process.generatorPath = cms.Path()
if runOnMC:
    # setting up vertex weighting 
    process.load('CMGTools.Common.generator.vertexWeight.vertexWeight_cff')
    process.generatorPath += process.vertexWeightSequence 

    # input needed for all recoil corrections 
    process.load('CMGTools.Common.generator.metRecoilCorrection.metRecoilCorrection_cff')
    process.generatorPath += process.cmgPFJetForRecoil 
    process.generatorPath += process.genWorZ


# load the channel paths -------------------------------------------
process.load('CMGTools.H2TauTau.h2TauTau_cff')

if year=='2011':
    process.cmgTauMuCorSVFitPreSel.metsigSrc = cms.InputTag("PFMETSignificanceAK5")
    process.cmgTauEleCorSVFitPreSel.metsigSrc = cms.InputTag("PFMETSignificanceAK5")
    process.cmgMuEleCorSVFitPreSel.metsigSrc = cms.InputTag("PFMETSignificanceAK5")
    process.cmgDiTauCorSVFitPreSel.metsigSrc = cms.InputTag("PFMETSignificanceAK5")


# setting up the recoil correction according to the input file ---------------
if runOnMC:
    print sep_line
    from CMGTools.H2TauTau.tools.setupRecoilCorrection import setupRecoilCorrection
    setupRecoilCorrection( process )


# OUTPUT definition ----------------------------------------------------------
process.outpath = cms.EndPath()


# create the full schedule
process.schedule = cms.Schedule(
    process.generatorPath, 
    
    process.tauMuFullSelPath,    

    #Jose: make sure that your sequence is properly configured, I've only looked at tauMu
    process.tauEleFullSelPath,    
    
    process.muEleFullSelPath,    
    
    process.diTauFullSelPath,    
    
    process.outpath
    )




print sep_line
print 'INPUT:'
print sep_line
print process.source.fileNames
print
if runOnMC==False:
    print 'json:', json
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
# print 'baseline selection EDM output'.ljust(justn), baselineName
# print 'basic selection EDM output'.ljust(justn), basicName
# print 'histograms output'.ljust(justn), histName 
# print 'Debug event content'.ljust(justn), debugEventContent

# you can enable printouts of most modules like this:
# process.cmgTauMuCorPreSelSVFit.verbose = True

# systematic shift on tau energy scale 
# process.cmgTauScaler.cfg.nSigma = -1

from CMGTools.H2TauTau.tools.setupOutput import *
addTauMuOutput( process, debugEventContent, addPreSel=False)
addTauEleOutput( process, debugEventContent, addPreSel=False)
addMuEleOutput( process, debugEventContent, addPreSel=False)
addDiTauOutput( process, debugEventContent, addPreSel=False)


# use standard leptons - could also clone the sequence.
#process.cmgTauMu.cfg.leg2Collection = 'cmgMuonSelStdLep'
# to relax mu ID:
#process.cmgTauMu.cuts.baseline.muLeg.id = cms.PSet()

#process.cmgTauEle.cfg.leg2Collection = 'cmgElectronSelStdLep'

#process.cmgMuEle.cfg.leg1Collection = 'cmgMuonSelStdLep'
#process.cmgMuEle.cfg.leg2Collection = 'cmgElectronSelStdLep'
process.cmgMuEle.cuts.baseline.muLeg.id = cms.PSet()

