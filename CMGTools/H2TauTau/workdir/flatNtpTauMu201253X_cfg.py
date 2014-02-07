import FWCore.ParameterSet.Config as cms
import os

process = cms.Process("FLATNTP")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) ) 
process.maxLuminosityBlocks = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
evReportFreq = 1000

#######Define the samples to process

dataset_user  = os.environ['SAMPLEOWNER']
sampleName = os.environ['SAMPLENAME']
sampleJobIdx = int(os.environ['SAMPLEJOBIDX'])
sampleMergeFactor = int(os.environ['SAMPLEMERGEFACTOR'])


####---->
#dataset_user  = 'cmgtools'
#sampleName = 'HiggsVBF125'
#sampleName = 'TauPlusX2012Cv1'
#sampleName = 'Embedded2012Cv1'
#sampleJobIdx = 0
#sampleMergeFactor = 200

#########################

process.analysis = cms.Path() 


######The analysis module
process.load('CMGTools.H2TauTau.tools.joseFlatNtpSample_cfi')
process.flatNtp = process.flatNtpTauMu.clone()
from CMGTools.H2TauTau.tools.joseFlatNtpSample53X_cff import configureFlatNtpSampleTauMu2012
configureFlatNtpSampleTauMu2012(process.flatNtp,sampleName)
process.flatNtp.metType = 2 #1 PFMET, 2 mva met, 3 mva met presel
process.flatNtp.runSVFit = 2 #1 old #2 new
#process.flatNtp.recoilCorrection = 0 #0 no, 1 Z, 2 W


### input files
#inputfiles = "tauMu_fullsel_tree_CMG_.*root"
inputfiles = "cmgTuple.*root"
dataset_name = process.flatNtp.path.value()
firstfile = sampleJobIdx * sampleMergeFactor
lastfile = (sampleJobIdx + 1 ) * sampleMergeFactor
print dataset_user
print dataset_name
print inputfiles
print firstfile
print lastfile

#get input files
from CMGTools.Production.datasetToSource import *
process.source = datasetToSource( dataset_user, dataset_name, inputfiles)
process.source.fileNames = process.source.fileNames[firstfile:lastfile]



##diff in di-muon veto
#process.source.fileNames = ['/store/cmst3/user/cmgtools/CMG/TauPlusX/Run2012C-24Aug2012-v1/AOD/V5_B/PAT_CMG_V5_14_0/cmgTuple_233.root']
#process.source.eventsToProcess = cms.untracked.VEventRange('198230:123724743')




#process.source = cms.Source(
#    "PoolSource",
#    fileNames = cms.untracked.vstring(
#    #'/store/cmst3/user/cmgtools/CMG/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_2_0/cmgTuple_0.root'
#    #'/store/cmst3/user/cmgtools/CMG/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1/cmgTuple_0.root'
#    #'file:../../../Common/prod/TEST/cmgTuple_HToTauTau.root'
#    'file:./tauMu_fullsel_tree_CMG.root'
#    )
#    )

#process.source.fileNames = ['file:./tauMu_fullsel_tree_CMG.root']



#print process.source.fileNames
#print process.source.eventsToProcess
#process.flatNtp.printSelectionPass = 2


# set up JSON ---------------------------------------------------------------
if process.flatNtp.dataType != 0 :
   json = process.flatNtp.jsonfile.value()
   print 'json:', json
   from CMGTools.Common.Tools.applyJSON_cff import applyJSON
   applyJSON(process, json )
   print process.PoolSource.lumisToProcess

## run the vertex weights
if process.flatNtp.dataType == 0:
   process.load('CMGTools.RootTools.utils.vertexWeight.vertexWeight_cff')
   process.genSequence = cms.Sequence(
      process.vertexWeightSequence 
      )
   process.analysis += process.genSequence 


###kinematic weights for embedded samples
if process.flatNtp.embeddedKinWeightFile.value() != '' :
   process.load('TauAnalysis/MCEmbeddingTools/embeddingKineReweight_cff')
   process.embeddingKineReweightRECembedding.inputFileName = cms.FileInPath(process.flatNtp.embeddedKinWeightFile.value())
   process.analysis += process.embeddingKineReweightSequenceRECembedding

##create the good primary vertices
process.load("CommonTools.ParticleFlow.goodOfflinePrimaryVertices_cfi")
process.analysis += process.goodOfflinePrimaryVertices


###Apply Tau ES corrections
process.load('CMGTools.Utilities.tools.cmgTauESCorrector_cfi')
process.analysis +=  process.cmgTauESCorrector
if process.flatNtp.correctTauES != 1:
   process.cmgTauESCorrector.cfg.OneProngNoPi0Correction = 1.000
   process.cmgTauESCorrector.cfg.OneProng1Pi0Correction = 1.000
   process.cmgTauESCorrector.cfg.OneProng1Pi0CorrectionPtSlope = 0.0
   process.cmgTauESCorrector.cfg.ThreeProngCorrection = 1.000
   process.cmgTauESCorrector.cfg.ThreeProngCorrectionPtSlope = 0.0
   

###apply Tau ES shifts
process.load('CMGTools.Common.factories.cmgTauScaler_cfi')
process.analysis +=  process.cmgTauScaler
process.cmgTauScaler.cfg.inputCollection = 'cmgTauESCorrector'
#process.cmgTauScaler.cfg.uncertainty = 0.03
#process.cmgTauScaler.cfg.nSigma = 1.0



##create mu-tau candidates
process.load('CMGTools.Common.factories.cmgTauMu_cfi')
process.cmgTauMu.cfg.leg1Collection = 'cmgTauScaler'
process.cmgTauMu.cfg.metCollection = 'cmgPFMETRaw'
process.analysis +=  process.cmgTauMu


###MVA MET

if process.flatNtp.metType ==2 :
   #need to use uncorrected Tau for MVA MET computation
   process.cmgTauMuMVAMET = process.cmgTauMu.clone()
   process.cmgTauMuMVAMET.cfg.leg1Collection = 'cmgTauSel'
   process.load("CMGTools.Common.eventCleaning.goodPVFilter_cfi")
   process.load("CMGTools.Utilities.mvaMET.mvaMETTauMu_cfi")
   process.mvaMETTauMu.recBosonSrc = 'cmgTauMuMVAMET'
   process.load("CMGTools.Common.factories.cmgBaseMETFromPFMET_cfi")
   process.mvaBaseMETTauMu = process.cmgBaseMETFromPFMET.clone()
   process.mvaBaseMETTauMu.cfg.inputCollection = 'mvaMETTauMu'
   process.mvaMETSequence = cms.Sequence(
      process.cmgTauMuMVAMET + #need to use the uncorrected Tau and muon
      process.goodPVFilter + #quality vertices
      process.mvaMETTauMu + #reco::PFMET object and cmg::PFMetSignificance object
      process.mvaBaseMETTauMu # cmg::BaseMET object
      )
   process.analysis  += process.mvaMETSequence
   #process.mvaMETTauMu.verbose = True
   
if process.flatNtp.metType == 3 :
   process.load("CMGTools.Utilities.mvaMET.mvaMETPreselLep_cff")
   process.analysis  += process.mvaMETPreselLepSequence
   process.flatNtp.mvaMETTag = 'cmgMvaMETPreselLep'
   process.flatNtp.mvaMETSigTag = 'mvaMETPreselLep'
   #process.mvaMETPreselLep.verbose = True

   

##schedule the analyzer
process.analysis += process.flatNtp
process.schedule = cms.Schedule(process.analysis)
process.TFileService = cms.Service("TFileService", fileName = cms.string("flatNtp.root"))


#####################################################
process.MessageLogger = cms.Service("MessageLogger",
    cerr = cms.untracked.PSet(
    FwkReport = cms.untracked.PSet(
    reportEvery = cms.untracked.int32(evReportFreq),
    optionalPSet = cms.untracked.bool(True),
    limit = cms.untracked.int32(10000000)
    ),
    optionalPSet = cms.untracked.bool(True),
    FwkJob = cms.untracked.PSet(
    optionalPSet = cms.untracked.bool(True),
    limit = cms.untracked.int32(0)
    ),    
    )
)
