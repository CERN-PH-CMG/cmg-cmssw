import FWCore.ParameterSet.Config as cms

process = cms.Process("PAT")

# global options
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 5000
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False),
                                      SkipEvent = cms.untracked.vstring('ProductNotFound')
                                      )

# event source
process.source = cms.Source("PoolSource",fileNames = cms.untracked.vstring())
process.source.fileNames=inputList
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# global tag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

from CMGTools.HtoZZ2l2nu.StandardSelections_cfi import getSelVersion
gt=''
if(getSelVersion()==2012) :
    process.load("Configuration.Geometry.GeometryIdeal_cff")
    if ( not runOnMC ):
        gt='GR_P_V42_AN2::All'
        try :
            if(dataTag=='Aug24'):     gt='FT_53_V10_AN2::All'
            elif(dataTag=='July13'):  gt='FT_53_V6_AN2::All'
            elif(dataTag=='Aug06'):   gt='FT_53_V6C_AN2::All'
            elif(dataTag=='2012Cv2'): gt='GR_P_V41_AN2::All'
            elif(dataTag=='2012D'):   gt='GR_P_V42_AN2::All'
            elif(dataTag=='2012Cv1'): gt='GR_P_V41_AN2::All'
            else :                    gt='GR_P_V41_AN1::All'
        except:
            print 'Assuming nominal Global Tag for prompt preco'

        #cf. https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideBTagJetProbabilityCalibration#Calibration_in_53x_Data_and_MC
        process.GlobalTag.toGet = cms.VPSet( cms.PSet(record = cms.string("BTagTrackProbability2DRcd"),
                                                      tag = cms.string("TrackProbabilityCalibration_2D_Data53X_v2"),
                                                      connect = cms.untracked.string("frontier://FrontierPrep/CMS_COND_BTAU")),
                                             cms.PSet(record = cms.string("BTagTrackProbability3DRcd"),
                                                      tag = cms.string("TrackProbabilityCalibration_3D_Data53X_v2"),
                                                      connect = cms.untracked.string("frontier://FrontierPrep/CMS_COND_BTAU"))
                                             )
    else :
        gt = 'START53_V10::All'
        process.GlobalTag.toGet = cms.VPSet(cms.PSet(record = cms.string("BTagTrackProbability2DRcd"),
                                                     tag = cms.string("TrackProbabilityCalibration_2D_MC53X_v2"),
                                                     connect = cms.untracked.string("frontier://FrontierPrep/CMS_COND_BTAU")),
                                            cms.PSet(record = cms.string("BTagTrackProbability3DRcd"),
                                                     tag = cms.string("TrackProbabilityCalibration_3D_MC53X_v2"),
                                                     connect = cms.untracked.string("frontier://FrontierPrep/CMS_COND_BTAU"))
                                            )
else:
    process.load("Configuration.StandardSequences.Geometry_cff")
    if ( not runOnMC ) : gt='GR_R_44_V13::All'
    else               :
        gt='START44_V12::All'
        process.GlobalTag.toGet = cms.VPSet(cms.PSet(record = cms.string("BTagTrackProbability2DRcd"),
                                                     tag = cms.string("TrackProbabilityCalibration_2D_MC_80_Ali44_v1"),
                                                     connect = cms.untracked.string("frontier://FrontierPrep/CMS_COND_BTAU")),
                                            cms.PSet(record = cms.string("BTagTrackProbability3DRcd"),
                                                     tag = cms.string("TrackProbabilityCalibration_3D_MC_80_Ali44_v1"),
                                                     connect = cms.untracked.string("frontier://FrontierPrep/CMS_COND_BTAU"))
                                            )

print 'Using the following global tag %s'%gt
process.GlobalTag.globaltag = gt

process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

## Output Module Configuration
from CMGTools.HtoZZ2l2nu.OutputConfiguration_cff import configureOutput
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('patTuple.root'),
                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
                               outputCommands = cms.untracked.vstring('keep *'))
process.out.fileName = cms.untracked.string(outFile)


################
# PRESELECTION #
################
from CMGTools.HtoZZ2l2nu.PreselectionSequences_cff import addPreselectionSequences
from CMGTools.HtoZZ2l2nu.PreselectionSequences_cff import addLumifilter
if(not runOnMC ):
    addPreselectionSequences(process)
    #addLumifilter(process,'/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-200601_8TeV_PromptReco_Collisions12_JSON_v2.txt')
    
                      
from CMGTools.HtoZZ2l2nu.SkimSequences_cff import addDileptonSkim, addPhotonSkim
addDileptonSkim(process)
addPhotonSkim(process,selVersion=getSelVersion())

if runOnMC : jecLevels=['L1FastJet','L2Relative','L3Absolute']
else       : jecLevels=['L1FastJet','L2Relative','L3Absolute','L2L3Residual']

#############################################
#  DEFINITION OF THE PFBRECO+PAT SEQUENCES  #
#############################################
# load the PAT config
process.load("PhysicsTools.PatAlgos.patSequences_cff")
from PhysicsTools.PatAlgos.tools.pfTools import *

#configure PF2PAT
usePF2PAT(process,runPF2PAT=True, jetAlgo='AK5', runOnMC=runOnMC, postfix="PFlow",typeIMetCorrections=True,jetCorrections=('AK5PFchs',jecLevels))
if(getSelVersion()==2012) : useGsfElectrons(process,'PFlow',"04") # to change isolation cone size to 0.3 as it is recommended by EGM POG, use "04" for cone size 0.4

#configure std pat to use pf jets/MET
from PhysicsTools.PatAlgos.tools.metTools import *
addPfMET(process, 'PF')
from PhysicsTools.PatAlgos.tools.jetTools import *
switchJetCollection(process,cms.InputTag('ak5PFJets'),
                    doJTA        = True,
                    doBTagging   = True,
                    jetCorrLabel = ('AK5PF',jecLevels),
                    doType1MET   = True,
                    genJetCollection=cms.InputTag("ak5GenJets"),
                    doJetID      = True
                    )
if not runOnMC:
    removeMCMatchingPF2PAT(process,'')
    process.patMETsPF.addGenMET=cms.bool(False)

# MVA id
process.load('EGamma.EGammaAnalysisTools.electronIdMVAProducer_cfi')
process.mvaID = cms.Sequence(  process.mvaTrigV0 + process.mvaNonTrigV0 )

# add old VBTF ids
process.load("ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff")
applyPostfix(process,'patElectrons','').electronIDSources = cms.PSet( simpleEleId95relIso= cms.InputTag("simpleEleId95relIso"),
                                                                   simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
                                                                   simpleEleId85relIso= cms.InputTag("simpleEleId85relIso"),
                                                                   simpleEleId80relIso= cms.InputTag("simpleEleId80relIso"),
                                                                   simpleEleId70relIso= cms.InputTag("simpleEleId70relIso"),
                                                                   simpleEleId60relIso= cms.InputTag("simpleEleId60relIso"),
                                                                   mvaTrigV0 = cms.InputTag("mvaTrigV0"),
                                                                   mvaNonTrigV0 = cms.InputTag("mvaNonTrigV0"),
                                                                   )

# rho for JEC and isolation
from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
process.kt6PFJetsForIso = kt4PFJets.clone( rParam = cms.double(0.6),
                                           doAreaFastjet = cms.bool(True),
                                           doRhoFastjet = cms.bool(True),
                                           Rho_EtaMax = cms.double(2.5),
                                           Ghost_EtaMax = cms.double(2.5) )
process.pfOnlyNeutrals = cms.EDFilter("PdgIdPFCandidateSelector",
                                      src = cms.InputTag("particleFlow"),
                                      pdgId = cms.vint32(22,111,130,310,2112)
                                      )
process.kt6PFJetsCentralNeutral = process.kt6PFJetsForIso.clone( src = cms.InputTag("pfOnlyNeutrals"),
                                                                 Ghost_EtaMax = cms.double(3.1),
                                                                 Rho_EtaMax = cms.double(2.5),
                                                                 inputEtMin = cms.double(0.5)
                                                                 )
if(getSelVersion()==2011) :
    process.kt6PFJets = kt4PFJets.clone( rParam = cms.double(0.6),
                                         doAreaFastjet = cms.bool(True),
                                         doRhoFastjet = cms.bool(True) )
    process.rhoSequence=cms.Sequence( process.kt6PFJets + process.kt6PFJetsForIso + process.pfOnlyNeutrals + process.kt6PFJetsCentralNeutral )
else:
    process.rhoSequence=cms.Sequence( process.kt6PFJetsForIso + process.pfOnlyNeutrals + process.kt6PFJetsCentralNeutral )
    
#inclusive vertex finder
process.load("RecoBTag.Configuration.RecoBTag_cff")
process.load("RecoJets.JetAssociationProducers.ak5JTA_cff")
process.load("RecoBTau.JetTagComputer.jetTagRecord_cfi")
process.load("RecoVertex.AdaptiveVertexFinder.inclusiveVertexing_cff")
process.load("RecoBTag.SecondaryVertex.combinedSecondaryVertexPositiveES_cfi")
process.load("RecoBTag.SecondaryVertex.combinedSecondaryVertexPositiveBJetTags_cfi")
process.ak5JetTracksAssociatorAtVertex.jets = "selectedPatJets"
process.myInclusiveSecondaryVertexTagInfosFiltered = process.inclusiveSecondaryVertexFinderTagInfosFiltered.clone()
process.mySimpleInclusiveSecondaryVertexHighPurBJetTags = process.simpleInclusiveSecondaryVertexHighPurBJetTags.clone(
    tagInfos = cms.VInputTag(cms.InputTag("myInclusiveSecondaryVertexTagInfosFiltered"))
    )
process.mySimpleInclusiveSecondaryVertexHighEffBJetTags = process.simpleInclusiveSecondaryVertexHighEffBJetTags.clone(
    tagInfos = cms.VInputTag(cms.InputTag("myInclusiveSecondaryVertexTagInfosFiltered"))
    )
process.combinedInclusiveSecondaryVertexPositiveBJetTags = process.combinedSecondaryVertexPositiveBJetTags.clone(
    tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfos"), cms.InputTag("myInclusiveSecondaryVertexTagInfosFiltered"))
    )

if(getSelVersion()==2011) :  process.inclusiveMergedVerticesFiltered.secondaryVertices = cms.InputTag("inclusiveVertices")
process.ivfSequence=cms.Sequence(process.ak5JetTracksAssociatorAtVertex
                                 *process.btagging
                                 *process.inclusiveVertexing
                                 *process.inclusiveMergedVerticesFiltered
                                 *process.myInclusiveSecondaryVertexTagInfosFiltered
                                 *process.mySimpleInclusiveSecondaryVertexHighEffBJetTags
                                 *process.mySimpleInclusiveSecondaryVertexHighPurBJetTags
                                 *process.combinedInclusiveSecondaryVertexPositiveBJetTags)

#save also the tag infos
for pf in ['PFlow','']:
    getattr(process,"patJets"+pf).addTagInfos = cms.bool(True)
    getattr(process,"patJets"+pf).tagInfoSources = cms.VInputTag('secondaryVertexTagInfosAOD'+pf,'secondaryVertexNegativeTagInfosAOD'+pf)

#####################
#  PATH DEFINITION  #
#####################
print ' ******* Defining paths to be run'

# counters that can be used at analysis level to know the processed events
process.startCounter = cms.EDProducer("EventCountProducer")
process.endCounter = cms.EDProducer("EventCountProducer")

#pat sequence
process.patSequence = cms.Sequence( process.startCounter
                                    + process.rhoSequence
                                    + process.mvaID
                                    + process.simpleEleIdSequence
                                    + getattr(process,"patPF2PATSequencePFlow")
                                    + process.patDefaultSequence
                                    + process.btagging 
                                    + process.ivfSequence 
                                    )

# define the paths
if(runStd) : process.patOnlyPath = cms.Path(process.startCounter * process.patSequence )
if(runOnMC):
    process.llPath = cms.Path(process.startCounter * process.llCandidateSequence * process.patSequence )
    process.photonPath = cms.Path(process.startCounter * process.photonCandidateSequence * process.patSequence )
else:
    process.llPath = cms.Path(process.startCounter * process.preselection * process.llCandidateSequence * process.patSequence )
    process.photonPath = cms.Path(process.startCounter * process.preselection * process.photonCandidateSequence * process.patSequence )
process.e = cms.EndPath( process.endCounter*process.out )

######################################
# ANALYSIS                           #
######################################
from CMGTools.HtoZZ2l2nu.Analysis_cff import defineAnalysis
defineAnalysis(process)

#######################################
# SCHEDULE THE EXECUTION OF THE PATHS #
#######################################
if(not runStd) :
    configureOutput(process,selPaths=['patOnlyPath'],outFile=outFile)
    if(runOnMC) : process.schedule = cms.Schedule( process.patOnlyPath, process.e )
    else        : process.schedule = cms.Schedule( process.patOnlyPath, process.e )
else :
    configureOutput(process,selPaths=['llPath','photonPath'],outFile=outFile)
    if(runFull) :
        #process.TFileService = cms.Service("TFileService", fileName = cms.string("analysis.root"))
        process.TFileService = cms.Service("TFileService", fileName = cms.string(outFile))
        process.e = cms.EndPath( process.endCounter )
        if(runOnMC) : process.schedule = cms.Schedule( process.llPath, process.photonPath, process.analysis )
        else        : process.schedule = cms.Schedule( process.llPath, process.photonPath, process.analysis )
    else :
        if(runOnMC) : process.schedule = cms.Schedule( process.llPath, process.photonPath, process.e )
        else        : process.schedule = cms.Schedule( process.llPath, process.photonPath, process.e )


print '******************'
print process.out.fileName
print '*******************'
