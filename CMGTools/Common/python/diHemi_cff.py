import FWCore.ParameterSet.Config as cms

from CMGTools.Common.hemi_cff import *
from CMGTools.Common.factories.cmgDiHemi_cfi import *

diHemiSequence = cms.Sequence(
    hemiSequence +
    cmgDiHemi 
)
