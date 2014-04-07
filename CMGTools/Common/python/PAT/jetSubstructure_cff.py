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



    # JETS PRUNED AK8 ----------------------------

    from RecoJets.JetProducers.ak5PFJetsPruned_cfi import ak5PFJetsPruned
    ak8PFJetsCHSpruned = ak5PFJetsPruned.clone(
        src = 'pfNoPileUp',
        jetPtMin = cms.double(10.0),
        doAreaFastjet = cms.bool(True),
	rParam = cms.double(0.8),
        )

    jetSource = 'ak8PFJetsCHSpruned'

    # corrections 
    from PhysicsTools.PatAlgos.recoLayer0.jetCorrFactors_cfi import *
    patJetCorrFactorsAK8CHSpruned = patJetCorrFactors.clone()
    patJetCorrFactorsAK8CHSpruned.src = jetSource
    # will need to add L2L3 corrections in the cfg
    patJetCorrFactorsAK8CHSpruned.levels = ['L1FastJet', 'L2Relative', 'L3Absolute']
    patJetCorrFactorsAK8CHSpruned.payload = 'AK8PFchs'
    patJetCorrFactorsAK8CHSpruned.useRho = True

    # parton and gen jet matching

    from PhysicsTools.PatAlgos.mcMatchLayer0.jetMatch_cfi import *
    patJetPartonMatchAK8CHSpruned = patJetPartonMatch.clone()
    patJetPartonMatchAK8CHSpruned.src = jetSource
    patJetGenJetMatchAK8CHSpruned = patJetGenJetMatch.clone()
    patJetGenJetMatchAK8CHSpruned.src = jetSource
    patJetGenJetMatchAK8CHSpruned.matched = 'ak8GenJetsNoNu'

    from PhysicsTools.PatAlgos.mcMatchLayer0.jetFlavourId_cff import *
    patJetPartonAssociationAK8CHSpruned = patJetPartonAssociation.clone()
    patJetPartonAssociationAK8CHSpruned.jets = jetSource

    # pat jets

    from PhysicsTools.PatAlgos.producersLayer1.jetProducer_cfi import *
    patJetsAK8CHSpruned = patJets.clone()
    patJetsAK8CHSpruned.jetSource = jetSource
    patJetsAK8CHSpruned.addJetCharge = False
    patJetsAK8CHSpruned.embedCaloTowers = False
    patJetsAK8CHSpruned.embedPFCandidates = False
    patJetsAK8CHSpruned.addAssociatedTracks = False
    patJetsAK8CHSpruned.addBTagInfo = False
    patJetsAK8CHSpruned.addDiscriminators = False
    patJetsAK8CHSpruned.getJetMCFlavour = False
    patJetsAK8CHSpruned.jetCorrFactorsSource = cms.VInputTag(cms.InputTag('patJetCorrFactorsAK8CHSpruned'))
    patJetsAK8CHSpruned.genPartonMatch = cms.InputTag('patJetPartonMatchAK8CHSpruned')
    patJetsAK8CHSpruned.genJetMatch = cms.InputTag('patJetGenJetMatchAK8CHSpruned')

    from PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi import *
    selectedPatJetsAK8CHSpruned = selectedPatJets.clone()
    selectedPatJetsAK8CHSpruned.src = 'patJetsAK8CHSpruned'
    selectedPatJetsAK8CHSpruned.cut = 'pt()>10'

    from RecoJets.Configuration.RecoGenJets_cff import ak8GenJetsNoNu
    jetMCSequenceAK8CHSpruned = cms.Sequence(
        patJetPartonMatchAK8CHSpruned +
        ak8GenJetsNoNu +
        patJetGenJetMatchAK8CHSpruned
        )

    from CMGTools.Common.factories.cmgStructuredPFJet_cfi import cmgStructuredPFJet
    from CMGTools.Common.skims.cmgStructuredPFJetSel_cfi import cmgStructuredPFJetSel

    cmgStructuredPFJetAK8 = cmgStructuredPFJet.clone()
    cmgStructuredPFJetAK8.cfg.inputCollection = cms.InputTag("selectedPatJetsAK8CHSpruned")
    cmgStructuredPFJetAK8.cfg.subjetCollection = cms.InputTag("ak8PFJetsCHSpruned:SubJets")
    cmgStructuredPFJetAK8Sel = cmgStructuredPFJetSel.clone()
    cmgStructuredPFJetAK8Sel.src = cms.InputTag("cmgStructuredPFJetAK8")

    pfStructuredJetAK8Sequence = cms.Sequence(
        cmgStructuredPFJetAK8 + 
        cmgStructuredPFJetAK8Sel
    )

    PATCMGJetSequenceAK8CHSpruned = cms.Sequence(
        ak8PFJetsCHSpruned +
        jetMCSequenceAK8CHSpruned +
        patJetCorrFactorsAK8CHSpruned +
        patJetsAK8CHSpruned +
        selectedPatJetsAK8CHSpruned +
	pfStructuredJetAK8Sequence
        )
