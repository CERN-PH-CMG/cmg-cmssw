# Auto generated configuration file
# using: 
# Revision: 1.353 
# Source: /local/reps/CMSSW.admin/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: CMG/JetIDAnalysis/SingleGluonGun_cfi --step GEN,SIM,DIGI,L1,DIGI2RAW,HLT:GRun,RAW2DIGI,L1Reco,RECO --conditions START50_V5::All --pileup 2012_Startup_50ns_PoissonOOTPU --datamix NODATAMIXER --eventcontent RECOSIM --datatier GEN-SIM-RECO -n 10
import FWCore.ParameterSet.Config as cms

process = cms.Process('HLT')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mix_2012_Startup_50ns_PoissonOOTPU_cfi')
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
    input = cms.untracked.int32(-1)
)

from CMG.JetIDAnalysis.minBiasSource_cfg import readFiles as FullMinBiasFileNames
process.mix.input.fileNames = FullMinBiasFileNames
#process.mix.input.fileNames = cms.untracked.vstring('/store/relval/CMSSW_4_4_2_patch7/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG/START44_V9_special_111126-v1/0072/269DF920-8118-E111-945D-003048FFD76E.root',
#						    '/store/relval/CMSSW_4_4_2_patch7/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG/START44_V9_special_111126-v1/0072/2E28D79F-8218-E111-9E38-003048679084.root',
#						    '/store/relval/CMSSW_4_4_2_patch7/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG/START44_V9_special_111126-v1/0072/5C4758B3-C818-E111-92FC-003048678FFA.root',
#						    '/store/relval/CMSSW_4_4_2_patch7/RelValMinBias/GEN-SIM-DIGI-RAW-HLTDEBUG/START44_V9_special_111126-v1/0071/4466B2D8-6518-E111-A761-0026189438FD.root')
import random
random.seed(process.RandomNumberGeneratorService.generator.initialSeed.value())
for i in xrange(0,1000) : random.random()
random.shuffle(process.mix.input.fileNames)
process.mix.input.nbPileupEvents.probFunctionVariable = cms.vint32(range(50))
process.mix.input.nbPileupEvents.probValue = cms.vdouble(1.17547e-05,0.000117547,0.000587733,0.00195911,0.00489777,0.00979555,0.0163259,0.0233227,0.0291534,0.0323927,0.0323927,0.0323927,0.0323927,0.0323927,0.0323927,0.0323927,0.0323927,0.0323927,0.0323927,0.0323927,0.0323927,0.0323927,0.0323927,0.0323927,0.0323927,0.0323927,0.0323927,0.0323927,0.0323927,0.0323927,0.0323927,0.0313478,0.0293885,0.0267168,0.0235737,0.020206,0.0168383,0.0136527,0.0107785,0.00829112,0.00621834,0.00455001,0.00325,0.00226744,0.00154598,0.00103066,0.000672167,0.000429043,0.000268152,0.000164175)


# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.353 $'),
    annotation = cms.untracked.string('CMG/JetIDAnalysis/SingleGluonGun_cfi nevts:10'),
    name = cms.untracked.string('PyReleaseValidation')
)

# Output definition

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RECOSIMEventContent.outputCommands,
    fileName = cms.untracked.string('SingleGluonGun_cfi_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_RAW2DIGI_L1Reco_RECO_PU.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM-RECO')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)

# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'START50_V15::All'

process.generator = cms.EDProducer("Pythia6PtYDistGun",
    PGunParameters = cms.PSet(
        MaxY = cms.double(5.0),
        MinPt = cms.double(3.0),
        ParticleID = cms.vint32(1),
        MaxPhi = cms.double(3.14159265359),
        MinY = cms.double(-5.0),
        YBinning = cms.int32(500),
        MinPhi = cms.double(-3.14159265359),
        kinematicsFile = cms.FileInPath('CMG/JetIDAnalysis/data/ptgun.root'),
        MaxPt = cms.double(300.0),
        PtBinning = cms.int32(100000)
    ),
    pythiaPylistVerbosity = cms.untracked.int32(1),
    firstRun = cms.untracked.uint32(1),
    Verbosity = cms.untracked.int32(0),
    psethack = cms.string('single quark pt 3-300'),
    pythiaHepMCVerbosity = cms.untracked.bool(True),
    maxEventsToPrint = cms.untracked.int32(5),
    PythiaParameters = cms.PSet(
        pythiaUESettings = cms.vstring('MSTU(21)=1     ! Check on possible errors during program execution', 
            'MSTJ(22)=2     ! Decay those unstable particles', 
            'PARJ(71)=10 .  ! for which ctau  10 mm', 
            'MSTP(33)=0     ! no K factors in hard cross sections', 
            'MSTP(2)=1      ! which order running alphaS', 
            'MSTP(51)=10042 ! structure function chosen (external PDF CTEQ6L1)', 
            'MSTP(52)=2     ! work with LHAPDF', 
            'PARP(82)=1.921 ! pt cutoff for multiparton interactions', 
            'PARP(89)=1800. ! sqrts for which PARP82 is set', 
            'PARP(90)=0.227 ! Multiple interactions: rescaling power', 
            'MSTP(95)=6     ! CR (color reconnection parameters)', 
            'PARP(77)=1.016 ! CR', 
            'PARP(78)=0.538 ! CR', 
            'PARP(80)=0.1   ! Prob. colored parton from BBR', 
            'PARP(83)=0.356 ! Multiple interactions: matter distribution parameter', 
            'PARP(84)=0.651 ! Multiple interactions: matter distribution parameter', 
            'PARP(62)=1.025 ! ISR cutoff', 
            'MSTP(91)=1     ! Gaussian primordial kT', 
            'PARP(93)=10.0  ! primordial kT-max', 
            'MSTP(81)=21    ! multiple parton interactions 1 is Pythia default', 
            'MSTP(82)=4     ! Defines the multi-parton model'),
        parameterSets = cms.vstring('pythiaUESettings')
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
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.digitisation_step,process.L1simulation_step,process.digi2raw_step)
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.RECOSIMoutput_step])
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 

