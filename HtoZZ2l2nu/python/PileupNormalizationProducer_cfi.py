import FWCore.ParameterSet.Config as cms

##
## pileup normalization scenarios
##
puWeights = cms.EDProducer("PileupNormalizationProducer",
                           integerWeightsOnly = cms.bool(False),
                           mc   = cms.string('/afs/cern.ch/user/p/psilva/public/Pileup/Summer11Truth.root'),
                           data = cms.string('/afs/cern.ch/user/p/psilva/public/Pileup/PileupTruth2011AplusB.root'),
                           puWeightFile = cms.string('/afs/cern.ch/user/p/psilva/public/Pileup/Weight3D_2011AplusB.root'),
                           use3D = cms.bool(True)
                           )

puFilter = cms.EDFilter("PileupFilter",
                        source = cms.InputTag("puWeights:renPuWeight")
                        )
