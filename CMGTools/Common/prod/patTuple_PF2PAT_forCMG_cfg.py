## import skeleton process
from PhysicsTools.PatAlgos.patTemplate_cfg import *

# the source is already defined in patTemplate_cfg.
# overriding source and various other things
#process.load("CommonTools.ParticleFlow.Sources.source_ZtoEles_DBS_312_cfi")
#process.source = cms.Source("PoolSource", 
#     fileNames = cms.untracked.vstring('file:myAOD.root')
#)

# process.source.fileNames = cms.untracked.vstring(['/store/relval/CMSSW_4_2_3/RelValZTT/GEN-SIM-RECO/START42_V12-v2/0062/4CEA9C47-287B-E011-BAB7-00261894396B.root'])
process.source.fileNames = cms.untracked.vstring(['file:PFAOD.root'])

# process.GlobalTag.globaltag = cms.string( 'START42_V12::All' )

runOnMC = True
hpsTaus = False

# process.load("CommonTools.ParticleFlow.Sources.source_ZtoMus_DBS_cfi")
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(500) )
process.out.fileName = cms.untracked.string('patTuple_PF2PAT.root')

# load the PAT config
process.load("PhysicsTools.PatAlgos.patSequences_cff")


# Configure PAT to use PF2PAT instead of AOD sources
# this function will modify the PAT sequences. It is currently 
# not possible to run PF2PAT+PAT and standart PAT at the same time
from PhysicsTools.PatAlgos.tools.pfTools import *

# PF2PAT+PAT sequence 1:
# no lepton cleaning, AK5PFJets

postfixAK5 = "AK5"
jetAlgoAK5="AK5"
usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgoAK5, runOnMC=runOnMC, postfix=postfixAK5,
          jetCorrections=('AK5PFchs', ['L1FastJet','L2Relative','L3Absolute']))


from CommonTools.ParticleFlow.Tools.enablePileUpCorrection import enablePileUpCorrection
enablePileUpCorrection( process, postfix=postfixAK5)

# process.patJetCorrFactorsAK5.rho = cms.InputTag("kt6PFJetsAK5", "rho")
getattr(process,"pfNoMuon"+postfixAK5).enable = False 
getattr(process,"pfNoElectron"+postfixAK5).enable = False 
getattr(process,"pfNoTau"+postfixAK5).enable = False 
getattr(process,"pfNoJet"+postfixAK5).enable = True


if hpsTaus:
    adaptPFTaus(process,"hpsPFTau",postfix=postfixAK5)

# curing a weird bug in PAT..
from CMGTools.Common.PAT.removePhotonMatching import removePhotonMatching
removePhotonMatching( process, postfixAK5 )

# PF2PAT+PAT sequence 2:
# lepton cleaning, AK5PFJets

from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet
postfixLC = 'LC'
# just cloning the first sequence, and enabling lepton cleaning 
cloneProcessingSnippet(process, getattr(process, 'patPF2PATSequence'+postfixAK5), postfixLC)

postfixAK5LC = postfixAK5+postfixLC
getattr(process,"pfNoMuon"+postfixAK5LC).enable = True
getattr(process,"pfNoElectron"+postfixAK5LC).enable = True 
getattr(process,"pfIsolatedMuons"+postfixAK5LC).combinedIsolationCut = 0.2
getattr(process,"pfIsolatedElectrons"+postfixAK5LC).combinedIsolationCut = 0.2

#COLIN : need to add the VBTF e and mu id

# PF2PAT+PAT sequence 3
# no lepton cleaning, AK7PFJets

postfixAK7 = "AK7"
jetAlgoAK7="AK7"

#COLIN : argh! AK7PFchs does not seem to exist yet...
# Maxime should maybe contact the JEC group if he wants them 
usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgoAK7, runOnMC=runOnMC, postfix=postfixAK7,
          jetCorrections=('AK7PF', ['L1FastJet','L2Relative','L3Absolute']))

enablePileUpCorrection( process, postfix=postfixAK7)

if hpsTaus:
    adaptPFTaus(process,"hpsPFTau",postfix=postfixAK7)

getattr(process,"pfNoMuon"+postfixAK7).enable = False 
getattr(process,"pfNoElectron"+postfixAK7).enable = False 
getattr(process,"pfNoTau"+postfixAK7).enable = False 
getattr(process,"pfNoJet"+postfixAK7).enable = True

removePhotonMatching( process, postfixAK7 )


# Let it run
process.p = cms.Path(
    getattr(process,"patPF2PATSequence"+postfixAK5)
    + getattr(process,"patPF2PATSequence"+postfixAK5LC) 
    + getattr(process,"patPF2PATSequence"+postfixAK7) 
)

# Add PF2PAT output to the created file
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
#process.load("CommonTools.ParticleFlow.PF2PAT_EventContent_cff")

# process.out.outputCommands =  cms.untracked.vstring('drop *')

process.out.outputCommands = cms.untracked.vstring('drop *',
                                                   # 'keep recoPFCandidates_particleFlow_*_*',
                                                   *patEventContentNoCleaning
                                                   )
# stuff kept by PAT, but that we don't need:
process.out.outputCommands.extend(
    [ 'drop *_selectedPatMuonsAK7_*_*',
      'drop *_selectedPatElectronsAK7_*_*',
      'drop *_selectedPatTausAK7_*_*',
      'drop CaloTowers_*_*_*',
      'drop patMETs_*_*_*',
      'keep patMETs_patMETsAK5_*_*',
      'drop patPFParticles_*_*_*'
    ] )



process.MessageLogger.cerr.FwkReport.reportEvery = 10
