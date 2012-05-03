## import skeleton process
from PhysicsTools.PatAlgos.patTemplate_cfg import *

### MASTER FLAGS  ######################################################################

# turn this on if you want to pick a relval in input (see below)
## pickRelVal = False

# turn on when running on MC
runOnMC = True

runCMG = True

# AK5 sequence with pileup substraction is the default
# the other sequences can be turned off with the following flags.
runAK5NoPUSub = True
runOnV4 = False


doJetPileUpCorrection = True

# for jet substructure you need extra tags in 4XX and 52X:
# for 4XX:
# scram setup $CMS_PATH/slc5_amd64_gcc434/external/fastjet-toolfile/1.0-cms7/etc/scram.d/fastjet.xml
# addpkg RecoJets/JetAlgorithms V04-04-00
# addpkg RecoJets/JetProducers V05-10-02
# for 52X:
# addpkg RecoJets/JetProducers V05-10-02
runJetSubstructure = False

#add the L2L3Residual corrections only for data
if runOnMC:#MC
    jetCorrections=['L1FastJet','L2Relative','L3Absolute']
else:#Data
    jetCorrections=['L1FastJet','L2Relative','L3Absolute','L2L3Residual']

# process.load("CommonTools.ParticleFlow.Sources.source_ZtoMus_DBS_cfi")
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(500) )
process.MessageLogger.cerr.FwkReport.reportEvery = 10

from CMGTools.Common.Tools.getGlobalTag import getGlobalTag
process.GlobalTag.globaltag = cms.string(getGlobalTag(runOnMC))

sep_line = "-" * 50
print sep_line
print 'running the following PFBRECO+PAT sequences:'
print '\tAK5'
if runAK5NoPUSub: print '\tAK5NoPUSub'
print 'produce CMG tuple: ', runCMG
print 'run on MC        : ', runOnMC
print 'run on V4        : ', runOnV4
print 'run jet substructure : ', runJetSubstructure
print sep_line
print 'Global tag       : ', process.GlobalTag.globaltag
print sep_line

### SOURCE DEFINITION  ################################################################

# print 'generate source'

from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
   # 'cmgtools_group',
   #'/Tau/Run2011A-May10ReReco-v1/AOD/V4',
   #'/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V4'
   'CMS',
   '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12-PU_S7_START52_V5-v2/AODSIM'
   # '/DoubleElectron/Run2012A-PromptReco-v1/AOD'
   )


# ProductionTasks.py will override this change
process.source.fileNames = process.source.fileNames[:10]

print 'PFBRECO+PAT+CMG for files:'
print process.source.fileNames

# sys.exit(1)

### DEFINITION OF THE PFBRECO+PAT SEQUENCES #############################################



# load the PAT config
process.load("PhysicsTools.PatAlgos.patSequences_cff")
process.out.fileName = cms.untracked.string('patTuple_PF2PAT.root')

# Configure PAT to use PFBRECO instead of AOD sources
# this function will modify the PAT sequences. It is currently 
# not possible to run PFBRECO+PAT and standart PAT at the same time
from PhysicsTools.PatAlgos.tools.pfTools import *

# ---------------- rho calculation for JEC ----------------------

from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets

process.kt6PFJets = kt4PFJets.clone(
    rParam = cms.double(0.6),
    doAreaFastjet = cms.bool(True),
    doRhoFastjet = cms.bool(True),
)

#compute rho correction for lepton isolation
process.kt6PFJetsCHS = process.kt6PFJets.clone( src = cms.InputTag("pfNoElectronAK5") )
process.kt6PFJetsForIso = process.kt6PFJets.clone( Rho_EtaMax = cms.double(2.5), Ghost_EtaMax = cms.double(2.5) )
process.kt6PFJetsCHSForIso = process.kt6PFJets.clone( Rho_EtaMax = cms.double(2.5), Ghost_EtaMax = cms.double(2.5),
         src = cms.InputTag("pfNoElectronAK5") )

# ---------------- Sequence AK5 ----------------------


process.eIdSequence = cms.Sequence()

# PFBRECO+PAT sequence 1:
# no lepton cleaning, AK5PFJets

postfixAK5 = "AK5"
jetAlgoAK5="AK5"

#COLIN : we will need to add the L2L3Residual when they become available! also check the other calls to the usePFBRECO function. 
usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgoAK5, runOnMC=runOnMC, postfix=postfixAK5,
          jetCorrections=('AK5PFchs', jetCorrections))

def removeUseless( modName ):
    getattr(process,"patDefaultSequence"+postfixAK5).remove(
        getattr(process, modName+postfixAK5)
        )

removeUseless( "produceCaloMETCorrections" )
removeUseless( "pfCandsNotInJet" )
removeUseless( "pfJetMETcorr" )
removeUseless( "pfCandMETcorr" )
removeUseless( "pfchsMETcorr" )
removeUseless( "pfType1CorrectedMet" )
removeUseless( "pfType1p2CorrectedMet" )

# removing stupid useless stuff from our muons:
getattr(process,"patMuons"+postfixAK5).embedCaloMETMuonCorrs = False 
getattr(process,"patMuons"+postfixAK5).embedTcMETMuonCorrs = False
#but embed the tracker track for cutting on 
getattr(process,"patMuons"+postfixAK5).embedTrack = True

# removing default cuts on muons 
getattr(process,"pfMuonsFromVertexAK5").dzCut = 99
getattr(process,"pfMuonsFromVertexAK5").d0Cut = 99
getattr(process,"pfSelectedMuonsAK5").cut="pt()>3"

# removing default cuts on electrons 
getattr(process,"pfElectronsFromVertexAK5").dzCut = 99
getattr(process,"pfElectronsFromVertexAK5").d0Cut = 99
getattr(process,"pfSelectedElectronsAK5").cut="pt()>5"


if doJetPileUpCorrection:
    from CommonTools.ParticleFlow.Tools.enablePileUpCorrection import enablePileUpCorrection
    enablePileUpCorrection( process, postfix=postfixAK5)
    # avoid double calculation of rho (introduced by jetTools in PAT)
    getattr(process,'patDefaultSequence'+postfixAK5).remove(getattr(process,'ak5PFJets'+postfixAK5))
    getattr(process,'patDefaultSequence'+postfixAK5).remove(getattr(process,'kt6PFJets'+postfixAK5))
    getattr(process,'patDefaultSequence'+postfixAK5).remove(getattr(process,'kt6PFJets'+postfixAK5))
    getattr(process,"patJetCorrFactors"+postfixAK5).rho = cms.InputTag("kt6PFJets", "rho")

#configure the taus
print "*************reconfiguring the taus*********"
from CMGTools.Common.PAT.tauTools import *
#  note that the following disables the tau cleaning in patJets, tau id dies at |eta| >3.0  
adaptSelectedPFJetForHPSTau(process,jetSelection="pt()>15.0 && abs(eta())<3.0",postfix=postfixAK5)
# non-isolated taus are currently needed for Higgs-> tau tau
removeHPSTauIsolation(process,postfix=postfixAK5)
# apply pT cut on the reconstructed taus to save cpu and patTuple size (cut must be consistent with above cut on Jets)
applyPostfix(process,"pfTausBaseDiscriminationByDecayModeFinding",postfixAK5).minTauPt = 15.0
# in 5_2 tau input collection to PAT was switched to unselected taus so PAT sequence consumes too much cpu and patTuple blows up 
switchInputTausToPAT(process,postfix=postfixAK5)
# embed just the tracks (PF candidates are saved in the patTuple)
embedTracksInTaus(process,postfix=postfixAK5,enable=True)
embedPFCandidatesInTaus(process,postfix=postfixAK5,enable=False)
   
# curing a weird bug in PAT..
from CMGTools.Common.PAT.removePhotonMatching import removePhotonMatching
removePhotonMatching( process, postfixAK5 )

getattr(process,"pfNoMuon"+postfixAK5).enable = False 
getattr(process,"pfNoElectron"+postfixAK5).enable = False 
getattr(process,"pfNoTau"+postfixAK5).enable = False 
getattr(process,"pfNoJet"+postfixAK5).enable = True
getattr(process,"pfIsolatedMuons"+postfixAK5).isolationCut = 999999
getattr(process,"pfIsolatedElectrons"+postfixAK5).isolationCut = 999999

# insert the PFMET sifnificance calculation
from CMGTools.Common.PAT.addMETSignificance_cff import addMETSig
addMETSig( process, postfixAK5 )

from  CMGTools.External.pujetidsequence_cff import puJetId
process.puJetIdAK5 = puJetId.clone( jets = 'selectedPatJetsAK5')
process.patDefaultSequenceAK5.replace( process.selectedPatJetsAK5,
                                       process.selectedPatJetsAK5 + process.puJetIdAK5 )

# %%%%%%%%%%%%%%%%%%%%%%%%%

# adding "standard muons and electons"
# (made from all reco muons, and all gsf electrons, respectively)
process.patMuons.embedTcMETMuonCorrs = False
process.patMuons.embedCaloMETMuonCorrs = False
process.patMuons.embedTrack = True
process.patElectrons.pfElectronSource = 'particleFlow'
process.eleIsoSequence = setupPFElectronIso(process, 'gsfElectrons', 'PFIso')
process.muIsoSequence = setupPFMuonIso(process, 'muons', 'PFIso')
adaptPFIsoMuons( process, applyPostfix(process,"patMuons",""), 'PFIso')
adaptPFIsoElectrons( process, applyPostfix(process,"patElectrons",""), 'PFIso')
process.stdMuonSeq = cms.Sequence(
    process.pfParticleSelectionSequence +
    process.muIsoSequence +
    process.makePatMuons +
    process.selectedPatMuons
    )
process.stdElectronSeq = cms.Sequence(
    process.pfParticleSelectionSequence +
    process.eleIsoSequence +
    process.makePatElectrons +
    process.selectedPatElectrons
    )

if not runOnMC:
    process.stdMuonSeq.remove( process.muonMatch )
    process.stdElectronSeq.remove( process.electronMatch )
    process.patMuons.embedGenMatch = False
    process.patElectrons.embedGenMatch = False

# adding vbtf and cic electron IDs to both electron collections
from CMGTools.Common.PAT.addPATElectronID_cff import addPATElectronID
addPATElectronID( process, 'patDefaultSequence', postfixAK5)
addPATElectronID( process, 'stdElectronSequence', '')

# adding MIT electron id
from CMGTools.Common.PAT.addMITElectronID import addMITElectronID
#COLIN removing MIT MVA, can't run in 52
# addMITElectronID( process, 'selectedPatElectrons', 'patDefaultSequence', postfixAK5)
# addMITElectronID( process, 'selectedPatElectrons', 'stdElectronSeq', '')






# # adding custom detector based iso deposit ---> !!! this works only on V4 event content !!!
from RecoLocalCalo.EcalRecAlgos.EcalSeverityLevelESProducer_cfi import *
from CMGTools.Common.PAT.addLeptCustomIsoDeposit_cff import addMuonCustomIsoDeposit
from CMGTools.Common.PAT.addLeptCustomIsoDeposit_cff import addElectronCustomIsoDeposit

# COLIN REMOVED CANNOT RUN
# uncomment -> segv 
addMuonCustomIsoDeposit( process, 'patDefaultSequence', postfixAK5)
addMuonCustomIsoDeposit( process, 'stdMuonSeq', '')
addElectronCustomIsoDeposit( process, 'patDefaultSequence', postfixAK5)
addElectronCustomIsoDeposit( process, 'stdElectronSeq', '')


#add pat conversions
process.patConversions = cms.EDProducer("PATConversionProducer",
    electronSource = cms.InputTag("selectedPatElectrons")  
)


# ---------------- Jet substructure sequence ---------------

if runJetSubstructure:
    from CMGTools.Common.PAT.addJetSubstructure_cff import *
    addJetSubstructureSequence( process , jetCorrections , runOnMC )

#Jose: electrons not supported by our V4 samples -> removing them completely !!
if runOnV4:
    print "!!!!!!!!!!!!!!!! removing pat electrons completely for V4 "
    getattr(process,"patDefaultSequence"+postfixAK5).remove(getattr(process,"patElectronIDSequence"))
    getattr(process,"patDefaultSequence"+postfixAK5).remove(getattr(process,"patElectronIsoDeposit"))
    getattr(process,"patDefaultSequence"+postfixAK5).remove(getattr(process,"patElectrons"+postfixAK5))
    getattr(process,"patDefaultSequence"+postfixAK5).remove(getattr(process,"selectedPatElectrons"+postfixAK5))
    getattr(process,"patDefaultSequence"+postfixAK5).remove(getattr(process,"countPatElectrons"+postfixAK5))
    getattr(process,"patDefaultSequence"+postfixAK5).remove(getattr(process,"countPatLeptons"+postfixAK5)) 

# ---------------- Sequence AK5NoPUSub, pfNoPileUp switched off ---------------

# PFBRECO+PAT sequence 2:
# pfNoPileUp switched off, AK5PFJets. This sequence is a clone of the AK5 sequence defined previously.

if runAK5NoPUSub:
    print 'Preparing AK5NoPUSub sequence...',

    postfixNoPUSub = 'NoPUSub'
    postfixAK5NoPUSub = postfixAK5+postfixNoPUSub

    from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet
    cloneProcessingSnippet(process, getattr(process, 'patPF2PATSequence'+postfixAK5), postfixNoPUSub)

    getattr(process,"pfNoPileUp"+postfixAK5NoPUSub).enable = False
    getattr(process,"patJetCorrFactors"+postfixAK5NoPUSub).payload = "AK5PF"
    
    # disable embedding of genjets in PAT jets to avoid duplication of the genjet collection
    if runOnMC:
        process.patJetsAK5.embedGenJetMatch=False
        process.patJetsAK5NoPUSub.embedGenJetMatch=False
        process.patJetGenJetMatchAK5NoPUSub.matched=cms.InputTag("ak5GenJetsNoNu")
        getattr(process,"patDefaultSequence"+postfixAK5NoPUSub).remove(getattr(process,"genForPF2PATSequence"+postfixNoPUSub))
    # disable embedding of PFparticles in PAT jets to avoid duplication of the PFparticles collection
    process.pfJetsAK5NoPUSub.src=cms.InputTag("particleFlow")
    process.pfNoJetAK5NoPUSub.bottomCollection=cms.InputTag("particleFlow")
    process.pfTauPFJets08RegionAK5NoPUSub.pfSrc=cms.InputTag("particleFlow")
    process.pfTauTagInfoProducerAK5NoPUSub.PFCandidateProducer=cms.InputTag("particleFlow")
    process.pfTausBaseAK5NoPUSub.builders[0].pfCandSrc=cms.InputTag("particleFlow")
    process.patJetsAK5.embedPFCandidates=False
    process.patJetsAK5NoPUSub.embedPFCandidates=False
    # do not rereconstruct standard ak5PFJets if available in PFAOD
    if not runOnV4:
        process.PFBRECOAK5NoPUSub.remove(process.pfJetSequenceAK5NoPUSub)
        process.patJetsAK5NoPUSub.jetSource = cms.InputTag("ak5PFJets")
        process.patJetCorrFactorsAK5NoPUSub.src = cms.InputTag("ak5PFJets")
	process.jetTracksAssociatorAtVertexAK5NoPUSub.jets = cms.InputTag("ak5PFJets")
	process.pfJetsForHPSTauAK5NoPUSub.src = cms.InputTag("ak5PFJets")
	process.pfMETAK5NoPUSub.jets = cms.InputTag("ak5PFJets")
	process.softMuonTagInfosAODAK5NoPUSub.jets = cms.InputTag("ak5PFJets")
	process.PFMETSignificanceAK5NoPUSub.inputPFJets = cms.InputTag("ak5PFJets")
	if runOnMC:
            process.patJetGenJetMatchAK5NoPUSub.src = cms.InputTag("ak5PFJets")
            process.patJetPartonMatchAK5NoPUSub.src = cms.InputTag("ak5PFJets")
            process.patJetPartonAssociationAK5NoPUSub.jets = cms.InputTag("ak5PFJets")
    
    print 'Done'

# ---------------- Common stuff ---------------

process.load('CMGTools.Common.gen_cff')


process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")
process.patTrigger.processName = cms.string('*')

### PATH DEFINITION #############################################

# counters that can be used at analysis level to know the processed events
process.prePathCounter = cms.EDProducer("EventCountProducer")
process.postPathCounter = cms.EDProducer("EventCountProducer")

# trigger information (no selection)

process.p = cms.Path( process.prePathCounter + process.patTriggerDefaultSequence )
# make kt6PFjets if not available in PFAOD
if runOnV4:
    process.p += process.kt6PFJets

# PFBRECO+PAT ---

process.p += getattr(process,"patPF2PATSequence"+postfixAK5)

process.p += process.kt6PFJetsCHS
process.p += process.kt6PFJetsForIso
process.p += process.kt6PFJetsCHSForIso

process.stdLeptonSequence = cms.Sequence(
    process.stdMuonSeq
    + process.stdElectronSeq 
    )

process.p += process.stdLeptonSequence
process.p += process.patConversions

if runJetSubstructure:
    process.p += process.jetSubstructureSequence

if runAK5NoPUSub:
    process.p += getattr(process,"patPF2PATSequence"+postfixAK5NoPUSub)


#Mike:Adding MIT MVA Isolation from muons
process.load('CMGTools.Common.PAT.patMuonMVAIsolation_cff')
process.p+=process.muonMVAIsolation





#Jose: electrons not supported by our V4 samples -> removing them completely !!
#Cristina: removing as well the conversions that require the electrons and the V5 reco conversions collection
if runOnV4:
    print "!!!!!!!!!!!!!!!! removing standard pat electrons completely for V4 "
    getattr(process,"stdLeptonSequence").remove(getattr(process,"stdElectronSeq"))
    getattr(process,"").remove(getattr(process,"patConversions"))


# event cleaning (in tagging mode, no event rejected)

process.load('CMGTools.Common.eventCleaning.eventCleaning_cff')

process.p += process.eventCleaningSequence

# gen ---- 

if runOnMC:
    process.p += process.genSequence 

process.p += getattr(process,"postPathCounter") 
 
# CMG ---

if runCMG:
    
    process.load('CMGTools.Common.analysis_cff')
    # running on PFAOD -> calo objects are not available.
    # we'll need to reactivate caloMET, though
    # process.p += process.analysisSequence

    from CMGTools.Common.Tools.visitorUtils import replacePostfix
    
    from CMGTools.Common.Tools.tuneCMGSequences import * 
    tuneCMGSequences(process, postpostfix='CMG')

    process.cmgPUJetMva.jetids = 'puJetIdAK5'
    process.cmgPFJet.cfg.puVariables = 'puJetIdAK5'

    process.p += process.analysisSequence

    from CMGTools.Common.PAT.addStdLeptons import addCmgMuons, addCmgElectrons
    process.cmgStdLeptonSequence = cms.Sequence(
        addCmgMuons( process, 'StdLep', 'selectedPatMuons'  )
        + addCmgElectrons( process, 'StdLep', 'selectedPatElectrons'  ) 
        )
    #COLIN REMOVED CANNOT RUN
    process.cmgObjectSequence += process.cmgStdLeptonSequence

    #Jose: electrons not supported by our V4 samples -> removing them completely !!
    if runOnV4:
        print "!!!!!!!!!!!!!!!! removing cmg electrons completely for V4 "
        getattr(process,"cmgObjectSequence").remove(getattr(process,"electronSequence")) 
        getattr(process,"skimSequence").remove(getattr(process,"electronSkimSequence")) 
        getattr(process,"collectionSizeSequence").remove(getattr(process,"electronSelSize"))
        getattr(process,"cmgStdLeptonSequence").remove(getattr(process,"electronSequenceStdLep"))
        getattr(process,"cutSummarySequence").remove(getattr(process,"electronCutSummarySequence"))
        
    if runAK5NoPUSub:
        cloneProcessingSnippet(process, getattr(process, 'analysisSequence'), 'AK5NoPUSubCMG')
        replacePostfix(getattr(process,"analysisSequenceAK5NoPUSubCMG"),'AK5','AK5NoPUSub') 
        process.p += process.analysisSequenceAK5NoPUSubCMG




### OUTPUT DEFINITION #############################################

# PFBRECO+PAT ---

# Add PFBRECO output to the created file
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

# add counters to the pat-tuple
process.out.outputCommands.extend(['keep edmMergeableCounter_*_*_*',
                                   'keep *_patConversions_*_*'])

if runJetSubstructure:
    # keep collections needed for jet substructure studies
    process.out.outputCommands.extend(['keep recoPFJets_ak5PrunedPFlow_SubJets_*'])
    process.out.outputCommands.extend(['keep patJets_selectedPatJetsAK5PrunedPF_*_*'])
    # save only one genjet collection instead of the two embedded in pat jets
    process.out.outputCommands.extend(['drop recoGenJets_selectedPatJetsAK5PrunedPF_genJets_*'])
    # save only one PFparticles collection instead of the two embedded in pat jets
    process.out.outputCommands.extend(['drop recoPFCandidates_selectedPatJetsAK5PrunedPF_pfCandidates_*'])

if runAK5NoPUSub:
    # save only one genjet collection instead of the two embedded in pat jets
    process.out.outputCommands.extend(['drop recoGenJets_selectedPatJetsAK5_genJets_*'])
    process.out.outputCommands.extend(['drop recoGenJets_selectedPatJetsAK5NoPUSub_genJets_*'])
    process.out.outputCommands.extend(['keep recoGenJets_ak5GenJetsNoNu_*_*'])
    # save only one PFparticles collection instead of the two embedded in pat jets
    process.out.outputCommands.extend(['drop recoPFCandidates_selectedPatJetsAK5_pfCandidates_*'])
    process.out.outputCommands.extend(['drop recoPFCandidates_selectedPatJetsAK5NoPUSub_pfCandidates_*'])
    process.out.outputCommands.extend(['keep recoPFCandidates_pfNoElectronAK5_*_*'])


# CMG ---

process.outcmg = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('tree_CMG.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    outputCommands = everything,
    dropMetaData = cms.untracked.string('PRIOR')
    )
process.outcmg.outputCommands.extend( [ # 'keep patMuons_selectedPatMuonsAK5*_*_*',
                                        # 'keep patElectrons_selectedPatElectronAK5*_*_*',
                                       'keep edmMergeableCounter_*_*_*'] )

if runCMG:
    process.outpath += process.outcmg


#Patrick: Make the embedded track available for electrons (curing a bug in PAT)
process.patElectronsAK5.embedTrack = True
process.patElectrons.embedTrack = True


# COLIN : commenting the calibration out (leads to a value map crash in patElectrons )

## #electron energy-scale corrections (from Claude C., Paramatti & al.)
## print sep_line
## print "Replacing gsfElectrons with calibratedGsfElectrons..."
## for modulename in process.p.moduleNames():
##     module = getattr(process, modulename)
##     ml = dir(module)
##     for attr in ml:
##         v = getattr(module,attr)
##         if (isinstance(v, cms.InputTag) and v == cms.InputTag("gsfElectrons")):
##             setattr(module,attr,"calibratedGsfElectrons")
##             #print "Setting ",
##             #print module,
##             #print ".", 
##             #print attr,
##             #print " = ", 
##             #print getattr(module,attr)
## process.load("EgammaCalibratedGsfElectrons.CalibratedElectronProducers.calibratedGsfElectrons_cfi")
## process.calibratedGsfElectrons.isMC = cms.bool(runOnMC)
## process.calibratedGsfElectrons.updateEnergyError = cms.bool(True)

## process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",

##     # Include a PSet for each module label that needs a
##     # random engine.  The name is the module label.
##     # You must supply a seed or seeds.
##     # Optionally an engine type can be specified
##     #t1 = cms.PSet(
##     #    initialSeed = cms.untracked.uint32(81)
##     #),
##     calibratedGsfElectrons = cms.PSet(
##         initialSeed = cms.untracked.uint32(1),
##         engineName = cms.untracked.string('TRandom3')
##     ),
## )

## from CMGTools.Common.Tools.setupEleEnergyCorrections import setupEleEnergyCorrections
## setupEleEnergyCorrections(process)

## print "Setting process.calibratedGsfElectrons.inputDataset=",
## print process.calibratedGsfElectrons.inputDataset

## process.PFBRECOAK5.replace(process.goodOfflinePrimaryVertices,
##                            process.goodOfflinePrimaryVertices+
##                            process.calibratedGsfElectrons
##                            )

#COLIN removing MIT ID, can't run in 52
process.selectedPatMuonsAK5.cut = 'pt()>3'
process.selectedPatElectronsAK5.cut = 'pt()>5'
# process.preselectedPatElectronsAK5.cut = 'pt()>5'
process.selectedPatMuons.cut = 'pt()>3'
process.selectedPatElectrons.cut = 'pt()>5'
# process.preselectedPatElectrons.cut = 'pt()>5'

if runOnMC is False:
    print 'OVERRIDING datasetToSource TO TEST RUNNING ON DATA'
    process.source.fileNames = ['/store/data/Run2012A/DoubleMu/AOD/PromptReco-v1/000/191/859/66D9EE0B-EC8C-E111-9346-001D09F2AD84.root']
