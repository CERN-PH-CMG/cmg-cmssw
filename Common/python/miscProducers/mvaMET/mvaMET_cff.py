import FWCore.ParameterSet.Config as cms

from CMGTools.Common.eventCleaning.goodPVFilter_cfi import goodPVFilter 
from CMGTools.Common.miscProducers.mvaMET.mvaMETTauMu_cfi import * 

mvaMETSequence = cms.Sequence(
    goodPVFilter + 
    mvaMETTauMu
    )
