# Auto generated configuration file
# using: 
# Revision: 1.372.2.3 
# Source: /local/reps/CMSSW.admin/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: Configuration/GenProduction/python/EightTeV/POWHEG_PYTHIA6_Tauola_H_ZZ_2l2nu_8TeV_cff.py --filein=/store/cmst3/user/psilva/Powheg/VBF_600/events_600.lhe --fileout=events_600.lhe.root --filetype=LHE -s GEN --pileup=2012_Startup_50ns_PoissonOOTPU --mc --geometry DB --conditions=auto:startup --eventcontent=LHE --datatier LHE --number=-1
import FWCore.ParameterSet.Config as cms

process = cms.Process('GEN')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mix_E7TeV_Fall2011_Reprocess_50ns_PoissonOOTPU_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic8TeVCollision_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("LHESource",
    fileNames = cms.untracked.vstring('/store/cmst3/user/psilva/Madgraph/VBFZ_interf_7TeV/unweighted_events.lhe')
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.3 $'),
    annotation = cms.untracked.string('POWHEG + PYTHIA6 + Tauola - Higgs -> ZZ -> 2l2nu at 8TeV'),
    name = cms.untracked.string('$Source: /local/reps/CMSSW/UserCode/CMG/CMGTools/HtoZZ2l2nu/test/gen/LHEtoEDM_cfg.py,v $')
)

# Output definition

process.LHEoutput = cms.OutputModule("PoolOutputModule",
                                     splitLevel = cms.untracked.int32(0),
                                     eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
                                     outputCommands = process.AODSIMEventContent.outputCommands,
                                     fileName = cms.untracked.string('unweighted_events.lhe.root'),
                                     dataset = cms.untracked.PSet( filterName = cms.untracked.string(''),
                                                                   dataTier = cms.untracked.string('LHE')
                                                                   ),
                                     SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring()
                                                                        )
                                     )

# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'START44_V5::All'

# Path and EndPath definitions
process.endjob_step = cms.EndPath(process.endOfProcess)
process.LHEoutput_step = cms.EndPath(process.LHEoutput)

# Schedule definition
process.schedule = cms.Schedule(process.endjob_step,process.LHEoutput_step)


