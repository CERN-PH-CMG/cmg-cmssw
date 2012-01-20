import FWCore.ParameterSet.Config as cms
import sys 
import os 

process = cms.Process("PFAOD")

if len( sys.argv )!=3:
    raise Exception('you have to do: cmsRun file_cfg.py input_root_file')

thisCfg = sys.argv[1]

input = sys.argv[2]
sampleStr = os.path.splitext( input )[0]


print 'running: ', thisCfg
print 'input file', input

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#     '/store/relval/CMSSW_4_2_3/RelValZTT/GEN-SIM-RECO/START42_V12-v2/0062/4CEA9C47-287B-E011-BAB7-00261894396B.root'
    'file:' + input 
    )
)


process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))
#WARNING!
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(2000) )

process.load("Configuration.EventContent.EventContent_cff")
process.out = cms.OutputModule(
    "PoolOutputModule",
    process.AODSIMEventContent,
    fileName = cms.untracked.string( '' ),
    )

process.load("CommonTools.ParticleFlow.PF2PAT_EventContent_cff")
process.out.outputCommands.extend( process.prunedAODForPF2PATEventContent.outputCommands )

process.endpath = cms.EndPath(
    process.out
    )


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10


