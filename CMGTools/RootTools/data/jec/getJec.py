import FWCore.ParameterSet.Config as cms
process = cms.Process("jectxt")
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# define your favorite global tag (OLD GTs)
#process.GlobalTag.globaltag = 'PHYS14_25_V2::All'  
# define your favorite global tag (newer GTs in condDBv2)
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = '74X_mcRun2_asymptotic_v2')
process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_CONDITIONS'
process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))
process.source = cms.Source("EmptySource")
process.read = cms.EDAnalyzer('JetCorrectorDBReader',  
    # below is the communication to the database 
    payloadName    = cms.untracked.string('AK4PFchs'),
    # this is used ONLY for the name of the printed txt files. You can use any name that you like, 
    # but it is recommended to use the GT name that you retrieved the files from.
    globalTag      = cms.untracked.string(process.GlobalTag.globaltag.value().replace("::All","")),  
    printScreen    = cms.untracked.bool(False),
    createTextFile = cms.untracked.bool(True)
)
process.p = cms.Path(process.read)
