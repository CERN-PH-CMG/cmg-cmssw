# Auto generated configuration file
# using: 
# Revision: 1.372.2.1 
# Source: /local/reps/CMSSW.admin/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: hzha --step SIM,DIGI,L1,DIGI2RAW,RAW2DIGI,L1Reco,RECO --conditions START52_V5::All --datamix NODATAMIXER --eventcontent RECOSIM,AODSIM --datatier GEN-SIM-RECO,AODSIM --no_exec -n 10 --filein file:hzha.root
import FWCore.ParameterSet.Config as cms

process = cms.Process('RECO')

# import of standard configurations
process.load("IOMC.RandomEngine.IOMC_cff")
process.load('IOMC.EventVertexGenerators.VtxSmearedGauss_cfi')
process.load("PhysicsTools.HepMCCandAlgos.genParticles_cfi")
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32( ==NEVT== )
)

# Input source
process.source = cms.Source("EmptySource")

#process.source = cms.Source("PoolSource",
#    secondaryFileNames = cms.untracked.vstring(),
#    fileNames = cms.untracked.vstring('file:hzha.root')
#)

process.options = cms.untracked.PSet(

)

#HZHA generator 
process.RandomNumberGeneratorService.generator.initialSeed = ==SEED==
process.generator = cms.EDFilter("HZHAGeneratorFilter",
  CardsPath = cms.string("GeneratorInterface/HZHAInterface/data/hzha04.cards")
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.3 $'),
    annotation = cms.untracked.string('hzha nevts:10'),
    name = cms.untracked.string('PyReleaseValidation')
)

# Output definition

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RECOSIMEventContent.outputCommands,
    fileName = cms.untracked.string('reco.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM-RECO')
    )
)

process.AODSIMoutput = cms.OutputModule("PoolOutputModule",
    eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
    outputCommands = process.AODSIMEventContent.outputCommands,
    fileName = cms.untracked.string('aod.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('AODSIM')
    )
)

# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'START52_V5::All'

# Remove HF (by setting large threshold on HF tower energies)
process.towerMakerPF.HF1Threshold = 1E9
process.towerMakerPF.HF2Threshold = 1E9
process.towerMaker.HF1Threshold = 1E9
process.towerMaker.HF2Threshold = 1E9
process.towerMakerWithHO.HF1Threshold = 1E9
process.towerMakerWithHO.HF2Threshold = 1E9

# LEP3 Beam Spot (in cm) for generation step
process.VtxSmeared.SigmaX = 0.0060
process.VtxSmeared.SigmaY = 0.00004
process.VtxSmeared.SigmaZ = 0.3

# LEP3 Beam Spot (in cm) for reconstruction step
process.load("RecoVertex.BeamSpotProducer.BeamSpotFakeParameters_cfi")
process.es_prefer_beamspot = cms.ESPrefer("BeamSpotFakeConditions")
process.BeamSpotFakeConditions.X0 = cms.double(0.)
process.BeamSpotFakeConditions.Y0 = cms.double(0.)
process.BeamSpotFakeConditions.Z0 = cms.double(0.)
process.BeamSpotFakeConditions.dxdz = cms.double(0.)
process.BeamSpotFakeConditions.dydz = cms.double(0.)
process.BeamSpotFakeConditions.sigmaZ = cms.double(0.3)
process.BeamSpotFakeConditions.widthX = cms.double(0.00004)
process.BeamSpotFakeConditions.widthY = cms.double(0.006)
process.BeamSpotFakeConditions.emittanceX = cms.double(0.)
process.BeamSpotFakeConditions.emittanceY = cms.double(0.)
process.BeamSpotFakeConditions.betaStar = cms.double(0.)
process.BeamSpotFakeConditions.errorX0 = cms.double(0.)
process.BeamSpotFakeConditions.errorY0 = cms.double(0.)
process.BeamSpotFakeConditions.errorZ0 = cms.double(0.)
process.BeamSpotFakeConditions.errordxdz = cms.double(0.)
process.BeamSpotFakeConditions.errordydz = cms.double(0.)
process.BeamSpotFakeConditions.errorSigmaZ = cms.double(0.)
process.BeamSpotFakeConditions.errorWidth = cms.double(0.)
 

# Path and EndPath definitions
process.generation_step = cms.Path(process.generator+process.genParticles)
process.simulation_step = cms.Path(process.psim)
process.digitisation_step = cms.Path(process.pdigi)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)
process.AODSIMoutput_step = cms.EndPath(process.AODSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,
                                process.simulation_step,
                                process.digitisation_step,
                                process.L1simulation_step,
                                process.digi2raw_step,
                                process.raw2digi_step,
                                process.L1Reco_step,
                                process.reconstruction_step,
                                process.endjob_step,
                                process.RECOSIMoutput_step,
                                process.AODSIMoutput_step)

