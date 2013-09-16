import FWCore.ParameterSet.Config as cms

from CMGTools.Common.Tools.cmsswRelease import isNewerThan

if isNewerThan('CMSSW_5_2_0'):
    # JETS PRUNED ----------------------------

    # for jet substructure you need extra tags in 4XX and 52X:
    # for 4XX:
    # scram setup $CMS_PATH/slc5_amd64_gcc434/external/fastjet-toolfile/1.0-cms7/etc/scram.d/fastjet.xml
    # addpkg RecoJets/JetAlgorithms V04-04-00
    # addpkg RecoJets/JetProducers V05-10-02
    # for 52X:
    # addpkg RecoJets/JetProducers V05-10-02

    from RecoJets.JetProducers.ak5PFJetsPruned_cfi import ak5PFJetsPruned
    ak5PFJetsCHSpruned = ak5PFJetsPruned.clone(
        src = 'pfNoPileUp',
        jetPtMin = cms.double(10.0),
        doAreaFastjet = cms.bool(True)
        )

    jetSource = 'ak5PFJetsCHSpruned'

    # corrections 
    from PhysicsTools.PatAlgos.recoLayer0.jetCorrFactors_cfi import *
    patJetCorrFactorsCHSpruned = patJetCorrFactors.clone()
    patJetCorrFactorsCHSpruned.src = jetSource
    # will need to add L2L3 corrections in the cfg
    patJetCorrFactorsCHSpruned.levels = ['L1FastJet', 'L2Relative', 'L3Absolute']
    patJetCorrFactorsCHSpruned.payload = 'AK5PFchs'
    patJetCorrFactorsCHSpruned.useRho = True

    # parton and gen jet matching

    from PhysicsTools.PatAlgos.mcMatchLayer0.jetMatch_cfi import *
    patJetPartonMatchCHSpruned = patJetPartonMatch.clone()
    patJetPartonMatchCHSpruned.src = jetSource
    patJetGenJetMatchCHSpruned = patJetGenJetMatch.clone()
    patJetGenJetMatchCHSpruned.src = jetSource
    patJetGenJetMatchCHSpruned.matched = 'ak5GenJetsNoNu'

    from PhysicsTools.PatAlgos.mcMatchLayer0.jetFlavourId_cff import *
    patJetPartonAssociationCHSpruned = patJetPartonAssociation.clone()
    patJetPartonAssociationCHSpruned.jets = jetSource

    # pat jets

    from PhysicsTools.PatAlgos.producersLayer1.jetProducer_cfi import *
    patJetsCHSpruned = patJets.clone()
    patJetsCHSpruned.jetSource = jetSource
    patJetsCHSpruned.addJetCharge = False
    patJetsCHSpruned.embedCaloTowers = False
    patJetsCHSpruned.embedPFCandidates = False
    patJetsCHSpruned.addAssociatedTracks = False
    patJetsCHSpruned.addBTagInfo = False
    patJetsCHSpruned.addDiscriminators = False
    patJetsCHSpruned.getJetMCFlavour = False
    patJetsCHSpruned.jetCorrFactorsSource = cms.VInputTag(cms.InputTag('patJetCorrFactorsCHSpruned'))
    patJetsCHSpruned.genPartonMatch = cms.InputTag('patJetPartonMatchCHSpruned')
    patJetsCHSpruned.genJetMatch = cms.InputTag('patJetGenJetMatchCHSpruned')

    from PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi import *
    selectedPatJetsCHSpruned = selectedPatJets.clone()
    selectedPatJetsCHSpruned.src = 'patJetsCHSpruned'
    selectedPatJetsCHSpruned.cut = 'pt()>10'

    jetMCSequenceCHSpruned = cms.Sequence(
        patJetPartonMatchCHSpruned +
        patJetGenJetMatchCHSpruned
        )

    from CMGTools.Common.factories.cmgStructuredPFJet_cfi import cmgStructuredPFJet
    from CMGTools.Common.skims.cmgStructuredPFJetSel_cfi import cmgStructuredPFJetSel

    pfStructuredJetSequence = cms.Sequence(
        cmgStructuredPFJet + 
        cmgStructuredPFJetSel
    )

    PATCMGJetSequenceCHSpruned = cms.Sequence(
        ak5PFJetsCHSpruned +
        jetMCSequenceCHSpruned +
        patJetCorrFactorsCHSpruned +
        patJetsCHSpruned +
        selectedPatJetsCHSpruned +
	pfStructuredJetSequence
        )



    # JETS PRUNED AK7 ----------------------------

    from RecoJets.JetProducers.ak5PFJetsPruned_cfi import ak5PFJetsPruned
    ak7PFJetsCHSpruned = ak5PFJetsPruned.clone(
        src = 'pfNoPileUp',
        jetPtMin = cms.double(10.0),
        doAreaFastjet = cms.bool(True),
	rParam = cms.double(0.7),
        )

    jetSource = 'ak7PFJetsCHSpruned'

    # corrections 
    from PhysicsTools.PatAlgos.recoLayer0.jetCorrFactors_cfi import *
    patJetCorrFactorsAK7CHSpruned = patJetCorrFactors.clone()
    patJetCorrFactorsAK7CHSpruned.src = jetSource
    # will need to add L2L3 corrections in the cfg
    patJetCorrFactorsAK7CHSpruned.levels = ['L1FastJet', 'L2Relative', 'L3Absolute']
    patJetCorrFactorsAK7CHSpruned.payload = 'AK7PFchs'
    patJetCorrFactorsAK7CHSpruned.useRho = True

    # parton and gen jet matching

    from PhysicsTools.PatAlgos.mcMatchLayer0.jetMatch_cfi import *
    patJetPartonMatchAK7CHSpruned = patJetPartonMatch.clone()
    patJetPartonMatchAK7CHSpruned.src = jetSource
    patJetGenJetMatchAK7CHSpruned = patJetGenJetMatch.clone()
    patJetGenJetMatchAK7CHSpruned.src = jetSource
    patJetGenJetMatchAK7CHSpruned.matched = 'ak7GenJetsNoNu'

    from PhysicsTools.PatAlgos.mcMatchLayer0.jetFlavourId_cff import *
    patJetPartonAssociationAK7CHSpruned = patJetPartonAssociation.clone()
    patJetPartonAssociationAK7CHSpruned.jets = jetSource

    # pat jets

    from PhysicsTools.PatAlgos.producersLayer1.jetProducer_cfi import *
    patJetsAK7CHSpruned = patJets.clone()
    patJetsAK7CHSpruned.jetSource = jetSource
    patJetsAK7CHSpruned.addJetCharge = False
    patJetsAK7CHSpruned.embedCaloTowers = False
    patJetsAK7CHSpruned.embedPFCandidates = False
    patJetsAK7CHSpruned.addAssociatedTracks = False
    patJetsAK7CHSpruned.addBTagInfo = False
    patJetsAK7CHSpruned.addDiscriminators = False
    patJetsAK7CHSpruned.getJetMCFlavour = False
    patJetsAK7CHSpruned.jetCorrFactorsSource = cms.VInputTag(cms.InputTag('patJetCorrFactorsAK7CHSpruned'))
    patJetsAK7CHSpruned.genPartonMatch = cms.InputTag('patJetPartonMatchAK7CHSpruned')
    patJetsAK7CHSpruned.genJetMatch = cms.InputTag('patJetGenJetMatchAK7CHSpruned')

    from PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi import *
    selectedPatJetsAK7CHSpruned = selectedPatJets.clone()
    selectedPatJetsAK7CHSpruned.src = 'patJetsAK7CHSpruned'
    selectedPatJetsAK7CHSpruned.cut = 'pt()>10'

    from RecoJets.Configuration.RecoGenJets_cff import ak7GenJetsNoNu
    jetMCSequenceAK7CHSpruned = cms.Sequence(
        patJetPartonMatchAK7CHSpruned +
        ak7GenJetsNoNu +
        patJetGenJetMatchAK7CHSpruned
        )

    from CMGTools.Common.factories.cmgStructuredPFJet_cfi import cmgStructuredPFJet
    from CMGTools.Common.skims.cmgStructuredPFJetSel_cfi import cmgStructuredPFJetSel

    cmgStructuredPFJetAK7 = cmgStructuredPFJet.clone()
    cmgStructuredPFJetAK7.cfg.inputCollection = cms.InputTag("selectedPatJetsAK7CHSpruned")
    cmgStructuredPFJetAK7.cfg.subjetCollection = cms.InputTag("ak7PFJetsCHSpruned:SubJets")
    cmgStructuredPFJetAK7Sel = cmgStructuredPFJetSel.clone()
    cmgStructuredPFJetAK7Sel.src = cms.InputTag("cmgStructuredPFJetAK7")

    pfStructuredJetAK7Sequence = cms.Sequence(
        cmgStructuredPFJetAK7 + 
        cmgStructuredPFJetAK7Sel
    )

    PATCMGJetSequenceAK7CHSpruned = cms.Sequence(
        ak7PFJetsCHSpruned +
        jetMCSequenceAK7CHSpruned +
        patJetCorrFactorsAK7CHSpruned +
        patJetsAK7CHSpruned +
        selectedPatJetsAK7CHSpruned +
	pfStructuredJetAK7Sequence
        )
