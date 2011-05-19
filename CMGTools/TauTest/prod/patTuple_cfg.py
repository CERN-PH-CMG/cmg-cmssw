from PhysicsTools.PatAlgos.patTemplate_cfg import *
import os.path, tempfile

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))
runOnMC = True
#process.source.fileNames = cms.untracked.vstring(['/store/relval/CMSSW_4_2_3/RelValZTT/GEN-SIM-RECO/START42_V12-v2/0062/4CEA9C47-287B-E011-BAB7-00261894396B.root'])
#process.source.fileNames = cms.untracked.vstring(['/store/relval/CMSSW_4_2_0_pre8/RelValTTbar/GEN-SIM-RECO/START42_V7-v1/0042/4C740530-8B56-E011-8CCE-0018F3D095FC.root'])
#process.source.fileNames = cms.untracked.vstring(['file:/afs/cern.ch/user/b/benitezj/scratch0/CMSSW_4_2_3_patch1/src/CommonTools/ParticleFlow/test/PFAOD.root'])
#process.source.fileNames = cms.untracked.vstring(['file:/afs/cern.ch/user/c/cbern/scratch0/CMG/PrunedAOD/prunedAOD.root'])
#process.source.fileNames = cms.untracked.vstring(['file:/afs/cern.ch/user/b/benitezj/scratch0/CMSSW_4_2_3_patch1/src/CommonTools/ParticleFlow/test/PFAOD_4_2_3_RelValZTT.root'])
process.source.fileNames = cms.untracked.vstring(['file:/afs/cern.ch/user/b/benitezj/scratch0/Tuples/tmp/prunedAOD_RelValTTbar.root'])

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000))
process.out.fileName = cms.untracked.string(os.path.expandvars('Tuples/tmp/patTuple_RelValTTbar.root'))
process.MessageLogger.cerr.FwkReport.reportEvery = 100

##from JetEnCorr2011 needed for AK5PFchs
## Geometry and Detector Conditions (needed for a few patTuple production steps)
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string( 'START42_V12::All' )
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
process.load('RecoJets.Configuration.RecoPFJets_cff')
process.kt6PFJets.doRhoFastjet = True
process.ak5PFJets.doAreaFastjet = True
JetCorrectionService = cms.string('ak5PFL1FastL2L3Residual')


process.load("PhysicsTools.PatAlgos.patSequences_cff")
from PhysicsTools.PatAlgos.tools.pfTools import *
postfixAK5 = "AK5"
jetAlgoAK5 = "AK5"
usePF2PAT(process,runPF2PAT=True,
          jetAlgo=jetAlgoAK5,
          runOnMC=runOnMC,
          postfix=postfixAK5,
          jetCorrections=('AK5PFchs', ['L1FastJet','L2Relative','L3Absolute']),
          )

###
def removePhotonMatching( process, postfix ):
   getattr(process,'patDefaultSequence'+postfix).remove( getattr(process,'photonMatch'+postfix) ) 

removePhotonMatching( process, 'AK5' )
#removePhotonMatching( process, 'AK7' )



###Change the Tau producer to HPS
adaptPFTaus(process,"hpsPFTau",postfix=postfixAK5)

###must be set on collection with postfix because this is the collection which gets saved ??

process.patTausAK5.embedLeadTrack                        = cms.bool(True)
process.patTausAK5.embedSignalTracks                     = cms.bool(True)
process.patTausAK5.embedIsolationTracks                  = cms.bool(True)
process.patTausAK5.embedLeadPFCand                       = cms.bool(True)
process.patTausAK5.embedLeadPFChargedHadrCand            = cms.bool(True)
process.patTausAK5.embedLeadPFNeutralCand                = cms.bool(True)
process.patTausAK5.embedSignalPFCands                    = cms.bool(True)
process.patTausAK5.embedSignalPFChargedHadrCands         = cms.bool(True)
process.patTausAK5.embedSignalPFNeutralHadrCands         = cms.bool(True)
process.patTausAK5.embedSignalPFGammaCands               = cms.bool(True)
process.patTausAK5.embedIsolationPFCands                 = cms.bool(True)
process.patTausAK5.embedIsolationPFChargedHadrCands      = cms.bool(True)
process.patTausAK5.embedIsolationPFNeutralHadrCands      = cms.bool(True)
process.patTausAK5.embedIsolationPFGammaCands            = cms.bool(True)


process.p = cms.Path(process.kt6PFJets * process.ak5PFJets * getattr(process,"patPF2PATSequence"+postfixAK5))

from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
process.out.outputCommands = cms.untracked.vstring('drop *',                                                   
                                                   "keep *_selectedPatTaus"+postfixAK5+"_*_*",
                                                   )










#                                                   'keep recoPFCandidates_particleFlow_*_*',
#                                                   # Gen information
#                                                   'keep *_genParticles_*_*',
#                                                   'keep *_generator_*_*',
#                                                   'keep *_genMetTrue_*_*',
#                                                   'keep recoGenJets_ak5GenJets_*_*',
#                                                   #Counters
#                                                   'keep *_MEtoEDMConverter_*_*',
#                                                   'keep GenRunInfoProduct_*_*_*',
#                                                   # Vertex info
#                                                   'keep recoVertexs_*_*_*',
#                                                   #'keep *_generalTracks_*_*',
#                                                   'keep recoBeamSpot_*_*_*',
#                                                   # Trigger
#                                                   'keep L1GlobalTriggerObjectMapRecord_*_*_*',
#                                                   'keep *_TriggerResults_*_*',
#                                                   'keep *_hltTriggerSummaryAOD_*_*',
#                                                   "keep *_*_*_PAT",
#                                                   *patEventContentNoCleaning,
