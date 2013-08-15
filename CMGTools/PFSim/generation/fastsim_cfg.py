# Auto generated configuration file
# using: 
# Revision: 1.381.2.18 
# Source: /local/reps/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: TTbar_Tauola_7TeV_cfi.py -s GEN,FASTSIM,HLT:GRun --conditions=auto:startup_GRun --pileup=NoPileUp --geometry DB --eventcontent=AODSIM --datatier AODSIM -n 10 --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('HLT')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('FastSimulation.Configuration.EventContent_cff')
process.load('FastSimulation.PileUpProducer.PileUpSimulator_NoPileUp_cff')
process.load('FastSimulation.Configuration.Geometries_START_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('FastSimulation.Configuration.FamosSequences_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedParameters_cfi')
process.load('HLTrigger.Configuration.HLT_GRun_Famos_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.381.2.18 $'),
    annotation = cms.untracked.string('bla bla'),
    name = cms.untracked.string('PyReleaseValidation')
)

# Output definition

process.AODSIMoutput = cms.OutputModule("PoolOutputModule",
    eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
    outputCommands = process.AODSIMEventContent.outputCommands,
    fileName = cms.untracked.string('AODSIM.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('AODSIM')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)

# Additional output definition

# Other statements
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
process.famosSimHits.SimulateCalorimetry = True
process.famosSimHits.SimulateTracking = True
process.simulation = cms.Sequence(process.simulationWithFamos)
process.HLTEndSequence = cms.Sequence(process.reconstructionWithFamos)
process.Realistic8TeVCollisionVtxSmearingParameters.type = cms.string("BetaFunc")
process.famosSimHits.VertexGenerator = process.Realistic8TeVCollisionVtxSmearingParameters
process.famosPileUp.VertexGenerator = process.Realistic8TeVCollisionVtxSmearingParameters

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:startup_GRun', '')

# process.load('CMGTools.PFSim.generators.ee_hzha')
# process.load('CMGTools.PFSim.generators.ee_zz')
process.load('CMGTools.PFSim.generators.ee_qq')

# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen_genonly)
process.reconstruction = cms.Path(process.reconstructionWithFamos)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.AODSIMoutput_step = cms.EndPath(process.AODSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step)
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.reconstruction,
			 process.AODSIMoutput_step
			 ])
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 

# customisation of the process.

# Automatic addition of the customisation function from HLTrigger.Configuration.customizeHLTforMC
from HLTrigger.Configuration.customizeHLTforMC import customizeHLTforMC 

#call to customisation function customizeHLTforMC imported from HLTrigger.Configuration.customizeHLTforMC
process = customizeHLTforMC(process)

# End of customisation functions

def removeUselessReco(process):
	'''removing that leads to a problem in b tagging sequences.'''
	process.reconstructionWithFamos.remove( process.caloJetMet )
	process.reconstructionWithFamos.remove( process.famosTauTaggingSequence )
	

def tuneProcessForTLEP( process ):
	process.load("RecoVertex.BeamSpotProducer.BeamSpotFakeParameters_cfi")
	process.es_prefer_beamspot = cms.ESPrefer("BeamSpotFakeConditions")

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


tuneProcessForTLEP( process )
# removeUselessReco( process )
