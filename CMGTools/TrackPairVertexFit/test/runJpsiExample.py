import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'START53_LV6::All' # Legacy MC
# process.GlobalTag.globaltag = 'FT_R_53_LV5::All' # DATA
# process.GlobalTag.globaltag = 'GR_R_53_V21A::All' # random, to check
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(24000) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        '/store/cmst3/user/cmgtools/CMG/MuOnia/Run2011A-12Oct2013-v1/AOD/PAT_CMG_V5_18_0/cmgTuple_979.root'
    )
)

process.demo = cms.EDAnalyzer('JpsiVertexFitExample'
)
process.p = cms.Path(process.demo)
