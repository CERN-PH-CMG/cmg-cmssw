import FWCore.ParameterSet.Config as cms

vertexCollection = "offlinePrimaryVertices"

def configureCmgTuple( process, runOnMC ):

    #########
    process.setName_('PATCMG')    
        
    #######################PF2PAT#########################################
    process.load("PhysicsTools.PatAlgos.patSequences_cff")
    from PhysicsTools.PatAlgos.tools.pfTools import *
    postfixAK5 = "AK5"
    jetAlgoAK5 = "AK5"
    usePF2PAT(process,
              runPF2PAT=True,
              jetAlgo=jetAlgoAK5,
              runOnMC=runOnMC,
              postfix=postfixAK5,
              jetCorrections=('AK5PFchs', ['L1FastJet','L2Relative','L3Absolute']))

    ####2011 Jet energy corrections
    process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
    process.load('RecoJets.Configuration.RecoPFJets_cff')
    process.kt6PFJets.doRhoFastjet = True
    process.ak5PFJets.doAreaFastjet = True
    JetCorrectionService = cms.string('ak5PFL1FastL2L3Residual')

    #####needed otherwise complains about no kt6PFJets
    from CommonTools.ParticleFlow.Tools.enablePileUpCorrection import enablePileUpCorrection
    enablePileUpCorrection( process, postfix=postfixAK5)

    ######embedding of tracks in pat taus
    from CMGTools.Common.PAT.embedPFCandidatesInTaus import embedPFCandidatesInTaus
    embedPFCandidatesInTaus( process, postfix=postfixAK5, enable=True )

    ###switch taus to HPS, apply cut on input jets, note this affects tau cleaning on patJets
    process.pfJetsPtCutAK5 = cms.EDFilter("RecoPFJetSelector",
                                          src = cms.InputTag( "pfJets"+postfixAK5 ),
                                          cut = cms.string( "pt() > 15.0" )
                                          )    
    getattr(process, "pfJetSequence"+postfixAK5).replace(
        applyPostfix(process,"pfJets",postfixAK5),
        applyPostfix(process,"pfJets",postfixAK5)+process.pfJetsPtCutAK5
        )
    applyPostfix(process,"pfJetTracksAssociatorAtVertex",postfixAK5).jets = "pfJetsPtCut"+postfixAK5
    applyPostfix(process,"pfTauPFJets08Region",postfixAK5).src = "pfJetsPtCut"+postfixAK5
    applyPostfix(process,"pfJetsPiZeros",postfixAK5).jetSrc = "pfJetsPtCut"+postfixAK5
    applyPostfix(process,"pfJetsLegacyTaNCPiZeros",postfixAK5).jetSrc = "pfJetsPtCut"+postfixAK5
    applyPostfix(process,"pfJetsLegacyHPSPiZeros",postfixAK5).jetSrc = "pfJetsPtCut"+postfixAK5
    applyPostfix(process,"pfTausBase",postfixAK5).jetSrc = "pfJetsPtCut"+postfixAK5
    applyPostfix(process,"pfNoTau",postfixAK5).bottomCollection = "pfJetsPtCut"+postfixAK5
    adaptPFTaus(process,"hpsPFTau",postfix=postfixAK5)


    ##### curing a weird bug in PAT needed for prunedAOD
    from CMGTools.Common.PAT.removePhotonMatching import removePhotonMatching
    removePhotonMatching( process, postfixAK5 )


    getattr(process,"pfNoMuon"+postfixAK5).enable = False 
    getattr(process,"pfNoElectron"+postfixAK5).enable = False 
    getattr(process,"pfNoTau"+postfixAK5).enable = False 
    getattr(process,"pfNoJet"+postfixAK5).enable = True
    getattr(process,"pfIsolatedMuons"+postfixAK5).combinedIsolationCut = 999999
    getattr(process,"pfIsolatedElectrons"+postfixAK5).combinedIsolationCut = 999999


    #####################################################################
    #########           PF2PAT path
    ######################################################################
    process.p = cms.Path()

    process.p += getattr(process,"patPF2PATSequence"+postfixAK5)

        
    ######################CMG sequence##################################
    process.load('CMGTools.Common.tau_cff')
    process.tauFactory.inputCollection = cms.InputTag("selectedPatTaus"+postfixAK5)    
    process.tauFactory.leptonFactory.vertexCollection = cms.InputTag(vertexCollection)
    process.tauFactory.leptonFactory.vertexType = cms.int32(0) # default is beamspot=1, primVertex=0
    process.tauFactory.leptonFactory.photonsIsoPar.vetoes = cms.VPSet() # remove photon isolation (out of date)
    process.tauFactory.rhoPFJetsCollection = cms.InputTag("kt6PFJets"+postfixAK5)
    process.cmgTau.cfg = process.tauFactory.clone()

    ##patMuons_selectedPatMuonsAK5__PAT
    process.load('CMGTools.Common.muon_cff')
    process.muonFactory.inputCollection = cms.InputTag("selectedPatMuons"+postfixAK5)
    process.muonFactory.leptonFactory.trackType = cms.int32(0)  # 0 = global track
    process.muonFactory.leptonFactory.vertexCollection = cms.InputTag(vertexCollection) 
    process.muonFactory.leptonFactory.vertexType = cms.int32(0)   # default is beamspot=1, primVertex=0
    process.muonFactory.leptonFactory.photonsIsoPar.vetoes = cms.VPSet() # remove photon isolation (out of date)
    process.cmgMuon.cfg = process.muonFactory.clone()    
    process.cmgMuon.cuts = cms.PSet(pt_default = cms.string(" pt() > 0.0 ")) #remove the vbtf cuts and isolation cuts
    process.muonSequence = cms.Sequence(process.cmgMuon + process.cmgMuonSel) # remove the diMuonSequence otherwise complains about missing cuts

    ##patElectrons_selectedPatElectronsAK5__PAT
    process.load('CMGTools.Common.electron_cff')
    process.electronFactory.inputCollection = cms.InputTag("selectedPatElectrons"+postfixAK5)
    process.electronFactory.leptonFactory.vertexCollection = cms.InputTag(vertexCollection)
    process.electronFactory.leptonFactory.vertexType = cms.int32(0)   # default is beamspot=1, primVertex=0
    process.electronFactory.leptonFactory.photonsIsoPar.vetoes = cms.VPSet() # remove photon isolation (out of date)
    process.cmgElectron.cfg = process.electronFactory.clone()
    process.cmgElectron.cuts = cms.PSet( pt_default = cms.string(" pt() > 0.0 ")) #remove the vbtf cuts and isolation cuts
    process.electronSequence = cms.Sequence(process.cmgElectron + process.cmgElectronSel) # remove the diElectronSequence otherwise complains about missing cuts


    ##patMETs_patMETsAK5__PAT --> dont know how to configure
    process.load('CMGTools.Common.met_cff')
    #process.baseMETFactory.inputCollection = cms.InputTag("patMETsAK5")
    #process.cmgBaseMET.cfg = process.baseMETFactory.clone()
    process.cmgBaseMET.cfg.inputCollection = "patMETs"+postfixAK5 
    process.cmgPFMET = process.cmgBaseMET.clone()



    ####Remove events without leptons
    from CMGTools.Common.skims.cmgCandMerge_cfi import *
    process.cmgLeptonSelMerged = cmgCandMerge.clone(
        src = cms.VInputTag(cms.InputTag("cmgTauSel"),
                            cms.InputTag("cmgElectronSel"),
                            cms.InputTag("cmgMuonSel"),                    
                            )
        )
    process.cmgLeptonSelMergedFilter = cms.EDFilter("CandViewCountFilter",
                                                    src = cms.InputTag("cmgLeptonSelMerged"),
                                                    minNumber = cms.uint32(1),
                                                    )

    #####Remove events which dont have a primary vertex otherwise job will crash
    process.primVertexFilter = cms.EDFilter("VertexSelector",
                                            src = cms.InputTag(vertexCollection),
                                            cut = cms.string(""),
                                            filter = cms.bool(True),
                                            )

    
    ###################################################
    ######        PAT2CMG path
    ###################################################
    process.analysisSequence = cms.Sequence(process.primVertexFilter
                                            
                                            +process.tauSequence
                                            +process.muonSequence
                                            +process.electronSequence                                        
                                            +process.pfSimpleMetSequence                                        

                                            +process.cmgLeptonSelMerged
                                            +process.cmgLeptonSelMergedFilter
                                            )


    process.p += process.analysisSequence


    #############################################################
    process.out.outputCommands = cms.untracked.vstring( 'drop *')

    ############################PAT collections#######################
    #from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning, patTriggerEventContent, patTriggerStandAloneEventContent
    #process.out.outputCommands.extend( patEventContentNoCleaning )
    ####tuning the PAT event content to our needs
    #from CMGTools.Common.eventContent.patEventContentCMG_cff import patEventContentCMG
    #process.out.outputCommands.extend( patEventContentCMG )

    #process.out.outputCommands.append('keep reco*_*_*_*')
    #process.out.outputCommands.append('keep pfJets_pfJetsPtCutAK5_*_*')
    #process.out.outputCommands.append('keep recoTracks_generalTracks_*_*')
    process.out.outputCommands.append("keep recoVertexs_"+vertexCollection+"_*_*")
    #process.out.outputCommands.append('keep patTaus_selectedPatTausAK5_*_*')
    #process.out.outputCommands.append('keep patMuons_selectedPatMuonsAK5_*_*')
    #process.out.outputCommands.append('keep patElectrons_selectedPatElectronsAK5_*_*')
    #process.out.outputCommands.append('keep patMETs_patMETsAK5_*_*')


    #######################CMG collections#############################
    process.out.outputCommands.append( 'keep cmgTaus_cmgTauSel_*_*' )
    process.out.outputCommands.append( 'keep cmgMuons_cmgMuonSel_*_*')
    process.out.outputCommands.append( 'keep cmgElectrons_cmgElectronSel_*_*')
    process.out.outputCommands.append( 'keep cmgBaseMETs_cmgPFMET_*_*')



    ######################################
    ####Gen particles
    ######################################
#    if runOnMC:
#        ##recoGenParticles_genParticlesStatus3__PAT
#        process.load('CMGTools.Common.gen_cff')
#       # process.genParticleFactory.inputCollection = cms.InputTag("genParticlesStatus3")
#       # process.cmgGenParticle.cfg = process.genParticleFactory.clone()
#        process.p += process.genSequence
#        
#    if runOnMC:
#        from CMGTools.Common.eventContent.gen_cff import gen 
#        process.out.outputCommands.extend( gen )
        

