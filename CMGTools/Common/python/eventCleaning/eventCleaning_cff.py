#Colin&Will : this file is obsolete and will be removed soon

import FWCore.ParameterSet.Config as cms

from RecoParticleFlow.PostProcessing.selectGoodPFEvents_cff import *

# HCAL noise filter

# from CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi import *

# see https://hypernews.cern.ch/HyperNews/CMS/get/JetMET/1196.html
# HBHENoiseFilterResultProducer.minIsolatedNoiseSumE = 999999.
# HBHENoiseFilterResultProducer.minNumIsolatedNoiseChannels = 999999
# HBHENoiseFilterResultProducer.minIsolatedNoiseSumEt = 999999.

from CMGTools.Common.eventCleaning.HBHEFilters_cff import * 

# ECAL missing channels

# TP filter

# from JetMETAnalysis.ecalDeadCellTools.RA2TPfilter_cff import *
# ecalDeadCellTPfilter.taggingMode = True

# ecalDeadCellTaggingSequence = cms.Sequence(
#    ecalDeadCellTPfilter
#    )

# Filter against bad recovery of EE rechits

from CMGTools.Common.eventCleaning.recovRecHitFilter_cfi import *

# Filter against ECAL+CSC noise

from CMGTools.Common.eventCleaning.eeNoiseFilter_cfi import *

# Tracking failure filter

from CMGTools.Common.eventCleaning.trackingFailureFilter_cfi import *

# PV filter in tagging mode

from CMGTools.Common.eventCleaning.goodPrimaryVertexFilter_cfi import *

# scraping filter in tagging mode

from CMGTools.Common.eventCleaning.scrapingFilter_cfi import *


eventCleaningTaggingSequence = cms.Sequence(
    # ecalDeadCellTaggingSequence + 
    HBHEFiltersTaggingSequence + 
    selectGoodPFEventsTaggingSequence +
    recovRecHitFilter +
    eeNoiseFilter +
    trackingFailureFilterCMG +
    goodPrimaryVertexFilter +
    scrapingFilter 
    )
from CMGTools.Common.eventCleaning.goodPVFilter_cfi import *

eventCleaningFilteringSequence = cms.Sequence(
    scrapingFilter +
    goodPVFilter
    )

eventCleaningSequence = cms.Sequence(
    eventCleaningTaggingSequence 
    # eventCleaningFilteringSequence     # Now we apply the filters in tagging mode
    # selectGoodPFEventsSequence +
    # note: the following will produce a boolean in the EDM but
    # does not filter events
    # HBHENoiseFilterResultProducer +
    # HBHEFiltersTaggingSequence + 
    # selectGoodPFEventsTaggingSequence
    #COLIN: uncomment the following when the filter is running in tagging mode
    # + ecalDeadCellTaggingSequence
    )

# importing paths and output modules to select bad events in RECOSIM format
# one path per filter.
# each path should be added to the schedule, and the corresponding
# output modules to the endpath
# from RecoParticleFlow.PostProcessing.badPFEvents_cff import *
