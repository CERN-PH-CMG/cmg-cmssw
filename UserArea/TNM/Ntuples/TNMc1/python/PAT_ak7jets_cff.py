import FWCore.ParameterSet.Config as cms

# JETS  AK7 ----------------------------

from RecoJets.JetProducers.ak5PFJets_cfi import ak5PFJets
ak7PFJetsCHS = ak5PFJets.clone(
    src = 'pfNoPileUp',
    jetPtMin = cms.double(10.0),
    doAreaFastjet = cms.bool(True),
    rParam = cms.double(0.7),
    )

jetSource = 'ak7PFJetsCHS'

# corrections 
from PhysicsTools.PatAlgos.recoLayer0.jetCorrFactors_cfi import *
patJetCorrFactorsAK7CHS = patJetCorrFactors.clone()
patJetCorrFactorsAK7CHS.src = jetSource
# will need to add L2L3 corrections in the cfg
patJetCorrFactorsAK7CHS.levels = ['L1FastJet', 'L2Relative', 'L3Absolute']
patJetCorrFactorsAK7CHS.payload = 'AK7PFchs'
patJetCorrFactorsAK7CHS.useRho = True

# parton and gen jet matching

from PhysicsTools.PatAlgos.mcMatchLayer0.jetMatch_cfi import *
patJetPartonMatchAK7CHS = patJetPartonMatch.clone()
patJetPartonMatchAK7CHS.src = jetSource
patJetGenJetMatchAK7CHS = patJetGenJetMatch.clone()
patJetGenJetMatchAK7CHS.src = jetSource
patJetGenJetMatchAK7CHS.matched = 'ak7GenJetsNoNu'

from PhysicsTools.PatAlgos.mcMatchLayer0.jetFlavourId_cff import *
patJetPartonAssociationAK7CHS = patJetPartonAssociation.clone()
patJetPartonAssociationAK7CHS.jets = jetSource

# pat jets

from PhysicsTools.PatAlgos.producersLayer1.jetProducer_cfi import *
patJetsAK7CHS = patJets.clone()
patJetsAK7CHS.jetSource = jetSource
patJetsAK7CHS.addJetCharge = False
patJetsAK7CHS.embedCaloTowers = False
patJetsAK7CHS.embedPFCandidates = False
patJetsAK7CHS.addAssociatedTracks = False
patJetsAK7CHS.addBTagInfo = False
patJetsAK7CHS.addDiscriminators = False
patJetsAK7CHS.getJetMCFlavour = False
patJetsAK7CHS.jetCorrFactorsSource = cms.VInputTag(cms.InputTag('patJetCorrFactorsAK7CHS'))
patJetsAK7CHS.genPartonMatch = cms.InputTag('patJetPartonMatchAK7CHS')
patJetsAK7CHS.genJetMatch = cms.InputTag('patJetGenJetMatchAK7CHS')

from PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi import *
selectedPatJetsAK7CHS = selectedPatJets.clone()
selectedPatJetsAK7CHS.src = 'patJetsAK7CHS'
selectedPatJetsAK7CHS.cut = 'pt()>20'

from RecoJets.Configuration.RecoGenJets_cff import ak7GenJetsNoNu
from RecoJets.Configuration.GenJetParticles_cff import genParticlesForJetsNoNu
jetMCSequenceAK7CHS = cms.Sequence(
    patJetPartonMatchAK7CHS +
    genParticlesForJetsNoNu +
    ak7GenJetsNoNu +
    patJetGenJetMatchAK7CHS
    )

PATCMGJetSequenceAK7CHS = cms.Sequence(
    ak7PFJetsCHS +
    jetMCSequenceAK7CHS +
    patJetCorrFactorsAK7CHS +
    patJetsAK7CHS +
    selectedPatJetsAK7CHS
    )
