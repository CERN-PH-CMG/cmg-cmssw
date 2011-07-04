import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.pfTools import *
from PhysicsTools.PatAlgos.tools.trigTools import *
from RecoJets.Configuration.RecoPFJets_cff import kt6PFJets
from CommonTools.ParticleFlow.Tools.enablePileUpCorrection import enablePileUpCorrection
from PhysicsTools.PatAlgos.tools.trackTools import *

##
## add trigger matching for the leptons
##
def addTriggerMatchingForLeptons(process, postfix='') :
    # define the trigger matchers
    process.muTriggerMatchPF = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                               src     = cms.InputTag( "selectedPatMuons"+postfix ),
                                               matched = cms.InputTag( "patTrigger" ),
                                               matchedCuts = cms.string( 'type( "TriggerMuon" ) && ( path("HLT_Mu8_*") || path("HLT_Mu12_*") || path("HLT_Mu13_*") || path("HLT_DoubleMu7_*") )' ), 
                                               maxDPtRel   = cms.double( 0.5 ), # no effect here
                                               maxDeltaR   = cms.double( 0.5 ),
                                               maxDeltaEta = cms.double( 0.2 ), # no effect here
                                               # definition of matcher output
                                               resolveAmbiguities    = cms.bool( False ),
                                               resolveByMatchQuality = cms.bool( False )
                                               )
    
    process.eleTriggerMatchPF = cms.EDProducer( "PATTriggerMatcherDRLessByR",
                                                src     = cms.InputTag( "selectedPatElectrons"+postfix ),
                                                matched = cms.InputTag( "patTrigger" ),
                                                #matchedCuts = cms.string( 'type( "TriggerL1NoIsoEG" ) || type( "TriggerL1IsoEG" ) || type( "TriggerElectron" )' ),
                                                matchedCuts = cms.string( 'type( "TriggerElectron" )' ),
                                                maxDPtRel   = cms.double( 0.5 ), # no effect here
                                                maxDeltaR   = cms.double( 0.5 ),
                                                maxDeltaEta = cms.double( 0.2 ), # no effect here
                                                # definition of matcher output
                                                resolveAmbiguities    = cms.bool( False ),
                                                resolveByMatchQuality = cms.bool( False )
                                                )

    from PhysicsTools.PatAlgos.tools.coreTools import removeCleaning
    removeCleaning( process )
    setattr( process, 'muTriggerMatch' + postfix, process.muTriggerMatchPF )
    setattr( process, 'eleTriggerMatch' + postfix, process.eleTriggerMatchPF )
    switchOnTriggerMatching( process, triggerMatchers = [ 'muTriggerMatchPFlow','eleTriggerMatchPFlow' ], sequence = 'patPF2PATSequence' + postfix )
    removeCleaningFromTriggerMatching( process, sequence = 'patPF2PATSequence' + postfix )

##
## adds pat sequence
##
def addPatSequence(process, runOnMC, addPhotons=False) :
    
    postfix = "PFlow"

    #jet energy corrections
    # cf. https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookJetEnergyCorrections#JetEnCor2011V2
    # cf. https://twiki.cern.ch/twiki/bin/viewauth/CMS/JECDataMC
    # please create a link to the JEC db wherever you run the job
    # do not forget to add it to the crab file under [USER] with additional_input_files  = JECxxxx.db
    jetAlgo='AK5'
    jecSetPF = jetAlgo+'PFchs'
    #jecSetPF = jetAlgo+'PF'
    jecLevels=['L1FastJet','L2Relative','L3Absolute']
    if(not runOnMC) : jecLevels.append( 'L2L3Residual' )
    process.load("CondCore.DBCommon.CondDBCommon_cfi")
    process.jec = cms.ESSource("PoolDBESSource",
                               DBParameters = cms.PSet( messageLevel = cms.untracked.int32(0) ),
                               timetype = cms.string('runnumber'),
                               toGet = cms.VPSet( cms.PSet(record = cms.string('JetCorrectionsRecord'),
                                                           tag    = cms.string('JetCorrectorParametersCollection_Jec11V2_AK5PF'),
                                                           label  = cms.untracked.string('AK5PF')
                                                           )
                                                  ),
                               connect = cms.string('sqlite:Jec11V2.db')
                               )
    # Add an es_prefer statement to get your new JEC constants from the sqlite file, rather than from the global tag
    process.es_prefer_jec = cms.ESPrefer('PoolDBESSource','jec')


    #start PF2PAT
    usePF2PAT(process,
              runPF2PAT=True,
              runOnMC=runOnMC,
              jetAlgo=jetAlgo,
              postfix=postfix,
              jetCorrections=(jecSetPF, jecLevels)
              )
    enablePileUpCorrection( process, postfix=postfix )

    #disable mc matching for photons
    removeMCMatching(process,names=['Photons'],postfix=postfix)
    process.patElectronsPFlow.embedTrack=True
    process.patMuonsPFlow.embedTrack=True
           
    #configure top projections
    getattr(process,"pfNoPileUp"+postfix).enable = True
    getattr(process,"pfNoMuon"+postfix).enable = False #True
    getattr(process,"pfNoMuon"+postfix).verbose = False
    getattr(process,"pfNoElectron"+postfix).enable = False #True
    getattr(process,"pfNoTau"+postfix).enable = False
    getattr(process,"pfNoJet"+postfix).enable = False

    #fix isolation to use a cone of 0.3 for both electrons and muons
    applyPostfix(process,"isoValMuonWithNeutral",postfix).deposits[0].deltaR = cms.double(0.3)
    applyPostfix(process,"isoValMuonWithCharged",postfix).deposits[0].deltaR = cms.double(0.3)
    applyPostfix(process,"isoValMuonWithPhotons",postfix).deposits[0].deltaR = cms.double(0.3)
    applyPostfix(process,"pfIsolatedMuons",postfix).combinedIsolationCut = cms.double(9999.)
    applyPostfix(process,"isoValElectronWithNeutral",postfix).deposits[0].deltaR = cms.double(0.3)
    applyPostfix(process,"isoValElectronWithCharged",postfix).deposits[0].deltaR = cms.double(0.3)
    applyPostfix(process,"isoValElectronWithPhotons",postfix).deposits[0].deltaR = cms.double(0.3)
    applyPostfix(process,"pfIsolatedElectrons",postfix).combinedIsolationCut = cms.double(9999.)
    
    #CiC electron ID
    process.load( "RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentificationV06_cfi" )
    process.eidCiCSequence = cms.Sequence(
        process.eidVeryLooseMC
        + process.eidLooseMC
        + process.eidMediumMC
        + process.eidTightMC
        + process.eidSuperTightMC
        + process.eidHyperTight1MC
        )
    applyPostfix( process, 'patElectrons', postfix ).electronIDSources = cms.PSet(
        eidVeryLooseMC = cms.InputTag("eidVeryLooseMC"),
        eidLooseMC = cms.InputTag("eidLooseMC"),
        eidMediumMC = cms.InputTag("eidMediumMC"),
        eidTightMC = cms.InputTag("eidTightMC"),
        eidSuperTightMC = cms.InputTag("eidSuperTightMC"),
        eidHyperTight1MC = cms.InputTag("eidHyperTight1MC")
        )

    #add trigger match
    addTriggerMatchingForLeptons(process,postfix=postfix)

    #puffo met
    process.load("WWAnalysis.Tools.chargedMetProducer_cfi")
    process.chargedMetProducer.collectionTag = cms.InputTag("pfNoPileUpPFlow")
                                    
    if(addPhotons) :
        # temporarily use std photons (switch to PF in 43x cf. with Daniele how to do it)
        process.load('PhysicsTools.PatAlgos.producersLayer1.photonProducer_cff')
        process.patPhotons.addGenMatch=cms.bool(False)

        #create the path
        process.patDefaultSequence = cms.Sequence(
            process.eidCiCSequence*
            getattr(process,"patPF2PATSequence"+postfix)*
            process.chargedMetProducer*
            process.patPhotons
            )
    else :
        process.patDefaultSequence = cms.Sequence(
            process.eidCiCSequence*
            getattr(process,"patPF2PATSequence"+postfix)*
            process.chargedMetProducer
            )
        
    # make pat-tracks (does not work with PF2PAT...)
    # it is better to use isolated PF candidates from the pfNoPileup collection
    #    makeTrackCandidates(process,
    #                        label        = 'TrackCands',
    #                        tracks       = cms.InputTag('generalTracks'),
    #                        particleType = 'pi+',
    #                        preselection = 'pt > 10',
    #                        selection    = 'pt > 10',
    #                        isolation    = {'tracker':0.3, 'ecalTowers':0.3, 'hcalTowers':0.3},
    #                        isoDeposits  = [],
    #                        mcAs         = None
    #                        )      
  
    print " *** PAT path has been defined"
    


