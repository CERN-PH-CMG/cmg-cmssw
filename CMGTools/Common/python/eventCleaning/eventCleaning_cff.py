import FWCore.ParameterSet.Config as cms

from RecoParticleFlow.PostProcessing.selectGoodPFEvents_cff import *

from CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi import *

# see https://hypernews.cern.ch/HyperNews/CMS/get/JetMET/1196.html
HBHENoiseFilterResultProducer.minIsolatedNoiseSumE = 999999.
HBHENoiseFilterResultProducer.minNumIsolatedNoiseChannels = 999999
HBHENoiseFilterResultProducer.minIsolatedNoiseSumEt = 999999.


# currently running:
#  - PF inconsistent muon filter
#  - PF greedy muon filter
eventCleaningSequence = cms.Sequence(
    # selectGoodPFEventsSequence +
    # note: the following will produce a boolean in the EDM but
    # does not filter events
    #COLIN : good idea! should do the same for PF filters
    HBHENoiseFilterResultProducer +
    selectGoodPFEventsTaggingSequence
    )

# importing paths and output modules to select bad events in RECOSIM format
# one path per filter.
# each path should be added to the schedule, and the corresponding
# output modules to the endpath
# from RecoParticleFlow.PostProcessing.badPFEvents_cff import *
