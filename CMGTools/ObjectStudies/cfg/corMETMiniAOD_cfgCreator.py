import FWCore.ParameterSet.Config as cms
import os

##
# This file corresponds to 
# https://twiki.cern.ch/twiki/bin/view/CMS/MissingETUncertaintyPrescription#Instructions_for_7_4_X 
##

from optparse import OptionParser
parser = OptionParser()
parser.add_option("--outputFile", dest="outputFile", default="MetType1_dump.py", type="string", action="store", help="output file")
parser.add_option("--GT", dest="GT", default='MCRUN2_74_V9A::All', type="string", action="store", help="Global Tag")
parser.add_option("--jecDBFile", dest="jecDBFile", default="", type="string", action="store", help="jec DB File")
parser.add_option("--jecEra", dest="jecEra", default='', type="string", action="store", help="jecEra")
parser.add_option("--maxEvents", dest="maxEvents", default=-1, type="int", action="store", help="maxEvents")
(options, args) = parser.parse_args()

#print options.outputFile, options.GT

# Define the CMSSW process
process = cms.Process("RERUN")

# Load the standard set of configuration modules
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# Message Logger settings
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1

# Set the process options -- Display summary at the end, enable unscheduled execution
process.options = cms.untracked.PSet( 
    allowUnscheduled = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(False) 
)

# How many events to process
process.maxEvents = cms.untracked.PSet( 
   input = cms.untracked.int32(options.maxEvents)
)

### =====================================================================================================

from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag.globaltag = options.GT

usePrivateSQlite = options.jecDBFile!=''
if usePrivateSQlite:
    from CondCore.DBCommon.CondDBSetup_cfi import *
    process.jec = cms.ESSource("PoolDBESSource",CondDBSetup,
                               connect = cms.string('sqlite_file:'+os.path.expandvars(options.jecDBFile)),
                               toGet =  cms.VPSet(
            cms.PSet(
                record = cms.string("JetCorrectionsRecord"),
                tag = cms.string("JetCorrectorParametersCollection_"+options.jecEra+"_AK4PF"),
                label= cms.untracked.string("AK4PF")
                ),
            cms.PSet(
                record = cms.string("JetCorrectionsRecord"),
                tag = cms.string("JetCorrectorParametersCollection_"+options.jecEra+"_AK4PFchs"),
                label= cms.untracked.string("AK4PFchs")
                ),
            )
                               )
    process.es_prefer_jec = cms.ESPrefer("PoolDBESSource",'jec')

### =====================================================================================================


# Define the input source
process.source = cms.Source("PoolSource", 
    fileNames = cms.untracked.vstring([
            'root://eoscms//eos/cms/store/mc/RunIISpring15DR74/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/Asympt50ns_MCRUN2_74_V9A-v2/60000/001C7571-0511-E511-9B8E-549F35AE4FAF.root' 
    ])
)


### =====================================================================================================
from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD

#default configuration for miniAOD reprocessing, change the isData flag to run on data
runMetCorAndUncFromMiniAOD(process, isData=False)

### -------------------------------------------------------------------
### the lines below remove the L2L3 residual uncertainties when processing data
### -------------------------------------------------------------------
process.patPFMetT1T2Corr.jetCorrLabelRes = cms.InputTag("L3Absolute")
process.patPFMetT1T2SmearCorr.jetCorrLabelRes = cms.InputTag("L3Absolute")
process.patPFMetT2Corr.jetCorrLabelRes = cms.InputTag("L3Absolute")
process.patPFMetT2SmearCorr.jetCorrLabelRes = cms.InputTag("L3Absolute")
process.shiftedPatJetEnDown.jetCorrLabelUpToL3Res = cms.InputTag("ak4PFCHSL1FastL2L3Corrector")
process.shiftedPatJetEnUp.jetCorrLabelUpToL3Res = cms.InputTag("ak4PFCHSL1FastL2L3Corrector")


process.MINIAODSIMoutput = cms.OutputModule("PoolOutputModule",
    compressionLevel = cms.untracked.int32(4),
    compressionAlgorithm = cms.untracked.string('LZMA'),
    eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
    outputCommands = cms.untracked.vstring( "keep *_slimmedMETs_*_*",
                                            "keep *_patPFMetT1Txy_*_*",
#                                            "keep patJets_*_*_RERUN", #for debugging only
                                            ),
    fileName = cms.untracked.string('corMETMiniAOD.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('')
    ),
    dropMetaData = cms.untracked.string('ALL'),
    fastCloning = cms.untracked.bool(False),
    overrideInputFileSplitLevels = cms.untracked.bool(True)
)


process.endpath = cms.EndPath(process.MINIAODSIMoutput)

ofile = os.path.expandvars(options.outputFile)
os.remove(dumpFile)
dumpFile  = open(ofile, "w")
dumpFile.write(process.dumpPython())
dumpFile.close()
print "Written preprocessor cfg to %s"%ofile
