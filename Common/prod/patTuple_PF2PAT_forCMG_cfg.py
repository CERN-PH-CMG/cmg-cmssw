## import skeleton process
from PhysicsTools.PatAlgos.patTemplate_cfg import *

### MASTER FLAGS  ######################################################################

# turn this on if you want to pick a relval in input (see below)
pickRelVal = False

# turn on when running on MC
runOnMC = True

# AK5 sequence with no cleaning is the default
# the other sequences can be turned off with the following flags.
#JOSE: no need to run these guys for what you are up to
runAK5LC = False 
runAK7 = False

#COLIN: will need to include the event filters in tagging mode

#COLIN : reactivate HPS when bugs corrected
hpsTaus = True

#COLIN: the following leads to rare segmentation faults
doJetPileUpCorrection = True

#patTaus can now be saved even when running the CMG sequence.
doEmbedPFCandidatesInTaus = True

runCMG = True


#add the L2L3Residual corrections only for data
if runOnMC:#MC
    jetCorrections=['L1FastJet','L2Relative','L3Absolute']
else:#Data
    jetCorrections=['L1FastJet','L2Relative','L3Absolute','L2L3Residual']

# process.load("CommonTools.ParticleFlow.Sources.source_ZtoMus_DBS_cfi")
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = 10

from CMGTools.Common.Tools.getGlobalTag import getGlobalTag
process.GlobalTag.globaltag = cms.string(getGlobalTag(runOnMC))

sep_line = "-" * 50
print sep_line
print 'running the following PF2PAT+PAT sequences:'
print '\tAK5'
if runAK5LC: print '\tAK5LC'
if runAK7: print '\tAK7'
print 'embedding in taus: ', doEmbedPFCandidatesInTaus
print 'HPS taus         : ', hpsTaus
print 'produce CMG tuple: ', runCMG
print 'run on MC        : ', runOnMC
print sep_line
print 'Global tag       : ', process.GlobalTag.globaltag
print sep_line

### SOURCE DEFINITION  ################################################################

from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    # to test MC:
    # 'cbern',
    # '/VBF_HToTauTau_M-120_7TeV-powheg-pythia6-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2',
    'CMS',
    '/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_Chamonix12_START44_V10-v2/AODSIM',
    # to test Data:
    # 'cmgtools',
    # '/DoubleElectron/Run2011A-HZZ-PromptSkim-v6/AOD/V2', 
    '.*root'
    ) 

if pickRelVal:
    process.source = cms.Source(
        "PoolSource",
        fileNames = cms.untracked.vstring(
        pickRelValInputFiles( cmsswVersion  = 'CMSSW_4_3_0_pre2'
                              , relVal        = 'RelValZmumuJets_Pt_20_300PU1'
                              , globalTag     = 'MC_42_V9_PU_E7TeV_AVE_2_BX2808'
                              , numberOfFiles = -1
                              )
        )
        )

# process.source.fileNames = ['file:AOD_TTJets_all.root']

# print "WARNING!!!!!!!!!!!!!!!! remove the following line (see .cfg) before running on the batch!"
process.source.fileNames = process.source.fileNames[:10]

print 'PF2PAT+PAT+CMG for files:'
print process.source.fileNames

### DEFINITION OF THE PF2PAT+PAT SEQUENCES #############################################


# load the PAT config
process.load("PhysicsTools.PatAlgos.patSequences_cff")
process.out.fileName = cms.untracked.string('patTuple_PF2PAT.root')

# Configure PAT to use PF2PAT instead of AOD sources
# this function will modify the PAT sequences. It is currently 
# not possible to run PF2PAT+PAT and standart PAT at the same time
from PhysicsTools.PatAlgos.tools.pfTools import *

# ---------------- Sequence AK5 ----------------------


process.eIdSequence = cms.Sequence()

# PF2PAT+PAT sequence 1:
# no lepton cleaning, AK5PFJets

postfixAK5 = "AK5"
jetAlgoAK5="AK5"

#COLIN : we will need to add the L2L3Residual when they become available! also check the other calls to the usePF2PAT function. 
usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgoAK5, runOnMC=runOnMC, postfix=postfixAK5,
          jetCorrections=('AK5PFchs', jetCorrections))


if doJetPileUpCorrection:
    from CommonTools.ParticleFlow.Tools.enablePileUpCorrection import enablePileUpCorrection
    enablePileUpCorrection( process, postfix=postfixAK5)

#configure the taus
from CMGTools.Common.PAT.tauTools import *
if doEmbedPFCandidatesInTaus:
    embedPFCandidatesInTaus( process, postfix=postfixAK5, enable=True )
if hpsTaus:
    adaptPFTaus(process,"hpsPFTau",postfix=postfixAK5)
    #  note that the following disables the tau cleaning in patJets
    adaptSelectedPFJetForHPSTau(process,jetSelection="pt()>15.0",postfix=postfixAK5)
    # currently (Sept 27,2011) there are three sets of tau isolation discriminators better to choose in CMG tuples.
    removeHPSTauIsolation(process,postfix=postfixAK5)

   
# curing a weird bug in PAT..
from CMGTools.Common.PAT.removePhotonMatching import removePhotonMatching
removePhotonMatching( process, postfixAK5 )

getattr(process,"pfNoMuon"+postfixAK5).enable = False 
getattr(process,"pfNoElectron"+postfixAK5).enable = False 
getattr(process,"pfNoTau"+postfixAK5).enable = False 
getattr(process,"pfNoJet"+postfixAK5).enable = True
getattr(process,"pfIsolatedMuons"+postfixAK5).isolationCut = 999999
getattr(process,"pfIsolatedElectrons"+postfixAK5).isolationCut = 999999

# adding vbtf and cic electron IDs
from CMGTools.Common.PAT.addPATElectronID_cff import addPATElectronID
addPATElectronID( process, postfixAK5 , runOnMC )

# insert the PFMET sifnificance calculation
from CMGTools.Common.PAT.addMETSignificance_cff import addMETSig
addMETSig( process, postfixAK5 )


# ---------------- Sequence AK5LC, lepton x-cleaning ---------------

# PF2PAT+PAT sequence 2:
# lepton cleaning, AK5PFJets. This sequence is a clone of the AK5 sequence defined previously.
# just modifying the x-cleaning parameters, and the isolation cut for x-cleaning

if runAK5LC:
    print 'cloning AK5 sequence to prepare AK5LC sequence...'

    from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet
    postfixLC = 'LC'
    # just cloning the first sequence, and enabling lepton cleaning 
    cloneProcessingSnippet(process, getattr(process, 'patPF2PATSequence'+postfixAK5), postfixLC)

    postfixAK5LC = postfixAK5+postfixLC
    getattr(process,"pfNoMuon"+postfixAK5LC).enable = True
    getattr(process,"pfNoElectron"+postfixAK5LC).enable = True 
    getattr(process,"pfIsolatedMuons"+postfixAK5LC).isolationCut = 0.2
    getattr(process,"pfIsolatedElectrons"+postfixAK5LC).isolationCut = 0.2

    #COLIN : need to add the VBTF e and mu id

    # configure MET significance
    getattr(process,"PFMETSignificance"+postfixAK5LC).inputPATElectrons = cms.InputTag('patElectrons'+postfixAK5LC)   
    getattr(process,"PFMETSignificance"+postfixAK5LC).inputPATMuons = cms.InputTag('patMuons'+postfixAK5LC)


    print 'cloning AK5 sequence to prepare AK5LC sequence...Done'

# ---------------- Sequence AK7, no lepton x-cleaning ---------------

# PF2PAT+PAT sequence 3
# no lepton cleaning, AK7PFJets

if runAK7: 
    postfixAK7 = "AK7"
    jetAlgoAK7="AK7"

    #COLIN : argh! AK7PFchs does not seem to exist yet...
    # Maxime should maybe contact the JEC group if he wants them 
    usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgoAK7, runOnMC=runOnMC, postfix=postfixAK7,
              jetCorrections=('AK7PF', jetCorrections))

    # if doJetPileUpCorrection:
    #    enablePileUpCorrection( process, postfix=postfixAK7)

    # no need for taus in AK7 sequence. could remove the whole tau sequence to gain time?
    # if hpsTaus:
    #    adaptPFTaus(process,"hpsPFTau",postfix=postfixAK7)

    # no top projection: 
    getattr(process,"pfNoMuon"+postfixAK7).enable = False 
    getattr(process,"pfNoElectron"+postfixAK7).enable = False 
    getattr(process,"pfNoTau"+postfixAK7).enable = False 
    getattr(process,"pfNoJet"+postfixAK7).enable = True

    removePhotonMatching( process, postfixAK7 )

# addPATElectronID( process, postfixAK7 , runOnMC )

# addMETSig(process,postfixAK7)


# ---------------- Common stuff ---------------

process.load('CMGTools.Common.gen_cff')


process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")
process.patTrigger.processName = cms.string('*')

### PATH DEFINITION #############################################


# trigger information (no selection)

process.p = cms.Path( process.patTriggerDefaultSequence )


# PF2PAT+PAT ---

process.p += getattr(process,"patPF2PATSequence"+postfixAK5)

if runAK5LC:
    process.p += getattr(process,"patPF2PATSequence"+postfixAK5LC) 

if runAK7:
    process.p += getattr(process,"patPF2PATSequence"+postfixAK7) 

# event cleaning (in tagging mode, no event rejected)

process.load('CMGTools.Common.eventCleaning.eventCleaning_cff')

process.p += process.eventCleaningSequence

# gen ---- 

if runOnMC:
    process.p += process.genSequence 

 
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

from CMGTools.Common.eventContent.everything_cff import everything 
process.out.outputCommands.extend( everything )

# tuning the PAT event content to our needs
from CMGTools.Common.eventContent.patEventContentCMG_cff import patEventContentCMG
process.out.outputCommands.extend( patEventContentCMG )

# CMG ---


process.outcmg = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('tree_CMG.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    outputCommands = everything,
    dropMetaData = cms.untracked.string('PRIOR')
    )

if runCMG:
    process.outpath += process.outcmg

process.load("CMGTools.Common.runInfoAccounting_cff")
process.ria = cms.Sequence(
    process.runInfoAccountingDataSequence
    )
if runOnMC:
    process.ria = cms.Sequence(
        process.runInfoAccountingSequence
    )

process.outpath += process.ria
    

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histograms_CMG.root"))
