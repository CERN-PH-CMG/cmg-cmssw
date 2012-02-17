from PhysicsTools.PatAlgos.patTemplate_cfg import *

# global options
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True),
                                      SkipEvent = cms.untracked.vstring('ProductNotFound')
                                      )
process.MessageLogger.cerr.FwkReport.reportEvery = 5000

# event source  
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring()
                            )
from CMGTools.HtoZZ2l2nu.localPatTuples_cff import configureFromCommandLine
castorDir, outputFile, process.source.fileNames = configureFromCommandLine()
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

#get the global tag
runOnMC=True
if(castorDir.find('Reco')>0 or castorDir.find('ReReco')>0 ): runOnMC=False
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
if ( not runOnMC ): process.GlobalTag.globaltag = 'GR_R_44_V13::All'
else:               process.GlobalTag.globaltag = 'START44_V12::All'

# jet energy corrections
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')


# preselection filters
from CMGTools.HtoZZ2l2nu.PreselectionSequences_cff import addPreselectionSequences,addLumifilter
if(not runOnMC ):  addPreselectionSequences(process)


# trigger filter
from CMGTools.HtoZZ2l2nu.TriggerSequences_cff import addTriggerSequence
if(not runOnMC):  addTriggerSequence(process,trigFilter)

# pat sequences
from CMGTools.HtoZZ2l2nu.PatSequences_cff import addPatSequence
addPatSequence(process,runOnMC,True)

# dilepton filters
from CMGTools.HtoZZ2l2nu.DileptonFilterSequences_cff import addDileptonFilters
addDileptonFilters(process)

# event counters
process.startCounter = cms.EDProducer("EventCountProducer")
process.endCounter = process.startCounter.clone()


# histogram service
process.TFileService = cms.Service("TFileService", fileName = cms.string(outputFile) )

# our MET producer
from CMGTools.HtoZZ2l2nu.ClusteredPFMetProducer_cfi import ClusteredPFMetProducer
process.ClusteredPFMetProducer = ClusteredPFMetProducer.clone()
process.ClusteredPFMetProducerPt5 = ClusteredPFMetProducer.clone( minJetPt = cms.double(5.0) )
process.ClusteredPFMetProducerPt10 = ClusteredPFMetProducer.clone( minJetPt = cms.double(10.0) )
process.ClusteredPFMetSequence = cms.Sequence(process.ClusteredPFMetProducer*process.ClusteredPFMetProducerPt5*process.ClusteredPFMetProducerPt10)

#pileup normalization
from CMGTools.HtoZZ2l2nu.PileupNormalizationProducer_cfi import puWeights
process.puWeights      = puWeights.clone( data = cms.string('/afs/cern.ch/user/p/psilva/public/Pileup/Pileup2011AplusB.root'),
                                                                                    mc   = cms.string('/afs/cern.ch/user/p/psilva/public/Pileup/Summer11Observed.root')
                                                                                    )
process.puWeightSequence = cms.Sequence(process.puWeights)


#
# configure the analyzer (cf. base values are in the StandardSelections_cfi)
#
from CMGTools.HtoZZ2l2nu.StandardSelections_cfi import *
process.evAnalyzer = cms.EDAnalyzer("DileptonPlusMETEventAnalyzer",
                                    dtag=cms.string('h2zz'),
                                    Trigger = BaseTriggerSelection.clone(),
                                    Generator = BaseGeneratorSelection.clone(),
                                    Vertices = BaseVertexSelection.clone(),
                                    Photons = BasePhotonsSelection.clone(),
                                    LooseMuons = BaseLooseMuonsSelection.clone(),
                                    Muons = BaseMuonsSelection.clone(),
                                    LooseElectrons = BaseLooseElectronsSelection.clone(),
                                    Electrons = BaseElectronsSelection.clone(),
                                    Dileptons = BaseDileptonSelection.clone(),
                                    Jets = BaseJetSelection.clone(),
                                    AssocJets = AssocJetSelection.clone(),
                                    MET = BaseMetSelection.clone()
                                    )

process.analysis = cms.Sequence( process.ClusteredPFMetSequence*process.puWeightSequence*process.evAnalyzer )

# define the paths
if(runOnMC):
    process.eePath = cms.Path(process.startCounter * process.patDefaultSequence * process.eeCandidateSequence * process.analysis )
    process.mumuPath  = cms.Path(process.startCounter * process.patDefaultSequence * process.mumuCandidateSequence * process.analysis )
    process.emuPath  = cms.Path(process.startCounter * process.patDefaultSequence * process.emuCandidateSequence * process.analysis )
else:
    process.eePath = cms.Path(process.startCounter * process.preselection * process.trigSequence * process.patDefaultSequence * process.eeCandidateSequence * process.analysis )
    process.mumuPath  = cms.Path(process.startCounter * process.preselection * process.trigSequence * process.patDefaultSequence * process.mumuCandidateSequence * process.analysis )
    process.emuPath  = cms.Path(process.startCounter * process.preselection * process.trigSequence * process.patDefaultSequence * process.emuCandidateSequence * process.analysis )

# all done, schedule the execution
if(runOnMC):
    from CMGTools.HtoZZ2l2nu.GeneratorLevelSequences_cff import addGeneratorLevelSequence
    addGeneratorLevelSequence(process)
    process.schedule = cms.Schedule( process.genLevelPath, process.eePath, process.mumuPath, process.emuPath )
else :
    process.schedule = cms.Schedule( process.eePath, process.mumuPath, process.emuPath )

print "Scheduling the following modules"
print process.schedule
print "Events will be selected for the following paths:"
print process.out.SelectEvents.SelectEvents
