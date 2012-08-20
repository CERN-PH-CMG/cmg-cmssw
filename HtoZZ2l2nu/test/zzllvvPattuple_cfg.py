import FWCore.ParameterSet.Config as cms

process = cms.Process("PAT")

# global options
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 5000
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True),
                                      SkipEvent = cms.untracked.vstring('ProductNotFound')
                                      )

# event source
process.source = cms.Source("PoolSource",fileNames = cms.untracked.vstring())
process.source.fileNames=inputList
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# global tag
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
gt=''
if ( not runOnMC ):
    gt='GR_P_V41_AN1::All'
    if(process.source.fileNames[0].find('2012C')<0) : gt='GR_P_V39_AN1::All'
else :
    gt = 'START53_V10::All'
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

if(runOnMC) :
    from CMGTools.HtoZZ2l2nu.GeneratorLevelSequences_cff import addGeneratorLevelSequence
    addGeneratorLevelSequence(process)


################
# PRESELECTION #
################
from CMGTools.HtoZZ2l2nu.PreselectionSequences_cff import addPreselectionSequences
from CMGTools.HtoZZ2l2nu.PreselectionSequences_cff import addLumifilter
if(not runOnMC ):
    addPreselectionSequences(process)
    #addLumifilter(process,'/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-195016_8TeV_PromptReco_Collisions12_JSON_v2.txt')
    
                      
from CMGTools.HtoZZ2l2nu.SkimSequences_cff import addDileptonSkim, addPhotonSkim
addDileptonSkim(process)
addPhotonSkim(process)


if runOnMC : jecLevels=['L1FastJet','L2Relative','L3Absolute']
else       : jecLevels=['L1FastJet','L2Relative','L3Absolute','L2L3Residual']

#############################################
#  DEFINITION OF THE PFBRECO+PAT SEQUENCES  #
#############################################
# load the PAT config
process.load("PhysicsTools.PatAlgos.patSequences_cff")
from PhysicsTools.PatAlgos.tools.pfTools import *

postfix = "PFlowNoPuSub"
jetAlgo="AK5"
usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo, runOnMC=True, postfix="PFlow",typeIMetCorrections=True,jetCorrections=('AK5PFchs',jecLevels))
usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo, runOnMC=True, postfix=postfix,typeIMetCorrections=True,jetCorrections=('AK5PF',jecLevels))

# to use GsfElectrons instead of PF electrons
# this will destory the feature of top projection which solves the ambiguity between leptons and jets because
# there will be overlap between non-PF electrons and jets even though top projection is ON!
useGsfElectrons(process,postfix,"03") # to change isolation cone size to 0.3 as it is recommended by EGM POG, use "04" for cone size 0.4

# add old VBTF ids
process.load("ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff")
applyPostfix(process,"patElectrons",postfix).electronIDSources = cms.PSet( simpleEleId95relIso= cms.InputTag("simpleEleId95relIso"),
                                                                           simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
                                                                           simpleEleId85relIso= cms.InputTag("simpleEleId85relIso"),
                                                                           simpleEleId80relIso= cms.InputTag("simpleEleId80relIso"),
                                                                           simpleEleId70relIso= cms.InputTag("simpleEleId70relIso"),
                                                                           simpleEleId60relIso= cms.InputTag("simpleEleId60relIso")
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

#inclusive vertex finder
process.load('RecoVertex/AdaptiveVertexFinder/inclusiveVertexing_cff')

#####################
#  PATH DEFINITION  #
#####################
print ' ******* Defining paths to be run'

# counters that can be used at analysis level to know the processed events
process.startCounter = cms.EDProducer("EventCountProducer")
process.endCounter = cms.EDProducer("EventCountProducer")

#pat sequence
process.patSequence = cms.Sequence( process.startCounter
                                    + process.kt6PFJetsForIso + process.pfOnlyNeutrals + process.kt6PFJetsCentralNeutral
                                    + process.inclusiveVertexing + process.simpleEleIdSequence
                                    + getattr(process,"patPF2PATSequencePFlow")
                                    + getattr(process,"patPF2PATSequence"+postfix)
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
    if(runOnMC) : process.schedule = cms.Schedule( process.genLevelPath, process.patOnlyPath, process.e )
    else        : process.schedule = cms.Schedule( process.genLevelPath, process.patOnlyPath, process.e )
else :
    configureOutput(process,selPaths=['llPath','photonPath'],outFile=outFile)
    if(runFull) :
        process.TFileService = cms.Service("TFileService", fileName = cms.string("analysis.root"))
        process.e = cms.EndPath( process.endCounter )
        if(runOnMC) : process.schedule = cms.Schedule( process.genLevelPath, process.llPath, process.photonPath, process.analysis )
        else        : process.schedule = cms.Schedule( process.llPath, process.photonPath, process.analysis )
    else :
        if(runOnMC) : process.schedule = cms.Schedule( process.genLevelPath, process.llPath, process.photonPath, process.e )
        else        : process.schedule = cms.Schedule( process.llPath, process.photonPath, process.e )


print '******************'
print process.out.fileName
print '*******************'
