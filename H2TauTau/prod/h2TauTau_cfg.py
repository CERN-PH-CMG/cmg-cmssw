import FWCore.ParameterSet.Config as cms

from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,cmsswIs52X

sep_line = '-'*70

########## CONTROL CARDS

process = cms.Process("H2TAUTAU")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(2000) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

# -1 : process all files
numberOfFilesToProcess = 10

debugEventContent = False

#tau-mu, tau-ele, di-tau, all
channel = 'tau-mu'
jetRecalib = False
useCHS = False 
newSVFit = False 
tauScaling = 0

print sep_line
print 'channel', channel
print 'jet recalib', jetRecalib
print 'useCHS', useCHS
print 'newSVFit', newSVFit
print 'tau scaling =', tauScaling

##########



# Input  & JSON             -------------------------------------------------


# process.setName_('H2TAUTAU')

dataset_user = 'cmgtools' 
dataset_name = '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_V5_4_0'
dataset_files = 'cmgTuple.*root'

# creating the source
from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    dataset_user,
    dataset_name,
    dataset_files,
    )

process.source.fileNames = ['file:VBF_HToTauTau.root']

# restricting the number of files to process to a given number
if numberOfFilesToProcess>0:
    process.source.fileNames = process.source.fileNames[:numberOfFilesToProcess]


###ProductionTaskHook$$$
    
runOnMC = process.source.fileNames[0].find('Run201')==-1 and process.source.fileNames[0].find('embedded')==-1

# Sequence & path definition -------------------------------------------------


# set up JSON ---------------------------------------------------------------
if runOnMC==False:
    from CMGTools.H2TauTau.tools.setupJSON import setupJSON
    json = setupJSON(process)



# load the channel paths -------------------------------------------
process.load('CMGTools.H2TauTau.h2TauTau_cff')

# setting up the recoil correction according to the input file ---------------

print sep_line
from CMGTools.H2TauTau.tools.setupRecoilCorrection import setupRecoilCorrection
# WARNING DISABLING RECOIL CORRECTIONS FOR 2012!!!
setupRecoilCorrection( process, runOnMC, True, cmsswIs52X())

# OUTPUT definition ----------------------------------------------------------
process.outpath = cms.EndPath()

# generator ----------------------------------------------
if not runOnMC:
    process.tauMuPath.remove( process.genSequence )
    process.tauElePath.remove( process.genSequence )
    process.diTauPath.remove( process.genSequence )
    

#Jose: process.schedule doesn't have a += operator?
if channel=='all':
    process.schedule = cms.Schedule(
        process.tauMuPath,
        process.tauElePath,
        # process.muElePath,    
        process.diTauPath,
        process.outpath
        )
elif channel=='tau-mu':
    process.schedule = cms.Schedule(
        process.tauMuPath,
        process.outpath
        )
elif channel=='tau-ele':
    process.schedule = cms.Schedule(
        process.tauElePath,
        process.outpath
        )
elif channel=='di-tau':
    process.schedule = cms.Schedule(
        process.diTauPath,
        process.outpath
        )
else:
    raise ValueError('unrecognized channel')    



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
process.cmgTauScaler.cfg.nSigma = tauScaling

from CMGTools.H2TauTau.tools.setupOutput import *
if channel=='tau-mu' or channel=='all':
    addTauMuOutput( process, debugEventContent, addPreSel=False)
if channel=='tau-ele' or channel=='all':
    addTauEleOutput( process, debugEventContent, addPreSel=False)
## if channel=='mu-ele' or channel=='all':
##     addMuEleOutput( process, debugEventContent, addPreSel=False)
if channel=='di-tau' or channel=='all':
    addDiTauOutput( process, debugEventContent, addPreSel=False)




# Message logger setup.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )


# Jet recalibration

if jetRecalib: 
    process.load('Configuration.StandardSequences.Services_cff')
    process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
    GT = None
    if runOnMC:
        if cmsswIs44X():
            GT = 'START44_V13::All'
        else:
            GT = 'START52_V10::All'
    else:
        if cmsswIs44X():
            GT = 'GR_R_44_V15::All'
        else:
            GT = 'GR_R_52_V8::All'  
    process.GlobalTag.globaltag = GT
    from CMGTools.Common.miscProducers.cmgPFJetCorrector_cfi import cmgPFJetCorrector
    process.cmgPFJetSel = cmgPFJetCorrector.clone(src='cmgPFJetSel',
                                                  payload='AK5PF')
    process.cmgPFJetSelCHS = cmgPFJetCorrector.clone(src='cmgPFJetSelCHS',
                                                     payload='AK5PFchs')

    if runOnMC:
        process.cmgPFJetSel.levels = cms.vstring('L1FastJet','L2Relative','L3Absolute')
        process.cmgPFJetSelCHS.levels = cms.vstring('L1FastJet','L2Relative','L3Absolute')
    else:
        process.cmgPFJetSel.levels = cms.vstring('L1FastJet','L2Relative','L3Absolute','L2L3Residual')
        process.cmgPFJetSelCHS.levels = cms.vstring('L1FastJet','L2Relative','L3Absolute','L2L3Residual')

    process.tauMuPath.insert(0, process.cmgPFJetSel)
    process.tauElePath.insert(0, process.cmgPFJetSel)
    process.diTauPath.insert(0, process.cmgPFJetSel)

    process.tauMuPath.insert(0, process.cmgPFJetSelCHS)
    process.tauElePath.insert(0, process.cmgPFJetSelCHS)
    process.diTauPath.insert(0, process.cmgPFJetSelCHS)

    print sep_line
    print 'Jet recalibration with GLOBAL TAG', GT

if useCHS:
    process.cmgPFJetForRecoil.src = 'cmgPFJetSelCHS'

if newSVFit:
    process.cmgTauMuCorSVFitPreSel.SVFitVersion = 2
    process.cmgTauEleCorSVFitPreSel.SVFitVersion = 2
    process.MessageLogger.cerr.FwkReport.reportEvery = 1
else:
    process.cmgTauMuCorSVFitPreSel.SVFitVersion = 1
    process.cmgTauEleCorSVFitPreSel.SVFitVersion = 1

# process.tauMu_fullsel_tree_CMG.SelectEvents = cms.untracked.PSet()
