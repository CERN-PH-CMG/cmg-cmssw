#
# cmsrel CMSSW_4_4_2_patch9
#
# Auto generated configuration fileimport FWCore.Parame
# using: 
# Revision: 1.341 
# Source: /cvs/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: SingleQuarkGun_cfi.py --step GEN,SIM,DIGI,L1,DIGI2RAW,HLT:GRun,RAW2DIGI,L1Reco,RECO --conditions START44_V10::All --pileup mix_E7TeV_Chamonix2012_50ns_PoissonOOT --datamix NODATAMIXER --eventcontent AODSIM --datatier AODSIM -n 25
import FWCore.ParameterSet.Config as cms

process = cms.Process('HLT')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mix_E7TeV_Fall2011_Reprocess_50ns_PoissonOOTPU_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic7TeV2011Collision_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('HLTrigger.Configuration.HLT_GRun_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')


process.maxEvents = cms.untracked.PSet(
	input = cms.untracked.int32(XXX_NEVENTS_XXX)
	)

process.RandomNumberGeneratorService.generator.initialSeed = cms.untracked.uint32(XXX_SEED_XXX)
process.RandomNumberGeneratorService.mix.initialSeed = cms.untracked.uint32(XXX_SEED_XXX)

# Input source
process.source = cms.Source("PoolSource",
			    secondaryFileNames = cms.untracked.vstring(),
			    fileNames = cms.untracked.vstring('/store/cmst3/user/psilva/Madgraph/VBFZ_interf_7TeV/unweighted_events.lhe.root'),
			    skipEvents = cms.untracked.uint32(XXX_SKIP_XXX)
			    )

process.options = cms.untracked.PSet(
	
)

process.mix.input.fileNames = cms.untracked.vstring('rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/04AE43EF-1C58-E011-82C3-0015C5E5B22E.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/0C723277-8458-E011-B6BD-000AE488BBBA.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/220C1ADB-9B59-E011-992C-0015178C4B28.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/2ED9C9F9-4A58-E011-9EC6-0022198273F0.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/36371B4C-4B58-E011-A853-0024E850DF73.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/36E1C33B-4A58-E011-B606-0024E85A4FEF.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/486182FA-B659-E011-BB64-00151796C470.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/4E077A9F-AA59-E011-809A-0024E876635F.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/521C3627-B259-E011-BC6F-00151796C140.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/528E75E9-B159-E011-8212-00151796C0F0.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/62D7FA7B-4159-E011-91E2-0024E8766422.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/6AD0394D-4A58-E011-8E43-0024E85A4EB3.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/706173B0-4858-E011-8BC0-BC305B390A25.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/7C0B1D56-2F58-E011-9B8C-00188B89EEFA.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/7CB4CB0F-EA59-E011-B9DE-00A0D1EEAA00.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/7E7892B7-4858-E011-BF8C-BC305B390AA7.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/82C56689-B959-E011-8F4D-00A0D1EEF6B8.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/9EDA8825-1258-E011-9A20-000AE4889FB6.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/9EF955E3-B659-E011-AC50-00151796D708.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/A0533002-B759-E011-B1CA-00151796C1CC.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/B2F2175B-B159-E011-96FA-0024E87681FD.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/B6A75789-2C58-E011-9B2A-00266CF83958.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/BE360FA7-8759-E011-9CC3-00266CF833EC.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/C81C116C-3658-E011-88DC-001D09645B2D.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/CA1242FC-4A58-E011-9237-0022198273C0.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/CCE20350-4A58-E011-A42C-0024E85A4EB3.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/D03AC72A-2858-E011-ADDA-0026B95A45EC.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/D05C8EE0-2E58-E011-89E4-0026B93F4ADF.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/D09019E2-7B59-E011-8C63-00266CF834E0.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/D22D24DA-B659-E011-8E1E-0015178C4BE4.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/D639D3CC-A559-E011-A5BB-00266CF85EA8.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/D653CC4C-4B58-E011-9B56-0024E850DF73.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/E2A32C80-B959-E011-AB98-00266CF253C4.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/E82FAC88-E559-E011-A0E9-00151796C06C.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/EE85ECB8-4258-E011-9189-00266CF8377C.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/EE932991-B959-E011-BBC1-00151796C1CC.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/F4BDF130-B859-E011-9A12-0015178C6630.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/FAC66105-6059-E011-AC34-00A0D1EEF69C.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/FC4CB683-B959-E011-9E1C-00266CF2580C.root',
						    'rfio:///castor/cern.ch/user/t/trocino/ZZllnn/MinBias_START311_V2/F67D4F59-0858-E011-A223-00215A491956.root')

import random
random.seed(process.RandomNumberGeneratorService.generator.initialSeed.value())
random.shuffle(process.mix.input.fileNames)



# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.6 $'),
    annotation = cms.untracked.string('cmsRun_FullSim_44x.py nevts:25'),
    name = cms.untracked.string('PyReleaseValidation')
)

# Output definition
process.AODSIMoutput = cms.OutputModule("PoolOutputModule",
					eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
					outputCommands = process.AODSIMEventContent.outputCommands,
					fileName = cms.untracked.string('/afs/cern.ch/user/p/psilva/work/FullSim/VBFZ_interf_7TeV/Events_XXX_I_XXX.root'),
					dataset = cms.untracked.PSet( filterName = cms.untracked.string(''),
								      dataTier = cms.untracked.string('AODSIM')
								      ),
					SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('generation_step')
									   )
					)

# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'START44_V10::All'

from Configuration.Generator.PythiaUEZ2Settings_cfi import *

process.generator = cms.EDFilter("Pythia6HadronizerFilter",
			 pythiaPylistVerbosity = cms.untracked.int32(0),
			 filterEfficiency = cms.untracked.double(0.254),
			 pythiaHepMCVerbosity = cms.untracked.bool(False),
			 comEnergy = cms.double(7000.0),
			 maxEventsToPrint = cms.untracked.int32(0),
			 PythiaParameters = cms.PSet(
	pythiaUESettings = cms.vstring(
	'MSTJ(11)=3      ! Choice of the fragmentation function',
	'MSTP(81)=1      ! multiple parton interactions 1 is Pythia default',
	'MSTP(82)=4      ! Defines the multi-parton model',
	'PARP(67)=2.5    ! amount of initial-state radiation',
	'PARP(85)=1.0    ! gluon prod. mechanism in MI',
	'PARP(86)=1.0    ! gluon prod. mechanism in MI',
	'PARP(64)=0.2    ! ',
	'PARP(91)=2.1    ! kt distribution'
	),
	processParameters = cms.vstring('MSEL=0         ! User defined processes',
					'PMAS(5,1)=4.4   ! b quark mass',
					'PMAS(6,1)=172.4 ! t quark mass',
					'MSTJ(1)=1       ! Fragmentation/hadronization on or off',
					'MSTP(61)=1      ! Parton showering on or off'),
	parameterSets = cms.vstring('pythiaUESettings', 'processParameters')
	)
			 )




# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim)
process.digitisation_step = cms.Path(process.pdigi)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.AODSIMoutput_step = cms.EndPath(process.AODSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.digitisation_step,process.L1simulation_step,process.digi2raw_step)
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.AODSIMoutput_step])
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 

