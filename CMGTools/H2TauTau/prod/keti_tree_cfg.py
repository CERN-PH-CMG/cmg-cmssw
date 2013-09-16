import FWCore.ParameterSet.Config as cms

########## CONTROL CARDS

process = cms.Process("LORTREE")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )


##########


# Input  & JSON             -------------------------------------------------


process.setName_('LORTREE')


from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    'cmgtools',
    '/VBF_HToTauTau_M-120_7TeV-powheg-pythia6-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_4_1',
    'tree.*root'
    )

process.source.fileNames = process.source.fileNames[:5]
    
# Sequence & path definition -------------------------------------------------

process.rootTuplePFJets = cms.EDProducer(
    "RootTupleMakerV2_PFJets",
    InputTag = cms.InputTag('cmgPFJetSel'),
    Prefix = cms.string('PFJet'),
    Suffix = cms.string(''),
    MaxSize = cms.uint32(10),
    #                                 JECUncertainty = cms.string('CondFormats/JetMETObjects/data/Spring10_Uncertainty_AK5PF.txt'),
    JECUncertainty = cms.string('AK5PF'),
    ReadJECuncertainty = cms.bool(False)
    #                                 ApplyResidualJEC = cms.bool(False),
    #                                 ResidualJEC = cms.string('CondFormats/JetMETObjects/data/Spring10_L2L3Residual_AK5PF.txt')
    )

process.rootTupleTree = cms.EDAnalyzer("RootTupleMakerV2_Tree",
    outputCommands = cms.untracked.vstring(
        'drop *',
        'keep *_rootTupleEvent_*_*',
        'keep *_rootTupleEventSelection_*_*',
        'keep *_rootTuplePFJets_*_*',
        'keep *_rootTuplePFTaus_*_*',
        'keep *_rootTupleElectrons_*_*',
        'keep *_rootTuplePFMET_*_*',
        'keep *_rootTuplePFMETType1Cor_*_*',
        'keep *_rootTupleMuons_*_*',
        'keep *_rootTupleTrigger_*_*',
        'keep *_rootTupleVertex_*_*',
        'keep *_rootTupleGenEventInfo_*_*',
        'keep *_rootTupleGenParticles_*_*',
        'keep *_rootTupleGenJets_*_*',
        'keep *_rootTupleGenMETTrue_*_*'
    )
)



# running only the tauMu selection and counting
process.p = cms.Path(
    process.rootTuplePFJets
    + process.rootTupleTree
)

# OUTPUT definition ----------------------------------------------------------

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("keti_tree.root")
    )

print process.source
