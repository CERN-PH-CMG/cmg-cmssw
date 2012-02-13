import FWCore.ParameterSet.Config as cms

########## CONTROL CARDS

process = cms.Process("LORTREE")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

# -1 : process all files
numberOfFilesToProcess = 10
runOnMC = True 
dataset_user = 'cbern' 
dataset_name = '/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_5_0/H2TAUTAU_Feb2'
dataset_pattern = 'tauMu_fullsel_tree.*root'


##########


# Input  & JSON             -------------------------------------------------


process.setName_('LORTREE')


from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    dataset_user,
    dataset_name,
    'tauMu_fullsel.*root')


# process.source.fileNames = ['file:h2TauTau_fullsel_tree_CMG.root']
    
# Sequence & path definition -------------------------------------------------

process.muTauStreamAnalyzer = cms.EDAnalyzer(
    "MuTauStreamAnalyzer",
    diTaus         = cms.InputTag("cmgTauMuCorSVFitFullSel"),
    jets           = cms.InputTag("cmgPFJetSel"),
    newJets        = cms.InputTag(""),
    #COLIN warning uncorrected MET. corrected MET can be obtained from the diTau (I think)
    met            = cms.InputTag("cmgPFMET"),
    rawMet         = cms.InputTag("cmgPFMET"),
    #COLIN what are these 2 collections of muons? 
    muons          = cms.InputTag("cmgMuonSel"),
    muonsRel       = cms.InputTag("cmgMuonSel"),
    vertices       = cms.InputTag("offlinePrimaryVertices"),
    triggerResults = cms.InputTag("patTriggerEvent"),
    isMC           = cms.bool(runOnMC),
    genJets        = cms.InputTag("genJetSel"), 
    deltaRLegJet   = cms.untracked.double(0.5),
    minCorrPt      = cms.untracked.double(15.),
    minJetID       = cms.untracked.double(0.5), # 1=loose,2=medium,3=tight
    verbose        = cms.untracked.bool( False ),
    )

# running only the tauMu selection and counting
process.p = cms.Path(
    process.muTauStreamAnalyzer
)

# OUTPUT definition ----------------------------------------------------------

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("treeMuTauStream.root")
    )
