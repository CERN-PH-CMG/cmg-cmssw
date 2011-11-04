import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.pfTools import *
from PhysicsTools.PatAlgos.tools.trigTools import *
from RecoJets.Configuration.RecoPFJets_cff import kt6PFJets
from CommonTools.ParticleFlow.Tools.enablePileUpCorrection import enablePileUpCorrection
from PhysicsTools.PatAlgos.tools.trackTools import *
from PhysicsTools.PatAlgos.tools.metTools import *
    
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
def addPatSequence(process, runOnMC, addPhotons=True) :

    #PF2PAT
    postfix = "PFlow"
    
    process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

    #jet energy corrections
    # cf. https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookJetEnergyCorrections#JetEnCor2011V2
    # cf. https://twiki.cern.ch/twiki/bin/viewauth/CMS/JECDataMC
    jetAlgo='AK5'
    jecSetPF = jetAlgo+'PFchs'
    jecLevels=['L1FastJet','L2Relative','L3Absolute']
    if(not runOnMC) : jecLevels.append( 'L2L3Residual' )

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
    getattr(process,"pfNoMuon"+postfix).enable = True
    getattr(process,"pfNoMuon"+postfix).verbose = False
    getattr(process,"pfNoElectron"+postfix).enable = True
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
    
    #electron ID
    process.load("ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff")
    process.load("RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentificationV06_DataTuning_cfi")
    process.load("RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentificationV06_cfi")
    process.electronIDSequence = cms.Sequence(
        process.simpleEleIdSequence +
        process.eidVeryLoose +
        process.eidLoose +
        process.eidMedium +
        process.eidTight +
        process.eidSuperTight+
        process.eidVeryLooseMC +
        process.eidLooseMC +
        process.eidMediumMC +
        process.eidTightMC +
        process.eidSuperTightMC
        )

    applyPostfix( process, 'patElectrons', postfix ).electronIDSources = cms.PSet(
        eidVBTF95 = cms.InputTag("simpleEleId95relIso"),
        eidVBTF90 = cms.InputTag("simpleEleId90relIso"),
        eidVBTF85 = cms.InputTag("simpleEleId85relIso"),
        eidVBTF80 = cms.InputTag("simpleEleId80relIso"),
        eidVBTF70 = cms.InputTag("simpleEleId70relIso"),
        eidVBTF60 = cms.InputTag("simpleEleId60relIso"),
        eidVeryLoose = cms.InputTag("eidVeryLoose"),
        eidLoose = cms.InputTag("eidLoose"),
        eidMedium = cms.InputTag("eidMedium"),
        eidTight = cms.InputTag("eidTight"),
        eidSuperTight = cms.InputTag("eidSuperTight"),
        eidVeryLooseMC = cms.InputTag("eidVeryLooseMC"),
        eidLooseMC = cms.InputTag("eidLooseMC"),
        eidMediumMC = cms.InputTag("eidMediumMC"),
        eidTightMC = cms.InputTag("eidTightMC"),
        eidSuperTightMC = cms.InputTag("eidSuperTightMC")       
        )

    #add secondary vertex mass to jets
    applyPostfix( process, 'patJets', postfix ).tagInfoSources = cms.VInputTag( cms.InputTag("secondaryVertexTagInfosAOD"+postfix) )
    applyPostfix( process, 'patJets', postfix ).userData.userFunctions = cms.vstring( "? hasTagInfo('secondaryVertex') && tagInfoSecondaryVertex('secondaryVertex').nVertices() > 0 ? tagInfoSecondaryVertex('secondaryVertex').secondaryVertex(0).p4().mass() : -999")
    applyPostfix( process, 'patJets', postfix ).userData.userFunctionLabels = cms.vstring('secvtxMass')
    
    #add trigger match
    addTriggerMatchingForLeptons(process,postfix=postfix)

    #puffo met
    process.load("WWAnalysis.Tools.chargedMetProducer_cfi")
    process.chargedMetProducer.collectionTag = cms.InputTag("particleFlow")
    process.trackMetProducer = process.chargedMetProducer.clone(minNeutralPt = 99999., maxNeutralEta = 0)
    
    #alternative met collections
    process.pfMETPFlowNoPileup = process.pfMETPFlow.clone(src=cms.InputTag("pfNoPileUpPFlow"))
    process.patMETsPFlowNoPileup = process.patMETsPFlow.clone(metSource=cms.InputTag("pfMETPFlowNoPileup"))

    process.pfMETPFlowPileup = process.pfMETPFlow.clone(jets=cms.InputTag("ak5PFJets"))
    process.patMETsPFlowPileup = process.patMETsPFlow.clone(metSource=cms.InputTag("pfMETPFlowPileup"))

    process.hzzmetSequence = cms.Sequence(process.chargedMetProducer*
                                          process.trackMetProducer*
                                          process.pfMETPFlowNoPileup*process.patMETsPFlowNoPileup*
                                          process.pfMETPFlowPileup*process.patMETsPFlowPileup)
    
    if(addPhotons) :
        # temporarily use std photons (switch to PF in 43x cf. with Daniele how to do it)
        process.load('PhysicsTools.PatAlgos.producersLayer1.photonProducer_cff')
        process.patPhotons.addGenMatch=cms.bool(False)

        #create the path
        process.patDefaultSequence = cms.Sequence(
            process.electronIDSequence*
            getattr(process,"patPF2PATSequence"+postfix)*
            process.hzzmetSequence*
            process.patPhotons
            )
    else :
        process.patDefaultSequence = cms.Sequence(
            process.electronIDSequence*
            getattr(process,"patPF2PATSequence"+postfix)*
            process.hzzmetSequence
            )
        

    print " *** PAT path has been defined"
    


