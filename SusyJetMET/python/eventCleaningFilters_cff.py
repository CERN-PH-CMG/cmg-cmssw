import FWCore.ParameterSet.Config as cms

from RecoParticleFlow.PostProcessing.selectGoodPFEvents_cff import *
from RecoParticleFlow.PostProcessing.selectEventsWithSmallDeltaPtMuons_cff import *

from UserCode.EcalDeadCellEventFilter.EcalDeadCellEventFilter_cff import *
from PhysicsTools.EcalAnomalousEventFilter.ecalanomalouseventfilter_cfi import *

# to study the filters
eventCleaningFiltersIgnore = cms.Sequence(
    # Muon Filters in RA2
    cms.ignore( greedyMuons ) +
    cms.ignore( inconsistentMuons ) +
    # Delta pt filter (dropped from RA2 )
    largeDeltaPtMuons +
    cms.ignore( filterLargeDeltaPtMuons ) +
    # BE filter
    cms.ignore( EcalAnomalousEventFilter )
    )

# main filtering sequence for analysis

eventCleaningFilters = cms.Sequence(
    # greedy and inconsistent muons
    selectGoodPFEventsSequence +
    # BE filter
    EcalAnomalousEventFilter
    )

# for the Data, we add TP filtering

eventCleaningFiltersIgnoreData = cms.Sequence(
    eventCleaningFiltersIgnore +
    cms.ignore( EcalDeadCellEventFilter ) 
    )

eventCleaningFiltersData = cms.Sequence(
    eventCleaningFilters +
    EcalDeadCellEventFilter
    )

