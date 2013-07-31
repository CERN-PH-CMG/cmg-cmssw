import FWCore.ParameterSet.Config as cms

# JETS  CA8 ----------------------------

from RecoJets.JetProducers.ak5PFJets_cfi import ak5PFJets
ca8PFJetsCHS = ak5PFJets.clone(
    src = 'pfNoPileUp',
    jetPtMin = cms.double(30.0),
    doAreaFastjet = cms.bool(True),
    rParam = cms.double(0.8),
    jetAlgorithm = cms.string("CambridgeAachen"),
    )

jetSource = 'ca8PFJetsCHS'

# corrections 
from PhysicsTools.PatAlgos.recoLayer0.jetCorrFactors_cfi import *
patJetCorrFactorsCA8CHS = patJetCorrFactors.clone()
patJetCorrFactorsCA8CHS.src = jetSource
# will need to add L2L3 corrections in the cfg
patJetCorrFactorsCA8CHS.levels = ['L1FastJet', 'L2Relative', 'L3Absolute']
patJetCorrFactorsCA8CHS.payload = 'AK7PFchs'
patJetCorrFactorsCA8CHS.useRho = True

# parton and gen jet matching

from PhysicsTools.PatAlgos.mcMatchLayer0.jetMatch_cfi import *
patJetPartonMatchCA8CHS = patJetPartonMatch.clone()
patJetPartonMatchCA8CHS.src = jetSource
patJetGenJetMatchCA8CHS = patJetGenJetMatch.clone()
patJetGenJetMatchCA8CHS.src = jetSource
patJetGenJetMatchCA8CHS.matched = 'ca8GenJetsNoNu'

from PhysicsTools.PatAlgos.mcMatchLayer0.jetFlavourId_cff import *
patJetPartonAssociationCA8CHS = patJetPartonAssociation.clone()
patJetPartonAssociationCA8CHS.jets = jetSource

# pat jets

from RecoJets.JetAssociationProducers.ak5JTA_cff import *
ca8CHSJetTracksAssociatorAtVertex=ak5JetTracksAssociatorAtVertex.clone()
ca8CHSJetTracksAssociatorAtVertex.jets=jetSource
from RecoBTag.Configuration.RecoBTag_cff import * # btagging sequence
impactParameterTagInfosCA8CHS=impactParameterTagInfos.clone()
impactParameterTagInfosCA8CHS.jetTracks='ca8CHSJetTracksAssociatorAtVertex'
secondaryVertexTagInfosCA8CHS=secondaryVertexTagInfos.clone()
secondaryVertexTagInfosCA8CHS.trackIPTagInfos='impactParameterTagInfosCA8CHS'
combinedSecondaryVertexBJetTagsCA8CHS=combinedSecondaryVertexBJetTags.clone()
combinedSecondaryVertexBJetTagsCA8CHS.tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfosCA8CHS"),
    cms.InputTag("secondaryVertexTagInfosCA8CHS"))
btaggingCA8CHS=cms.Sequence(ca8CHSJetTracksAssociatorAtVertex+impactParameterTagInfosCA8CHS+secondaryVertexTagInfosCA8CHS+combinedSecondaryVertexBJetTagsCA8CHS)

from PhysicsTools.PatAlgos.producersLayer1.jetProducer_cfi import *
patJetsCA8CHS = patJets.clone()
patJetsCA8CHS.jetSource = jetSource
patJetsCA8CHS.addJetCharge = False
patJetsCA8CHS.embedCaloTowers = False
patJetsCA8CHS.embedPFCandidates = False
patJetsCA8CHS.addAssociatedTracks = False
patJetsCA8CHS.addBTagInfo = True
patJetsCA8CHS.addDiscriminators = True
patJetsCA8CHS.tagInfoSources = cms.VInputTag(cms.InputTag("secondaryVertexTagInfosCA8CHS"))
patJetsCA8CHS.trackAssociationSource = cms.InputTag("ca8CHSJetTracksAssociatorAtVertex")
patJetsCA8CHS.discriminatorSources = cms.VInputTag(cms.InputTag("combinedSecondaryVertexBJetTagsCA8CHS"))
patJetsCA8CHS.getJetMCFlavour = False
patJetsCA8CHS.jetCorrFactorsSource = cms.VInputTag(cms.InputTag('patJetCorrFactorsCA8CHS'))
patJetsCA8CHS.genPartonMatch = cms.InputTag('patJetPartonMatchCA8CHS')
patJetsCA8CHS.genJetMatch = cms.InputTag('patJetGenJetMatchCA8CHS')

from PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi import *
selectedPatJetsCA8CHS = selectedPatJets.clone()
selectedPatJetsCA8CHS.src = 'patJetsCA8CHS'
selectedPatJetsCA8CHS.cut = 'pt()>30'

from RecoJets.Configuration.RecoGenJets_cff import ak7GenJetsNoNu
ca8GenJetsNoNu = ak7GenJetsNoNu.clone()
ca8GenJetsNoNu.rParam = 0.8
ca8GenJetsNoNu.jetAlgorithm = "CambridgeAachen"
ca8GenJetsNoNu.jetPtMin = 30
ca8GenJetsNoNu.doAreaFastjet = True


from PhysicsTools.PatAlgos.producersLayer1.jetProducer_cfi import *
patGenJetsCA8CHS = patJets.clone()
patGenJetsCA8CHS.jetSource = 'ca8GenJetsNoNu'
patGenJetsCA8CHS.addGenJetMatch = False
patGenJetsCA8CHS.addGenPartonMatch = False
patGenJetsCA8CHS.addJetCharge = False
patGenJetsCA8CHS.embedCaloTowers = False
patGenJetsCA8CHS.embedPFCandidates = False
patGenJetsCA8CHS.addAssociatedTracks = False
patGenJetsCA8CHS.addBTagInfo = False
patGenJetsCA8CHS.addDiscriminators = False
patGenJetsCA8CHS.getJetMCFlavour = False
patGenJetsCA8CHS.addJetCorrFactors = False

from RecoJets.Configuration.GenJetParticles_cff import genParticlesForJetsNoNu
jetMCSequenceCA8CHS = cms.Sequence(
    patJetPartonMatchCA8CHS +
    genParticlesForJetsNoNu +
    ca8GenJetsNoNu +
    patGenJetsCA8CHS +
    patJetGenJetMatchCA8CHS
    )

PATCMGJetSequenceCA8CHS = cms.Sequence(
    ca8PFJetsCHS +
    jetMCSequenceCA8CHS +
    patJetCorrFactorsCA8CHS +
    btaggingCA8CHS + # needs generalTracks
    patJetsCA8CHS +
    selectedPatJetsCA8CHS
    )

# JETS PRUNED CA8 ----------------------------

from RecoJets.JetProducers.ak5PFJetsPruned_cfi import ak5PFJetsPruned
ca8PFJetsCHSpruned = ak5PFJetsPruned.clone(
    src = 'pfNoPileUp',
    jetPtMin = cms.double(30.0),
    doAreaFastjet = cms.bool(True),
    rParam = cms.double(0.8),
    jetAlgorithm = cms.string("CambridgeAachen"),
    )

jetSource = 'ca8PFJetsCHSpruned'

# corrections 
from PhysicsTools.PatAlgos.recoLayer0.jetCorrFactors_cfi import *
patJetCorrFactorsCA8CHSpruned = patJetCorrFactors.clone()
patJetCorrFactorsCA8CHSpruned.src = jetSource
# will need to add L2L3 corrections in the cfg
patJetCorrFactorsCA8CHSpruned.levels = ['L1FastJet', 'L2Relative', 'L3Absolute']
patJetCorrFactorsCA8CHSpruned.payload = 'AK7PFchs'
patJetCorrFactorsCA8CHSpruned.useRho = True

# parton and gen jet matching

from PhysicsTools.PatAlgos.mcMatchLayer0.jetMatch_cfi import *
patJetPartonMatchCA8CHSpruned = patJetPartonMatch.clone()
patJetPartonMatchCA8CHSpruned.src = jetSource
patJetGenJetMatchCA8CHSpruned = patJetGenJetMatch.clone()
patJetGenJetMatchCA8CHSpruned.src = jetSource
patJetGenJetMatchCA8CHSpruned.matched = 'ca8GenJetsNoNu'

from PhysicsTools.PatAlgos.mcMatchLayer0.jetFlavourId_cff import *
patJetPartonAssociationCA8CHSpruned = patJetPartonAssociation.clone()
patJetPartonAssociationCA8CHSpruned.jets = jetSource

# pat jets

from RecoJets.JetAssociationProducers.ak5JTA_cff import *
ca8CHSprunedJetTracksAssociatorAtVertex=ak5JetTracksAssociatorAtVertex.clone()
ca8CHSprunedJetTracksAssociatorAtVertex.jets=jetSource
from RecoBTag.Configuration.RecoBTag_cff import * # btagging sequence
impactParameterTagInfosCA8CHSpruned=impactParameterTagInfos.clone()
impactParameterTagInfosCA8CHSpruned.jetTracks='ca8CHSprunedJetTracksAssociatorAtVertex'
secondaryVertexTagInfosCA8CHSpruned=secondaryVertexTagInfos.clone()
secondaryVertexTagInfosCA8CHSpruned.trackIPTagInfos='impactParameterTagInfosCA8CHSpruned'
combinedSecondaryVertexBJetTagsCA8CHSpruned=combinedSecondaryVertexBJetTags.clone()
combinedSecondaryVertexBJetTagsCA8CHSpruned.tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfosCA8CHSpruned"),
    cms.InputTag("secondaryVertexTagInfosCA8CHSpruned"))
btaggingCA8CHSpruned=cms.Sequence(ca8CHSprunedJetTracksAssociatorAtVertex+impactParameterTagInfosCA8CHSpruned+secondaryVertexTagInfosCA8CHSpruned+combinedSecondaryVertexBJetTagsCA8CHSpruned)

patJetsCA8CHSpruned = patJets.clone()
patJetsCA8CHSpruned.jetSource = jetSource
patJetsCA8CHSpruned.addJetCharge = False
patJetsCA8CHSpruned.embedCaloTowers = False
patJetsCA8CHSpruned.embedPFCandidates = False
patJetsCA8CHSpruned.addAssociatedTracks = False
patJetsCA8CHSpruned.addBTagInfo = True
patJetsCA8CHSpruned.addDiscriminators = True
patJetsCA8CHSpruned.tagInfoSources = cms.VInputTag(cms.InputTag("secondaryVertexTagInfosCA8CHSpruned"))
patJetsCA8CHSpruned.trackAssociationSource = cms.InputTag("ca8CHSprunedJetTracksAssociatorAtVertex")
patJetsCA8CHSpruned.discriminatorSources = cms.VInputTag(cms.InputTag("combinedSecondaryVertexBJetTagsCA8CHSpruned"))
patJetsCA8CHSpruned.getJetMCFlavour = False
patJetsCA8CHSpruned.jetCorrFactorsSource = cms.VInputTag(cms.InputTag('patJetCorrFactorsCA8CHSpruned'))
patJetsCA8CHSpruned.genPartonMatch = cms.InputTag('patJetPartonMatchCA8CHSpruned')
patJetsCA8CHSpruned.genJetMatch = cms.InputTag('patJetGenJetMatchCA8CHSpruned')

selectedPatJetsCA8CHSprunedPre = selectedPatJets.clone()
selectedPatJetsCA8CHSprunedPre.src = 'patJetsCA8CHSpruned'
selectedPatJetsCA8CHSprunedPre.cut = 'pt()>30'

ca8PrunedGenJetsNoNu = ak7GenJetsNoNu.clone()
ca8PrunedGenJetsNoNu.rParam = 0.8
ca8PrunedGenJetsNoNu.jetAlgorithm = "CambridgeAachen"
ca8PrunedGenJetsNoNu.doAreaFastjet = True
ca8PrunedGenJetsNoNu.usePruning = cms.bool(True)
ca8PrunedGenJetsNoNu.useExplicitGhosts = cms.bool(True)
ca8PrunedGenJetsNoNu.writeCompound = cms.bool(True)
ca8PrunedGenJetsNoNu.jetCollInstanceName = cms.string("SubJets")
ca8PrunedGenJetsNoNu.nFilt = cms.int32(2)
ca8PrunedGenJetsNoNu.zcut = cms.double(0.1)
ca8PrunedGenJetsNoNu.rcut_factor = cms.double(0.5)
ca8PrunedGenJetsNoNu.jetPtMin = 30

patGenJetsCA8CHSpruned = patJets.clone()
patGenJetsCA8CHSpruned.jetSource = 'ca8PrunedGenJetsNoNu'
patGenJetsCA8CHSpruned.addGenJetMatch = False
patGenJetsCA8CHSpruned.addGenPartonMatch = False
patGenJetsCA8CHSpruned.addJetCharge = False
patGenJetsCA8CHSpruned.embedCaloTowers = False
patGenJetsCA8CHSpruned.embedPFCandidates = False
patGenJetsCA8CHSpruned.addAssociatedTracks = False
patGenJetsCA8CHSpruned.addBTagInfo = False
patGenJetsCA8CHSpruned.addDiscriminators = False
patGenJetsCA8CHSpruned.getJetMCFlavour = False
patGenJetsCA8CHSpruned.addJetCorrFactors = False

#### Adding subjet b-tagging

from RecoJets.JetAssociationProducers.ak5JTA_cff import *
ca8CHSprunedSubjetsJetTracksAssociatorAtVertex=ak5JetTracksAssociatorAtVertex.clone()
ca8CHSprunedSubjetsJetTracksAssociatorAtVertex.jets=cms.InputTag('ca8PFJetsCHSpruned','SubJets')
from RecoBTag.Configuration.RecoBTag_cff import * # btagging sequence
impactParameterTagInfosCA8CHSprunedSubjets=impactParameterTagInfos.clone()
impactParameterTagInfosCA8CHSprunedSubjets.jetTracks='ca8CHSprunedSubjetsJetTracksAssociatorAtVertex'
secondaryVertexTagInfosCA8CHSprunedSubjets=secondaryVertexTagInfos.clone()
secondaryVertexTagInfosCA8CHSprunedSubjets.trackIPTagInfos='impactParameterTagInfosCA8CHSprunedSubjets'
combinedSecondaryVertexBJetTagsCA8CHSprunedSubjets=combinedSecondaryVertexBJetTags.clone()
combinedSecondaryVertexBJetTagsCA8CHSprunedSubjets.tagInfos = cms.VInputTag(cms.InputTag("impactParameterTagInfosCA8CHSprunedSubjets"),
    cms.InputTag("secondaryVertexTagInfosCA8CHSprunedSubjets"))
btaggingCA8CHSprunedSubjets=cms.Sequence(ca8CHSprunedSubjetsJetTracksAssociatorAtVertex+impactParameterTagInfosCA8CHSprunedSubjets+secondaryVertexTagInfosCA8CHSprunedSubjets+combinedSecondaryVertexBJetTagsCA8CHSprunedSubjets)

patJetsCA8CHSprunedSubjets = patJets.clone()
patJetsCA8CHSprunedSubjets.jetSource = cms.InputTag('ca8PFJetsCHSpruned','SubJets')
patJetsCA8CHSprunedSubjets.addGenJetMatch = False
patJetsCA8CHSprunedSubjets.addGenPartonMatch = False
patJetsCA8CHSprunedSubjets.addJetCharge = False
patJetsCA8CHSprunedSubjets.embedCaloTowers = False
patJetsCA8CHSprunedSubjets.embedPFCandidates = False
patJetsCA8CHSprunedSubjets.addAssociatedTracks = False
patJetsCA8CHSprunedSubjets.addBTagInfo = True
patJetsCA8CHSprunedSubjets.addDiscriminators = True
patJetsCA8CHSprunedSubjets.tagInfoSources = cms.VInputTag(cms.InputTag("secondaryVertexTagInfosCA8CHSprunedSubjets"))
patJetsCA8CHSprunedSubjets.trackAssociationSource = cms.InputTag("ca8CHSprunedSubjetsJetTracksAssociatorAtVertex")
patJetsCA8CHSprunedSubjets.discriminatorSources = cms.VInputTag(cms.InputTag("combinedSecondaryVertexBJetTagsCA8CHSprunedSubjets"))
patJetsCA8CHSprunedSubjets.getJetMCFlavour = False
patJetsCA8CHSprunedSubjets.addJetCorrFactors = False

selectedPatJetsCA8CHSpruned = cms.EDProducer("BoostedJetMerger",
                                                      jetSrc=cms.InputTag("selectedPatJetsCA8CHSprunedPre"),
                                                      subjetSrc=cms.InputTag("patJetsCA8CHSprunedSubjets")
    )

jetMCSequenceCA8CHSpruned = cms.Sequence(
    patJetPartonMatchCA8CHSpruned +
    genParticlesForJetsNoNu +
    ca8PrunedGenJetsNoNu +
    patGenJetsCA8CHSpruned +
    patJetGenJetMatchCA8CHSpruned
    )

PATCMGJetSequenceCA8CHSpruned = cms.Sequence(
    ca8PFJetsCHSpruned +
    jetMCSequenceCA8CHSpruned +
    patJetCorrFactorsCA8CHSpruned +
    btaggingCA8CHSpruned +
    patJetsCA8CHSpruned +
    selectedPatJetsCA8CHSprunedPre +
    btaggingCA8CHSprunedSubjets +
    patJetsCA8CHSprunedSubjets +
    selectedPatJetsCA8CHSpruned
    )

#### Adding Nsubjetiness

selectedPatJetsCA8CHSwithNsub = cms.EDProducer("NjettinessAdder",
                              src=cms.InputTag("selectedPatJetsCA8CHS"),
                              cone=cms.double(0.8)
                              )

#### Adding QJets

selectedPatJetsCA8CHSwithQjets = cms.EDProducer("QjetsAdder",
                               src=cms.InputTag("selectedPatJetsCA8CHSwithNsub"),
                               zcut=cms.double(0.1),
                               dcutfctr=cms.double(0.5),
                               expmin=cms.double(0.0),
                               expmax=cms.double(0.0),
                               rigidity=cms.double(0.1),
                               ntrial = cms.int32(50),
                               cutoff=cms.double(200.0),
                               jetRad= cms.double(0.8),
                               jetAlgo=cms.string("CA"),
                               preclustering = cms.int32(50),
                              )

ca8Jets = cms.Sequence( PATCMGJetSequenceCA8CHS + PATCMGJetSequenceCA8CHSpruned + selectedPatJetsCA8CHSwithNsub + selectedPatJetsCA8CHSwithQjets )
