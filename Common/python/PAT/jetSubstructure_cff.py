import FWCore.ParameterSet.Config as cms

from CMGTools.Common.Tools.cmsswRelease import cmsswIs52X

if cmsswIs52X():
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

    PATCMGJetSequenceCHSpruned = cms.Sequence(
        ak5PFJetsCHSpruned +
        jetMCSequenceCHSpruned +
        patJetCorrFactorsCHSpruned +
        patJetsCHSpruned +
        selectedPatJetsCHSpruned
        )
