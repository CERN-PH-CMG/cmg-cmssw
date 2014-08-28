import FWCore.ParameterSet.Config as cms
from CMGTools.Common.Tools.cmsswRelease import isNewerThan, cmsswIs44X

sep_line = '-'*70

process = cms.Process("H2TAUTAU")

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))

numberOfFilesToProcess = -1
debugEventContent = False

# tau-mu, tau-ele, di-tau, all
channel = 'tau-mu' # 'tau-mu' #'di-tau' 'all' 'tau-ele'
jetRecalib = False
useCHS = False

# newSVFit enables the svfit mass reconstruction used for the H->tau tau analysis.
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

# Input & JSON             -------------------------------------------------

dataset_user = 'htautau_group' 
dataset_name = '/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/Spring14dr-PU20bx25_POSTLS170_V5-v1/AODSIM/SS14/'
dataset_files = 'miniAOD-prod_PAT_.*root'

from CMGTools.Production.datasetToSource import datasetToSource
process.source = datasetToSource(
    dataset_user,
    dataset_name,
    dataset_files,
    )

process.source.inputCommands=cms.untracked.vstring(
    'keep *'
    )

process.options = cms.untracked.PSet(
        allowUnscheduled = cms.untracked.bool(True)
)

if numberOfFilesToProcess>0:
    process.source.fileNames = process.source.fileNames[:numberOfFilesToProcess]

runOnMC = process.source.fileNames[0].find('Run201')==-1 and process.source.fileNames[0].find('embedded')==-1

if runOnMC == False:
    from CMGTools.H2TauTau.tools.setupJSON import setupJSON
    json = setupJSON(process)


# load the channel paths -------------------------------------------
process.load('CMGTools.H2TauTau.h2TauTau_cff')

# setting up the recoil correction according to the input file ---------------
print sep_line
from CMGTools.H2TauTau.tools.setupRecoilCorrection import setupRecoilCorrection

recoilEnabled = False
setupRecoilCorrection( process, runOnMC,
                       enable=recoilEnabled, is53X=isNewerThan('CMSSW_5_2_X'))

from CMGTools.H2TauTau.tools.setupEmbedding import setupEmbedding

isEmbedded = setupEmbedding(process, channel)

# Adding jet collection
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'PLS170_V7AN1::All'

process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load('TrackingTools.TransientTrack.TransientTrackBuilder_cfi')
process.load('RecoBTag.Configuration.RecoBTag_cff')

from CMGTools.H2TauTau.objects.jetreco_cff import addAK4Jets

addAK4Jets(process)
process.tauMuPath.insert(0, process.jetSequenceAK4)

# OUTPUT definition ----------------------------------------------------------
process.outpath = cms.EndPath()

# don't apply Tau ES corrections for data (but do for embedded) or processes not containing real taus

signalTauProcess = (process.source.fileNames[0].find('HToTauTau') != -1) or (process.source.fileNames[0].find('DY') != -1) or isEmbedded

# UPDATE: Only apply Tau ES corrections for embedded
# signalTauProcess = isEmbedded

# if not runOnMC and not isEmbedded:

#import pdb; pdb.set_trace(); #pdb.tauMuPath

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


process.tauMuPath.remove(process.cmgPFJetForRecoilPresel)
process.tauMuPath.remove(process.cmgPFJetForRecoil)

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
print 'OUTPUT:'
print sep_line
justn = 30 
# print 'baseline selection EDM output'.ljust(justn), baselineName
# print 'basic selection EDM output'.ljust(justn), basicName
# print 'histograms output'.ljust(justn), histName 
# print 'Debug event content'.ljust(justn), debugEventContent

### Enable printouts like this:
# process.cmgTauMuCorSVFitPreSel.verbose = True
# process.mvaMETTauMu.verbose = True
# process.recoilCorMETTauMu.verbose= True

# systematic shift on tau energy scale 
# process.cmgTauScaler.cfg.nSigma = tauScaling

from CMGTools.H2TauTau.tools.setupOutput import addTauMuOutput, addTauEleOutput, addDiTauOutput
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


# # Jet recalibration
# if jetRecalib: 
#     process.load('Configuration.StandardSequences.Services_cff')
#     process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#     GT = None
#     if runOnMC:
#         if cmsswIs44X():
#             GT = 'START44_V13::All'
#         else:
#             GT = 'START52_V10::All'
#     else:
#         if cmsswIs44X():
#             GT = 'GR_R_44_V15::All'
#         else:
#             GT = 'GR_R_52_V8::All'  
#     process.GlobalTag.globaltag = GT
#     from CMGTools.Common.miscProducers.cmgPFJetCorrector_cfi import cmgPFJetCorrector
#     process.cmgPFJetSel = cmgPFJetCorrector.clone(src='cmgPFJetSel',
#                                                   payload='AK5PF')
#     process.cmgPFJetSelCHS = cmgPFJetCorrector.clone(src='cmgPFJetSelCHS',
#                                                      payload='AK5PFchs')

#     if runOnMC:
#         process.cmgPFJetSel.levels = cms.vstring('L1FastJet','L2Relative','L3Absolute')
#         process.cmgPFJetSelCHS.levels = cms.vstring('L1FastJet','L2Relative','L3Absolute')
#     else:
#         process.cmgPFJetSel.levels = cms.vstring('L1FastJet','L2Relative','L3Absolute','L2L3Residual')
#         process.cmgPFJetSelCHS.levels = cms.vstring('L1FastJet','L2Relative','L3Absolute','L2L3Residual')

#     process.tauMuPath.insert(0, process.cmgPFJetSel)
#     process.tauElePath.insert(0, process.cmgPFJetSel)
#     process.diTauPath.insert(0, process.cmgPFJetSel)

#     process.tauMuPath.insert(0, process.cmgPFJetSelCHS)
#     process.tauElePath.insert(0, process.cmgPFJetSelCHS)
#     process.diTauPath.insert(0, process.cmgPFJetSelCHS)

#     print sep_line
#     print 'Jet recalibration with GLOBAL TAG', GT

# if useCHS:
#     process.cmgPFJetForRecoil.src = 'cmgPFJetSelCHS'

if newSVFit:
    process.cmgTauMuCorSVFitPreSel.SVFitVersion = 2
    process.cmgTauEleCorSVFitPreSel.SVFitVersion = 2
    process.cmgDiTauCorSVFitPreSel.SVFitVersion = 2
else:
    process.cmgTauMuCorSVFitPreSel.SVFitVersion = 1
    process.cmgTauEleCorSVFitPreSel.SVFitVersion = 1
    process.cmgDiTauCorSVFitPreSel.SVFitVersion = 1

