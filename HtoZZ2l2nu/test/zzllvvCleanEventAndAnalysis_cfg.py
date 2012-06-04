import FWCore.ParameterSet.Config as cms

process = cms.Process("HtoZZto2l2nu")

process.load("Configuration.StandardSequences.Geometry_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.autoCond import autoCond
process.GlobalTag.globaltag=cms.string(autoCond.get('startup',autoCond['mc']))
#else process.GlobalTag=autoCond['com10']
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")

#the source is configured from the command line
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring()
                            )
from CMGTools.HtoZZ2l2nu.localPatTuples_cff import configureSourceFromCommandLine
castorDir, outputFile, process.source.fileNames = configureSourceFromCommandLine()

# histogram service
process.TFileService = cms.Service("TFileService", fileName = cms.string(outputFile) )

# our MET producer
from CMGTools.HtoZZ2l2nu.ClusteredPFMetProducer_cfi import ClusteredPFMetProducer
process.ClusteredPFMetProducer = ClusteredPFMetProducer.clone()

#pileup normalization
from CMGTools.HtoZZ2l2nu.PileupNormalizationProducer_cfi import puWeights
process.puWeights      = puWeights.clone( data = cms.string('$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/NewPileup2011AplusB.root'),
                                          mc   = cms.string('$CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/Summer11Observed.root')
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

#rho for muon isolation
process.load('CommonTools.ParticleFlow.ParticleSelectors.pfAllNeutralHadronsAndPhotons_cfi')
process.pfAllNeutralHadronsAndPhotons.src=cms.InputTag("particleFlow")
from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
process.kt6PFJetsCentralNeutral = kt4PFJets.clone(  src = cms.InputTag("pfAllNeutralHadronsAndPhotons"),
                                                    rParam = cms.double(0.6),
                                                    doAreaFastjet = cms.bool(True),
                                                    doRhoFastjet = cms.bool(True),
                                                    Ghost_EtaMax = cms.double(3.1),
                                                    Rho_EtaMax = cms.double(2.5),
                                                    inputEtMin = cms.double(0.5)
)
process.rhoForIsolationSequence = cms.Sequence(process.pfAllNeutralHadronsAndPhotons*process.kt6PFJetsCentralNeutral)



#MVAs for IDs
#process.load('EGamma.EGammaAnalysisTools.electronIdMVAProducer_cfi')
#process.mvaIDs = cms.Sequence(  process.mvaTrigV0 + process.mvaNonTrigV0 )


#the path to execute
process.p = cms.Path( #process.mvaIDs +
                      process.rhoForIsolationSequence +
                      process.ClusteredPFMetProducer +
                      process.puWeightSequence +
                      process.evAnalyzer)


# message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 500
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    SkipEvent = cms.untracked.vstring('ProductNotFound')
    )

