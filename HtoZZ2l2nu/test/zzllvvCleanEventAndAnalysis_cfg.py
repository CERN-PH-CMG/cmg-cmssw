import FWCore.ParameterSet.Config as cms

process = cms.Process("HtoZZto2l2nu")

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'START42_V13::All'
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

#the source is configured from the command line
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring()
                            )

# wrapper to query command line
from CMGTools.HtoZZ2l2nu.localPatTuples_cff import configureFromCommandLine
castorDir, outputFile, process.source.fileNames = configureFromCommandLine()

# histogram service
process.TFileService = cms.Service("TFileService", fileName = cms.string(outputFile) )

# higgs pT reweighter
from CMGTools.HtoZZ2l2nu.GeneratorLevelSequences_cff import addHiggsPtReweighting
needsPtReweight=addHiggsPtReweighting(process,castorDir)

# our MET prroducer
from CMGTools.HtoZZ2l2nu.ClusteredPFMetProducer_cfi import ClusteredPFMetProducer
process.ClusteredPFMetProducer = ClusteredPFMetProducer.clone()
process.ClusteredPFMetProducerPt5 = ClusteredPFMetProducer.clone( minJetPt = cms.double(5.0) )
process.ClusteredPFMetProducerPt10 = ClusteredPFMetProducer.clone( minJetPt = cms.double(10.0) )
process.ClusteredPFMetSequence = cms.Sequence(process.ClusteredPFMetProducer*process.ClusteredPFMetProducerPt5*process.ClusteredPFMetProducerPt10)

#pileup normalization
from CMGTools.HtoZZ2l2nu.PileupNormalizationProducer_cfi import puWeights
process.puWeights      = puWeights.clone( data = cms.string('/afs/cern.ch/user/p/psilva/public/Pileup/PileupTruth2011AplusB.root') )
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
                                    MET = BaseMetSelection.clone()
                                    ) 

#the path to execute
if(needsPtReweight) :
    process.p = cms.Path(process.ClusteredPFMetSequence*process.hkfactorSequence*process.puWeightSequence*process.evAnalyzer)
else :
    process.p = cms.Path(process.ClusteredPFMetSequence*process.puWeightSequence*process.evAnalyzer)

# message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 500
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    SkipEvent = cms.untracked.vstring('ProductNotFound')
    )

