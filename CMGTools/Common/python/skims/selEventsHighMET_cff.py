import FWCore.ParameterSet.Config as cms

from CMGTools.Common.skims.cmgPFMETSel_cfi import *
from CMGTools.Common.skims.cmgPFMETCount_cfi import *

selEventsHighMETSequence = cms.Sequence(
    cmgPFMETSel +
    cmgPFMETCount    
    )

