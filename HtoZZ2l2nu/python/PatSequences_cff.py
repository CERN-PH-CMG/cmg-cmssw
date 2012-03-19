import FWCore.ParameterSet.Config as cms
from PhysicsTools.PatAlgos.tools.pfTools import *
from PhysicsTools.PatAlgos.tools.trigTools import *
from RecoJets.Configuration.RecoPFJets_cff import kt6PFJets
from PhysicsTools.PatAlgos.tools.trackTools import *
from PhysicsTools.PatAlgos.tools.metTools import *
from SHarper.HEEPAnalyzer.HEEPSelectionCuts_cfi import *

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
    removeCleaningFromTriggerMatching( process ) #, sequence = 'patPF2PATSequence' + postfix )
    
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

    #rho computed up to 2.5
    process.load('RecoJets.Configuration.RecoPFJets_cff')
    process.kt6PFJets25 = process.kt6PFJets.clone( doRhoFastjet = True )
    process.kt6PFJets25.Rho_EtaMax = cms.double(2.5)

    #start PF2PAT
    usePF2PAT(process,
              runPF2PAT=True,
              runOnMC=runOnMC,
              jetAlgo=jetAlgo,
              postfix=postfix,
              jetCorrections=(jecSetPF, jecLevels),
              typeIMetCorrections=True
              )

    #compute rho and prepend to the jet clustering sequence in PF2PAT
    from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
    process.kt6PFJets = kt4PFJets.clone( rParam = cms.double(0.6),
                                         doAreaFastjet = cms.bool(True),
                                         doRhoFastjet = cms.bool(True)
                                         )
    getattr(process,'patJetCorrFactors'+postfix).rho = cms.InputTag("kt6PFJets", "rho")
    getattr(process,"patPF2PATSequence"+postfix).replace( getattr(process,"pfNoElectron"+postfix), getattr(process,"pfNoElectron"+postfix)*process.kt6PFJets )

          
    #configure top projections
    getattr(process,"pfNoPileUp"+postfix).enable = True
    getattr(process,"pfNoMuon"+postfix).enable = True
    getattr(process,"pfNoMuon"+postfix).verbose = False
    getattr(process,"pfNoElectron"+postfix).enable = True
    getattr(process,"pfNoTau"+postfix).enable = False
    getattr(process,"pfNoJet"+postfix).enable = False

    #muons
    process.patMuonsPFlow.embedTrack=True
    applyPostfix( process, 'pfIsolatedMuons', postfix ).isolationValueMapsCharged  = cms.VInputTag( cms.InputTag( 'muPFIsoValueCharged03' + postfix ) )
    applyPostfix( process, 'pfIsolatedMuons', postfix ).isolationValueMapsNeutral  = cms.VInputTag( cms.InputTag( 'muPFIsoValueNeutral03' + postfix ),
                                                                                                    cms.InputTag( 'muPFIsoValueGamma03' + postfix ) )
    applyPostfix( process, 'pfIsolatedMuons', postfix ).deltaBetaIsolationValueMap = cms.InputTag( 'muPFIsoValuePU03' + postfix )
    applyPostfix( process, 'patMuons', postfix ).isolationValues.pfNeutralHadrons   = cms.InputTag( 'muPFIsoValueNeutral03' + postfix )
    applyPostfix( process, 'patMuons', postfix ).isolationValues.pfPUChargedHadrons = cms.InputTag( 'muPFIsoValuePU03' + postfix )
    applyPostfix( process, 'patMuons', postfix ).isolationValues.pfPhotons          = cms.InputTag( 'muPFIsoValueGamma03' + postfix )
    applyPostfix( process, 'patMuons', postfix ).isolationValues.pfChargedHadrons   = cms.InputTag( 'muPFIsoValueCharged03' + postfix )
    #applyPostfix(process,"isoValMuonWithNeutral",postfix).deposits[0].deltaR = cms.double(0.3)
    #applyPostfix(process,"isoValMuonWithCharged",postfix).deposits[0].deltaR = cms.double(0.3)
    #applyPostfix(process,"isoValMuonWithPhotons",postfix).deposits[0].deltaR = cms.double(0.3)
    applyPostfix(process,"pfIsolatedMuons",postfix).isolationCut = cms.double(9999.)
        
    #electrons
    process.patElectronsPFlow.embedTrack=True
    applyPostfix( process, 'pfIsolatedElectrons', postfix ).isolationValueMapsCharged  = cms.VInputTag( cms.InputTag( 'elPFIsoValueCharged03' + postfix ) )
    applyPostfix( process, 'pfIsolatedElectrons', postfix ).isolationValueMapsNeutral  = cms.VInputTag( cms.InputTag( 'elPFIsoValueNeutral03' + postfix ),
                                                                                                        cms.InputTag( 'elPFIsoValueGamma03' + postfix ) )
    applyPostfix( process, 'pfIsolatedElectrons', postfix ).deltaBetaIsolationValueMap = cms.InputTag( 'elPFIsoValuePU03' + postfix )
    applyPostfix( process, 'patElectrons', postfix ).isolationValues.pfNeutralHadrons   = cms.InputTag( 'elPFIsoValueNeutral03' + postfix )
    applyPostfix( process, 'patElectrons', postfix ).isolationValues.pfPUChargedHadrons = cms.InputTag( 'elPFIsoValuePU03' + postfix )
    applyPostfix( process, 'patElectrons', postfix ).isolationValues.pfPhotons          = cms.InputTag( 'elPFIsoValueGamma03' + postfix )
    applyPostfix( process, 'patElectrons', postfix ).isolationValues.pfChargedHadrons   = cms.InputTag( 'elPFIsoValueCharged03' + postfix )
    applyPostfix( process, 'isoValElectronWithCharged', postfix ).deposits[0].deltaR = 0.3
    applyPostfix( process, 'isoValElectronWithNeutral', postfix ).deposits[0].deltaR = 0.3
    applyPostfix( process, 'isoValElectronWithPhotons', postfix ).deposits[0].deltaR = 0.3
    applyPostfix(process,"pfIsolatedElectrons",postfix).isolationCut = cms.double(9999.)

    ####################################################
    # ELECTRON IDs                                     #
    ####################################################       
    process.load("ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff")
    process.HEEPId = cms.EDProducer("HEEPIdValueMapProducer",
                                    eleLabel = cms.InputTag("gsfElectrons"),
                                    barrelCuts = cms.PSet(heepBarrelCuts),
                                    endcapCuts = cms.PSet(heepEndcapCuts)
                                    )
    process.HEEPId.barrelCuts.minEt = 5. #reset min et cut to 5 GeV
    process.HEEPId.endcapCuts.minEt = 5. #reset min et cut to 5 GeV 
    
    process.electronIDSequence = cms.Sequence(
        process.HEEPId +
        process.simpleEleIdSequence )

    #electrons
    applyPostfix( process, 'patElectrons', postfix ).electronIDSources = cms.PSet(
        eidHEEP = cms.InputTag("HEEPId"),
        eidVBTF95 = cms.InputTag("simpleEleId95relIso"),
        eidVBTF90 = cms.InputTag("simpleEleId90relIso"),
        eidVBTF85 = cms.InputTag("simpleEleId85relIso"),
        eidVBTF80 = cms.InputTag("simpleEleId80relIso"),
        eidVBTF70 = cms.InputTag("simpleEleId70relIso"),
        eidVBTF60 = cms.InputTag("simpleEleId60relIso")
        )

    #add secondary vertex mass to jets
    applyPostfix( process, 'patJets', postfix ).tagInfoSources = cms.VInputTag( cms.InputTag("secondaryVertexTagInfosAOD"+postfix) )
    applyPostfix( process, 'patJets', postfix ).userData.userFunctions = cms.vstring( "? hasTagInfo('secondaryVertex') && tagInfoSecondaryVertex('secondaryVertex').nVertices() > 0 ? tagInfoSecondaryVertex('secondaryVertex').secondaryVertex(0).p4().mass() : -999")
    applyPostfix( process, 'patJets', postfix ).userData.userFunctionLabels = cms.vstring('secvtxMass')

    removeSpecificPATObjects( process, names=['Taus'], postfix=postfix )
    removeSpecificPATObjects( process, names=['Taus'])

    #################################################
    # CUSTOM PAT                                    #
    #################################################
    process.load("PhysicsTools.PatAlgos.patSequences_cff")
    process.patElectrons.electronIDSources = process.patElectronsPFlow.electronIDSources
    applyPostfix( process, 'patMuons', '' ).embedCaloMETMuonCorrs=cms.bool(False)
    applyPostfix( process, 'patMuons', '' ).embedTcMETMuonCorrs=cms.bool(False)
    switchJetCollection(process,
                        cms.InputTag('ak5PFJets'),
                        jetIdLabel = 'AK5',
                        doJTA        = True,
                        doBTagging   = True,
                        jetCorrLabel = ('AK5PF', jecLevels),
                        doType1MET   = False,
                        genJetCollection=cms.InputTag("ak5GenJets"),
                        doJetID      = True,
                        postfix=''
                        )
    process.patJets.embedPFCandidates = False
     
    #create the path
    process.patSequence = cms.Sequence(
        process.recoPFJets*
        process.kt6PFJets25*
        process.electronIDSequence*
        getattr(process,"patPF2PATSequence"+postfix)*
        process.patDefaultSequence
        )

    removeCleaning( process ) 
    getattr(process,'patSequence').remove( getattr(process,'photonMatch'+postfix) )
    removeSpecificPATObjects( process, names=['Taus','METs'] )
    if(not runOnMC) : removeMCMatching(process, names=['Electrons','Muons','Photons','Jets'])
    

    print " *** PAT path has been defined"
    


