import FWCore.ParameterSet.Config as cms
process = cms.Process("jectxt")
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
# define your favorite global tag
#process.GlobalTag.globaltag = 'START53_V10::All'  # HCP MC
#process.GlobalTag.globaltag = 'START53_V7G::All'  # Anna's MC
process.GlobalTag.globaltag = 'START53_V20::All'  # Latests as of 18 Apr 2013
#process.GlobalTag.globaltag = 'GR_P_V41_AN1::All' # HCP
#process.GlobalTag.globaltag = 'GR_P_V42_AN4::All' # Latest
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))
process.source = cms.Source("EmptySource")
process.readAK5PF    = cms.EDAnalyzer('JetCorrectorDBReader',  
    # below is the communication to the database 
    payloadName    = cms.untracked.string('AK5PF'),
    # this is used ONLY for the name of the printed txt files. You can use any name that you like, 
    # but it is recommended to use the GT name that you retrieved the files from.
    globalTag      = cms.untracked.string(process.GlobalTag.globaltag.value().replace("::All","")),  
    printScreen    = cms.untracked.bool(False),
    createTextFile = cms.untracked.bool(True)
)
process.readAK5PFchs = process.readAK5PF.clone(payloadName = 'AK5PFchs')
process.p = cms.Path(process.readAK5PF + process.readAK5PFchs)
