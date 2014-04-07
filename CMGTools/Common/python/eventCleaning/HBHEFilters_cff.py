import FWCore.ParameterSet.Config as cms

## HBHE filters


from CMGTools.Common.eventCleaning.HBHENoiseFilter2010_cfi import *
from CMGTools.Common.eventCleaning.HBHENoiseFilter2011IsoDefault_cfi import *
from CMGTools.Common.eventCleaning.HBHENoiseFilter2011NonIsoRecommended_cfi import *


HBHEFiltersSequence = cms.Sequence(
   HBHENoiseFilter2010  +
   HBHENoiseFilter2011IsoDefault  +
   HBHENoiseFilter2011NonIsoRecommended
   )


from CMGTools.Common.eventCleaning.HBHENoiseFilterResultProducer2010_cfi import *
from CMGTools.Common.eventCleaning.HBHENoiseFilterResultProducer2011IsoDefault_cfi import *
from CMGTools.Common.eventCleaning.HBHENoiseFilterResultProducer2011NonIsoRecommended_cfi import *


HBHEFiltersTaggingSequence = cms.Sequence(
   HBHENoiseFilterResultProducer2010  +
   HBHENoiseFilterResultProducer2011IsoDefault  +
   HBHENoiseFilterResultProducer2011NonIsoRecommended
   )


