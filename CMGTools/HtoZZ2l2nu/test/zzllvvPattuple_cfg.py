from PhysicsTools.PatAlgos.patTemplate_cfg import *

# global tag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
if ( not runOnMC ): process.GlobalTag.globaltag = 'GR_R_42_V19::All' #'GR_R_42_V13::All'
else:               process.GlobalTag.globaltag = 'START42_V13::All' #'START42_V12::All'

# global options
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True),
                                      SkipEvent = cms.untracked.vstring('ProductNotFound')
                                      )
process.MessageLogger.cerr.FwkReport.reportEvery = 5000

# event source  
process.source.fileNames=inputList
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# preselection filters
from CMGTools.HtoZZ2l2nu.PreselectionSequences_cff import addPreselectionSequences,addLumifilter
if(not runOnMC ):
    addPreselectionSequences(process)
    #addLumifilter(process, lumilist )

# trigger filter
from CMGTools.HtoZZ2l2nu.TriggerSequences_cff import addTriggerSequence
if(not runOnMC): addTriggerSequence(process,trigFilter)

# pat sequences
from CMGTools.HtoZZ2l2nu.PatSequences_cff import addPatSequence
addPatSequence(process,runOnMC)

# dilepton filters
from CMGTools.HtoZZ2l2nu.DileptonFilterSequences_cff import addDileptonFilters
addDileptonFilters(process)

# event counters
process.startCounter = cms.EDProducer("EventCountProducer")
process.endCounter = process.startCounter.clone()

# define the paths
if(runOnMC):
    process.eePath = cms.Path(process.startCounter * process.patDefaultSequence * process.eeCandidateSequence )
    process.mumuPath  = cms.Path(process.startCounter * process.patDefaultSequence * process.mumuCandidateSequence )
    process.emuPath  = cms.Path(process.startCounter * process.patDefaultSequence * process.emuCandidateSequence )
else:
    process.eePath = cms.Path(process.startCounter * process.preselection * process.trigSequence * process.patDefaultSequence * process.eeCandidateSequence )
    process.mumuPath  = cms.Path(process.startCounter * process.preselection * process.trigSequence * process.patDefaultSequence * process.mumuCandidateSequence )
    process.emuPath  = cms.Path(process.startCounter * process.preselection * process.trigSequence * process.patDefaultSequence * process.emuCandidateSequence )
process.e = cms.EndPath( process.endCounter*process.out )

# all done, schedule the execution
if(runOnMC):
    from CMGTools.HtoZZ2l2nu.GeneratorLevelSequences_cff import addGeneratorLevelSequence
    addGeneratorLevelSequence(process)
    process.schedule = cms.Schedule( process.genLevelPath, process.eePath, process.mumuPath, process.emuPath, process.e )
else :
    process.schedule = cms.Schedule( process.eePath, process.mumuPath, process.emuPath, process.e )

# event output
from CMGTools.HtoZZ2l2nu.OutputConfiguration_cff import configureOutput
configureOutput(process)
process.out.fileName = cms.untracked.string(outFile)

print "Scheduling the following modules"
print process.schedule
print "Events will be selected for the following paths:"
print process.out.SelectEvents.SelectEvents
