import FWCore.ParameterSet.Config as cms
import os

process = cms.Process("FLATNTP")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) ) #---->
process.maxLuminosityBlocks = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
evReportFreq = 1

#######Define the samples to process
#dataset_user = 'benitezj'
#sampleTag = "/PAT_CMG_V5_4_1/H2TAUTAU_TauMu_V541June2"
#sampleTag = "/PAT_CMG_V5_4_0_NewType1MET/H2TAUTAU_TauMu2012_V5_4_0_NewType1MET"

#for data and MC samples
#sampleTag = "/H2TAUTAU_TauMu2012_V5_4_0_NewType1MET"

#for embedded samples
#sampleTag = "/TauMu2012V540"

#dataset_user = 'cmgtools'
sampleTag = ""


dataset_user  = os.environ['SAMPLEOWNER']
sampleName = os.environ['SAMPLENAME']
sampleJobIdx = int(os.environ['SAMPLEJOBIDX'])
sampleMergeFactor = int(os.environ['SAMPLEMERGEFACTOR'])


#####---->
#dataset_user  = 'benitezj'
#sampleName = 'HiggsVBF125'
#sampleJobIdx = 0
#sampleMergeFactor = 200


#########################

process.analysis = cms.Path() 


######The analysis module
process.load('CMGTools.H2TauTau.tools.joseFlatNtpSample_cfi')
process.flatNtp = process.flatNtpTauMu.clone()
from CMGTools.H2TauTau.tools.joseFlatNtpSample53X_cff import configureFlatNtpSampleTauMu2012
configureFlatNtpSampleTauMu2012(process.flatNtp,sampleName)
process.flatNtp.metType = 2
process.flatNtp.runSVFit = 1 #1 old #2 new
process.flatNtp.recoilCorrection = 0 #0 no, 1 Z, 2 W


### input files
#inputfiles = "tauMu_fullsel_tree_CMG_.*root"
inputfiles = "cmgTuple.*root"
dataset_name = process.flatNtp.path.value() + sampleTag
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


##difference in trigger
#process.source.eventsToProcess = cms.untracked.VEventRange('1:253983', '1:64131')
##isolation difference 

#process.source.eventsToProcess = cms.untracked.VEventRange('1:105103','1:258011','1:385416','1:579732','1:750487','1:844314','1:860080','1:887035')
#process.source.eventsToProcess = cms.untracked.VEventRange('1:105103')
#process.source.fileNames =  process.source.fileNames[90:99]

##issue with mu-Tau pair removed
#process.source.eventsToProcess = cms.untracked.VEventRange('1:385416')
#process.source.fileNames =  process.source.fileNames[65:70]
#process.flatNtp.diTauTag = 'cmgTauMu'
#process.flatNtp.metType = 2

##issues with muon iso
#process.source.eventsToProcess = cms.untracked.VEventRange('1:258011', '1:579732', '1:750487', '1:844314', '1:860080', '1:887035')
#process.source.eventsToProcess = cms.untracked.VEventRange('1:258011')
#process.source.eventsToProcess = cms.untracked.VEventRange('1:579732')
#process.source.eventsToProcess = cms.untracked.VEventRange('1:750487')
#process.source.eventsToProcess = cms.untracked.VEventRange('1:844314')
#process.source.eventsToProcess = cms.untracked.VEventRange('1:860080')
#process.source.eventsToProcess = cms.untracked.VEventRange('1:887035')


#---->
###MVA MET sync 
#process.source.fileNames = ['/store/cmst3/user/benitezj/CMG/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0_BleedingEdge_MET53X_Mar27/cmgTuple_94.root']
#process.source.eventsToProcess = cms.untracked.VEventRange('1:122718')
#process.flatNtp.printSelectionPass=1

#print process.source.eventsToProcess


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

print process.source.fileNames


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
   

##create the good primary vertices
process.load("CommonTools.ParticleFlow.goodOfflinePrimaryVertices_cfi")
process.analysis += process.goodOfflinePrimaryVertices



###Apply Tau ES corrections
process.load('CMGTools.Utilities.tools.cmgTauESCorrector_cfi')
process.analysis +=  process.cmgTauESCorrector
if process.flatNtp.correctTauES == 1:
   process.cmgTauESCorrector.cfg.OneProngNoPi0Correction = 1.000
   process.cmgTauESCorrector.cfg.OneProng1Pi0Correction = 1.015
   process.cmgTauESCorrector.cfg.OneProng1Pi0CorrectionPtSlope = 0.001
   process.cmgTauESCorrector.cfg.ThreeProngCorrection = 1.012
   process.cmgTauESCorrector.cfg.ThreeProngCorrectionPtSlope = 0.001
   

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

# event filter --------------------------------
process.load('CMGTools.Common.skims.cmgTauMuCount_cfi')
process.cmgTauMuCount.src = 'cmgTauMu'
process.cmgTauMuCount.minNumber = 1
process.analysis += process.cmgTauMuCount



###MVA MET
if process.flatNtp.metType == 3 :
   process.load("CMGTools.Common.eventCleaning.goodPVFilter_cfi")
   process.load("CMGTools.Utilities.mvaMET.mvaMETPreselLep_cfi")
   process.mvaMETSequence = cms.Sequence(
      process.goodPVFilter + 
      process.mvaMETPreselLep
      )
   process.analysis  += process.mvaMETSequence
   #process.mvaMETTauMu.verbose = True
   process.cmgTauMuMET = process.cmgTauMu.clone()
   process.cmgTauMuMET.cfg.leg1Collection = 'cmgTauScaler'
   process.cmgTauMuMET.cfg.metCollection = 'mvaMETPreselLep'
   process.analysis +=  process.cmgTauMuMET
   process.flatNtp.diTauTag = 'cmgTauMuMET'

   
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
      process.cmgTauMuMVAMET +
      process.goodPVFilter + 
      process.mvaMETTauMu + 
      process.mvaBaseMETTauMu
      )
   process.analysis  += process.mvaMETSequence
   #process.mvaMETTauMu.verbose = True
   


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

#process.source.duplicateCheckMode = cms.untracked.string("noDuplicateCheck")
#print process.dumpPython()
#process.load('CMGTools.Common.skims.cmgTauMuSel_cfi')
#process.cmgTauMuPreSel = process.cmgTauMuSel.clone()
#process.cmgTauMuPreSel.cut = cms.string('')
#process.cmgTauMuPreSel.cut = cms.string('pt()>0' )
###I think the this code was removing some good candidates:
#process.cmgTauMuPreSel.cut = cms.string('leg1().eta()!=leg2().eta() && leg1().pt()>20.0 && abs(leg1().eta())<2.3 && leg1().tauID("decayModeFinding")>0.5 && leg1().tauID("byRawIsoMVA")>-0.5 && leg2().pt()>20.0 && abs(leg2().eta())<2.1 && leg2().relIso(0.5,1)<0.5' )
#process.cmgTauMuPreSel.cut = cms.string('leg1().pt()>=20.0 && abs(leg1().eta())<=2.3 && leg1().tauID("decayModeFinding")>0.5 && leg1().tauID("byRawIsoMVA")>-0.5 && leg2().pt()>=20.0 && abs(leg2().eta())<=2.1 && leg2().relIso(0.5,1)<0.5' )
#process.analysis +=  process.cmgTauMuPreSel 
