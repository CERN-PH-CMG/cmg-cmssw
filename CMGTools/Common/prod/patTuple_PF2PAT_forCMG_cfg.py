## import skeleton process
from PhysicsTools.PatAlgos.patTemplate_cfg import *

### MASTER FLAGS  ######################################################################

# turn this on if you want to pick a relval in input (see below)
pickRelVal = False

# turn on when running on MC
runOnMC = False

# AK5 sequence with no cleaning is the default
# the other sequences can be turned off with the following flags.
#JOSE: no need to run these guys for what you are up to
runAK5LC = True
runAK7 = True

#COLIN: will need to include the event filters in tagging mode

#COLIN : reactivate HPS when bugs corrected
hpsTaus = False

#COLIN: the following leads to rare segmentation faults
doJetPileUpCorrection = True

#COLIN : when activating PFCandidate (and other stuff) embedding,
# patTaus cannot be stored together with cmgTaus...
doEmbedPFCandidatesInTaus = True

runCMG = True

if runCMG:
    doEmbedPFCandidatesInTaus = False

# process.load("CommonTools.ParticleFlow.Sources.source_ZtoMus_DBS_cfi")
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = 10

sep_line = "-" * 50
print sep_line
print 'running the following PF2PAT+PAT sequences:'
print '\tAK5'
if runAK5LC: print '\tAK5LC'
if runAK7: print '\tAK7'
print 'embedding in taus: ', doEmbedPFCandidatesInTaus
print 'HPS taus         : ', hpsTaus
print 'produce CMG tuple: ', runCMG
print sep_line


### SOURCE DEFINITION  ################################################################


# process.source.fileNames = cms.untracked.vstring(['/store/relval/CMSSW_4_2_3/RelValZTT/GEN-SIM-RECO/START42_V12-v2/0062/4CEA9C47-287B-E011-BAB7-00261894396B.root'])
# process.source.fileNames = cms.untracked.vstring(['file:PFAOD.root'])

# process.load("CMGTools.Common.sources.QCD_Pt_170to300_TuneZ2_7TeV_pythia6.Summer11_PU_S3_START42_V11_v2.AODSIM.source_cff")
# process.load("CMGTools.Common.sources.HT.Run2011A_May10ReReco_v1.AOD.source_cff")
# process.load("CMGTools.Common.sources.TT_TuneZ2_7TeV_pythia6_tauola.Summer11_PU_S3_START42_V11_v2.AODSIM.source_cff")
# process.load("CMGTools.Common.sources.QCD_Pt_30to50_TuneZ2_7TeV_pythia6.Summer11_PU_S3_START42_V11_v2.AODSIM.source_cff")
# process.load("CMGTools.Common.sources.QCD_Pt_50to80_TuneZ2_7TeV_pythia6.Summer11_PU_S3_START42_V11_v2.AODSIM.source_cff")
# process.load("CMGTools.Common.sources.QCD_Pt_80to120_TuneZ2_7TeV_pythia6.Summer11_PU_S3_START42_V11_v2.AODSIM.source_cff")
# process.load("CMGTools.Common.sources.QCD_Pt_120to170_TuneZ2_7TeV_pythia6.Summer11_PU_S3_START42_V11_v2.AODSIM.source_cff")
# process.load("CMGTools.Common.sources.QCD_Pt_170to300_TuneZ2_7TeV_pythia6.Summer11_PU_S3_START42_V11_v2.AODSIM.source_cff")
# process.load("CMGTools.Common.sources.QCD_Pt_470to600_TuneZ2_7TeV_pythia6.Summer11_PU_S3_START42_V11_v2.AODSIM.source_cff")
# process.load("CMGTools.Common.sources.QCD_Pt_600to800_TuneZ2_7TeV_pythia6.Summer11_PU_S3_START42_V11_v2.AODSIM.source_cff")
# process.load("CMGTools.Common.sources.QCD_Pt_800to1000_TuneZ2_7TeV_pythia6.Summer11_PU_S3_START42_V11_v2.AODSIM.source_cff")
# process.load("CMGTools.Common.sources.QCD_Pt_1000to1400_TuneZ2_7TeV_pythia6.Summer11_PU_S3_START42_V11_v2.AODSIM.source_cff")
# process.load("CMGTools.Common.sources.QCD_Pt_1400to1800_TuneZ2_7TeV_pythia6.Summer11_PU_S3_START42_V11_v2.AODSIM.source_cff")
process.load("CMGTools.Common.sources.HT.Run2011A_HighMET_PromptSkim_v4.RAW_RECO.source_cff")

print "WARNING!!!!!!!!!!!!!!!! remove the following line (see .cfg) before running on the batch!"
# process.source.fileNames = [process.source.fileNames[0]]

if pickRelVal:
    process.source = cms.Source(
        "PoolSource",
        fileNames = cms.untracked.vstring(
        pickRelValInputFiles( cmsswVersion  = 'CMSSW_4_2_3'
                              , relVal        = 'RelValQCD_FlatPt_15_3000'
                              , globalTag     = 'MC_42_V12'
                              , numberOfFiles = 1
                              )
        )
        )

print 'PF2PAT+PAT+CMG for files:'
print process.source.fileNames

### DEFINITION OF THE PF2PAT+PAT SEQUENCES #############################################

from CMGTools.Common.Tools.getGlobalTag import getGlobalTag
process.GlobalTag.globaltag = cms.string(getGlobalTag(runOnMC))

# load the PAT config
process.load("PhysicsTools.PatAlgos.patSequences_cff")
process.out.fileName = cms.untracked.string('patTuple_PF2PAT.root')

# Configure PAT to use PF2PAT instead of AOD sources
# this function will modify the PAT sequences. It is currently 
# not possible to run PF2PAT+PAT and standart PAT at the same time
from PhysicsTools.PatAlgos.tools.pfTools import *

# ---------------- Sequence AK5 ----------------------

# PF2PAT+PAT sequence 1:
# no lepton cleaning, AK5PFJets

postfixAK5 = "AK5"
jetAlgoAK5="AK5"
usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgoAK5, runOnMC=runOnMC, postfix=postfixAK5,
          jetCorrections=('AK5PFchs', ['L1FastJet','L2Relative','L3Absolute']))


if doJetPileUpCorrection:
    from CommonTools.ParticleFlow.Tools.enablePileUpCorrection import enablePileUpCorrection
    enablePileUpCorrection( process, postfix=postfixAK5)

from CMGTools.Common.PAT.embedPFCandidatesInTaus import embedPFCandidatesInTaus
if doEmbedPFCandidatesInTaus:
    embedPFCandidatesInTaus( process, postfix=postfixAK5, enable=True )

if hpsTaus:
    adaptPFTaus(process,"hpsPFTau",postfix=postfixAK5)

# curing a weird bug in PAT..
from CMGTools.Common.PAT.removePhotonMatching import removePhotonMatching
removePhotonMatching( process, postfixAK5 )

getattr(process,"pfNoMuon"+postfixAK5).enable = False 
getattr(process,"pfNoElectron"+postfixAK5).enable = False 
getattr(process,"pfNoTau"+postfixAK5).enable = False 
getattr(process,"pfNoJet"+postfixAK5).enable = True
getattr(process,"pfIsolatedMuons"+postfixAK5).combinedIsolationCut = 999999
getattr(process,"pfIsolatedElectrons"+postfixAK5).combinedIsolationCut = 999999


# ---------------- Sequence AK5, lepton x-cleaning ---------------

# PF2PAT+PAT sequence 2:
# lepton cleaning, AK5PFJets. This sequence is a clone of the AK5 sequence defined previously.
# just modifying the x-cleaning parameters, and the isolation cut for x-cleaning

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


# ---------------- Sequence AK7, no lepton x-cleaning ---------------

# PF2PAT+PAT sequence 3
# no lepton cleaning, AK7PFJets

postfixAK7 = "AK7"
jetAlgoAK7="AK7"

#COLIN : argh! AK7PFchs does not seem to exist yet...
# Maxime should maybe contact the JEC group if he wants them 
usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgoAK7, runOnMC=runOnMC, postfix=postfixAK7,
          jetCorrections=('AK7PF', ['L1FastJet','L2Relative','L3Absolute']))

if doJetPileUpCorrection:
    enablePileUpCorrection( process, postfix=postfixAK7)

# no need for taus in AK7 sequence. could remove the whole tau sequence to gain time?
# if hpsTaus:
#    adaptPFTaus(process,"hpsPFTau",postfix=postfixAK7)

getattr(process,"pfNoMuon"+postfixAK7).enable = False 
getattr(process,"pfNoElectron"+postfixAK7).enable = False 
getattr(process,"pfNoTau"+postfixAK7).enable = False 
getattr(process,"pfNoJet"+postfixAK7).enable = True

removePhotonMatching( process, postfixAK7 )


# ---------------- Common stuff ---------------

process.load('CMGTools.Common.gen_cff')


process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")
process.patTrigger.processName = cms.string('*')

### PATH DEFINITION #############################################


# trigger information (no selection)

process.p = cms.Path( process.patTriggerDefaultSequence )

# event cleaning (in tagging mode, no event rejected)

process.load('CMGTools.Common.eventCleaning.eventCleaning_cff')

process.p += process.eventCleaningSequence

# gen ---- 

if runOnMC:
    process.p += process.genSequence 

# PF2PAT+PAT ---

process.p += getattr(process,"patPF2PATSequence"+postfixAK5)

if runAK5LC:
    process.p += getattr(process,"patPF2PATSequence"+postfixAK5LC) 

if runAK7:
    process.p += getattr(process,"patPF2PATSequence"+postfixAK7) 
 
# CMG ---

if runCMG:
    
    process.load('CMGTools.Common.analysis_cff')
    # running on PFAOD -> calo objects are not available.
    # we'll need to reactivate caloMET, though
    # process.p += process.analysisSequence

    from CMGTools.Common.Tools.visitorUtils import replacePostfix
    
    cloneProcessingSnippet(process, getattr(process, 'analysisSequence'), 'AK5LCCMG')
    replacePostfix(getattr(process,"analysisSequenceAK5LCCMG"),'AK5','AK5LC') 
    
    cloneProcessingSnippet(process, getattr(process, 'analysisSequence'), 'AK7CMG')
    replacePostfix(getattr(process,"analysisSequenceAK7CMG"),'AK5','AK7') 
    
    from CMGTools.Common.Tools.tuneCMGSequences import * 
    tuneCMGSequences(process, postpostfix='CMG')

    process.p += process.analysisSequence

    if runAK5LC:
        process.p += process.analysisSequenceAK5LCCMG
        
    if runAK7:
        process.p += process.analysisSequenceAK7CMG

### OUTPUT DEFINITION #############################################

# PF2PAT+PAT ---

# Add PF2PAT output to the created file
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning, patTriggerEventContent, patTriggerStandAloneEventContent
process.out.outputCommands = cms.untracked.vstring('drop *',
                                                   *patEventContentNoCleaning
                                                   )
# add trigger information to the pat-tuple
process.out.outputCommands += patTriggerEventContent
process.out.outputCommands += patTriggerStandAloneEventContent

# add gen event content to the pat-tuple (e.g. status 3 GenParticles)
from CMGTools.Common.eventContent.gen_cff import gen 
process.out.outputCommands.extend( gen )

# tuning the PAT event content to our needs
from CMGTools.Common.eventContent.patEventContentCMG_cff import patEventContentCMG
process.out.outputCommands.extend( patEventContentCMG )

# event cleaning results
from CMGTools.Common.eventContent.eventCleaning_cff import eventCleaning
process.out.outputCommands.extend( eventCleaning )

# CMG ---

from CMGTools.Common.eventContent.everything_cff import everything 

process.outcmg = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('tree_CMG.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    outputCommands = everything,
    dropMetaData = cms.untracked.string('PRIOR')
    )

if runCMG:
    process.outpath += process.outcmg

if runOnMC:
    process.load("CMGTools.Common.runInfoAccounting_cfi")
    process.outpath += process.runInfoAccounting

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histograms_CMG.root"))

# process.Timing = cms.Service("Timing")

# print process.dumpPython()


