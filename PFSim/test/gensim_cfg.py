import FWCore.ParameterSet.Config as cms

process = cms.Process("Sim")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(10)


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(2000)
)

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    generator = cms.PSet(
        initialSeed = cms.untracked.uint32(123456789),
        engineName = cms.untracked.string('HepJamesRandom')
    )
)


from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
         'cmgtools_group',
         '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B',
         '.*root')
process.source.fileNames = process.source.fileNames[:5]

import pprint 
pprint.pprint( process.source.fileNames ) 

process.load('CMGTools.PFSim.genJets_cff')

process.pfsim = cms.EDProducer(
    'PFSimParticleProducer',
    hepmcSrc = cms.InputTag('generator'),
    verbose = cms.untracked.bool( False )
    )

process.jets = cms.EDProducer(
    'PFSimFastJetProducer',
    particleSrc = cms.InputTag('pfsim'),
    jetPtThreshold = cms.double(5.),
    )


process.p = cms.Path(
    process.genJetsSequence
    + process.pfsim 
    + process.jets
    )



process.out = cms.OutputModule(
    "PoolOutputModule",
    dataset = cms.untracked.PSet(
    dataTier = cms.untracked.string('SIM')
    ),
    fileName = cms.untracked.string('gensim.root'),
    outputCommands = cms.untracked.vstring('keep *')
)


process.outpath = cms.EndPath(process.out)


test = False
if test:
    process.maxEvents.input = 10
    process.pfsim.verbose = True
