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

process.mix.input.fileNames = cms.untracked.vstring(
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/0018E409-4555-E011-A7DE-D4856445D504.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/004940BA-B74F-E011-911B-003048CDCDD0.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/041EEBF0-6159-E011-9872-0015178C4B3C.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/0AE983CC-B34E-E011-A9D9-00145EFC5A2C.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/0CDCEAF3-2450-E011-AAC1-0026B95A456D.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/0E709C73-9F4F-E011-9F06-00266CF0291C.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/10919D06-5952-E011-A5B8-BC305B390AA7.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/18B53E59-F351-E011-9DFF-0026B94DBDD6.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/1CE59177-3155-E011-A33F-002219826BCD.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/1E685819-6052-E011-9B50-002219231C79.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/2204B9D7-0D4F-E011-8692-002219826F4D.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/24761FEA-C153-E011-B950-0024E85A4FE7.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/2611A17D-054F-E011-972F-002219826F27.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/263806C0-A954-E011-A246-003048CF6052.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/26AEA011-6A50-E011-9153-00221981B40C.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/2C56F283-3F54-E011-BF59-0026B9277A0B.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/3079E3A1-0350-E011-9F15-0022198273F8.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/3266D205-2853-E011-A785-0024E850DF73.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/361EE936-374F-E011-A1A7-BC305B390AB4.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/3C4E0A2D-6754-E011-89CC-000AE4893E36.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/404FC5CA-DA50-E011-A01A-001EC9ED8F27.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/44F4504A-AD51-E011-A313-001EC9ED5F33.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/461AC632-1B4F-E011-8B97-003048CF66D0.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/46C1DA76-4651-E011-852D-0024E85A4C00.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/5A3A854C-8852-E011-A77E-003048CDC876.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/5EEE757D-D350-E011-9AC5-0015C5EC47A2.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/62FAA08C-9F53-E011-92D1-0026B927865E.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/64311A50-334F-E011-A18F-003048CB7DA6.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/64A9BB30-CB4E-E011-A1BC-BC305B390AB4.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/68EA89A8-8950-E011-A435-0024E86E8D18.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/6C7A4523-574F-E011-9A0E-0026B93F4E32.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/784EA3F1-5850-E011-8C11-000AE488B8B8.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/7A039986-6854-E011-B695-0026B95A45FB.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/82EE1721-5E52-E011-A4CF-000AE48827E5.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/8458D51A-074F-E011-923E-0024E85A3F71.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/86180BC2-6051-E011-9795-0026B951BAB0.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/863FD463-854F-E011-BD11-0024E85A4BEC.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/881AAF77-CD4F-E011-AD2C-001EC9ED7E52.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/8856C1A7-8F55-E011-A0BF-002219826BAF.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/888A5F5E-804F-E011-AAAB-00145EDD74ED.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/8A5F55AD-F351-E011-831F-000AE4889FB6.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/8AE44A2A-094F-E011-A455-0024E85A3EE0.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/8CD6A099-C353-E011-B75E-0024E85A0924.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/903B795A-9552-E011-A2DC-0024E850DF94.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/90F5D15D-8650-E011-88F1-0026B9278644.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/947859F3-5055-E011-9CD6-BC305B390A59.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/9E74EEFA-EC52-E011-8F0D-00266CF8592C.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/A0A9FDC6-AB59-E011-9D3B-0026B94E287F.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/A278153D-3E4F-E011-B277-BC305B390A25.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/A6748600-9B54-E011-9DDE-BC305B390A25.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/A84BC8D7-6051-E011-BFF8-00266CF8377C.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/A8D643C9-AB59-E011-A3FB-0024E8768231.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/AE375D86-CF52-E011-83B3-0024E85A08FC.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/B0BA7B02-8D52-E011-8670-0026B9278603.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/B22492C9-0955-E011-B255-0015C5E5B335.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/B42491BC-7D51-E011-8CF8-0026B9278637.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/B4665CD0-F04E-E011-93EF-003048CF609C.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/B601014E-0355-E011-8265-0024E8768299.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/C23C0BC8-4E52-E011-9DB7-D4856445B5A4.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/C6D05558-1055-E011-ADB2-002219813E38.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/CEF6B75F-F14F-E011-8299-00266CF25C88.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/D461B45B-9D53-E011-B49F-00215A45F86A.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/D6B69948-0351-E011-87CE-0015C5E5B9A7.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/D82D0C3A-A252-E011-8520-000AE4882817.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/DA1BF3BF-3251-E011-BDAC-0024E85A3F0C.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/DE4DA547-8C50-E011-95AA-00266CF832D4.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/E222282B-0D4F-E011-8CE9-0026B94DBDF0.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/E2B92BB6-DA51-E011-AC9B-0026B927866B.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/E40CA309-8550-E011-A147-000AE488BBBA.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/E4312EE1-A052-E011-973E-0026B95A4E13.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/E43DA512-EA59-E011-B871-00266CF2580C.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/E4C3F598-6950-E011-8CF4-0024E85A4FD7.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/F25EE053-0453-E011-A37C-0022640691C8.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/F641077B-1D50-E011-84CE-00215AA64960.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/F68F69D2-A254-E011-AC0F-003048CF5F68.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/FA260B5A-7252-E011-AE81-00266CF8355C.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/FAD8F9B6-D659-E011-BE59-0024E8768072.root',
'/store/user/trocino/MinBias_START311_V2/GEN-SIM/FEFB0CD5-AB59-E011-8E0A-0024E87683DE.root'
)

import random
random.seed(process.RandomNumberGeneratorService.generator.initialSeed.value())
random.shuffle(process.mix.input.fileNames)



# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
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

