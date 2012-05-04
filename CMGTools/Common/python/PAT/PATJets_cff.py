import FWCore.ParameterSet.Config as cms

# from PhysicsTools.PatAlgos.patSequences_cff import *
from PhysicsTools.PatAlgos.producersLayer1.jetProducer_cff import *
from PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi import selectedPatJets

from CommonTools.ParticleFlow.ParticleSelectors.genericPFJetSelector_cfi import selectedPfJets

#FIXME: is this cut really necessary? does it play well with Phil's stuff?
# NOTE cutting on uncorrected jets, but no bias for corrected jet pT>7
ak5PFJetsSel = selectedPfJets.clone( src = 'ak5PFJets',
                                     cut = 'pt()>5' )

jetSource = 'ak5PFJetsSel'

# corrections 
from PhysicsTools.PatAlgos.recoLayer0.jetCorrFactors_cfi import *
patJetCorrFactors.src = jetSource
# will need to add L2L3 corrections in the cfg
patJetCorrFactors.levels = ['L1FastJet', 'L2Relative', 'L3Absolute']
patJetCorrFactors.payload = 'AK5PF'
patJetCorrFactors.useRho = True

patJets.jetSource = jetSource
patJets.addJetCharge = False
patJets.embedCaloTowers = False
patJets.embedPFCandidates = False
patJets.addAssociatedTracks = False

# b tagging 
from RecoJets.JetAssociationProducers.ak5JTA_cff import *
ak5JetTracksAssociatorAtVertex.jets = jetSource
from RecoBTag.Configuration.RecoBTag_cff import * # btagging sequence
softMuonTagInfos.jets = jetSource
softElectronTagInfos.jets = jetSource

# parton and gen jet matching

from CommonTools.ParticleFlow.genForPF2PAT_cff import * 

from PhysicsTools.PatAlgos.mcMatchLayer0.jetMatch_cfi import *
patJetPartonMatch.src = jetSource
patJetGenJetMatch.src = jetSource
patJetGenJetMatch.matched = 'ak5GenJetsNoNu'

from PhysicsTools.PatAlgos.mcMatchLayer0.jetFlavourId_cff import *
patJetPartonAssociation.jets = jetSource

# FIXME is it ok for jet studies?
selectedPatJets.cut = 'pt()>10'

from  CMGTools.External.pujetidsequence_cff import puJetId

jetMCSequence = cms.Sequence(
    patJetPartonMatch +
    patJetGenJetMatch
    )

PATJetSequence = cms.Sequence(
    ak5PFJetsSel + 
    jetMCSequence +
    ak5JetTracksAssociatorAtVertex + 
    btagging + 
    patJetCorrFactors +
    patJetFlavourId +
    patJets +
    selectedPatJets +
    puJetId 
    )
