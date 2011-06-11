import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.pfTools import *
from RecoJets.Configuration.RecoPFJets_cff import kt6PFJets

##
## adds pat sequence
##
def addPatSequence(p, runOnMC) :
    
    postfix = "PFlow"

    #jet energy corrections
    jecSetPF = 'AK5PFchs'
    jecLevels=['L1FastJet','L2Relative','L3Absolute']
    #if(not runOnMC) : jecLevels.append( 'L2L3Residual' )
    
    #start PF2PAT
    usePF2PAT(process,
              runPF2PAT=True,
              runOnMC=runOnMC,
              jetAlgo=jetAlgo,
              postfix=postfix,
              jetCorrections=(jecSetPF, jecLevels)
              )

    #configure top projections
    getattr(process,"pfNoPileUp"+postfix).enable = True
    getattr(process,"pfNoMuon"+postfix).enable = True
    getattr(process,"pfNoElectron"+postfix).enable = True
    getattr(process,"pfNoTau"+postfix).enable = False
    getattr(process,"pfNoJet"+postfix).enable = True
    getattr(process,"pfNoMuon"+postfix).verbose = False

    #fix isolation to use a cone of 0.3 for both electrons and muons
    applyPostfix(process,"isoValMuonWithNeutral",postfix).deposits[0].deltaR = cms.double(0.3)
    applyPostfix(process,"isoValMuonWithCharged",postfix).deposits[0].deltaR = cms.double(0.3)
    applyPostfix(process,"isoValMuonWithPhotons",postfix).deposits[0].deltaR = cms.double(0.3)
    applyPostfix(process,"isoValElectronWithNeutral",postfix).deposits[0].deltaR = cms.double(0.3)
    applyPostfix(process,"isoValElectronWithCharged",postfix).deposits[0].deltaR = cms.double(0.3)
    applyPostfix(process,"isoValElectronWithPhotons",postfix).deposits[0].deltaR = cms.double(0.3)

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
    
    # match the trigger information for leptons
    process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")
    process.patTrigger.onlyStandAlone = True
    process.patTrigger.processName  = '*'
    process.patTriggerEvent.processName = '*'
    
    #muons
    process.muonTriggerMatchHLT = cms.EDProducer( 'PATTriggerMatcherDRDPtLessByR',
                                                  src     = cms.InputTag( 'patMuons'+postfix ),
                                                  matched = cms.InputTag( 'patTrigger' ),
                                                  matchedCuts = cms.string( 'path( "HLT_*" )' ),
                                                  maxDPtRel = cms.double( 0.5 ),
                                                  maxDeltaR = cms.double( 0.5 ),
                                                  resolveAmbiguities    = cms.bool( True ), 
                                                  resolveByMatchQuality = cms.bool( True )
                                                  )
    process.patMuonsWithTrigger = cms.EDProducer( 'PATTriggerMatchMuonEmbedder',
                                                  src     = cms.InputTag(  'patMuons'+postfix ),
                                                  matches = cms.VInputTag('muonTriggerMatchHLT')
                                                  )
    # electrons
    process.eleTriggerMatchHLT = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
                                                 src     = cms.InputTag( "patElectrons" ),
                                                 matched = cms.InputTag( "patTrigger" ),
                                                 matchedCuts = cms.string( 'path( "HLT_*" )' ),
                                                 maxDPtRel = cms.double( 0.5 ),
                                                 maxDeltaR = cms.double( 0.5 ),
                                                 resolveAmbiguities    = cms.bool( True ),        # only one match per trigger object
                                                 resolveByMatchQuality = cms.bool( True )
                                                 )
    process.patElectronsWithTrigger = cms.EDProducer( "PATTriggerMatchElectronEmbedder",
                                                      src     = cms.InputTag(  "patElectrons" ),
                                                      matches = cms.VInputTag(cms.InputTag('eleTriggerMatchHLT'))
                                                      )
    
    # lepton pre-selector
    process.selectedPatMuons = cms.EDFilter("PATMuonRefSelector",
                                            src = cms.InputTag("patMuonsWithTrigger"),
                                            cut = cms.string("")
                                            )
    process.selectedPatElectrons = cms.EDFilter("PATElectronRefSelector",
                                                src = cms.InputTag("patElectronsWithTrigger"),
                                                cut = cms.string(""),
                                                )
    
    # Energy density per jet area
    process.kt6PFJetsChs = kt6PFJets.clone(
        rParam        = cms.double( 0.6 ),
        src           = cms.InputTag( 'pfNoElectron' ),
        doAreaFastjet = cms.bool( True ),
        doRhoFastjet  = cms.bool( True ),
        voronoiRfact  = cms.double( 0.9 )
        )
    
    #create the path
    process.patPath = cms.Path(
        process.kt6PFJetsChs*
        process.eidCiCSequence*
        getattr(process,"patPF2PATSequence"+postfix)*
        process.muonTriggerMatchHLT*
        process.patMuonsWithTrigger*
        process.selectedPatMuons*
        process.electronTriggerMatchHLT*
        process.patElectronsWithTrigger*
        process.selectedPatElectrons
        )
    print " *** PAT path has been defined"
    
