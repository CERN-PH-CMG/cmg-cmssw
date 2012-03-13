import FWCore.ParameterSet.Config as cms

process = cms.Process("HtoZZto2l2nu")

process.load("Configuration.StandardSequences.Geometry_cff")
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')


#the source is configured from the command line
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring()
                            )
from CMGTools.HtoZZ2l2nu.localPatTuples_cff import configureFromCommandLine
castorDir, outputFile, process.source.fileNames = configureFromCommandLine()

# histogram service
process.TFileService = cms.Service("TFileService", fileName = cms.string(outputFile) )

# our MET producer
from CMGTools.HtoZZ2l2nu.ClusteredPFMetProducer_cfi import ClusteredPFMetProducer
process.ClusteredPFMetProducer = ClusteredPFMetProducer.clone()

#pileup normalization
from CMGTools.HtoZZ2l2nu.PileupNormalizationProducer_cfi import puWeights
process.puWeights      = puWeights.clone( data = cms.string('/afs/cern.ch/user/p/psilva/public/Pileup/Pileup2011AplusB.root'),
                                          mc   = cms.string('/afs/cern.ch/user/p/psilva/public/Pileup/Summer11Observed.root')
                                          )
process.puWeightSequence = cms.Sequence(process.puWeights)

process.phoIdProducer = cms.EDProducer('PhoIDProd') 

#
# configure the analyzer (cf. base values are in the StandardSelections_cfi)
#
from CMGTools.HtoZZ2l2nu.StandardSelections_cfi import *
runLoose=False
if(runLoose) :
    process.evAnalyzer = cms.EDAnalyzer("DileptonPlusMETEventAnalyzer",
                                        dtag=cms.string('h2zz'),
                                        Trigger = BaseTriggerSelection.clone(),
                                        Generator = BaseGeneratorSelection.clone(),
                                        Vertices = BaseVertexSelection.clone(),
                                        Photons = BasePhotonsSelection.clone(),
                                        LooseMuons = BaseLooseMuonsSelection.clone( id = cms.string(''),
                                                                                    requireGlobal = cms.bool(False),
                                                                                    requireTracker = cms.bool(True),
                                                                                    maxRelIso = cms.double(999999.),
                                                                                    usePFIso = cms.bool(True)
                                                                                    ),
                                        Muons = BaseMuonsSelection.clone( requireGlobal = cms.bool(False),
                                                                          requireTracker = cms.bool(True),
                                                                          maxRelIso = cms.double(999999.),
                                                                          usePFIso = cms.bool(True)
                                                                          ),
                                        LooseElectrons = BaseLooseElectronsSelection.clone( id = cms.string(""),
                                                                                            maxRelIso=cms.double(99999.),
                                                                                            usePFIso = cms.bool(True)
                                                                                            ),
                                        Electrons = BaseElectronsSelection.clone( id = cms.string(''),
                                                                                  maxRelIso=cms.double(999999.),
                                                                                  usePFIso = cms.bool(True)
                                                                                  ),
                                        Dileptons = BaseDileptonSelection.clone(),
                                        Jets = BaseJetSelection.clone(),
                                        AssocJets = AssocJetSelection.clone(),
                                        MET = BaseMetSelection.clone()
                                        )

else:
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
    
#the path to execute
process.p = cms.Path(process.phoIdProducer*process.ClusteredPFMetProducer*process.puWeightSequence*process.evAnalyzer)


# message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 500
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    SkipEvent = cms.untracked.vstring('ProductNotFound')
    )

