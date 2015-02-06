import FWCore.ParameterSet.Config as cms

from CMGTools.Common.electron_cff import *
from CMGTools.Common.met_cff import *
from CMGTools.Common.factories.cmgWENu_cfi import *
from CMGTools.Common.skims.cmgWENuSel_cfi import *
from CMGTools.Common.skims.cmgWENuCount_cfi import *
from CMGTools.Common.histograms.wLorentzVector_cfi import *

wENuSequence = cms.Sequence(
    electronSequence + 
    metSequence + 
    cmgWENu + 
    cmgWENuSel +
    cmgWENuCount +
    wenuLorentzVector
)
