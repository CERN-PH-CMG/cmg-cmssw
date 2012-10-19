import FWCore.ParameterSet.Config as cms

from CMGTools.Common.Tools.cmsswRelease import cmsswIs44X,cmsswIs52X

sep_line = '-'*70

########## CONTROL CARDS

process = cms.Process("H2TAUTAU")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

# -1 : process all files
numberOfFilesToProcess = 100

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


dataset_user = 'cbern' 
# dataset_name = '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_V5_4_0'
# dataset_name = '/H2TAUTAU/Sync/GluGlu/AOD/PAT_CMG_V5_5_0'
# dataset_name = '/H2TAUTAU/Sync/2012/VBF/AOD/PAT_CMG_V5_5_1'
# dataset_name = '/TauPlusX/Run2012C-PromptReco-v2/AOD/PAT_CMG_V5_5_1_runrange_start-200601'
# dataset_name = '/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/PAT_CMG_V5_6_0_B'
# dataset_name = '/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
# dataset_name = '/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V9-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
# dataset_name = '/VBF_HToTauTau_M-125_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_5_1'

# dataset_name = '/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
# dataset_name = '/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
# dataset_name = '/DoubleMu/Run2011B-16Jan2012-v1/AOD/V5/PAT_CMG_V5_6_0_B'
# dataset_name = '/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B'
# dataset_name = '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0'
dataset_name = '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0/DIMU_Colin17Oct'
# dataset_name = '/TauPlusX/Run2012C-PromptReco-v2/AOD/PAT_CMG_V5_8_0'

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
    from CMGTools.H2TauTau.tools.setupJSON import setupJSON
    json = setupJSON(process)



# load the channel paths -------------------------------------------
process.load('CMGTools.H2TauTau.h2TauTau_cff')

# setting up the recoil correction according to the input file ---------------

print sep_line
from CMGTools.H2TauTau.tools.setupRecoilCorrection import setupRecoilCorrection
# WARNING DISABLING RECOIL CORRECTIONS FOR 2012!!!
setupRecoilCorrection( process, runOnMC, True, cmsswIs52X(), 'WJetsToLNu')

# OUTPUT definition ----------------------------------------------------------
process.outpath = cms.EndPath()

# generator ----------------------------------------------
if not runOnMC:
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



## process.pfMetForRegressionCor = cms.EDProducer(
##     'PFMetModificationProducer',
##     candSrc = cms.InputTag('cmgMuonSel:removed'),
##     metSrc = cms.InputTag('pfMetForRegression'),
##     operator = cms.string('-')
##     )
## process.pfMetForRegression = cms.EDProducer(
##     'PFMetModificationProducer',
##     candSrc = cms.InputTag('cmgMuonSel:correction'),
##     metSrc = cms.InputTag('pfMetForRegressionCor'),
##     operator = cms.string('-')
##     )

## process.nopuMetCor = process.pfMetForRegressionCor.clone(metSrc='nopuMet')
## process.nopuMet = process.pfMetForRegression.clone(metSrc='nopuMetCor')
## process.pcMetCor = process.pfMetForRegressionCor.clone(metSrc='pcMet')
## process.pcMet = process.pfMetForRegression.clone(metSrc='pcMetCor')
## process.tkMetCor = process.pfMetForRegressionCor.clone(metSrc='tkMet')
## process.tkMet = process.pfMetForRegression.clone(metSrc='tkMetCor')


## process.pfMetModificationSequence = cms.Sequence(
##     process.pfMetForRegressionCor +
##     process.pfMetForRegression +
##     process.nopuMetCor +
##     process.nopuMet +
##     process.pcMetCor +
##     process.pcMet +
##     process.tkMetCor +
##     process.tkMet
##     )



process.pfMetForRegression = cms.EDProducer(
    'PFMetModificationProducer',
    candSrc = cms.InputTag('cmgMuonSel:removed'),
    metSrc = cms.InputTag('pfMetForRegression'),
    operator = cms.string('-')
    )

process.nopuMet = process.pfMetForRegression.clone(metSrc='nopuMet')
process.pcMet = process.pfMetForRegression.clone(metSrc='pcMet')
process.tkMet = process.pfMetForRegression.clone(metSrc='tkMet')


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


if 1: 
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
process.MessageLogger.cerr.FwkReport.reportEvery = 100
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

process.cmgTauMu.cuts.baseline.tauLeg.iso = cms.string('leg1().tauID("byRawIsoMVA") > -9999')


