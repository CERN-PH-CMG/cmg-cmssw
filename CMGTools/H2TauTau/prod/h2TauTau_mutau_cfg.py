import FWCore.ParameterSet.Config as cms

from CMGTools.Common.Tools.cmsswRelease import isNewerThan, cmsswIs44X,cmsswIs52X

sep_line = '-'*70

########## CONTROL CARDS

process = cms.Process("H2TAUTAU")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5000) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

# -1 : process all files
numberOfFilesToProcess = 10

debugEventContent = False

#tau-mu, tau-ele, di-tau, all
channel = 'tau-mu' # 'tau-mu' #'di-tau' 'all' 'tau-ele'
jetRecalib = False
useCHS = False
#newSVFit enables the svfit mass reconstruction used for the H->tau tau analysis.
# if false, much faster processing but mass is wrong. 
newSVFit = True
tauScaling = 0
applyESCorr = True
# increase to 1000 before running on the batch, to reduce size of log files
# on your account
reportInterval = 1000

print sep_line
print 'channel', channel
print 'jet recalib', jetRecalib
print 'useCHS', useCHS
print 'newSVFit', newSVFit
print 'tau scaling =', tauScaling

##########



# Input  & JSON             -------------------------------------------------


dataset_user = 'cmgtools'


# dataset_name = '/VBF_HToTauTau_M-125_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
# dataset_name = '/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_14_0'
# dataset_name = '/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_16_0'
# dataset_name = '/TauPlusX/Run2012A-22Jan2013-v1/AOD/PAT_CMG_V5_16_0'
# dataset_name = '/DoubleMu/StoreResults-Run2012A_22Jan2013_v1_RHembedded_trans1_tau115_ptelec1_20had1_18_v1-f456bdbb960236e5c696adfe9b04eaae/USER/V5_B/PAT_CMG_V5_16_0'


# /GluGluToHToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_14_0

#dataset_user = 'cmgtools_group'
#dataset_name = '/WH_ZH_TTH_HToTauTau_M-125_8TeV-pythia6-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_14_0'

#dataset_name = '/DoubleMuParked/StoreResults-Run2012D_22Jan2013_v1_PFembedded_trans1_tau116_ptmu1_16had1_18_v1-5ef1c0fd428eb740081f19333520fdc8/USER/V5_B/PAT_CMG_V5_16_0'
dataset_name = '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_16_0'

#dataset_name = '/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_16_0'

dataset_user = 'steggema'
# dataset_name = '/Radion_hh_tautaubb_LR3tevLHC8_glufusion_MR300_GENSIM_19082013/caber-Radion_hh_tautaubb_LR3tevLHC8_glufusion_MR300_20082013_AODSIM-c8f8ed334db8a7d6f56c62266b1dfa5b/USER/PAT_CMG_V5_16_0'
dataset_name = '/AbelianZprime_hh_TauTauBB_LHC8_M700_GENSIM_29082012/caber-Radion_hh_TauTauBB_LHC8_M700_31082013_AODSIM-c8f8ed334db8a7d6f56c62266b1dfa5b/USER/PAT_CMG_V5_16_0'

dataset_files = 'cmgTuple.*root'

# creating the source
from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    dataset_user,
    dataset_name,
    dataset_files,
    )

process.source.inputCommands=cms.untracked.vstring(
    'keep *',
    'drop cmgStructuredPFJets_cmgStructuredPFJetSel__PAT'
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
    from CMGTools.H2TauTau.tools.setupJSON import setupJSON
    json = setupJSON(process)


# load the channel paths -------------------------------------------
process.load('CMGTools.H2TauTau.h2TauTau_cff')

# setting up the recoil correction according to the input file ---------------

print sep_line
from CMGTools.H2TauTau.tools.setupRecoilCorrection import setupRecoilCorrection

recoilEnabled = True
setupRecoilCorrection( process, runOnMC,
                       enable=recoilEnabled, is53X=isNewerThan('CMSSW_5_2_X'))


# Kinematic reweighting for the embedded samples from here https://twiki.cern.ch/twiki/bin/viewauth/CMS/MuonTauReplacementRecHit
# Can also put this into a separate file under tools

isEmbedded = process.source.fileNames[0].find('embedded') != -1

isRHEmbedded = process.source.fileNames[0].find('RHembedded') != -1

if isEmbedded and isRHEmbedded:
    process.load('TauAnalysis.MCEmbeddingTools.embeddingKineReweight_cff')

    if channel == 'all':
        print 'ERROR: not possible to run all the channels for the embedded samples right now'

    # for "standard" e+tau channel
    if channel == 'tau-ele':
        process.embeddingKineReweightRECembedding.inputFileName = cms.FileInPath("TauAnalysis/MCEmbeddingTools/data/embeddingKineReweight_ePtGt20tauPtGt18_recEmbedded.root")
        process.tauElePath.insert(-1, process.embeddingKineReweightSequenceRECembedding)

    # for e+tau channel of "soft lepton" analysis
    #embeddingKineReweightRECembedding.inputFileName = cms.FileInPath("TauAnalysis/MCEmbeddingTools/data/embeddingKineReweight_ePt9to30tauPtGt18_recEmbedded.root")

    # for "standard" mu+tau channel
    if channel == 'tau-mu':
        process.embeddingKineReweightRECembedding.inputFileName = cms.FileInPath("TauAnalysis/MCEmbeddingTools/data/embeddingKineReweight_muPtGt16tauPtGt18_recEmbedded.root")
        process.tauMuPath.insert(-1, process.embeddingKineReweightSequenceRECembedding)

    # for mu+tau channel of "soft lepton" analysis
    #embeddingKineReweightRECembedding.inputFileName = cms.FileInPath("TauAnalysis/MCEmbeddingTools/data/embeddingKineReweight_muPt7to25tauPtGt18_recEmbedded.root")

    # for tautau channel
    if channel == 'di-tau':
        process.embeddingKineReweightRECembedding.inputFileName = cms.FileInPath("TauAnalysis/MCEmbeddingTools/data/embeddingKineReweight_tautau_recEmbedded.root")
        process.diTauPath.insert(-1, process.embeddingKineReweightSequenceRECembedding)

    print "Embedded samples; using kinematic reweighting file:", process.embeddingKineReweightRECembedding.inputFileName

    # for emu, mumu and ee channels
    #embeddingKineReweightRECembedding.inputFileName = cms.FileInPath("TauAnalysis/MCEmbeddingTools/data/embeddingKineReweight_recEmbedding_emu.root")


# OUTPUT definition ----------------------------------------------------------
process.outpath = cms.EndPath()

# don't apply Tau ES corrections for data (but do for embedded) or processes not containing real taus

signalTauProcess = (process.source.fileNames[0].find('HToTauTau') != -1) or (process.source.fileNames[0].find('DY') != -1) or isEmbedded

# UPDATE: Only apply Tau ES corrections for embedded
# signalTauProcess = isEmbedded

# if not runOnMC and not isEmbedded:
if not signalTauProcess or not applyESCorr:
    print 'Not applying tau ES corrections as no process with real simulated taus or tau ES switched off'
    process.tauMuPath.remove( process.cmgTauMuCor )
    process.cmgTauMuTauPtSel.src = cms.InputTag('mvaMETTauMu')
    # process.diTauPath.remove( process.cmgDiTauCorPreSel )
    process.mvaMETDiTau.src = cms.InputTag('cmgDiTauPreSel')
    process.tauElePath.remove( process.cmgTauEleCor )
    process.cmgTauEleTauPtSel.src = cms.InputTag('mvaMETTauEle')
else:
    print 'Apply tau ES corrections'

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
# process.cmgTauMuCorSVFitPreSel.verbose = True
# process.mvaMETTauMu.verbose = True
# process.recoilCorMETTauMu.verbose= True

# systematic shift on tau energy scale 
# process.cmgTauScaler.cfg.nSigma = tauScaling

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
process.MessageLogger.cerr.FwkReport.reportEvery = reportInterval
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
    process.cmgDiTauCorSVFitPreSel.SVFitVersion = 2
else:
    process.cmgTauMuCorSVFitPreSel.SVFitVersion = 1
    process.cmgTauEleCorSVFitPreSel.SVFitVersion = 1
    process.cmgDiTauCorSVFitPreSel.SVFitVersion = 1

# process.tauMu_fullsel_tree_CMG.SelectEvents = cms.untracked.PSet()

# process.cmgTauMu.cuts.baseline.tauLeg.iso = cms.string('leg1().tauID("byRawIsoMVA") > 0.5')
process.cmgTauMu.cuts.baseline.tauLeg.iso = cms.string('leg1().tauID("byRawIsoMVA") > -9999')
process.cmgTauMu.cuts.baseline.tauLeg.iso = cms.string('leg1().tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") < 10.')
if 'Higgs' in dataset_name or 'HToTauTau' in dataset_name or isEmbedded:
    process.cmgTauMu.cuts.baseline.tauLeg.iso = cms.string('leg1().tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") < 1.5')
process.cmgTauMu.cuts.baseline.tauLeg.kinematics.pt = cms.string('leg1().pt() > 10.')
