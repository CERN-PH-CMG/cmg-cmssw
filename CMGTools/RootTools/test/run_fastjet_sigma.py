# Add the following to your cfg file to calculate the fastjet sigma in the preprocessor
#from PhysicsTools.Heppy.utils.cmsswPreprocessor import CmsswPreprocessor
#preprocessor = CmsswPreprocessor("%s/src/CMGTools/RootTools/test/run_fastjet_sigma.py" % os.environ['CMSSW_BASE'])
#susyMultilepton_globalVariables.append(NTupleVariable("sigma",  lambda ev: ev.sigma, float, help="fixedGridSigmaFastjetAll sigma"))

# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: miniAOD-mc -s PAT --mc --runUnscheduled --eventcontent MINIAOD --conditions Summer15_V5_p6_MC --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('RERUN')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:miniAOD-mc_RECO.root'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('miniAOD-mc nevts:1'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.MINIAODoutput = cms.OutputModule("PoolOutputModule",
    compressionAlgorithm = cms.untracked.string('LZMA'),
    compressionLevel = cms.untracked.int32(4),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string(''),
        filterName = cms.untracked.string('')
    ),
    dropMetaData = cms.untracked.string('ALL'),
    eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
    fastCloning = cms.untracked.bool(False),
    fileName = cms.untracked.string('miniAOD-mc_PAT.root'),
    outputCommands = cms.untracked.vstring('keep *_fixedGridSigmaFastjetAll_*_RERUN'),
    overrideInputFileSplitLevels = cms.untracked.bool(True)
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'MCRUN2_74_V9A', '')

#from RecoJets.JetProducers.fixedGridRhoProducerFastjet_cfi import fixedGridRhoFastjetAll
#process.fixedGridRhoFastjetAll = fixedGridRhoFastjetAll.clone()
#process.p = cms.Path(process.fixedGridRhoFastjetAll)

from CMGTools.RootTools.modules.fixedGridSigmaProducerFastjet_cfi import fixedGridSigmaFastjetAll
process.fixedGridSigmaFastjetAll = fixedGridSigmaFastjetAll.clone()
process.p = cms.Path(process.fixedGridSigmaFastjetAll)

# Path and EndPath definitions
process.endjob_step = cms.EndPath(process.endOfProcess)
process.endpath = cms.EndPath(process.MINIAODoutput)

# Schedule definition
process.schedule = cms.Schedule(process.p,process.endjob_step,process.endpath)

#do not add changes to your config after this point (unless you know what you are doing)
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process=convertToUnscheduled(process)
#process.load('Configuration.StandardSequences.PATMC_cff')
#from FWCore.ParameterSet.Utilities import cleanUnscheduled
#process=cleanUnscheduled(process)
#
## customisation of the process.
#
## Automatic addition of the customisation function from PhysicsTools.PatAlgos.slimming.miniAOD_tools
#from PhysicsTools.PatAlgos.slimming.miniAOD_tools import miniAOD_customizeAllMC 
#
##call to customisation function miniAOD_customizeAllMC imported from PhysicsTools.PatAlgos.slimming.miniAOD_tools
#process = miniAOD_customizeAllMC(process)
#
## End of customisation functions
