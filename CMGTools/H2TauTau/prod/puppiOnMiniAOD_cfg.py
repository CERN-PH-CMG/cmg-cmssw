import FWCore.ParameterSet.Config as cms

from CMGTools.Production.datasetToSource import datasetToSource

sep_line = '-'*70

process = cms.Process("H2TAUTAU")

process.maxEvents = cms.untracked.PSet(input=cms.untracked.int32(-1))

numberOfFilesToProcess = -1
debugEventContent = False

# choose from 'tau-mu' 'di-tau' 'tau-ele' 'mu-ele' 'all-separate', 'all'
# channel = 'all'
channel = 'all'
s = 'miniAOD-prod_PAT_.*root'

dataset_user = 'CMS'
dataset_name = '/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v3/MINIAODSIM'

dataset_files = '.*root'

process.source = datasetToSource(
    dataset_user,
    dataset_name,
    dataset_files,
)

process.source.inputCommands = cms.untracked.vstring(
    'keep *'
)

process.options = cms.untracked.PSet(
    allowUnscheduled=cms.untracked.bool(True)
)

if numberOfFilesToProcess > 0:
    process.source.fileNames = process.source.fileNames[:numberOfFilesToProcess]

addPuppi = True

# Adding jet collection
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'MCRUN2_74_V9::All'
# process.GlobalTag.globaltag = 'auto:run2_mc'

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

from CMGTools.H2TauTau.objects.tauCuts_cff import tauPreSelection
process.tauPreSelection = tauPreSelection.clone()

process.tauCount = cms.EDFilter(
    "CandViewCountFilter",
    src=cms.InputTag("tauPreSelection"),
    minNumber=cms.uint32(1),
)

if addPuppi:
    process.load('CommonTools/PileupAlgos/Puppi_cff')

    process.puppi.candName = cms.InputTag('packedPFCandidates')
    process.puppi.vertexName = cms.InputTag('offlineSlimmedPrimaryVertices')

    process.packedPFCandidatesWoMuon = cms.EDFilter("CandPtrSelector", src=cms.InputTag("packedPFCandidates"), cut=cms.string("fromPV>=2 && abs(pdgId)!=13 "))
    process.particleFlowNoMuonPUPPI = process.puppi.clone()
    process.particleFlowNoMuonPUPPI.candName = 'packedPFCandidatesWoMuon'

    from RecoMET.METProducers.PFMET_cfi import pfMet
    process.pfMetPuppi = pfMet.clone()
    process.pfMetPuppi.src = cms.InputTag('puppi')

    process.puppiPath = cms.Path(
        process.tauPreSelection +
        process.tauCount +
        process.puppi +
        process.packedPFCandidatesWoMuon +
        process.particleFlowNoMuonPUPPI +
        process.pfMetPuppi
    )


# OUTPUT definition ----------------------------------------------------------


from CMGTools.H2TauTau.eventContent.common_cff import common
process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName=cms.untracked.string('puppi_tau_study.root'),
    # save only events passing the full path
    # save PAT Layer 1 output; you need a '*' to
    # unpack the list of commands 'patEventContent'
    SelectEvents=cms.untracked.PSet(
        SelectEvents=cms.vstring('puppiPath')
    ),
    outputCommands=cms.untracked.vstring(common)
)

process.outpath = cms.EndPath(process.out)

# process.outpath.insert(0, process.out)

# Message logger setup.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.options = cms.untracked.PSet(wantSummary=cms.untracked.bool(False))
