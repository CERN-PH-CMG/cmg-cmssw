import FWCore.ParameterSet.Config as cms

from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets as ak4PF
from CMGTools.External.puJetIDAlgo_cff import met_53x, full_53x

pileupJetIdCalculatorMET = cms.EDProducer(
    'PileupJetIdProducer',
    produceJetIds = cms.bool(True),
    jetids = cms.InputTag(""),
    runMvas = cms.bool(False),
    jets = cms.InputTag("patJetsAK4PF"),
    vertexes = cms.InputTag("offlineSlimmedPrimaryVertices"),
    algos = cms.VPSet(cms.VPSet(met_53x)),
    rho     = cms.InputTag("kt6PFJets","rho"),
    jec     = cms.string("AK5PF"),
    applyJec = cms.bool(False),
    inputIsCorrected = cms.bool(True),
    residualsFromTxt = cms.bool(False),
    residualsTxt = cms.FileInPath("RecoJets/JetProducers/data/download.url")
)

pileupJetIdCalculatorFull = pileupJetIdCalculatorMET.clone(algos = cms.VPSet(cms.VPSet(full_53x)))


from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection

def addAK4Jets(process):
    process.ak4PFJets = ak4PF.clone(src = 'packedPFCandidates') 
    addJetCollection(
       process,
       postfix  = "",
       labelName = 'AK4PF',
       jetSource = cms.InputTag('ak4PFJets'),
       trackSource = cms.InputTag('unpackedTracksAndVertices'), 
       pvSource = cms.InputTag('unpackedTracksAndVertices'), 
       jetCorrections = ('AK4PF', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'Type-2'),
       btagDiscriminators = ['combinedSecondaryVertexBJetTags'],
       algo = 'AK', 
       rParam = 0.4
       )
    process.patJetGenJetMatchAK4PF.matched = "slimmedGenJets"
    process.patJetPartonMatchAK4PF.matched = "prunedGenParticles"
    process.patJetPartons.particles = "prunedGenParticles"
    process.patJetPartonsLegacy.src = "prunedGenParticles"
    process.patJetCorrFactorsAK4PF.primaryVertices = "offlineSlimmedPrimaryVertices"

    process.impactParameterTagInfosAK4PF.primaryVertex = cms.InputTag("unpackedTracksAndVertices")
    # process.inclusiveSecondaryVertexFinderTagInfosAK4PF.extSVCollection = cms.InputTag("unpackedTracksAndVertices","secondary","")
    # process.combinedSecondaryVertexBJetTagsAK4PF.trackMultiplicityMin = 1
    process.jetTracksAssociatorAtVertexAK4PF.tracks = cms.InputTag("unpackedTracksAndVertices")

    process.pileupJetIdCalculatorMET = pileupJetIdCalculatorMET.clone()
    process.pileupJetIdCalculatorFull = pileupJetIdCalculatorFull.clone()


    process.jetSequenceAK4 = cms.Sequence(
        process.ak4PFJets + 
        process.unpackedTracksAndVertices + 
        process.jetTracksAssociatorAtVertexAK4PF +
        process.patJetCorrFactorsAK4PF +
        process.patJetChargeAK4PF + 
        process.patJetPartons +
        process.patJetPartonMatchAK4PF +
        process.patJetGenJetMatchAK4PF +
        process.patJetFlavourAssociationAK4PF +
        process.patJetPartonsLegacy +
        process.patJetPartonAssociationLegacyAK4PF +
        process.patJetFlavourAssociationLegacyAK4PF +
        # process.inclusiveSecondaryVertexFinderTagInfosAK4PF + 
        process.impactParameterTagInfosAK4PF + 
        process.secondaryVertexTagInfosAK4PF + 
        process.combinedSecondaryVertexBJetTagsAK4PF +
        process.patJetsAK4PF +
        process.pileupJetIdCalculatorMET + 
        process.pileupJetIdCalculatorFull
        )
    return process.jetSequenceAK4
