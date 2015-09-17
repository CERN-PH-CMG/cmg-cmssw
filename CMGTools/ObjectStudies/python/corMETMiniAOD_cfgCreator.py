import FWCore.ParameterSet.Config as cms
import os

##
# This file corresponds to 
# https://twiki.cern.ch/twiki/bin/view/CMS/MissingETUncertaintyPrescription#Instructions_for_7_4_X 
##

from optparse import OptionParser
parser = OptionParser()
parser.add_option("--outputFile", dest="outputFile", default="MetType1_dump.py", type="string", action="store", help="output file")
parser.add_option("--GT", dest="GT", default='MCRUN2_74_V9A', type="string", action="store", help="Global Tag")
parser.add_option("--jecDBFile", dest="jecDBFile", default="", type="string", action="store", help="jec DB File")
parser.add_option("--uncFile", dest="uncFile", default="", type="string", action="store", help="jec Uncer File")
parser.add_option("--jecEra", dest="jecEra", default='', type="string", action="store", help="jecEra")
parser.add_option("--maxEvents", dest="maxEvents", default=-1, type="int", action="store", help="maxEvents")
parser.add_option("--removeResiduals", dest="removeResiduals", action="store_true", default=False, help="remove residual JEC?") 
parser.add_option("--isData", dest="isData", action="store_true", default=False, help="is data?") 
parser.add_option("--addReclusterTrackJetsAK4", dest="reclusterTrackJets", action="store_true", default=False, help="recluster AK4 track jets") 
(options, args) = parser.parse_args()

print "cmsswPreprocessor options: isData: %s, GT:%s, removeResiduals: %s jecEra: %s"%(options.isData, options.GT,  options.removeResiduals, options.jecEra)

#print options.outputFile, options.GT

# Define the CMSSW process
process = cms.Process("RERUN")

# Load the standard set of configuration modules
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

#configurable options =======================================================================
usePrivateSQlite=True #use external JECs (sqlite file)
applyResiduals=False #application of residual corrections. Have to be set to True once the 13 TeV residual corrections are available. False to be kept meanwhile. Can be kept to False later for private tests or for analysis checks and developments (not the official recommendation!).
#===================================================================

# Message Logger settings
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

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

fname = 'root://eoscms.cern.ch//store/data/Run2015B/JetHT/MINIAOD/PromptReco-v1/000/251/252/00000/263D331F-AF27-E511-969B-02163E012627.root' if options.isData else 'root://eoscms//eos/cms/store/mc/RunIISpring15DR74/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/Asympt50ns_MCRUN2_74_V9A-v2/60000/001C7571-0511-E511-9B8E-549F35AE4FAF.root'
# Define the input source
process.source = cms.Source("PoolSource", 
    fileNames = cms.untracked.vstring([ fname ])
)

### ---------------------------------------------------------------------------
### Removing the HF from the MET computation
### ---------------------------------------------------------------------------
process.noHFCands = cms.EDFilter("CandPtrSelector",
                                 src=cms.InputTag("packedPFCandidates"),
                                 cut=cms.string("abs(pdgId)!=1 && abs(pdgId)!=2 && abs(eta)<3.0")
                                 )



### =====================================================================================================
from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD

#uncertainty file
###jecUncertaintyFile="$CMSSW_BASE/src/CMGTools/RootTools/data/jec/Summer15_50nsV4_DATA_UncertaintySources_AK4PFchs.txt"

#default configuration for miniAOD reprocessing, change the isData flag to run on data
#for a full met computation, remove the pfCandColl input
runMetCorAndUncFromMiniAOD(process,
                           isData=options.isData,
#                           jecUncFile=uncFile
                           )

runMetCorAndUncFromMiniAOD(process,
                           isData=options.isData,
                           pfCandColl=cms.InputTag("noHFCands"),
#                           jecUncFile=uncFile,
                           postfix="NoHF"
                           )

### -------------------------------------------------------------------
### the lines below remove the L2L3 residual corrections when processing data
### -------------------------------------------------------------------
if options.removeResiduals:
    process.patPFMetT1T2Corr.jetCorrLabelRes = cms.InputTag("L3Absolute")
    process.patPFMetT1T2SmearCorr.jetCorrLabelRes = cms.InputTag("L3Absolute")
    process.patPFMetT2Corr.jetCorrLabelRes = cms.InputTag("L3Absolute")
    process.patPFMetT2SmearCorr.jetCorrLabelRes = cms.InputTag("L3Absolute")
    process.shiftedPatJetEnDown.jetCorrLabelUpToL3Res = cms.InputTag("ak4PFCHSL1FastL2L3Corrector")
    process.shiftedPatJetEnUp.jetCorrLabelUpToL3Res = cms.InputTag("ak4PFCHSL1FastL2L3Corrector")

    process.patPFMetT1T2CorrNoHF.jetCorrLabelRes = cms.InputTag("L3Absolute")
    process.patPFMetT1T2SmearCorrNoHF.jetCorrLabelRes = cms.InputTag("L3Absolute")
    process.patPFMetT2CorrNoHF.jetCorrLabelRes = cms.InputTag("L3Absolute")
    process.patPFMetT2SmearCorrNoHF.jetCorrLabelRes = cms.InputTag("L3Absolute")
    process.shiftedPatJetEnDownNoHF.jetCorrLabelUpToL3Res = cms.InputTag("ak4PFCHSL1FastL2L3Corrector")
    process.shiftedPatJetEnUpNoHF.jetCorrLabelUpToL3Res = cms.InputTag("ak4PFCHSL1FastL2L3Corrector")
### ------------------------------------------------------------------



process.MINIAODSIMoutput = cms.OutputModule("PoolOutputModule",
    compressionLevel = cms.untracked.int32(4),
    compressionAlgorithm = cms.untracked.string('LZMA'),
    eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
    outputCommands = cms.untracked.vstring( "keep *_slimmedMETs_*_*",
                                            "keep *_patPFMetT1Txy_*_*",
#                                            "keep patJets_*_*_RERUN", #for debugging only
                                            "keep *_slimmedMETsNoHF_*_*",
                                            "keep *_patPFMetT1TxyNoHF_*_*",
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

if options.reclusterTrackJets:
    process.MessageLogger.destinations = ['cerr'] # temporarily avoid a lot of printouts
    process.MessageLogger.cerr.threshold = cms.untracked.string('ERROR') # temporarily avoid a lot of printouts
    process.pfChargedCHS = cms.EDFilter("CandPtrSelector", src = cms.InputTag("packedPFCandidates"), cut = cms.string("fromPV && charge!=0"))
    from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets
    process.ak4PFChargedJetsCHS = ak4PFJets.clone(src = 'pfChargedCHS', doAreaFastjet = True)
    from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection
    addJetCollection(
        process,
        postfix = "",
        labelName = 'AK4ChargedPFCHS',
        jetSource = cms.InputTag('ak4PFChargedJetsCHS'),
        pvSource = cms.InputTag('offlineSlimmedPrimaryVertices'),
        pfCandidates = cms.InputTag('packedPFCandidates'),
        svSource = cms.InputTag('slimmedSecondaryVertices'),
        btagDiscriminators = ['None'],
        jetCorrections = ('AK4PFchs', [], 'None'),
        genJetCollection = cms.InputTag('slimmedGenJets'),
        genParticles = cms.InputTag('prunedGenParticles'),
        algo = 'AK',
        rParam = 0.4
        )
    process.MessageLogger.cerr.FwkReport.reportEvery = 1000
    process.MINIAODSIMoutput.outputCommands.append("keep patJets_patJetsAK4ChargedPFCHS__RERUN")
    process.p = cms.Path(process.pfChargedCHS*process.ak4PFChargedJetsCHS*process.patJetsAK4ChargedPFCHS)


process.endpath = cms.EndPath(process.MINIAODSIMoutput)

ofile = os.path.expandvars(options.outputFile)
if os.path.isfile(ofile): os.remove(ofile)
dumpFile  = open(ofile, "w")
dumpFile.write(process.dumpPython())
dumpFile.close()
print "Written preprocessor cfg to %s"%ofile
