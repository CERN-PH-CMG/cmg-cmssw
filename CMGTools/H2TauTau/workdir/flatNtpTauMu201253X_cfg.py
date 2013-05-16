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
process.flatNtp.runSVFit = 1 #1 old #2 new
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


#---->Prior to Apr1
##MVA MET sync 
#process.source.fileNames = ['/store/cmst3/user/benitezj/CMG/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0_BleedingEdge_MET53X/cmgTuple_94.root']
#process.source.fileNames = ['/store/cmst3/user/benitezj/CMG/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0_BleedingEdge_MET53X_Mar27/cmgTuple_94.root']
#process.source.fileNames = ['/store/cmst3/user/benitezj/CMG/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0_BleedingEdge_MET53X_Mar31/cmgTuple_94.root']
#process.source.fileNames = ['file:/afs/cern.ch/user/b/benitezj/scratch0/V5_13_0_BleedingEdge/CMGTools/CMSSW_5_3_7_patch4/src/CMGTools/Common/prod/cmgTuple.root']
#process.source.eventsToProcess = cms.untracked.VEventRange('1:122718')
#


##-->Post Apr1
#process.source.eventsToProcess = cms.untracked.VEventRange('1:124859','1:179645','1:215021','1:222966','1:248040','1:268951','1:352011','1:481278','1:548861','1:558612','1:605727','1:656800','1:666222','1:703011','1:938521','1:988135')

###MVA MET sync scatter values
#process.source.fileNames = ['/store/cmst3/user/benitezj/CMG/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_13_0_BleedingEdge_MET53X_Apr2/cmgTuple_1.root']
#process.source.fileNames = ['file:/afs/cern.ch/user/b/benitezj/scratch0/V5_13_0_BleedingEdge/CMGTools/CMSSW_5_3_7_patch4/src/CMGTools/Common/prod/cmgTuple.root']
#process.source.eventsToProcess = cms.untracked.VEventRange('1:263427')

###V5140 diff
#process.source.eventsToProcess = cms.untracked.VEventRange('1:105103', '1:385416')

##met diff in V5140
#process.source.fileNames = ['/store/cmst3/user/cmgtools/CMG/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_14_0/cmgTuple_10.root']
#process.source.eventsToProcess = cms.untracked.VEventRange('1:714501')

##Apr5
#process.source.eventsToProcess = cms.untracked.VEventRange('1:899539','1:900460','1:954582','1:954772','1:49710','1:62339','1:391350','1:391913','1:276575','1:278782')

##Apr15 Data
#process.source.eventsToProcess = cms.untracked.VEventRange('198207:50338540','198207:50792696','198207:51118645','198207:51956333','198271:122999097','198271:123394717','198271:123456618','198271:123569610','198271:123645734','198271:123707593','198271:123840060','198271:124306867','198271:124835490','198271:125180715','198271:125747832','198271:126168119','198271:126307173','198271:385109044','198271:385374559','198271:385395943','198271:385628362','198271:386093130','198271:386187916','198271:387152433','198271:387322250','198271:387448883','198271:387685839','198271:48841797','198271:49267736','198271:49536836','198271:49561875','198271:49640321','198271:49683750','198271:50419085','198271:51764025','198271:51777644','198271:52380619','198271:52415284','198271:52486746','198485:171387420','198487:316108063','198487:316147037','198487:318117809','198487:318592324','198487:319352672','198487:451570082','198487:451715381','198487:451797597','198487:451986288','198487:453186648','198487:453288749','198487:454063382','198487:454184595','198487:454203800','198487:454410893','198487:454419798')


##diff in di-muon veto
#process.source.fileNames = ['/store/cmst3/user/cmgtools/CMG/TauPlusX/Run2012C-24Aug2012-v1/AOD/V5_B/PAT_CMG_V5_14_0/cmgTuple_233.root']
#process.source.eventsToProcess = cms.untracked.VEventRange('198230:123724743')


##check on the embedded sample
#process.source.eventsToProcess = cms.untracked.VEventRange('198049:39003032','198116:52292486','198207:31745052','198207:73698870','198208:107422149','198208:118651506','198208:128512894','198208:22240519','198208:23798456','198208:28441906','198208:48407060','198210:19717791','198210:47125079','198212:196125834','198212:265673190','198212:2677682','198212:386331168','198212:41210292','198212:423841091','198212:61347663','198212:79661177','198212:84787504','198230:100784289','198230:135529768','198230:15361026','198230:159562044','198230:170885005','198230:183646477','198230:237527349','198230:284072030','198230:316933517')


###For Colin

#process.source.eventsToProcess = cms.untracked.VEventRange('1:110365','1:183683','1:54659','1:54814','1:750651','1:465090','1:172354','1:227219','1:785683','1:106165','1:72406','1:56969','1:132647','1:818777','1:347276','1:398253','1:212254','1:327286','1:472811','1:472893','1:443158','1:316268','1:487540','1:229720','1:360170','1:238699','1:268302','1:776229','1:992574','1:387821','1:411281','1:510809','1:523969','1:878407','1:661467','1:440127','1:331879','1:855028','1:655997','1:480384','1:591991','1:600255','1:343405','1:523634','1:755769','1:771203','1:695295','1:871737','1:362135','1:768438','1:922447','1:693666','1:798087','1:854552')

#process.source.eventsToProcess = cms.untracked.VEventRange('1:110365')

#process.source.eventsToProcess = cms.untracked.VEventRange('1:227219','1:818777','1:398253','1:212254','1:591991','1:922447')
#process.source.eventsToProcess = cms.untracked.VEventRange('1:227219')

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
print process.source.eventsToProcess
process.flatNtp.printSelectionPass = 2


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
if process.flatNtp.embeddedKinWeightFile != '' :
   process.load('TauAnalysis/MCEmbeddingTools/embeddingKineReweight_cff')
   process.embeddingKineReweightRECembedding.inputFileName = cms.FileInPath(process.flatNtp.embeddedKinWeightFile)

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
