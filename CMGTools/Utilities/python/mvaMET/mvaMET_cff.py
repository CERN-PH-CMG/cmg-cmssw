import FWCore.ParameterSet.Config as cms

from CMGTools.Common.eventCleaning.goodPVFilter_cfi import goodPVFilter 
from CMGTools.Utilities.mvaMET.mvaMETTauMu_cfi import * 
from CMGTools.Utilities.mvaMET.mvaMETTauEle_cfi import * 
from CMGTools.Utilities.mvaMET.mvaMETDiTau_cfi import * 

mvaMETSequence = cms.Sequence(
    goodPVFilter + 
    mvaMETTauMu + 
    mvaMETTauEle +
    mvaMETDiTau
    )
