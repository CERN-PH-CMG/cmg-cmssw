import FWCore.ParameterSet.Config as cms
import os

process = cms.Process("FLATNTP")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) ) 
process.maxLuminosityBlocks = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
evReportFreq = 10

#######Define the samples to process

dataset_user  = os.environ['SAMPLEOWNER']
sampleName = os.environ['SAMPLENAME']
sampleJobIdx = int(os.environ['SAMPLEJOBIDX'])
sampleMergeFactor = int(os.environ['SAMPLEMERGEFACTOR'])

#########################

process.analysis = cms.Path() 


######The analysis module
process.load('CMGTools.H2TauTau.tools.joseFlatNtpSample_cfi')
process.flatNtp = process.flatNtpTauEle.clone()
from CMGTools.H2TauTau.tools.joseFlatNtpSample44X_cff import configureFlatNtpSampleTauEle2011
configureFlatNtpSampleTauEle2011(process.flatNtp,sampleName)
process.flatNtp.metType = 2 #1 PFMET, 2 mva met, 3 mva met presel
process.flatNtp.runSVFit = 2 #1 old #2 new
#process.flatNtp.recoilCorrection = 0 #0 no, 1 Z, 2 W


### input files
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


#process.source.eventsToProcess = cms.untracked.VEventRange('170842:198469487','170854:397347639','170876:202308885','171102:17987727','171178:250914388','171178:730863165','171178:840226378','171369:56045961','171484:315839252','171578:224983770','171578:371964042','171578:386518713','171578:39628411','171812:195083180','171897:485384360','171897:493437621','171897:544652294','171921:152592698','171926:142804495','171926:83110914','172014:260483000','172033:338337471','172033:945992289','172163:421118950','172389:358813779','172389:38847333','172389:400761861','172389:451618484','172389:564813150','172400:845917816','172400:85188824','172411:293307393','172411:345235334')

print process.source.fileNames
#print process.source.eventsToProcess
#process.flatNtp.printSelectionPass = 1


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
process.load('CMGTools.Common.factories.cmgTauEle_cfi')
process.cmgTauEle.cfg.leg1Collection = 'cmgTauScaler'
process.cmgTauEle.cfg.metCollection = 'cmgPFMETRaw'
process.analysis +=  process.cmgTauEle

# event filter --------------------------------
process.load('CMGTools.Common.skims.cmgTauEleCount_cfi')
process.cmgTauEleCount.src = 'cmgTauEle'
process.cmgTauEleCount.minNumber = 1
process.analysis += process.cmgTauEleCount



###MVA MET

if process.flatNtp.metType ==2 :
   #need to use uncorrected Tau for MVA MET computation
   process.cmgTauEleMVAMET = process.cmgTauEle.clone()
   process.cmgTauEleMVAMET.cfg.leg1Collection = 'cmgTauSel'
   process.load("CMGTools.Common.eventCleaning.goodPVFilter_cfi")
   process.load("CMGTools.Utilities.mvaMET.mvaMETTauEle_cfi")
   process.mvaMETTauEle.recBosonSrc = 'cmgTauEleMVAMET'
   process.load("CMGTools.Common.factories.cmgBaseMETFromPFMET_cfi")
   process.mvaBaseMETTauEle = process.cmgBaseMETFromPFMET.clone()
   process.mvaBaseMETTauEle.cfg.inputCollection = 'mvaMETTauEle'
   process.mvaMETSequence = cms.Sequence(
      process.cmgTauEleMVAMET + #need to use the uncorrected Tau and muon
      process.goodPVFilter + #quality vertices
      process.mvaMETTauEle + #reco::PFMET object and cmg::PFMetSignificance object
      process.mvaBaseMETTauEle # cmg::BaseMET object
      )
   process.analysis  += process.mvaMETSequence
   #process.mvaMETTauEle.verbose = True
   
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
