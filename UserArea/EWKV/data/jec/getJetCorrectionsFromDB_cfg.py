import FWCore.ParameterSet.Config as cms

process = cms.Process("jectxt")
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

isMC=True

gt='FT_53_V21_AN4'
outName='Data'
if isMC :
    gt='START53_V23'
    outName='MC'
    
process.GlobalTag.globaltag = gt+'::All'
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))
process.source = cms.Source("EmptySource")
process.readAK5PFchs    = cms.EDAnalyzer('JetCorrectorDBReader',
                                         payloadName    = cms.untracked.string('AK5PFchs'),
                                         globalTag      = cms.untracked.string(outName),
                                         printScreen    = cms.untracked.bool(False),
                                         createTextFile = cms.untracked.bool(True)
                                         )

process.p = cms.Path(process.readAK5PFchs)

