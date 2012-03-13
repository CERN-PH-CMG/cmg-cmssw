import FWCore.ParameterSet.Config as cms

process = cms.Process("PAT")

# global options
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 5000
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True),
                                      SkipEvent = cms.untracked.vstring('ProductNotFound')
                                      )


# event source
process.source = cms.Source("PoolSource",fileNames = cms.untracked.vstring())
process.source.fileNames=inputList
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# global tag
process.load("Configuration.StandardSequences.Geometry_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
if ( not runOnMC ): process.GlobalTag.globaltag = 'GR_R_44_V13::All'
else:               process.GlobalTag.globaltag = 'START44_V12::All'
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

#standard PAT
process.load("PhysicsTools.PatAlgos.patSequences_cff")

## Output Module Configuration (expects a path 'p')
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('patTuple.root'),
                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
                               outputCommands = cms.untracked.vstring('keep *'))

# preselection filters
from CMGTools.HtoZZ2l2nu.PreselectionSequences_cff import addPreselectionSequences,addLumifilter
if(not runOnMC ):
    addPreselectionSequences(process)
    addLumifilter(process,'/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON.txt')
        
# dilepton filter
from CMGTools.HtoZZ2l2nu.DileptonFilterSequences_cff import addDileptonFilters, addPhotonFilters
addDileptonFilters(process)
addPhotonFilters(process)

# pat sequences
from CMGTools.HtoZZ2l2nu.PatSequences_cff import addPatSequence
addPatSequence(process,runOnMC)

# event counters
process.startCounter = cms.EDProducer("EventCountProducer")
process.endCounter = process.startCounter.clone()

# define the paths
if(runOnMC):
    process.llPath = cms.Path(process.startCounter * process.llCandidateSequence * process.patSequence )
    process.photonPath = cms.Path(process.startCounter * process.photonCandidateSequence * process.patSequence )
else:
    process.llPath = cms.Path(process.startCounter * process.preselection * process.llCandidateSequence * process.patSequence )
    process.photonPath = cms.Path(process.startCounter * process.preselection * process.photonCandidateSequence * process.patSequence )
process.e = cms.EndPath( process.endCounter*process.out )

# all done, schedule the execution
if(runOnMC):
    from CMGTools.HtoZZ2l2nu.GeneratorLevelSequences_cff import addGeneratorLevelSequence
    addGeneratorLevelSequence(process)
    process.schedule = cms.Schedule( process.genLevelPath, process.llPath, process.photonPath, process.e )
else :
    process.schedule = cms.Schedule( process.llPath, process.photonPath, process.e )

# event output
from CMGTools.HtoZZ2l2nu.OutputConfiguration_cff import configureOutput
configureOutput(process)
configureOutput(process,selPaths=['llPath','photonPath'])
process.out.fileName = cms.untracked.string(outFile)

print "Scheduling the following modules"
print process.schedule
print "Events will be selected for the following paths:"
print process.out.SelectEvents.SelectEvents
process.outpath = cms.EndPath(process.out)
