import FWCore.ParameterSet.Config as cms

# from PhysicsTools.PatAlgos.patSequences_cff import *
from PhysicsTools.PatAlgos.producersLayer1.jetProducer_cff import *
from PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi import selectedPatJets

from CommonTools.ParticleFlow.ParticleSelectors.genericPFJetSelector_cfi import selectedPfJets

#FIXME: is this cut really necessary? does it play well with Phil's stuff?
# NOTE cutting on uncorrected jets, but no bias for corrected jet pT>7
ak5PFJetsSel = selectedPfJets.clone( src = 'ak5PFJets',
                                     cut = 'pt()>0 && abs(eta())<5.0' )

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
from CMGTools.Common.Tools.cmsswRelease import isNewerThan
from CMGTools.Common.skims.cmgCandSel_cfi import cmgCandSel

## add secondar vertex mass information
## can be used in analysis level: jet->sourcePtr()->get()->userFloat("secvtxMass"); 
patJets.addTagInfos = True
patJets.tagInfoSources = cms.VInputTag(
   cms.InputTag("secondaryVertexTagInfos")
   )
patJets.userData.userFunctions = cms.vstring( "? hasTagInfo('secondaryVertex') && tagInfoSecondaryVertex('secondaryVertex').nVertices() > 0 ? "
                                                     "tagInfoSecondaryVertex('secondaryVertex').secondaryVertex(0).p4().mass() : 0",
                                              "? hasTagInfo('secondaryVertex') && tagInfoSecondaryVertex('secondaryVertex').nVertices() > 0 ? "
                                                     "tagInfoSecondaryVertex('secondaryVertex').flightDistance(0).value() : 0",
                                              "? hasTagInfo('secondaryVertex') && tagInfoSecondaryVertex('secondaryVertex').nVertices() > 0 ? "
                                                     "tagInfoSecondaryVertex('secondaryVertex').flightDistance(0).error() : 0",
)
patJets.userData.userFunctionLabels = cms.vstring('secvtxMass','Lxy','LxyErr')

# parton and gen jet matching

from CommonTools.ParticleFlow.genForPF2PAT_cff import * 
from PhysicsTools.PatAlgos.mcMatchLayer0.jetMatch_cfi import *
patJetPartonMatch.src = jetSource
patJetGenJetMatch.src = jetSource
patJetGenJetMatch.matched = 'ak5GenJetsNoNu'

from PhysicsTools.PatAlgos.mcMatchLayer0.jetFlavourId_cff import *
patJetPartonAssociation.jets = jetSource

jetsPtGt1Cut = '(neutralHadronEnergy())/(correctedJet(0).pt()/pt()*energy())  < 0.99 && (neutralEmEnergy()/(correctedJet(0).pt()/pt()*energy())) < 0.99 && (nConstituents()) > 1    && ((abs(eta())  < 2.4  && chargedHadronEnergy()/(correctedJet(0).pt()/pt()*energy()) > 0 && chargedEmEnergy()      /(correctedJet(0).pt()/pt()*energy()) < 0.99 && chargedMultiplicity() > 0)   ||  abs(eta())  > 2.4) '
if not isNewerThan('CMSSW_5_2_0'):
    # addtl cut needed due to different MVA MET training in 44X
    jetsPtGt1Cut = ' && '.join([jetsPtGt1Cut,'pt()>1'])
jetsPtGt1 = cmgCandSel.clone( src = 'patJets', cut = jetsPtGt1Cut )

from CMGTools.Common.miscProducers.collectionSize.candidateSize_cfi import candidateSize
nJetsPtGt1 = candidateSize.clone( src = 'jetsPtGt1' )

# jet extender
patJetsWithVar = cms.EDProducer('JetExtendedProducer',
    jets     = cms.InputTag('selectedPatJets'),
    vertices = cms.InputTag('goodOfflinePrimaryVertices'),
    #debug   = cms.untracked.bool(True),
    payload  = cms.string('AK5PF')
)

outPFCand = cms.EDProducer('VbfHbbPFCandOutOfJets',
    jets         = cms.InputTag('patJetsWithVar'),
    vtx          = cms.InputTag('goodOfflinePrimaryVertices'),
    pfcandidates = cms.InputTag('particleFlow'),
    btagger      = cms.string('combinedSecondaryVertexBJetTags')
)
from RecoJets.JetProducers.ak5PFJets_cfi import *
ak5SoftPFJetsForVbfHbb = ak5PFJets.clone(src = 'outPFCand',jetPtMin = 1.0)

selectedPatJets.cut = ''

from  CMGTools.External.pujetidsequence_cff import puJetId
puJetId.jets = cms.InputTag('patJetsWithVar')

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
    jetsPtGt1 +
    nJetsPtGt1 + 
    selectedPatJets +
    patJetsWithVar +
    puJetId +
    outPFCand +
    ak5SoftPFJetsForVbfHbb
    )
