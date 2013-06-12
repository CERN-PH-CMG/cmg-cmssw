import FWCore.ParameterSet.Config as cms

from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,cmsswIs52X

sep_line = '-'*70

########## CONTROL CARDS

process = cms.Process("H2TAUTAU")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

# -1 : process all files
numberOfFilesToProcess = 10

debugEventContent = False

#tau-mu, tau-ele, di-tau, all
channel    = 'di-tau'
jetRecalib = False
useCHS     = False 
newSVFit   = True
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
#dataset_name = '/Tau/Run2012D-PromptReco-v1/AOD/PAT_CMG_V5_8_0'
# dataset_name = '/W2JetsToLNu_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0'
# dataset_name = '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0'
# dataset_name = '/DY1JetsToLL_M-50_TuneZ2Star_8TeV-madgraph/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0'
#dataset_name = '/WH_ZH_TTH_HToTauTau_M-110_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0'
#dataset_name = '/DoubleMu/StoreResults-DoubleMu_Run2012B_13Jul2012_v4_embedded_trans1_tau132_pttau1_17had2_17_v1-f456bdbb960236e5c696adfe9b04eaae/USER/PAT_CMG_V5_8_0'
dataset_name = '/GluGluToHToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0'
#dataset_name = '/SUSYBBHToTauTau_M-80_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0'
dataset_files = 'cmgTuple.*root'

# creating the source
from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    dataset_user,
    dataset_name,
    dataset_files,
    )

# process.source.fileNames = ['file:VBF_HToTauTau.root']

# restricting the number of files to process to a given number
if numberOfFilesToProcess>0:
    process.source.fileNames = process.source.fileNames[:numberOfFilesToProcess]


###ProductionTaskHook$$$
    
runOnMC = process.source.fileNames[0].find('Run201')==-1 and process.source.fileNames[0].find('embedded')==-1

# Sequence & path definition -------------------------------------------------


# set up JSON ---------------------------------------------------------------
if runOnMC==False:
    from CMGTools.Common.Tools.applyJSON_cff import applyJSON
    json="/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-208686_8TeV_PromptReco_Collisions12_JSON.txt"
    #json="/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Reprocessing/Cert_190456-196531_8TeV_13Jul2012ReReco_Collisions12_JSON_v2.txt"
    applyJSON(process, json )



# load the channel paths -------------------------------------------
process.load('CMGTools.H2TauTau.h2TauTau_cff')

# setting up the recoil correction according to the input file ---------------

print sep_line
from CMGTools.H2TauTau.tools.setupRecoilCorrection import setupRecoilCorrection,lookup,rootfile_dir
# WARNING DISABLING RECOIL CORRECTIONS FOR 2012!!!
setupRecoilCorrection( process, runOnMC, True, cmsswIs52X(), None, channel)

# OUTPUT definition ----------------------------------------------------------
process.outpath = cms.EndPath()

# generator ----------------------------------------------
if not runOnMC:
    process.tauMuPath.remove( process.genSequence )
    process.tauElePath.remove( process.genSequence )
    process.diTauPath.remove( process.genSequence )
    

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
#process.cmgTauScaler.cfg.nSigma = tauScaling # old method
process.cmgDiTauCorPreSel.cfg.nSigma = tauScaling # new method

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
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )


# Jet recalibration

if jetRecalib: 
    process.load('Configuration.StandardSequences.Services_cff')
    process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
    GT = None
    if cmsswIs44X():
      if runOnMC:
        GT = 'START44_V13::All'
      else:
        GT = 'GR_R_44_V15::All'
    else:
      if runOnMC:
        GT = 'START53_V10::All' # for 53X MC in >= 533
        # GT = 'START53_V7E::All' # for 53X MC in < 533
        # GT = 'START52_V10::All' # for 52X MC
      else:
        GT = 'GR_P_V41_AN1::All' # for 53X data in >= 533
        # GT = 'GR_P_V40_AN1::All' # for 53X data in < 533
        # GT = 'GR_R_52_V8::All' # for 52X data 
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
    process.cmgDiTauSVFit.SVFitVersion = 2
    #process.MessageLogger.cerr.FwkReport.reportEvery = 1
else:
    process.cmgTauMuCorSVFitPreSel.SVFitVersion = 1
    process.cmgTauEleCorSVFitPreSel.SVFitVersion = 1
    process.cmgDiTauSVFit.SVFitVersion = 1

# process.tauMu_fullsel_tree_CMG.SelectEvents = cms.untracked.PSet()
