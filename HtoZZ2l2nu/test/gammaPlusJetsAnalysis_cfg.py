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


# pat sequences
from CMGTools.HtoZZ2l2nu.PatSequences_cff import addPatSequence
addPatSequence(process,runOnMC,True)

# event counters
process.startCounter = cms.EDProducer("EventCountProducer")

#event analysis
process.load('CMGTools.HtoZZ2l2nu.GammaPlusJetsEventAnalyzer_cfi')
process.load('CMGTools.HtoZZ2l2nu.PileupNormalizationProducer_cfi')
process.TFileService = cms.Service("TFileService", fileName = cms.string(outFile) )

# all done, schedule the execution
process.p = cms.Path( process.startCounter * process.patDefaultSequence * process.evAnalyzer )

# no event output
del process.outpath

print "Scheduling the following modules (without edm output)"
print process.p
