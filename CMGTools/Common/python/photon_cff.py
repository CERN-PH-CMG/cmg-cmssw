import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgPhoton_cfi import *
from CMGTools.Common.skims.cmgPhotonSel_cfi import *

photonSequence = cms.Sequence(
    cmgPhoton 
    +cmgPhotonSel 
)
