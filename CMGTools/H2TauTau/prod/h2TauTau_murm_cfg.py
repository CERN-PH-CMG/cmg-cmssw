import FWCore.ParameterSet.Config as cms
import pprint

from CMGTools.Common.Tools.cmsswRelease import isNewerThan, cmsswIs44X,cmsswIs52X

sep_line = '-'*70

########## CONTROL CARDS

process = cms.Process("H2TAUTAU")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

# -1 : process all files
numberOfFilesToProcess = -1

debugEventContent = False

#tau-mu, tau-ele, di-tau, all
channel    = 'tau-mu' # 'tau-mu' #'di-tau' 'all' 'tau-ele'
jetRecalib = False
useCHS     = False 
newSVFit   = False
tauScaling = 0
reportInterval = 500

print sep_line
print 'channel'      , channel
print 'jet recalib'  , jetRecalib
print 'useCHS'       , useCHS
print 'newSVFit'     , newSVFit
print 'tau scaling =', tauScaling

##########



# Input  & JSON             -------------------------------------------------
# dataset_user  = 'cmgtools_group' 
# dataset_name  = '/GluGluToHToTauTau_M-120_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_V5_16_0'

# dataset_user  = 'cmgtools'
# dataset_name  = '/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_16_0'

#dataset_user  = 'cmgtools' 
#dataset_name  = '/TauParked/Run2012D-22Jan2013-v1/AOD/PAT_CMG_V5_16_0'
dataset_files = 'cmgTuple.*root'

dataset_user = 'cbern'
dataset_name = '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_16_0/DIMU_Colin_17Jun2013'
# dataset_name = '/TauPlusX/Run2012D-22Jan2013-v1/AOD/PAT_CMG_V5_16_0/DIMU_Colin_17Jun2013'

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
                       enable=recoilEnabled,
                       is53X=isNewerThan('CMSSW_5_2_X'),
                       channel = channel,
                       mode='WJetsToLNu')


# Kinematic reweighting for the embedded samples from here https://twiki.cern.ch/twiki/bin/viewauth/CMS/MuonTauReplacementRecHit
# Can also put this into a separate file under tools

isEmbedded = process.source.fileNames[0].find('embedded') != -1

if isEmbedded:
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

# generator ----------------------------------------------
if not runOnMC :
    if not isEmbedded :
      process.diTauPath.remove( process.cmgDiTauCor )          
      process.cmgDiTauPtSel.src = cms.InputTag('mvaMETDiTau')   
      process.tauMuPath.remove( process.cmgTauMuCor )                   ## boh
      process.cmgTauMuTauPtSel.src = cms.InputTag('mvaMETTauMu')        ## boh    
      process.tauElePath.remove( process.cmgTauEleCor )                 ## boh
      process.cmgTauEleTauPtSel.src = cms.InputTag('mvaMETTauEle')      ## boh

    process.tauMuPath.remove( process.genSequence )
    process.tauElePath.remove( process.genSequence )
    process.diTauPath.remove( process.genSequence )


# muon removal sequence
# ----

# need to clean that up a bit...
# mu faking tau small, but we could think of removing tau candidates corresponding to the removed mu

process.cmgMuonSel = cms.EDFilter(
    "MuonRemover",
    leptonSrc = cms.InputTag('cmgMuonSel'),
    verbose = cms.untracked.bool( False )
    )

# need to remove the leading jet corresponding to the removed muon

process.cmgPFJetSel =  cms.EDProducer(
    "DeltaRVetoProducerPFJet",
    inputCollection = cms.InputTag('cmgPFJetSel'),
    MatchingParams = cms.VPSet(
    cms.PSet(                                     
       vetoCollection=cms.InputTag("cmgMuonSel:removed"),
       minDeltaR=cms.double(0.3),
       removeMatchedObject=cms.bool(True)
       )
    ),
    verbose = cms.untracked.bool(False)
    )

process.cmgTauSel =  cms.EDProducer(
    "DeltaRVetoProducerTau",
    inputCollection = cms.InputTag('cmgTauSel'),
    MatchingParams = cms.VPSet(
    cms.PSet(                                     
       vetoCollection=cms.InputTag("cmgMuonSel:removed"),
       minDeltaR=cms.double(0.3),
       removeMatchedObject=cms.bool(True)
       )
    ),
    verbose = cms.untracked.bool(False)
    )

process.pfMetForRegression = cms.EDProducer(
    'PFMetModificationProducer',
    candSrc = cms.InputTag('cmgMuonSel:removed'),
    metSrc = cms.InputTag('pfMetForRegression'),
    operator = cms.string('-')
    )

process.nopuMet = process.pfMetForRegression.clone(metSrc='nopuMet')
process.pcMet = process.pfMetForRegression.clone(metSrc='pcMet')
process.tkMet = process.pfMetForRegression.clone(metSrc='tkMet')


process.mvaMETTauMu.leadJetSrc = 'cmgPFJetSel'

process.pfMetModificationSequence = cms.Sequence(
    process.pfMetForRegression +
    process.nopuMet +
    process.pcMet +
    process.tkMet
    )

process.muRmSequence = cms.Sequence(
    process.cmgMuonSel +
    process.cmgPFJetSel +
    process.cmgTauSel + 
    process.pfMetModificationSequence    
    ) 

process.tauMuPath.insert(0, process.muRmSequence) 


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
pprint.pprint( process.source.fileNames[:5] )
print '...'
print 'total number of files =', len(process.source.fileNames) 
print
if runOnMC==False:
    print 'json:', json
print
print sep_line
print 'PROCESSING'
print sep_line
print 'runOnMC:', runOnMC
print 'isEmbedded:', isEmbedded
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
process.cmgDiTauCor.cfg.nSigma = tauScaling

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
# process.cmgTauMu.cuts.baseline.tauLeg.iso = cms.string('leg1().tauID("byRawIsoMVA") > -9999')
# process.cmgTauMu.cuts.baseline.tauLeg.iso = cms.string('leg1().tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") < 10.')

process.cmgTauMu.cuts.baseline.tauLeg.kinematics.pt = cms.string('leg1().pt() > 30.')


# process.cmgDiTau.cuts.baseline.tau1Leg.iso = cms.string('leg1().tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") < 10.')
# process.cmgDiTau.cuts.baseline.tau2Leg.iso = cms.string('leg2().tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") < 10.')
# process.cmgDiTau.cuts.baseline.tau1Leg.kinematics.pt  = cms.string('leg1().pt()>35.')
# process.cmgDiTau.cuts.baseline.tau2Leg.kinematics.pt  = cms.string('leg2().pt()>35.')
# process.cmgDiTau.cuts.baseline.tau1Leg.kinematics.eta = cms.string('abs(leg1().eta())<2.1')
# process.cmgDiTau.cuts.baseline.tau2Leg.kinematics.eta = cms.string('abs(leg2().eta())<2.1')





